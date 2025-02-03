#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "cfg_items.h"
#include "main.h"
#include "gui.h"
#include "game.h"
#include "resource.h"
#include "random.h"
#include "util.h"

#include "lists.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

#define N1 4  //Количество картинок для появления шарика
#define N2 12 //Количество картинок для прыжка шарика
#define N3 9  //Количество картинок для удаления шарика

#define MAX_MAP_X 9   //Максимальный размер поля по x
#define MAX_MAP_Y 10  //Максимальный размер поля по y
#define MIN_MAP_X 2   //Минимальный размер поля по x
#define MIN_MAP_Y 6   //Минимальный размер поля по y

#define MIN_DEL_BALLS 2 //Минимальное количество удаляемых шариков

///////////////////////////////////////////////////////////
int cur_resource = 0;
int scr_w = 0;
int scr_h = 0;
int game_h = 0;

int CELL_SIZE  = _CELL_SIZE_;
int TOP_HEIGHT = _TOP_HEIGHT_;
int NUMBER_W   = _NUMBER_W_;
int NUMBER_H   = _NUMBER_H_;
int POINTS_W   = _POINTS_W_;

int CELL_SIZE_1 = _CELL_SIZE_ + 1;
int TOP_HEIGHT_NUMBER_H = (_TOP_HEIGHT_ - _NUMBER_H_)/2;
int NUMBER_H_1 = _NUMBER_H_ + 1;
int NUMBER_W_1 = _NUMBER_W_ - 1;
int POINTS_W_1 = _POINTS_W_ + 1;
int NUMBER_W_2 = _NUMBER_W_ * 4 + 5;
int NUMBER_W_3 = _NUMBER_W_ + 1;

#define _W1_ (_NUMBER_W_ + 4)
#define _W2_ (_W1_ + (_NUMBER_W_ - 1))
#define _W6_ (_W2_ + (_POINTS_W_ + 1))
#define _W3_ (_W6_ + (_NUMBER_W_ - 1))
#define _W4_ (_W3_ + (_NUMBER_W_ - 1))
#define _W7_ (_W4_ + (_POINTS_W_ + 1))
#define _W5_ (_W7_ + (_NUMBER_W_ - 1))

int W1 = _W1_;
int W2 = _W2_;
int W6 = _W6_;
int W3 = _W3_;
int W4 = _W4_;
int W7 = _W7_;
int W5 = _W5_;

GC* hMGC = 0;
GVI_GC gvigcmem = 0;
GVI_GC gvigcobj = 0;

int mgc_w = 0;
int mgc_h = 0;

TEXTID menuId = 0x6FFFFFFF;
int menu_h = 0;

int cell_size = 0;
int top_height = 0;

pCG bmp_0,bmp_prestand, bmp_stand, bmp_jump[N2], bmp_explode[N3], bmp_appear[N1], bmp_numbers, bmp_points;
pCG *bmp[6];

u16 timerGametime = 0;
u16 timerGamestate = 0;
///////////////////////////////////////////////////////////

struct map_t
{
  //Состояние автомата для этой ячейки
  int y;
  //Цвет шарика в этой ячейке
  int color;
  //Цвет шарика-подсказки в этой ячейке
  //(когда игровой шарик проходит через эту ячейку, тогда color != pre_color)
  int pre_color;
  //Номер картинки, выводимой в текущий момент, при появлении, удалении, прыжке шарика
  int num_pic;
}
// Игровое поле
map[MAX_MAP_X][MAX_MAP_Y];

// Цвет игрового шарика
int ball_color;

struct info
{
  int score;
  int time;
  wchar_t name[MAX_NAME_LEN+1];
}
// Лучшие результаты
leaders[BEST_RESULTTS_NUM];

// Минимальный результат, чтобы попасть в таблицу
const info null_leader={100,3600,L"Noname"};

// Состояние автомата управления игрой
int y_lines;

// Время игры
int gametime = 0;
int gametime_enable = 0;
// Очки
int gamescore = 0;

// Тип игры: 0-easy, 1-normal, 2-hard, 3-custom
int gametype = 1;

// Состояние игры: 0 - новая, 1 - незаконченная, 2 - законченная
int gamestate = 0;

//Статус игры: 0 - не играют, 1 - играют
int gamestatus = 0;

// Текущий размер поля
int max_x = 9; // по x
int max_y = 9; // по y
//Количество появляющихся шаров
int app_balls = 3;
//Количество удаляемых шаров
int del_balls = 5;

//#define LOGGING //Включаем логирование

#ifdef LOGGING
const char* aCell_states[6] =
{
  "Absence of a ball",
  "Help",
  "Ball occurrence",
  "The ball stands",
  "The ball jumps",
  "Ball removal"};

const char* aLines_states[6] =
{
  "Search of the game ball",
  "Search where to send the game ball",
  "Movement of the game ball",
  "Removal of lines",
  "Occurrence of new balls",
  "Cleanup"};

void WriteLog(char *buf)
{
  int flog;
  wchar_t path[256];
  char msg[512];
  DATETIME dt;

  REQUEST_DATEANDTIME_GET(SYNC,&dt);

  wstrcpy(path,GetDir(DIR_OTHER|MEM_EXTERNAL));
  if ((flog=_fopen(path,L"LinesLog.txt",FSX_O_CREAT|FSX_O_APPEND|FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0)) < 0) { return; }

  sprintf(msg, "[%02d:%02d:%02d] %s", dt.time.hour, dt.time.min, dt.time.sec, buf);
  fwrite(flog,msg,strlen(msg));
  fclose(flog);
}

#endif

// Класс "Ячейка"
class cell
{
public:
  int posx; // Позиция ячейки на поле (слева направо от 0)
  int posy; // Позиция ячейки на поле (сверху вниз от 0)
  bool operator ==(const cell & b) const
  {return (b.posx == posx && b.posy == posy);}
  bool operator !=(const cell & b) const
  {return (!(*this == b));}
  int & State() const
  {return map[posx][posy].y;}
  int & Color() const
  {return map[posx][posy].color;}
  int & PreColor() const
  {return map[posx][posy].pre_color;}
  int & NumPic() const
  {return map[posx][posy].num_pic;}

