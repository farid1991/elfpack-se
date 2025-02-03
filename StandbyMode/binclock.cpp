#include "..\include\Lib_Clara.h"
#include "binclock.h"
#include "config_data.h"
#include "misc_util.h"
#include "MediaInfo.h"
#include "conf_loader.h"

/*
Rop - A raster-operation code. These codes define how the color data for
      the source rectangle is to be combined with the color data for
      the destination rectangle to achieve the final color.

      The following list shows some common raster operation codes.
*/
#define SRCCOPY             0xCC /* dest = source */
#define SRCPAINT            0xEE /* dest = source OR dest */
#define SRCAND              0x88 /* dest = source AND dest */
#define SRCINVERT           0x66 /* dest = source XOR dest */
#define SRCERASE            0x44 /* dest = source AND (NOT dest ) */
#define NOTSRCCOPY          0x33 /* dest = (NOT source) */
#define NOTSRCERASE         0x11 /* dest = (NOT src) AND (NOT dest) */
#define MERGECOPY           0xC0 /* dest = (source AND pattern) */
#define MERGEPAINT          0xBB /* dest = (NOT source) OR dest */
#define PATCOPY             0xF0 /* dest = pattern */
#define PATPAINT            0xFB /* dest = (pattern OR (NOT source)) OR dest */
#define PATINVERT           0x5A /* dest = pattern XOR dest */
#define DSTINVERT           0x55 /* dest = (NOT dest) */
#define BLACKNESS           0x00 /* dest = BLACK */
#define WHITENESS           0xFF /* dest = WHITE */

///////////////////


int clock3_show = 0;

//ресурсы
GC* hMGC = 0;
GVI_GC hrc = 0;
int mgc_w = 0, mgc_h = 0;
int off = 0, on = 0, mask = 0;
//маска
GVI_BMP bmpMask = 0;
GVI_GC  hdcMask = 0;
//часы
GC* hMGCClock = 0;
GVI_BMP bmpClock = 0;
GVI_GC  hdcClock = 0;
int clock_w = 0, clock_h = 0;
int clockLeds[6][4] = {
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};

//           col row
const int map[6][4] = {
  {0, 0, 1, 1},
  {1, 1, 1, 1},
  {0, 1, 1, 1},
  {1, 1, 1, 1},
  {0, 1, 1, 1},
  {1, 1, 1, 1}
};

//                Dec Bits
const int digital[10][4] = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 1 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 1 },
  { 0, 1, 0, 0 },
  { 0, 1, 0, 1 },
  { 0, 1, 1, 0 },
  { 0, 1, 1, 1 },
  { 1, 0, 0, 0  },
  { 1, 0, 0, 1  },
};

GC* LoadExternalResource(int *width, int *height)
{
  FSTAT _fstat;
  wchar_t path[256];
  wchar_t name[256];
  extractdir(path,name,cfg_clock3_resource);

  if(width) *width = 0;
  if(height) *height = 0;

  IMAGEID id;
  if(fstat(path, name, &_fstat)==0)
  {
      if(ImageID_Get(path, name, &id)<0)
      {
        return 0;
      }
  }
  else return 0;

  int Width  = GetImageWidth(id);
  int Height = GetImageHeight(id);

  //создаём контекст памяти
  GC* gc = GC_CreateMemoryGC(Width,Height,16,0,0,0);
  //делаем заливку в памяти
  GC_DrawFRect (gc, clBlack, 0, 0, Width, Height);
  //рисуем символ в памяти
  if(gc) putchar(gc,0,0,0,0,id);
  ImageID_Free(id);
  if(width) *width = Width / 3;
  if(height) *height = Height;

  return gc;
}

int FreeResource(GC* gc)
{
  if(gc) GC_FreeGC(gc);
  gc = 0;
  return 0;
}

int FreeGviResource(GVI_GC gvigc)
{
  if(gvigc) GVI_DeleteMemoryGC(gvigc);
  gvigc = 0;
  return 0;
}

