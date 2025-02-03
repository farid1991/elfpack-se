#ifndef _MAIN_H_
  #define _MAIN_H_

#include "phone.h"

#define STATUS_ICN_ROW_MAX    64
#define STATUS_ICN_COLUMN_MAX 64

#define APP_ICN_ROW_MAX    64
#define APP_ICN_COLUMN_MAX 64

#define STATUS_HEIGHT_128  14
#define STATUS_HEIGHT_160  14
#define STATUS_HEIGHT_208  18
#define STATUS_HEIGHT_220  18
#define STATUS_HEIGHT_320  25

#define SOFT_HEIGHT_128  18
#define SOFT_HEIGHT_160  18
#define SOFT_HEIGHT_208  26
#define SOFT_HEIGHT_220  26
#define SOFT_HEIGHT_320  30

enum location_t {
    NONE    = 0,
    STATUS  = 1,
    SCREEN  = 2,
    SOFT    = 3
};

enum align_t {
    LEFT = 0,
    RIGHT,
    CENTER
};

enum day_format_t {
   AS_IS = 0,
   UPPER,
   LOWER
};

extern wchar_t tmppath[256];
extern wchar_t tmpname[256];

extern int DisplayWidth;  //ширина дисплея в пикселях
extern int DisplayHeight; //высота дисплея в пикселях

extern int isA2;
extern int ChipID;
extern phone_t phoneType;

extern bool LANG;
extern bool SleepModeIsDeactivated;
extern bool IsInStandby;

int isBookmanDaemonBook(BOOK * book);
int isTimerBook(BOOK * book);
int isStopWatchBook(BOOK * book);
int isJavaBook(BOOK *bk);
void InvalidateAll(void);
GUI *GetStandbyBookGui(void);
wchar_t *convert_first_symbol(wchar_t *str, int format);
void win12512unicode(wchar_t *ws, const char *s, int len);
void myDrawString(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor, int type);
void DrawOwnRect(int _x1, int _y1,int _x2 ,int _y2,int pen_color,int brush_color);

void UpdateSystemShowParams(void);

#endif //_MAIN_H_