  // автомат "ячейка"
  void ACell(int e) const
  {
    int &y=State();

    #ifdef LOGGING
      int y_old = y;
    #endif

    switch(y)
    {
    // Отсутствие шара
    case 0:
      if (e==1)         {z0();  y=3;}
      if (e==2 && x0()) {z4();  y=1;}
      else if (e==2)    {z1();  y=1;}
      break;
    // Подсказка
    case 1:
      if (e==1)         {z2();  y=3;}
      if (e==3)                 y=2;
      break;
    // Появление шара
    case 2:
      if (e==3 && x1()) {z6();  y=3;}
      else if (e==3)    {z5();}
      break;
    // Шар стоит
    case 3:
      if (e==0 && x0() )  {z4();  y=1;}
      else if (e==0)      {z3();  y=0;}
      if (e==4)                   y=5;
      if (e==5)                   y=4;
      break;
    // Шар прыгает
    case 4:
      if (e==0)           {z6();z3();  y=0;}
      if (e==6)           {z6();    y=3;}
      if (e==5 && x2())   {z6();}
      else if (e==5)      {z5();}
      break;
    // Удаление шара
    case 5:
      if (e==4 && x3() && x0())   {z6();z4();  y=1;}
      else if (e==4 && x3())      {z6();z3();  y=0;}
      else if (e==4)              {z5();}
      break;
    }

    #ifdef LOGGING
      if (y!=y_old)
      {
        char log[512];
        sprintf(log,"=> Cell (%d,%d) from state \"%s\" goto in state \"%s\".\n",posx+1,posy+1,aCell_states[y_old],aCell_states[y]);
        WriteLog(log);
      }
    #endif


    DrawState();
  }

  void DrawState() const
  {
    CANVAS_Get_GviGC( bmp[State()][NumPic()]->pcanvas ,&gvigcobj);
    GVI_BitBlt(gvigcmem,posx*CELL_SIZE, TOP_HEIGHT+posy*CELL_SIZE, CELL_SIZE_1, CELL_SIZE_1, gvigcobj, Color()*CELL_SIZE, 0, 204, 0, 0, 0);
  }

private:
  //ВХОДЯЩИЕ ПЕРЕМЕННЫЕ

  //Требуется восстановление подсказки
  bool x0() const
  {return PreColor()!=-1;}
  //Последняя стадия появления шара
  bool x1() const
  {return (NumPic()==N1-1);}
  //Последняя стадия прыжка для шара
  bool x2() const
  {return (NumPic()==N2-1);}
  //Последняя стадия удаления шара
  bool x3() const
  {return (NumPic()==N3-1);}

  //ВЫХОДЯЩИЕ ВОЗДЕЙСТВИЯ

  //Установить в ячейке двигающийся шар
  void z0() const
  {Color()=ball_color;}
  //Генерировать подсказку
  void z1() const
  {Color()=random(7);}
  //Очистить ячейку
  void z2() const
  {PreColor()=Color();Color()=ball_color;}
  //Установить в ячейке с подсказкой двигающийся шар
  void z3() const
  {Color()=0;}
  //Восстановить подсказку
  void z4() const
  {Color()=PreColor();PreColor()=-1;}
  //Вывести на экран следующую картинку для текущего состояния
  void z5() const
  {++NumPic();}
  //Вывести на экран первую картинку для текущего состояния
  void z6() const
  {NumPic()=0;}

};

//Игровой шар
cell ball;
//Выбранный шар (ткнули 'мышкой')
cell click_ball;

//Список появляющихся шаров
aux::list<cell> *appear_list = 0;
//Список удаляющихся шаров
aux::list<cell> *explode_list = 0;
//Путь по которому проходит двигающийся шар
aux::list<cell> *path = 0;

//Иконка курсора
wchar_t cursor = 0xFFFF;
//Координаты курсора
int cursor_x = 0; // по x
int cursor_y = 0; // по y


// Прототипы функций, встречаемых далее в программе
void ALines(int);
void z0();
void z1_1();
void z1_2();
void z1_3();
void z2_1();
void z2_2();
void z2_3();
void z2_4();
void z3_1();
void z3_2();
void z4_1();
void z4_2();

bool xk0();
bool xk1();
bool xk2();
bool x0();
bool x1();
bool x2();
bool x3();
bool x4();
bool x5();
bool x6();

bool FindEmptyCell(cell &);
void GenerateAppearList();
void CheckAppearList();
bool FindPath(const cell &, const cell &);
bool CheckLines(const cell &);
bool Valid(const cell &);

void UpdateBestResult();
void GameOver();
void NewGame();
void DrawTime();
void DrawScore();
void DrawTop();

void CheckCustomParameters();
void GetInfo();
void WriteInfo();

wchar_t snd_path[256];
int play_cantmove = 0;
int play_cantmove_timeout = 0;

void playFile(const wchar_t *filename)
{
  if((!GetSilent() || (cfg_silent==1)))
  {
    char vol = cfg_vol;
//    GetAudioSettings(2,&vol);
    PlayFileV(snd_path,filename,vol);
  }
}

void PlayJump()
{
  if(cfg_snd_jump) playFile(L"jump.wav");
}

void PlayMove()
{
  if(cfg_snd_move) playFile(L"move.wav");
}

void PlayDestroy()
{
  if(cfg_snd_destroy) playFile(L"destroy.wav");
}

void PlayCantmove()
{
  if(cfg_snd_cantmove) playFile(L"cantmove.wav");
}


/*
   if(GetVibrator(0,0) && (cfg_vibra==1))
   {
      PAudioControl pAC = AudioControl_Init();
      if( !pAC ) pAC = *GetAudioControlPtr();
      AudioControl_Vibrate(pAC, 400, 50, cfg_vibra_time*1000);
   }
*/


bool isGameActivated()
{
  return gamestatus == 1;
}

bool isGameNotEnded()
{
  return gamestate == 1;
}

void SetNewGameState()
{
  gamestate = 0;
}

void SetContinueGameState()
{
  if(gamestate == 0) gamestate = 1;
}

void SetEndGameState()
{
  gamestate = 2;
}

void InitScreen()
{
  scr_w = Display_GetWidth(0);
  scr_h = Display_GetHeight(0);
}

static void InitData()
{
  CELL_SIZE_1 = CELL_SIZE + 1;

  W1 = (NUMBER_W + 4);
  W2 = (W1 + (NUMBER_W - 1));
  W6 = (W2 + (POINTS_W + 1));
  W3 = (W6 + (NUMBER_W - 1));
  W4 = (W3 + (NUMBER_W - 1));
  W7 = (W4 + (POINTS_W + 1));
  W5 = (W7 + (NUMBER_W - 1));

  TOP_HEIGHT_NUMBER_H = (TOP_HEIGHT-NUMBER_H)/2;
  NUMBER_H_1 = NUMBER_H + 1;
  NUMBER_W_1 = NUMBER_W - 1;
  POINTS_W_1 = POINTS_W + 1;

  NUMBER_W_2 = NUMBER_W * 4 + 5;
  NUMBER_W_3 = NUMBER_W + 1;
}

