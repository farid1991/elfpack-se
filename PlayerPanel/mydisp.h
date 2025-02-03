#ifndef _MYDISP_H_
	#define _MYDISP_H_

#include "..\include\Lib_Clara.h"
#include "..\\classlib\cl.h"
#include "config_data.h"
#include "CurrentTrack.h"
#include "metadata.h"
#include "decoder.h"
#include "draw.h"
#include "rand.h"

extern wchar_t dir[512];

enum ANIMATION_TYPES
{
	UP,DOWN,LEFT,RIGHT,
	UPLEFT,UPRIGHT,DOWNLEFT,DOWNRIGHT
};

enum DRAW_ELEMS
{
	ARTIST,ALBUM,TITLE,YEAR,GENRE,CHANNEL,BITRATE,FREQUENCY,CURRENT_TIME,FULL_TIME,LEFT_TIME,TRACK_COUNT,CURRENT_TRACK,MYSTR,

	strid_count
};

enum PLAYSTATE{STOP,PLAY,PAUSE,SCROLLRIGHT1,SCROLLRIGHT2,SCROLLLEFT1,SCROLLLEFT2};

enum IMAGES
{
	BACKGROUND,PROGRESSBAR,NOCOVER,

	ImgCount
};

enum MoveTypes{ End, Resume, Overflow};

typedef struct{
	int IsEnabled;
	CFG_HDR cfghdr1;
	RECT Rect;
	CFG_HDR cfghdr2;
	unsigned int NormalColor;
	CFG_HDR cfghdr3;
	unsigned int BorderColor;
	CFG_HDR cfghdr4;
	int Font;
	CFG_HDR cfghdr5;
	int Align;
	CFG_CBOX_ITEM cfgcbox17[3];
	CFG_HDR cfghdr7;
	CFG_HDR cfghdr8;
	CFG_HDR cfghdr9;
}DrawStruct1;

typedef struct{
	int IsEnabled;
	CFG_HDR cfghdr1;
	RECT Rect;
	CFG_HDR cfghdr2;
	wchar_t Mask[64];
	CFG_HDR cfghdr3;
	unsigned int NormalColor;
	CFG_HDR cfghdr4;
	unsigned int BorderColor;
	CFG_HDR cfghdr5;
	int Font;
	CFG_HDR cfghdr6;
	int Align;
	CFG_CBOX_ITEM cfgcbox17[3];
	CFG_HDR cfghdr7;
	CFG_HDR cfghdr8;
	CFG_HDR cfghdr9;
}DrawStruct2;

class CMyDisp:public CDispObjT<CMyDisp>
{
public:
  CMetaData *mdata;
  TRACK_DESC *track_desc;
  BOOK * audiobook;
  bool isDraw;
  int strid[strid_count];
  IMAGEID ImgID[ImgCount];
  wchar_t cover;
  u16 timer;
  int WinX;
  int WinY;
  int StartX;
  int StartY;
  int OldStartX;
  int OldStartY;
  int AnimType;
  int AnimReturn;
  void (*OnDestr)(CMyDisp *cmd);
  static char* getName();
  void onDraw(int a,int b,int c);
  void onKey(int key,int,int repeat,int type);
  void onRefresh();
  int onCreate();
  void onDestroy();
  void TrackInfoInit();
  void TrackInfoDestroy();
  void OnPlay();
  void CreateAnimation(int type,RECT window);
  static void onAnimationTimer( u16 timerID, LPARAM lparam);
  void DestroyAnimation(int type,RECT window,void (*)(CMyDisp*));
  static void onAnimation2Timer( u16 timerID, LPARAM lparam);
  void OnPlayingTime(PLAY_TIME *play_time);
  void DrawElement1(DrawStruct1 DrawElems[],int count);
  void DrawElement2(DrawStruct2 DrawElems[],int count);
};


#endif
