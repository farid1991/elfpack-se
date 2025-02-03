#ifndef _CURRENT_TRACK_H_
	#define _CURRENT_TRACK_H_

#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"

  typedef struct
  {
    char dummy[0x24];
    int hour;
    int min;
    int sec;
  } PLAY_TIME;

  typedef struct
  {
    wchar_t *path;
    wchar_t *name;
    int fulltime;
	int *time;
    int pos;
    int tracks_count;
  } TRACK_DESC;

  typedef struct
  {
    u16 unk;
    u16 unk2;
    wchar_t * path;
    wchar_t * name;
    wchar_t * MIME;
    int fulltime;
  } BOOK_PLAYER_TRACK_DESC;

  typedef struct
  {
	char dummy[0x19];
	char PlayStatus;
    char dummy0[0x42];
    wchar_t pos;
    char dummy1[0xE];
    int time;
    char dummy2[0x4];
    BOOK_PLAYER_TRACK_DESC * dsc;
    char dummy3[2];
    wchar_t pos2;
    wchar_t pos3;
    char dummy4[22];
    int tracks_count;
  }BOOK_PLAYER_2020;

  void TrackDesc_Free(TRACK_DESC *track_desc);
  bool TrackDesc_Compare(TRACK_DESC *t1, TRACK_DESC *t2);
  TRACK_DESC * TrackDesc_Get(BOOK *bk);
#endif