GC* LoadInternalResource(const char *res, int len)
{
  IMAGEID id;
  char *buf = new char[len];
  if(buf)
  {
    memcpy(buf,res,len);
    if(ImageID_GetIndirect(buf, len, 0, L"png", &id) < 0)
    {
      return 0;
    }
  }
  else return 0;

  int Width  = GetImageWidth(id);
  int Height = GetImageHeight(id);

  GC* gc = GC_CreateMemoryGC(Width,Height,16,0,0,0);
  if(gc) GC_PutChar(gc,0,0,0,0,id);
  ImageID_Free(id);
  return gc;
}

GC* LoadExternalResource(const wchar_t *res, int *width, int *height)
{
  FSTAT _fstat;
  wchar_t path[128];
  getdir(path,resource_path);

  IMAGEID id;
  if(fstat(path, res, &_fstat)==0)
  {
      if(ImageID_Get(path, res, &id)<0)
      {
        return 0;
      }
  }
  else return 0;

  int Width  = GetImageWidth(id);
  int Height = GetImageHeight(id);

  GC* gc = GC_CreateMemoryGC(Width,Height,16,0,0,0);
  if(gc) GC_PutChar(gc,0,0,0,0,id);
  ImageID_Free(id);
  if(width) *width = Width;
  if(height) *height = Height;

  return gc;
}

int FreeResource(GC* gc)
{
  if(gc) GC_FreeGC(gc);
  gc = 0;
  return 0;
}

void LoadInternalResources()
{
  CELL_SIZE  = _CELL_SIZE_;
  TOP_HEIGHT = _TOP_HEIGHT_;
  NUMBER_W   = _NUMBER_W_;
  NUMBER_H   = _NUMBER_H_;
  POINTS_W   = _POINTS_W_;

  bmp_0 = LoadInternalResource(IDB_0, IDB_0_len);
  bmp_prestand = LoadInternalResource(IDB_PRESTAND, IDB_PRESTAND_len);

  bmp_appear[0] = LoadInternalResource(IDB_APPEAR_1, IDB_APPEAR_1_len);
  bmp_appear[1] = LoadInternalResource(IDB_APPEAR_2, IDB_APPEAR_2_len);
  bmp_appear[2] = LoadInternalResource(IDB_APPEAR_3, IDB_APPEAR_3_len);
  bmp_appear[3] = LoadInternalResource(IDB_APPEAR_4, IDB_APPEAR_4_len);

  bmp_stand = LoadInternalResource(IDB_STAND, IDB_STAND_len);

  bmp_jump[0] = LoadInternalResource(IDB_JUMP_3, IDB_JUMP_3_len);
  bmp_jump[1] = LoadInternalResource(IDB_JUMP_2, IDB_JUMP_2_len);
  bmp_jump[2] = LoadInternalResource(IDB_JUMP_1, IDB_JUMP_1_len);
  bmp_jump[3] = LoadInternalResource(IDB_JUMP_2, IDB_JUMP_2_len);
  bmp_jump[4] = LoadInternalResource(IDB_JUMP_3, IDB_JUMP_3_len);
  bmp_jump[5] = LoadInternalResource(IDB_STAND, IDB_STAND_len);
  bmp_jump[6] = LoadInternalResource(IDB_JUMP_4, IDB_JUMP_4_len);
  bmp_jump[7] = LoadInternalResource(IDB_JUMP_5, IDB_JUMP_5_len);
  bmp_jump[8] = LoadInternalResource(IDB_JUMP_6, IDB_JUMP_6_len);
  bmp_jump[9] = LoadInternalResource(IDB_JUMP_5, IDB_JUMP_5_len);
  bmp_jump[10] = LoadInternalResource(IDB_JUMP_4, IDB_JUMP_4_len);
  bmp_jump[11] = LoadInternalResource(IDB_STAND, IDB_STAND_len);

  bmp_explode[0] = LoadInternalResource(IDB_EXPLODE_1, IDB_EXPLODE_1_len);
  bmp_explode[1] = LoadInternalResource(IDB_EXPLODE_2, IDB_EXPLODE_2_len);
  bmp_explode[2] = LoadInternalResource(IDB_EXPLODE_3, IDB_EXPLODE_3_len);
  bmp_explode[3] = LoadInternalResource(IDB_EXPLODE_4, IDB_EXPLODE_4_len);
  bmp_explode[4] = LoadInternalResource(IDB_EXPLODE_5, IDB_EXPLODE_5_len);
  bmp_explode[5] = LoadInternalResource(IDB_EXPLODE_6, IDB_EXPLODE_6_len);
  bmp_explode[6] = LoadInternalResource(IDB_EXPLODE_7, IDB_EXPLODE_7_len);
  bmp_explode[7] = LoadInternalResource(IDB_EXPLODE_8, IDB_EXPLODE_8_len);
  bmp_explode[8] = LoadInternalResource(IDB_EXPLODE_9, IDB_EXPLODE_9_len);

  bmp_numbers = LoadInternalResource(IDB_NUMBERS, IDB_NUMBERS_len);
  bmp_points  = LoadInternalResource(IDB_POINTS, IDB_POINTS_len);

  cur_resource = 0;
}

