#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "book.h"
#include "gui.h"

#include "nes.h"
#include "rom.h"
#include "cpu.h"
#include "timing.h"

#include "input.h"

#include <math.h>

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

#include "palettes/palette_l.h"
//#include "palettes/palette_nester.h"
//#include "palettes/palette_nesticle.h"

MyGUI *MyGui;

unsigned char *video_buffer;

int *screenbuf;

int DWidth;
int DHeight;

void InitVars()
{
  DWidth = Display_GetWidth(0);
  DHeight = Display_GetHeight(0);
}

void DrawStr(wchar_t *str, int x, int y, int color)
{
  SetFont(0xC);
  int sid = TextID_Create( str, ENC_UCS2, TEXTID_ANY_LEN );
  DrawString(sid, 0, x, y, 0xFFFF, y+0xC, 60, 0, color, 0 );
  TextID_Destroy(sid);
}

void video2screen()
{
  int x_ratio = (int)((256<<16)/DWidth) +1;
  int y_ratio = (int)((LAST_DISPLAYED_LINE<<16)/DHeight) +1;
  int x2, y2;
  for (int i=0;i<DHeight;i++)
  {
    for (int j=0;j<DWidth;j++)
    {
      x2 = ((j*x_ratio)>>16);
      y2 = ((i*y_ratio)>>16);
      screenbuf[(i*DWidth)+j] = *(int*)default_palette[video_buffer[(y2*256)+x2]];
    }
  }
}

void GUI_Redraw(DISP_OBJ *dd, int a, int b, int c)
{
  GC *gc = get_DisplayGC();
  int gc_xx = GC_GetXX( gc );
  GC_SetXX( gc, 1 );

  GC_WritePixels( gc, 0, 0, DWidth, DHeight, screenbuf );

  wchar_t buf[0x100];
  snwprintf( buf, MAXELEMS(buf), L"fps %d", timing_fps);

  DrawStr( buf, 0, 0, 0xFFFFFFFF );

  GC_SetXX( gc, gc_xx );
}

void GUI_OnKey(DISP_OBJ *db, int key, int, int repeat, int type)
{
  int mode;
  if( type == KBD_SHORT_PRESS || type == KBD_LONG_PRESS )
    mode = 1;
  else if( type == KBD_SHORT_RELEASE || type == KBD_LONG_RELEASE )
    mode = 0;
  else
    return;

  if( key == key_a )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_A] = mode;
  else if( key == key_b )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_B] = mode;
  else if( key == key_select )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_SELECT] = mode;
  else if( key == key_start )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_START] = mode;
  else if( key == key_up )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_UP] = mode;
  else if( key == key_down )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_DOWN] = mode;
  else if( key == key_left )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_LEFT] = mode;
  else if( key == key_right )
    buttons[INPUT_PLAYER_1][INPUT_DEVICE_BUTTON_RIGHT] = mode;
}

int GUI_OnCreate(DISP_OBJ *)
{
  return 1;
}

void GUI_OnClose(DISP_OBJ *)
{
}


void GUI_Exit(BOOK *bk,GUI*)
{
  UI_Event_toBookID( ELF_TERMINATE_EVENT , BookObj_GetBookID(MyBook) );
}


void GUI_Construct(DISP_DESC *dd)
{
  DISP_DESC_SetName( dd, "TEST_DD" );
  DISP_DESC_SetSize( dd, sizeof(DISP_OBJ) );

  DISP_DESC_SetOnCreate( dd, GUI_OnCreate);
  DISP_DESC_SetOnClose( dd, GUI_OnClose);

  DISP_DESC_SetOnRedraw( dd, GUI_Redraw );
  DISP_DESC_SetOnKey( dd, GUI_OnKey );
}

void GUI_Destruct(GUI *dd)
{
  return;
}

GUI *GUI_Create()
{
  MyGui = new MyGUI;

  if ( !GUIObject_Create( MyGui, GUI_Destruct, GUI_Construct, MyBook, 0, 0, 0) )
  {
    delete MyGui;
    return 0;
  }

  InitVars();

  screenbuf = (int*)malloc(DWidth*DHeight*sizeof(int));
  video_buffer = (unsigned char*)malloc(TOTAL_LINES_PAL*256);

  memset(screenbuf, 0, DWidth*DHeight*sizeof(int));
  memset(video_buffer, 0, TOTAL_LINES_PAL*256 );

  BookObj_AddGUIObject( MyBook, MyGui );
  GUIObject_SetStyle( MyGui, UI_OverlayStyle_TrueFullScreen );
  GUIObject_SetTitleType( MyGui , 1 );
  GUIObject_SoftKeys_Hide( MyGui );
  GUIObject_Show(MyGui);

  GUIObject_SoftKeys_SetAction( MyGui, ACTION_BACK, GUI_Exit );

  if( !start_emu() )
  {
    delete MyGui;
    return 0;
  }

  return MyGui;
}

void GUI_Destroy()
{
  //stop_emu();

  if( MyGui )
    MyGui = (MyGUI *)GUIObject_Destroy(MyGui);

  mfree(screenbuf);
  mfree(video_buffer);
}