void UpdateBinClock(TIME *tm)
{
    int digits[] = { 0,0,0,0,0,0 };

    digits[0] = tm->hour / 10;
    digits[1] = tm->hour % 10;
    digits[2] = tm->min / 10;
    digits[3] = tm->min % 10;
    digits[4] = tm->sec / 10;
    digits[5] = tm->sec % 10;

    for(int column = 0; column < 6; column++)
      for(int row = 0; row < 4; row++)
      {
        if(map[column][row])
        {
          int newState = digital[digits[column]][row];
          if(clockLeds[column][row] != newState)
          {
            clockLeds[column][row] = newState;
            if(newState)
              GVI_BitBlt(hdcClock, column*mgc_w, row*mgc_h, mgc_w , mgc_h, hrc, on, 0, SRCCOPY, 0, 0, 0);
            else
              GVI_BitBlt(hdcClock, column*mgc_w, row*mgc_h, mgc_w , mgc_h, hrc, off, 0, SRCCOPY, 0, 0, 0);
          }
        }
      }
}

void ShowBinClock(int y, int location)
{
  if(cfg_crc == REF_CRC32)
  if((clock3_show) && (location == SCREEN))
  {
    GVI_GC hdc;
    void *GC_DISP=get_DisplayGC ();
    //получаем GVI_ контекст из GC_
    CANVAS_Get_GviGC( ((GC *)GC_DISP)->pcanvas, &hdc);
    // Накладываем маску на те места, где будет помещён битмап.
    GVI_BitBlt(hdc, cfg_clock3_x, cfg_clock3_y - y, clock_w , clock_h, hdcMask, 0, 0, SRCAND, 0, 0, 0);
    // XOR-им битмап с фоном на конечном GVI_ контексте.
    GVI_BitBlt(hdc, cfg_clock3_x, cfg_clock3_y - y, clock_w , clock_h, hdcClock, 0, 0, SRCPAINT, 0, 0, 0);
  }
}

void UpdateBinClockParams(void)
{
  FreeResource(hMGC);
  FreeGviResource(hdcClock);
  FreeGviResource(hdcMask);

  if(cfg_clock3_show)
  {
    GVI_GC hdc;
    void *GC_DISP=get_DisplayGC ();
    //получаем GVI_ контекст из GC_
    CANVAS_Get_GviGC( ((GC *)GC_DISP)->pcanvas, &hdc);

    //загружаем ресурсы
    hMGC = LoadExternalResource(&mgc_w, &mgc_h);
    if(!hMGC)
    {
        wchar_t temp[128];
        #ifdef ENG
          snwprintf(temp, MAXELEMS(temp), _T("Can't load resource for the binary clock"));
        #else
          snwprintf(temp, MAXELEMS(temp), _T("Не могу загрузить ресурсы для двоичных часов"));
        #endif
        MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
    }
    off  = 0;
    on   = mgc_w;
    mask = 2 * mgc_w;
    //получаем GVI_ контекст из GC_
    CANVAS_Get_GviGC( hMGC->pcanvas ,&hrc);

    //рассчитываем размеры часов
    clock_w = 6 * mgc_w;
    clock_h = 4 * mgc_h;

    //создаём контекст для часов
    bmpClock = GVI_CreateBitmap(clock_w, clock_h, 16);
    hdcClock = GVI_CreateMemoryGC(bmpClock);
    //делаем заливку
    GVI_FillSolidRectangle(hdcClock, 0, 0, clock_w, clock_h, clBlack);

    // Создаём маску.
    bmpMask = GVI_CreateBitmap(clock_w, clock_h, 1);
    hdcMask = GVI_CreateMemoryGC(bmpMask);

    for(int column = 0; column < 6; column++)
    {
      for(int row = 0; row < 4; row++)
      {
        if(map[column][row])
          GVI_BitBlt(hdcClock, column*mgc_w, row*mgc_h, mgc_w , mgc_h, hrc, mask, 0, SRCCOPY, 0, 0, 0);
      }
    }
    // Создаём маску для битмапа путём вызова BitBlt из исходного
    // битмапа на монохромный битмап.
    GVI_BitBlt(hdcMask, 0, 0, clock_w , clock_h, hdcClock, 0, 0, NOTSRCCOPY, 0, 0, 0);

    //Инициализация часов
    for(int column = 0; column < 6; column++)
    {
      for(int row = 0; row < 4; row++)
      {
        clockLeds[column][row] = 0;
        if(map[column][row])
          GVI_BitBlt(hdcClock, column*mgc_w, row*mgc_h, mgc_w , mgc_h, hrc, off, 0, SRCCOPY, 0, 0, 0);
      }
    }

  }
}

void UpdateBinClockShowParams(void)
{
  clock3_show = cfg_clock3_show && (!(PlayerIsStarted && cfg_clock3_not_show_if_player));
}

void FreeBinClockParams(void)
{
  FreeResource(hMGC);
  FreeGviResource(hdcMask);
  FreeGviResource(hdcClock);
}










