#ifndef _MEDIAINFO_H_
  #define _MEDIAINFO_H_

#include "main.h"

extern IMAGEID cover_image;

extern wchar_t cover_file[64];

extern TEXTID nameID;
extern TEXTID albumID;
extern TEXTID artistID;
extern TEXTID yearID;
extern TEXTID genreID;
extern TEXTID frequencyID;
extern TEXTID bitrateID;
extern TEXTID channelsID;

extern int nameWidth;
extern int albumWidth;
extern int artistWidth;
extern int yearWidth;
extern int genreWidth;
extern int frequencyWidth;
extern int bitrateWidth;
extern int channelsWidth;

extern int coverWidth;
extern int coverHeight;

extern int nameX;
extern int albumX;
extern int artistX;
extern int yearX;
extern int genreX;
extern int frequencyX;
extern int bitrateX;
extern int channelsX;

extern bool hastags;
extern bool hasheaders;

extern bool PlayerIsStarted;

int onMediaplayerCreated(void *msg, BOOK* bk);
int onMediaplayerDestroyed(void *msg, BOOK* bk);
int onMediaplayerPlayPressed(void *msg, BOOK* bk);
int onMediaplayerPausePressed(void *msg, BOOK* bk);
int onMediaplayerAudioPlayingTime(void *msg, BOOK* bk);
int onFmRadioModeUpdate(void *msg, BOOK* bk);
int onFmRadioRdsNewData(void *msg, BOOK* bk);
//int onFmRadioRdsState(void *msg, BOOK* bk);
void UpdateRadioInfo(void);
void UpdatePlayerInfo(void);
void UpdatePositions(void);
void ShowMediaInfo(int y, int location);
void ShowMediaImages(int y, int location);
void UpdateMediaInfoParams(void);
void FreeMediaInfoParams(void);

#endif //_MEDIAINFO_H_
