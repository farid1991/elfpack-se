
#ifndef _STRUCTBOOK_H_
  #define _STRUCTBOOK_H_

static const char NameMyElf[]="Tetris";
typedef struct
{
  wchar_t name[10][8];
  int rezultat[10];
}TABL;

typedef struct
{
  TABL*te;
  TABL*tn;
  TABL*th;
}TAB;

typedef struct
{
  wchar_t names[7][128];
  int on;
}IMGN;

typedef struct
{
  wchar_t names[6][256];
  int status[6];
  int sound;
}SOUND;

typedef struct
{
  SOUND*sound;
  IMGN*im;
  TAB*rez;
  int coloron;
  int colors[8];
  int buttons[2][9];
}NASTROIKI;

typedef struct
{
  wchar_t ImageID;
  u16 ImageHandle;
  signed int x;
  signed int y;
  signed int mx;
  signed int my;
  bool isImage;
}IMG;

typedef struct
{
  IMG*im[7];
  int fi[7];
}IMAGES;

typedef struct
{
  int x;
  int y;
  int cub;
  int FP;
  int FS;
}DISPLAY;

typedef struct
{
  DISP_OBJ deob;
  BOOK bk;
  GUI*guido;
  GUI*gui1;
  GUI_LIST*gui;
  IMAGES*image;
  int lvl;
  int speed;
  int point;
  NASTROIKI*nast;
  int pause;
  DISPLAY*dis[3];
  DISP_OBJ_ONKEY_METHOD OldKey1;
  GUI*guigame;
  int megaspeed;
  wchar_t path[128];
  wchar_t name[128];
  int gtimer;
  u16 ftimer;
  int iptimer;
  u16 ptimer;
  RECT*r;
}MyBook;

typedef struct
{
  BOOK * book;
}MSG;



typedef struct
{
  char pole[5][5];
  char newpole[5][5];
  char ID;
  char newID;
  int x;
  int y;
  int col;
  int CubID;
}FIGURA;

typedef struct
{
  char pole[12][24];
  int point;
  char lvl;
  char speed;
  char megaspeed;
  int color[12][24];
  int CubID[12][24];
}GAME;


#endif //_STRUCTBOOK_H_