void LoadExternalResources()
{
  int error = 0;
  int width = 0, height = 0;
  int width2 = 0, height2 = 0;

  bmp_0 = LoadExternalResource(L"0.png", &width, &height);

  if(!bmp_0 || (width != height)) error = 1;
  else
  {
    CELL_SIZE = width - 1;
    height2   = CELL_SIZE + 1;
    width2    = (CELL_SIZE * 7) + 1;
  }

  bmp_prestand = LoadExternalResource(L"prestand.png", &width, &height);
  if(!bmp_prestand || (width != width2) || (height != height2)) error = 1;

  bmp_appear[0] = LoadExternalResource(L"appear_1.png", &width, &height);
  if(!bmp_appear[0] || (width != width2) || (height != height2)) error = 1;
  bmp_appear[1] = LoadExternalResource(L"appear_2.png", &width, &height);
  if(!bmp_appear[1] || (width != width2) || (height != height2)) error = 1;
  bmp_appear[2] = LoadExternalResource(L"appear_3.png", &width, &height);
  if(!bmp_appear[2] || (width != width2) || (height != height2)) error = 1;
  bmp_appear[3] = LoadExternalResource(L"appear_4.png", &width, &height);
  if(!bmp_appear[3] || (width != width2) || (height != height2)) error = 1;

  bmp_stand = LoadExternalResource(L"stand.png", &width, &height);
  if(!bmp_prestand || (width != width2) || (height != height2)) error = 1;

  bmp_jump[0] = LoadExternalResource(L"jump_3.png", &width, &height);
  if(!bmp_jump[0] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[1] = LoadExternalResource(L"jump_2.png", &width, &height);
  if(!bmp_jump[1] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[2] = LoadExternalResource(L"jump_1.png", &width, &height);
  if(!bmp_jump[2] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[3] = LoadExternalResource(L"jump_2.png", &width, &height);
  if(!bmp_jump[3] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[4] = LoadExternalResource(L"jump_3.png", &width, &height);
  if(!bmp_jump[4] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[5] = LoadExternalResource(L"stand.png", &width, &height);
  if(!bmp_jump[5] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[6] = LoadExternalResource(L"jump_4.png", &width, &height);
  if(!bmp_jump[6] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[7] = LoadExternalResource(L"jump_5.png", &width, &height);
  if(!bmp_jump[7] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[8] = LoadExternalResource(L"jump_6.png", &width, &height);
  if(!bmp_jump[8] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[9] = LoadExternalResource(L"jump_5.png", &width, &height);
  if(!bmp_jump[9] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[10] = LoadExternalResource(L"jump_4.png", &width, &height);
  if(!bmp_jump[10] || (width != width2) || (height != height2)) error = 1;
  bmp_jump[11] = LoadExternalResource(L"stand.png", &width, &height);
  if(!bmp_jump[11] || (width != width2) || (height != height2)) error = 1;

  bmp_explode[0] = LoadExternalResource(L"explode_1.png", &width, &height);
  if(!bmp_explode[0] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[1] = LoadExternalResource(L"explode_2.png", &width, &height);
  if(!bmp_explode[1] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[2] = LoadExternalResource(L"explode_3.png", &width, &height);
  if(!bmp_explode[2] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[3] = LoadExternalResource(L"explode_4.png", &width, &height);
  if(!bmp_explode[3] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[4] = LoadExternalResource(L"explode_5.png", &width, &height);
  if(!bmp_explode[4] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[5] = LoadExternalResource(L"explode_6.png", &width, &height);
  if(!bmp_explode[5] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[6] = LoadExternalResource(L"explode_7.png", &width, &height);
  if(!bmp_explode[6] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[7] = LoadExternalResource(L"explode_8.png", &width, &height);
  if(!bmp_explode[7] || (width != width2) || (height != height2)) error = 1;
  bmp_explode[8] = LoadExternalResource(L"explode_9.png", &width, &height);
  if(!bmp_explode[8] || (width != width2) || (height != height2)) error = 1;

  bmp_numbers = LoadExternalResource(L"numbers.png", &width, &height);
  if(!bmp_numbers) error = 1;
  else
  {
    NUMBER_H = height - 1;
    NUMBER_W = ((width - 1)/10) + 1;
    if((((NUMBER_W - 1)*10)+1) != width) error = 1;
  }
  bmp_points  = LoadExternalResource(L"points.png", &width, &height);
  if(!bmp_points || (height != (NUMBER_H + 1))) error = 1;
  else
  {
    POINTS_W = width - 1;
  }

  TOP_HEIGHT = NUMBER_H + 10;

  cur_resource = 1;

  if(error)
  {
    FreeResources();
    LoadInternalResources();
  }
}

void LoadResources()
{
  bmp[0] = &bmp_0;
  bmp[1] = &bmp_prestand;
  bmp[2] = &bmp_appear[0];
  bmp[3] = &bmp_stand;
  bmp[4] = &bmp_jump[0];
  bmp[5] = &bmp_explode[0];

  if(resource == 1)
    LoadExternalResources();
  else
    LoadInternalResources();

  InitData();

}

void FreeResources()
{
  FreeResource(bmp_0);
  FreeResource(bmp_prestand);

  FreeResource(bmp_appear[0]);
  FreeResource(bmp_appear[1]);
  FreeResource(bmp_appear[2]);
  FreeResource(bmp_appear[3]);

  FreeResource(bmp_stand);

  FreeResource(bmp_jump[0]);
  FreeResource(bmp_jump[1]);
  FreeResource(bmp_jump[2]);
  FreeResource(bmp_jump[3]);
  FreeResource(bmp_jump[4]);
  FreeResource(bmp_jump[5]);
  FreeResource(bmp_jump[6]);
  FreeResource(bmp_jump[7]);
  FreeResource(bmp_jump[8]);
  FreeResource(bmp_jump[9]);
  FreeResource(bmp_jump[10]);
  FreeResource(bmp_jump[11]);

  FreeResource(bmp_explode[0]);
  FreeResource(bmp_explode[1]);
  FreeResource(bmp_explode[2]);
  FreeResource(bmp_explode[3]);
  FreeResource(bmp_explode[4]);
  FreeResource(bmp_explode[5]);
  FreeResource(bmp_explode[6]);
  FreeResource(bmp_explode[7]);
  FreeResource(bmp_explode[8]);

  FreeResource(bmp_numbers);
  FreeResource(bmp_points);
}

void onTimerGametime(u16 timerID, LPARAM lparam)
{
  if(gametime_enable)
  {
    gametime++;
    DrawTime();
    RedrawGUI = 1;
  }
  Timer_ReSet(&timerGametime,1000,onTimerGametime,0);
}

void onTimerGamestate(u16 timerID, LPARAM lparam)
{
  ALines(1);
  RedrawGUI = 1;
  Timer_ReSet(&timerGamestate,(int)lparam,onTimerGamestate,lparam);
}

//Автомат "Управление игрой"
void ALines(int e)
{
  int y_old = y_lines;

  switch(y_lines)
  {
  //Поиск игрового шара
  case 0:
    if (e==0 && xk1() ) { z0(); y_lines=1;}
    break;
  //Поиск куда послать игровой шар
  case 1:
//    if (e==0 && xk0() && x0() ) { z1_2(); z1_1(); y_lines=2;}
    if (e==0 && xk0())
    {
      if ( x0()) { play_cantmove = 0; z1_2(); z1_1(); y_lines=2;}
      else { play_cantmove = 1; play_cantmove_timeout = 5; }
    }
    else
    if (e==0 && xk1() ) { z1_2(); z0(); z1_3();}
    else
    if (e==0 && xk2() ) { z1_2(); y_lines=0;}
    else
    if (e==1) { z1_3(); }
    break;
  //Передвижение игрового шара
  case 2:
    if  (e==1 && x1() && x2() ) { z2_2(); z2_3(); y_lines=3;}
    else if (e==1 && x1() ) { z2_2(); z2_4(); y_lines=4;}
    else if (e==1) { z2_1(); }
    break;
  //Удаление линии
  case 3:
    if (e==1 && x3() ) { z3_2(); y_lines=0;}
    else if (e==1) { z3_1(); }
    break;
  //Появление новых шаров
  case 4:
    if (e==1 && x4() && x5() ) { z4_2(); z2_3(); y_lines=3;}
    else if (e==1 && x4() ) { z4_2(); y_lines=0;}
    else if (e==1) { z4_1(); }
    break;
  //перед началом продолжения игры
  case 5:
    if (x6()) { z2_3(); y_lines = 3;}
    else { y_lines=0;}
    break;
  }

  #ifdef LOGGING
    if (y_lines!=y_old)
    {
      char log[512];
      sprintf(log," The automaton of game from state \"%s\" goto in state \"%s\".\n",aLines_states[y_old],aLines_states[y_lines]);
      WriteLog(log);
    }
  #endif

  if (y_old!=y_lines)
  switch(y_lines)
  {
  case 1:
    z1_3();
    break;
  case 2:
    z2_1();
    break;
  case 3:
    z3_1();
    break;
  case 4:
    z4_1();
    break;
  }
}

//ВХОДНЫЕ ПЕРЕМЕННЫЕ
//Выбранная ячейка пуста
bool xk0()
{
  return (click_ball.State()==0 || click_ball.State()==1);
}
//В выбранной ячейке находиться шар
bool xk1()
{
  return (click_ball.State()==3);
}
//В выбранной ячейке находиться прыгающий шар
bool xk2()
{
  return (click_ball.State()==4);
}
//Существует путь от активной ячейки до выбранной
bool x0()
{
  return FindPath(ball,click_ball);
}
//Передвижение закончилось
bool x1()
{
  return path->empty();
}
//Требуется удалить линии (после передвижения)
bool x2()
{
  return CheckLines(ball);
}
//Удаление закончилось
bool x3()
{
  aux::list<cell>::iterator itr = explode_list->begin();
  return (itr->State()==0);//N3

}
//Появление закончилось
bool x4()
{
  aux::list<cell>::iterator itr = appear_list->begin();
  return (itr->State()==3);//N1
}
//Требуется удалить линии (после появления шаров)
bool x5()
{
  aux::list<cell>::iterator itr = appear_list->begin();
  while (itr!=appear_list->end())
    CheckLines(*itr++);

  return (explode_list->size()!=0);
}
//Требуется удалить линии (перед началом продолжения игры)
bool x6()
{
  cell tmp_ball;
  for(int i=0;i<max_x;i++)
  for(int j=0;j<max_y;j++)
  {
    tmp_ball.posx = i;
    tmp_ball.posy = j;
    CheckLines(tmp_ball);
  }
  return (explode_list->size()!=0);
}

// ВЫХОДНЫЕ ВОЗДЕЙСТВИЯ
//Запустить прыжок
void z0()
{
  PlayJump();
  Timer_ReSet(&timerGamestate,50,onTimerGamestate,(LPARAM)50);
  ball=click_ball;
}
//Запустить передвижение
void z1_1()
{
  Timer_ReSet(&timerGamestate,50,onTimerGamestate,(LPARAM)50);
  ball_color=ball.Color();
}
//Закончить прыжок
void z1_2()
{
  if(timerGamestate) Timer_Kill(&timerGamestate);
  timerGamestate = 0;
  ball.ACell(6);
}
//Прыгать
void z1_3()
{
  if(!play_cantmove)
  {
    if(play_cantmove_timeout)
    {
      play_cantmove_timeout--;
    }
    else
    {
      if(ball.NumPic() == 9) PlayJump();
    }
  }
  else
  {
    play_cantmove = 0;
    PlayCantmove();
  }
  ball.ACell(5);
}
//Передвинуть шар на следующую ячейку
void z2_1()
{
  PlayMove();
  ball.ACell(0);
  ball=path->front();
  path->pop_front();
  ball.ACell(1);
}
//Закончить передвижение
void z2_2()
{
  if(timerGamestate) Timer_Kill(&timerGamestate);
  timerGamestate = 0;
}
//Запустить  удаление
void z2_3()
{
  PlayDestroy();
  Timer_ReSet(&timerGamestate,20,onTimerGamestate,(LPARAM)20);
}
//Запустить появление
void z2_4()
{
  CheckAppearList();
  Timer_ReSet(&timerGamestate,50,onTimerGamestate,(LPARAM)50);
}
//Удалить
void z3_1()
{
  aux::list<cell>::iterator itr = explode_list->begin();
  while (itr!=explode_list->end())
    (*itr++).ACell(4);
}
//Закончить удаление
void z3_2()
{
  if(timerGamestate) Timer_Kill(&timerGamestate);
  timerGamestate = 0;

  gamescore += (explode_list->size() - del_balls + 1) * explode_list->size();
  DrawScore();
  RedrawGUI = 1;

  explode_list->clear();
}
//Появление
void z4_1()
{
  aux::list<cell>::iterator itr=appear_list->begin();
  while (itr!=appear_list->end())
    (*itr++).ACell(3);
}
//Закончить появление
void z4_2()
{
  if(timerGamestate) Timer_Kill(&timerGamestate);
  timerGamestate = 0;
  GenerateAppearList();
}

bool is_added_to_explode_list(const cell &in)
{
  aux::list<cell>::iterator itr = explode_list->begin();
  while (itr!=explode_list->end())
  {
    cell l = *itr++;
    if((l.posx == in.posx) && (l.posy == in.posy)) return true;
  }
  return false;
}

//Проверка что через ячейку in проходит подлещащая удаления линия(или несколько линии)
bool CheckLines(const cell &in)
{
  int x=in.posx;
  int y=in.posy;
  int c=in.Color();
  int i,j;
  cell l;
  bool b=false;

  if(in.State() == 0) return false;

  i=1;while ((x+i<max_x)&&(map[x+i][y].y == 3)&&(map[x+i][y].color == c)) ++i;
  j=1;while((x-j>=0)&&(map[x-j][y].y == 3)&&(map[x-j][y].color == c)) ++j;
  if (j+i-1>=del_balls)
  {
    l.posx=x+i;
    l.posy=y;
    for(int k=0;k<i+j-1;k++)
    {
      l.posx--;
      if(!is_added_to_explode_list(l)) explode_list->push_back(l);
    }
    b=true;
  }

  i=1;while ((y+i<max_y)&&(map[x][y+i].y == 3)&&(map[x][y+i].color == c)) ++i;
  j=1;while((y-j>=0)&&(map[x][y-j].y == 3)&&(map[x][y-j].color == c)) ++j;
  if (j+i-1>=del_balls)
  {
    l.posx=x;
    l.posy=y+i;
    for(int k=0;k<i+j-1;k++)
    {
      l.posy--;
      if(!is_added_to_explode_list(l)) explode_list->push_back(l);
    }
    b=true;
  }

  i=1;while ((x+i<max_x)&&(y+i<max_y)&&(map[x+i][y+i].y == 3)&&(map[x+i][y+i].color == c)) ++i;
  j=1;while((x-j>=0)&&(y-j>=0)&&(map[x-j][y-j].y == 3)&&(map[x-j][y-j].color == c)) ++j;
  if (j+i-1>=del_balls)
  {
    l.posx=x+i;
    l.posy=y+i;
    for(int k=0;k<i+j-1;k++)
    {
      l.posx--;
      l.posy--;
      if(!is_added_to_explode_list(l)) explode_list->push_back(l);
    }
    b=true;
  }

  i=1;while ((x+i<max_x)&&(y-i>=0)&&(map[x+i][y-i].y == 3)&&(map[x+i][y-i].color == c)) ++i;
  j=1;while((x-j>=0)&&(y+j<max_y)&&(map[x-j][y+j].y == 3)&&(map[x-j][y+j].color == c)) ++j;
  if (j+i-1>=del_balls)
  {
    l.posx=x+i;
    l.posy=y-i;
    for(int k=0;k<i+j-1;k++)
    {
      l.posx--;
      l.posy++;
      if(!is_added_to_explode_list(l)) explode_list->push_back(l);
    }
    b=true;
  }

  if (b)
  {
    explode_list->remove(in);
    explode_list->push_back(in);
  }

  return b;
}

//Поиск пути следования шарика из ячейки from в in
bool FindPath(const cell &from, const cell &in)
{
  struct
  {
    cell pred;
    int mark;
  }
  v[MAX_MAP_X][MAX_MAP_Y];

  cell k,l;

  aux::list<cell> q;

  for (int i=0;i<max_x;i++)
  for(int j=0;j<max_y;j++)
    v[i][j].mark = 0;

  v[from.posx][from.posy].mark=1;
  q.push_back(from);

  while (!q.empty())
  {
    k=q.front();
    for (int i=0;i<4;i++)
    {
      l=k;
      switch(i)
      {
      case 0:
        l.posx--;
        break;
      case 1:
        l.posx++;
        break;
      case 2:
        l.posy--;
        break;
      case 3:
        l.posy++;
        break;
      }
      if (Valid(l) && !v[l.posx][l.posy].mark)
      {
        v[l.posx][l.posy].mark=1;
        v[l.posx][l.posy].pred=k;
        q.push_back(l);

        if (l==in)
        {
          do
          {
            path->push_front(l);
            l = v[l.posx][l.posy].pred;

          } while (l!=from);
          return true;
        }
      }
    }

    q.pop_front();

  }
  return false;
}

// Проверить список появляющихся шаров на предмент занятости соответствующей ячейки
void CheckAppearList()
{
  int tmp;
  aux::list<cell>::iterator itr=appear_list->begin();
  while (itr!=appear_list->end())
  {
    if (itr->State()==3)
    {
      tmp=itr->PreColor();
      itr->PreColor()=-1;
      FindEmptyCell(*itr);
      itr->PreColor()=tmp;
      (*itr).ACell(2);
    }
    ++itr;
  }
}

//Создать список появляющихся шаров
void GenerateAppearList()
{
  appear_list->clear();
  cell l;
  for(int i=0;i<app_balls;i++)
    if (FindEmptyCell(l))
    {
      appear_list->push_back(l);
      l.ACell(2);
    }
    else return;
}

//Найти пустую ячейку
bool FindEmptyCell(cell &in)
{
  cell l;
  l.posx=random(max_x);
  l.posy=random(max_y);

  if (l.State()==0)
  {
    in=l;
    return true;
  };

  for (int i=0;i<max_x*max_y;i++)
  {
    if (l.posx!=max_x-1) l.posx++;
    else if (l.posy!=max_y-1) {l.posy++;l.posx=0;}
      else {l.posx=0;l.posy=0;};

    if (l.State()==0)
    {
      in=l;
      return true;
    }
  }

  GameOver();
  return false;
}

//Проверить можно ли через ячейку in двигатся шару
bool Valid(const cell &in)
{
  return (in.posx >= 0) && (in.posx < max_x) && (in.posy >= 0) && (in.posy < max_y) && (in.State() == 0 || in.State() == 1);
}

void CleanGame()
{
    for(int i=0;i<max_x;i++)
    for(int j=0;j<max_y;j++)
    {
      if(map[i][j].y == 1)
      {
        map[i][j].y = 0;
        map[i][j].color = 0;
      }
      if(map[i][j].y > 0) map[i][j].y = 3;
      map[i][j].pre_color = -1;
      map[i][j].num_pic = 0;
    }
    y_lines = 5;
    ALines(1);
}

//Подготовка ресурсов для новой игры
void NewGame()
{
  gametime_enable = 0;

  if(gamestate != 1)
  {
    #ifdef LOGGING
      WriteLog("New game\n");
    #endif

    for (int i=0;i<max_x;i++)
    for(int j=0;j<max_y;j++)
    {
      map[i][j].y=0;
      map[i][j].color=0;
      map[i][j].pre_color=-1;
      map[i][j].num_pic=0;
    }

    y_lines =0;

    cell l;

    for(int i=0;i<del_balls;i++)
    {
      FindEmptyCell(l);
      ball_color=random(7);
      l.ACell(1);
    }

    cursor_x = 0;
    cursor_y = 0;

    gamescore = 0;
    gametime = 0;
  }
  else
  {
    #ifdef LOGGING
      WriteLog("Continue game\n");
    #endif
    CleanGame();
  }

  GenerateAppearList();

  Timer_ReSet(&timerGametime,1000,onTimerGametime,0);
}

//Обработка окончания игрового процесса
void GameOver()
{
  #ifdef LOGGING
    WriteLog("Game over\n");
  #endif

  if(timerGametime) Timer_Kill(&timerGametime);
  timerGametime = 0;

  if(timerGamestate) Timer_Kill(&timerGamestate);
  timerGamestate = 0;

  SetEndGameState();

  if (gametype<3)
  {
    UpdateBestResult();
  }
  //else MessageBox(hWnd,"Your custom game is over...", "Condolences",MB_OK);

  NewGame();
  DrawTop();
  cell l;
  for (l.posx=0;l.posx<max_x;l.posx++)
  for(l.posy=0;l.posy<max_y;l.posy++)
    l.DrawState();
  RedrawGUI = 1;

  //  BookObj_ReturnPage(FindBook(isLinesBook),NIL_EVENT);
}

//Отрисовать на табло продолжительность игры
void DrawTime()
{
  CANVAS_Get_GviGC( bmp_numbers->pcanvas ,&gvigcobj);
  int h=gametime;
  int s2=h%60; h/=60;
  int s1=s2%10; s2/=10;
  int m2=h%60; h/=60;
  int m1=m2%10; m2/=10;

  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W1,TOP_HEIGHT_NUMBER_H,NUMBER_W,NUMBER_H_1,gvigcobj,s1*NUMBER_W_1,0,204,0,0,0);
  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W2,TOP_HEIGHT_NUMBER_H,NUMBER_W,NUMBER_H_1,gvigcobj,s2*NUMBER_W_1,0,204,0,0,0);
  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W3,TOP_HEIGHT_NUMBER_H,NUMBER_W,NUMBER_H_1,gvigcobj,m1*NUMBER_W_1,0,204,0,0,0);
  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W4,TOP_HEIGHT_NUMBER_H,NUMBER_W,NUMBER_H_1,gvigcobj,m2*NUMBER_W_1,0,204,0,0,0);
  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W5,TOP_HEIGHT_NUMBER_H,NUMBER_W,NUMBER_H_1,gvigcobj,h*NUMBER_W_1,0,204,0,0,0);
  CANVAS_Get_GviGC( bmp_points->pcanvas ,&gvigcobj);
  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W6, TOP_HEIGHT_NUMBER_H,POINTS_W_1,NUMBER_H_1,gvigcobj,0,0,204,0,0,0);
  GVI_BitBlt(gvigcmem,max_x*CELL_SIZE-W7, TOP_HEIGHT_NUMBER_H,POINTS_W_1,NUMBER_H_1,gvigcobj,0,0,204,0,0,0);
}
//Отрисовать на табло текущие очки
void DrawScore()
{
  CANVAS_Get_GviGC( bmp_numbers->pcanvas ,&gvigcobj);
  int t=gamescore;
  for (int i=0; i<5; i++)
  {
    GVI_BitBlt(gvigcmem,NUMBER_W_2 - NUMBER_W_3*i,TOP_HEIGHT_NUMBER_H, NUMBER_W, NUMBER_H, gvigcobj, (t%10)*NUMBER_W_1, 0, 204, 0, 0, 0);
    t /=10;
  }
}
//Отрисовать табло
void DrawTop()
{
  DrawScore();
  DrawTime();
}

TEXTID BestResults()
{
  wchar_t res[256] = {0,};
  wchar_t val[100];
  int i, h ,m1,m2,s1,s2;

  for(int i=0;i<BEST_RESULTTS_NUM;i++)
  {
    int h=leaders[i].time;
    int hour = h/60/60;
    int min  = (h - hour * 60 * 60)/60;
    int sec  = h - hour * 60 * 60 - min * 60;
    snwprintf(val, MAXELEMS(val), L"%ls - %d %d:%02d:%02d\n", leaders[i].name, leaders[i].score, hour, min, sec);
    wstrcat(res,val);
  }
  return TextID_Create(res,ENC_UCS2,TEXTID_ANY_LEN);
}

void UpdateBestResult()
{
  int indx = -1;
  for(int i=0;i<BEST_RESULTTS_NUM;i++)
  {
    if (leaders[i].score<gamescore || (leaders[i].score==gamescore && leaders[i].time>gametime))
    {
      indx = i;
      break;
    }
  }
  if(indx != -1)
  {
    info old = leaders[indx];
    leaders[indx].score=gamescore;
    leaders[indx].time=gametime;
    for(int i=indx+1;i<BEST_RESULTTS_NUM;i++)
    {
      leaders[i] = old;
      old = leaders[i+1];
    }
    GetNewName(leaders[indx].name);
  }
}

//Проверить заданные параметры игры
void CheckCustomParameters()
{
  if (max_x < MIN_MAP_X) max_x = MIN_MAP_X;
  if (max_x > MAX_MAP_X) max_x = MAX_MAP_X;
  if (max_y < MIN_MAP_Y) max_y = MIN_MAP_Y;
  if (max_y > MAX_MAP_Y) max_y = MAX_MAP_Y;

  if (del_balls < MIN_DEL_BALLS) del_balls = MIN_DEL_BALLS;

  if (del_balls>(max_x>max_y?max_x:max_y)) del_balls = (max_x>max_y?max_x:max_y);

  if (app_balls+del_balls > max_x*max_y)
  {
    app_balls=max_x*max_y-del_balls;
  }
}

//Получение информации о лучших игроках
void GetInfo()
{
  FSTAT _fstat;
  int in;
  MyBOOK *bk = (MyBOOK *)FindBook(isLinesBook);
  wchar_t *path = bk->path;
  wchar_t *name = L"leaders.dat";

  if (fstat(path,name,&_fstat) != -1)
  {
    if ((in=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      for (int i=0;i<BEST_RESULTTS_NUM;i++)
        fread(in,&leaders[i],sizeof(info));
      fclose(in);
      }
  }
  else
  {
    for (int i=0;i<BEST_RESULTTS_NUM;i++)
      leaders[i]=null_leader;
  }
}

//Сохранить информацию о лучших игроках
void WriteInfo()
{
  int out;
  MyBOOK *bk = (MyBOOK *)FindBook(isLinesBook);
  wchar_t *path = bk->path;
  wchar_t *name = L"leaders.dat";

  if((out = _fopen(path,name,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    for (int i=0;i<BEST_RESULTTS_NUM;i++)
      fwrite(out,&leaders[i],sizeof(info));
    fclose(out);
  }
}

//Получение информации о игре
void GetGame()
{
  FSTAT _fstat;
  int in;
  MyBOOK *bk = (MyBOOK *)FindBook(isLinesBook);
  wchar_t *path = bk->path;
  wchar_t *name = L"lines.sav";

  if (fstat(path,name,&_fstat) != -1)
  {
    if ((in=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      fread(in,&cursor_x,sizeof(int));
      fread(in,&cursor_y,sizeof(int));
      fread(in,&gamestate,sizeof(int));
      fread(in,&gametype,sizeof(int));
      fread(in,&max_x,sizeof(int));
      fread(in,&max_y,sizeof(int));
      fread(in,&gamescore,sizeof(int));
      fread(in,&gametime,sizeof(int));
      fread(in,&app_balls,sizeof(int));
      fread(in,&del_balls,sizeof(int));
      fread(in,&map[0][0],sizeof(map_t)*MAX_MAP_X*MAX_MAP_Y);
      fclose(in);
    }
  }
  else
  {
    cursor_x = 0;
    cursor_y = 0;
    gamestate = 0;
    gametype = 1;
    max_x = 9;
    max_y = 9;
    gamescore = 0;
    gametime = 0;
    app_balls = 3;
    del_balls = 5;
  }

  cursor = bk->cursor;
}

//Сохранить информацию о игре
void WriteGame()
{
  int out;
  MyBOOK *bk = (MyBOOK *)FindBook(isLinesBook);
  wchar_t *path = bk->path;
  wchar_t *name = L"lines.sav";

  if((out = _fopen(path,name,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    fwrite(out,&cursor_x,sizeof(int));
    fwrite(out,&cursor_y,sizeof(int));
    fwrite(out,&gamestate,sizeof(int));
    fwrite(out,&gametype,sizeof(int));
    fwrite(out,&max_x,sizeof(int));
    fwrite(out,&max_y,sizeof(int));
    fwrite(out,&gamescore,sizeof(int));
    fwrite(out,&gametime,sizeof(int));
    fwrite(out,&app_balls,sizeof(int));
    fwrite(out,&del_balls,sizeof(int));
    fwrite(out,&map[0][0],sizeof(map_t)*MAX_MAP_X*MAX_MAP_Y);
    fclose(out);
  }
}

void ShowGame(void *gc, RECT *rc)
{
  DrawRect(rc->x1,rc->y1,rc->x2,rc->y2,clBlack,clBlack);
//  DrawRect(rc->x1,rc->y1,rc->x2,rc->y2,clRed,clRed);

  //получаем GVI_ контексты из GC_
  GVI_GC gvigcdisp;
  CANVAS_Get_GviGC( ((GC*)gc)->pcanvas,&gvigcdisp);

  //выводим кусок контекста памяти с масштабированием в экранный контекст
  GVI_StretchBlt(gvigcdisp, 0, 0, scr_w, game_h, gvigcmem, 0, 0, mgc_w, mgc_h, 204, 0, 0, 0);

  //рисуем символ в памяти
  GC_PutChar((GC*)gc,cursor_x*cell_size+(cell_size >> 1), top_height+cursor_y*cell_size+(cell_size >> 1),0,0,cursor);

  int oldfont = SetFont(FONT_E_14R);
  DrawString(menuId,1,0,scr_h-menu_h-3,scr_w,scr_h,0,1,clWhite,0xFF000000);
  SetFont(oldfont);
}

void InitGame()
{
  cell l;

  InitScreen();

  mgc_w  = CELL_SIZE * max_x + 1;
  mgc_h  = CELL_SIZE * max_y + TOP_HEIGHT + 2;
  cell_size = (scr_w / max_x);
  scr_w = cell_size * max_x;
  game_h = scr_w * mgc_h / mgc_w;
  top_height = scr_w * TOP_HEIGHT / mgc_w;

  //создаём контекст памяти
  hMGC = GC_CreateMemoryGC(mgc_w, mgc_h, 16, 0, 0, 0);
  //делаем заливку в памяти
  GC_DrawFRect (hMGC, clBlack, 0, 0, mgc_w, mgc_h);
  //получаем GVI_ контекст из GC_
  CANVAS_Get_GviGC( hMGC->pcanvas ,&gvigcmem);

  appear_list  = new aux::list<cell>;
  explode_list = new aux::list<cell>;
  path         = new aux::list<cell>;

  menuId = Language ? STR("Меню") : STR("Меню");
  int oldfont = SetFont(FONT_E_14R);
  menu_h = GetImageHeight(' ');
  SetFont(oldfont);

  NewGame();
  DrawTop();
  for (l.posx=0;l.posx<max_x;l.posx++)
  for(l.posy=0;l.posy<max_y;l.posy++)
    l.DrawState();

  Timer_ReSet(&timerGametime,1000,onTimerGametime,0);

  //SetContinueGameState();
}

void FreeGame()
{
  if(timerGametime) Timer_Kill(&timerGametime);
  timerGametime = 0;

  if(timerGamestate) Timer_Kill(&timerGamestate);
  timerGamestate = 0;

  if(hMGC) GC_FreeGC(hMGC);
  hMGC = 0;

  if(menuId != 0x6FFFFFFF) TextID_Destroy(menuId);
  menuId = 0x6FFFFFFF;

  if(appear_list) delete appear_list;
  if(explode_list) delete explode_list;
  if(path) delete path;
  appear_list = explode_list = path = 0;
}

void UpdateCursor(int dx, int dy)
{
  gametime_enable = 1;
  gamestatus = 1;

  cursor_x += dx;
  cursor_y += dy;

  if(cursor_x < 0) cursor_x = max_x - 1;
  if(cursor_x >= max_x) cursor_x = 0;

  if(cursor_y < 0) cursor_y = max_y - 1;
  if(cursor_y >= max_y) cursor_y = 0;
}

void onClickCursor()
{
  gametime_enable = 1;
  gamestatus = 1;
  SetContinueGameState();
  click_ball.posx = cursor_x;
  click_ball.posy = cursor_y;
  ALines(0);
}

void onEndGame()
{
//  GameOver();
}

