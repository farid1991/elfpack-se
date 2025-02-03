#include "..\include\Lib_Clara.h"
#include "MediaInfo.h"
#include "headerReader.h"
#include "tagReader.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"

#include "Images.h"
#include "TimeDateInfo.h"
#include "HeapInfo.h"
#include "BattTempInfo.h"
#include "MemInfo.h"
#include "OperatorInfo.h"
#include "CustomMsg.h"
#include "NetworkInfo.h"
#include "AlarmInfo.h"
#include "AppInfo.h"
#include "StatusInfo.h"
#include "CallsInfo.h"

enum fm_freq_name_t {
    FM_ALL_OFF = 0,
    FM_FREQ_NAME,
    FM_FREQ_ONLY,
    FM_NAME_ONLY
};

enum scroll_t {
    SWITCH_OFF = 0,
    SWITCH_ON,
    SWITCH_ON_IF_LONG
};

typedef struct
{
  u16 unk;
  u16 FirstElement;
  wchar_t * folder;
  wchar_t * name;
  wchar_t * mime;
  int fulltime;
}track_data_t;

typedef struct
{
  char unk[0x24];
  int hour;
  int min;
  int sec;
}cur_time_t;

RECT player_curbar = {0,0,0,0};

IMAGEID radio_image = NOIMAGE;
IMAGEID player_image = NOIMAGE;
IMAGEID cover_image = NOIMAGE;

wchar_t mp3name[256] = {0,};
wchar_t mp3path[256] = {0,};
int mp3fulltime = 0;
int mp3curtime = 0;
wchar_t cover_file[64] = {0,};

float PrevFrequency = 0;

bool RadioIsStarted  = false;
bool PlayerIsStarted = false;
bool PlayerIsStartedPrev = true;

u16 timer_media = 0;

TEXTID fmID = EMPTY_TEXTID;

TEXTID modeID = EMPTY_TEXTID;
TEXTID rdsstateID = EMPTY_TEXTID;
TEXTID rdsdataID = EMPTY_TEXTID;

TEXTID stereoID = EMPTY_TEXTID;
TEXTID rdsID = EMPTY_TEXTID;

TEXTID nameID = EMPTY_TEXTID;
TEXTID albumID = EMPTY_TEXTID;
TEXTID artistID = EMPTY_TEXTID;
TEXTID yearID = EMPTY_TEXTID;
TEXTID genreID = EMPTY_TEXTID;
TEXTID frequencyID = EMPTY_TEXTID;
TEXTID bitrateID = EMPTY_TEXTID;
TEXTID channelsID = EMPTY_TEXTID;

TEXTID fulltimeID = EMPTY_TEXTID;
TEXTID curtimeID = EMPTY_TEXTID;

int nameWidth = 0;
int albumWidth = 0;
int artistWidth = 0;
int yearWidth = 0;
int genreWidth = 0;
int frequencyWidth = 0;
int bitrateWidth = 0;
int channelsWidth = 0;
int fulltimeWidth = 0;
int curtimeWidth = 0;

int coverWidth = 0;
int coverHeight = 0;

int nameX = 0;
int albumX = 0;
int artistX = 0;
int yearX = 0;
int genreX = 0;
int frequencyX = 0;
int bitrateX = 0;
int channelsX = 0;
int fulltimeX = 0;
int curtimeX = 0;

bool hastags = false;
bool hasheaders = false;

track_data_t *get_track_data()
{
  BOOK *bookPlayer = FindBook(isAudioPlayerBook());
  if(bookPlayer)
  {
    if (ChipID == CHIPID_DB2020)
    {
      return (track_data_t *)(*(int*)((int)bookPlayer + 0x4c + 0x28));
    }
    return (track_data_t *)(*(int*)((int)bookPlayer + 0x4c + 0x20)); //db2010
  }
  return 0;
}

void GetFulltimeWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_fulltime_font);
  fulltimeWidth = Disp_GetTextIDWidth(fulltimeID,TextID_GetLength(fulltimeID));
  SetFont(oldfont);
}

void GetCurtimeWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_curtime_font);
  curtimeWidth = Disp_GetTextIDWidth(curtimeID,TextID_GetLength(curtimeID));
  SetFont(oldfont);
}

void ResetMP3Data()
{
  PlayerIsStarted = false;
  mp3path[0] = 0;
  mp3name[0] = 0;
  mp3fulltime = 0;
}

int onMediaplayerCreated(void *msg, BOOK* bk)
{
//  if (FindBook(isAudioPlayerBook()))
//  {
//  }
//  else
//  {
//  }
//  MessageBox(EMPTY_TEXTID,TextID_Create(L"CreatedEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return 0;
}

int onMediaplayerDestroyed(void *msg, BOOK* bk)
{
  ResetMP3Data();
//  MessageBox(EMPTY_TEXTID,TextID_Create(L"DestroyedEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return 0;
}

int onMediaplayerPlayPressed(void *msg, BOOK* bk)
{
  PlayerIsStarted = true;
  UpdatePlayerInfo();
//  MessageBox(EMPTY_TEXTID,TextID_Create(L"PlayPressedEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return 0;
}

int onMediaplayerPausePressed(void *msg, BOOK* bk)
{
  if (FindBook(isAudioPlayerBook()))
  {
    PlayerIsStarted = true;
    UpdatePlayerInfo();
  }
  else
  {
    ResetMP3Data();
  }
//  MessageBox(EMPTY_TEXTID,TextID_Create(L"PausePressedEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return 0;
}

int onMediaplayerAudioPlayingTime(void *msg, BOOK* bk)
{
  if (FindBook(isAudioPlayerBook()))
  {
    float k1,k2,n,m;
    wchar_t buf[32];
    cur_time_t *ct = (cur_time_t *)msg;
    mp3curtime = ((ct->hour * 60 + ct->min) * 60) + ct->sec;

    PlayerIsStarted = true;
    UpdatePlayerInfo();

    if(cfg_player_endtime_show == 0)
    {
      if(ct->hour) { snwprintf(buf,MAXELEMS(buf)-1,L"%02d:%02d:%02d", ct->hour, ct->min, ct->sec); }
      else         { snwprintf(buf,MAXELEMS(buf)-1,L"%02d:%02d", ct->min, ct->sec); }
    }
    else
    {
      int mp3endtime = mp3fulltime - mp3curtime;
      int hour = mp3endtime/60/60;
      int min  = (mp3endtime - hour * 60 * 60)/60;
      int sec  = mp3endtime - hour * 60 * 60 - min * 60;
      if(hour) { snwprintf(buf,MAXELEMS(buf)-1,L"%02d:%02d:%02d", hour, min, sec); }
      else     { snwprintf(buf,MAXELEMS(buf)-1,L"%02d:%02d", min, sec); }
    }
    if(curtimeID != EMPTY_TEXTID) TextID_Destroy(curtimeID);
    curtimeID = TextID_Create(buf,ENC_UCS2,MAXELEMS(buf));
    if(cfg_player_curtime_scroll) GetCurtimeWidth();
    else curtimeWidth = 0;

    n = cfg_player_timebar.x2 - cfg_player_timebar.x1;
    m = cfg_player_timebar.y2 - cfg_player_timebar.y1;
    k1 = m * mp3curtime/mp3fulltime;
    k2 = n * mp3curtime/mp3fulltime;
    if(n < m)
    {
      player_curbar.x1 = cfg_player_timebar.x1;
      player_curbar.y1 = cfg_player_timebar.y2 - k1;
      player_curbar.x2 = cfg_player_timebar.x2;
      player_curbar.y2 = cfg_player_timebar.y2;
    }
    else
    {
      player_curbar.x1 = cfg_player_timebar.x1;
      player_curbar.y1 = cfg_player_timebar.y1;
      player_curbar.x2 = cfg_player_timebar.x1 + k2;
      player_curbar.y2 = cfg_player_timebar.y2;
    }
  }
  else
  {
    ResetMP3Data();
  }
//  MessageBox(EMPTY_TEXTID,TextID_Create(L"AudioPlayingTimeEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return 0;
}

int onFmRadioModeUpdate(void *msg, BOOK* bk)
{
  char mode;
  if(phoneType == W710_R1JC002)
  {
    mode = *((char *)((int)msg+0x28)); // 1-стерео 0 - моно
  }
  else
  {
    mode = *((char *)((int)msg+0x24)); // 1-стерео 0 - моно
  }
  if(mode) modeID = stereoID;
  else modeID = EMPTY_TEXTID;
  return 0;
}

int onFmRadioRdsNewData(void *msg, BOOK* bk)
{
  if(rdsdataID != EMPTY_TEXTID) { TextID_Destroy(rdsdataID); }
  rdsdataID = EMPTY_TEXTID;
  BOOK *bookFM = FindBook(isFmRadioBook());
  if(bookFM)
  {
    wchar_t *sdr;
    if(phoneType == W710_R1JC002)
    {
      sdr = (wchar_t *)((int)bookFM+0x32); // текст SDR
    }
    else
    {
      sdr = (wchar_t *)((int)bookFM+0x2E); // текст SDR
    }
    rdsdataID = TextID_Create(sdr,ENC_UCS2,TEXTID_ANY_LEN);
  }
  return 0;
}

/*
int onFmRadioRdsState(void *msg, BOOK* bk)
{
  char rds = *((char *)((int)msg+0x24)); // если после смены канала: 1 - RDS
  if(rds) rdsstateID = rdsID;
  else rdsstateID = EMPTY_TEXTID;
  return 0;
}
*/

void UpdateRadioInfo(void)
{
      wchar_t fm[192];
      BOOK *bookFM = FindBook(isFmRadioBook());
      if(bookFM)
      {
        wchar_t buf[128];
        float CurrentFrequency;
        char indx;
        wchar_t *name;
        if(phoneType == W710_R1JC002)
        {
          CurrentFrequency= *(u16*)((int)bookFM+0x1C+0x12);
          indx = *((char *)((int)bookFM+0x2C));
          name = (wchar_t *)(indx * 0x26 + (int)bookFM + 0x24);
        }
        else
        {
          CurrentFrequency= *(u16*)((int)bookFM+0x1C+0xE);
          indx = *((char *)((int)bookFM+0x28));
          name = (wchar_t *)(indx * 0x26 + (int)bookFM + 0x20);
        }

        if(cfg_fm_show == FM_FREQ_NAME)
        {
          if(cfg_fm_name_first)
            snwprintf(buf,MAXELEMS(buf),L"%ls %5.1f MHz", name, CurrentFrequency/10.0);
          else
            snwprintf(buf,MAXELEMS(buf),L"%5.1f MHz %ls", CurrentFrequency/10.0, name);
        }
        else if(cfg_fm_show == FM_FREQ_ONLY)
        {
            snwprintf(buf,MAXELEMS(buf),L"%5.1f MHz", CurrentFrequency/10.0);
        }
        else if(cfg_fm_show == FM_NAME_ONLY)
        {
            snwprintf(buf,MAXELEMS(buf),L"%ls", name);
        }
        else buf[0] = 0;

        wstrcpy(fm,cfg_fm_msg);
        wstrcat(fm,buf);
        if(fmID != EMPTY_TEXTID)  TextID_Destroy(fmID);
        fmID = TextID_Create(fm,ENC_UCS2,MAXELEMS(fm));
        if(CurrentFrequency != PrevFrequency)
        {
          if(rdsdataID != EMPTY_TEXTID) { TextID_Destroy(rdsdataID); }
          rdsdataID = EMPTY_TEXTID;
          rdsstateID = EMPTY_TEXTID;
          PrevFrequency = CurrentFrequency;
        }

        RadioIsStarted = true;
      }
      else
      {
        RadioIsStarted = false;
        PrevFrequency = 0;
      }
}

void UpdatePlayerInfo(void)
{
    if(FindBook(isAudioPlayerBook()))
    {
      track_data_t *trackData = get_track_data();
      if(trackData)
      {
        if(wstrcmp(trackData->folder, mp3path) || wstrcmp(trackData->name, mp3name))
        {
          wstrncpy(mp3path, trackData->folder, MAXELEMS(mp3path));
          wstrncpy(mp3name, trackData->name, MAXELEMS(mp3name));

          mp3fulltime = trackData->fulltime;

          {
            wchar_t buf[32];
            int hour = mp3fulltime/60/60;
            int min  = (mp3fulltime - hour * 60 * 60)/60;
            int sec  = mp3fulltime - hour * 60 * 60 - min * 60;
            if(hour) { snwprintf(buf,MAXELEMS(buf)-1,L"%02d:%02d:%02d", hour, min, sec); }
            else     { snwprintf(buf,MAXELEMS(buf)-1,L"%02d:%02d", min, sec); }
            if(fulltimeID != EMPTY_TEXTID) TextID_Destroy(fulltimeID);
            fulltimeID = TextID_Create(buf,ENC_UCS2,MAXELEMS(buf));
            fulltimeWidth = curtimeWidth = 0;
            fulltimeX = curtimeX = 0;
            if(cfg_player_fulltime_scroll) GetFulltimeWidth();
          }

          // get ID3 info
          if(cfg_id3_use_int_func) tagReaderInt(mp3path, mp3name);
          else tagReader(mp3path, mp3name);

          // get header info
          headerReader(mp3path, mp3name);

          if(!hastags)
          {
            if(nameID != EMPTY_TEXTID) TextID_Destroy(nameID);
            nameID = TextID_Create(mp3name,ENC_UCS2,MAXELEMS(mp3name));
            if(cfg_player_title_scroll) GetNameWidth();
            else nameWidth = 0;
          }
        }
      }
      PlayerIsStarted = true;
    }
    else
    {
      ResetMP3Data();
    }
}

int UpdateX(int x, int width, int align, int scroll, int* cnt, int window)
{
  if((scroll == SWITCH_ON) || ((scroll == SWITCH_ON_IF_LONG) && (width > window)))
  {
    int maxX, initX;
    if(align == RIGHT)
    {
      maxX  = window;
      initX = width;
    }
    else if(align == LEFT)
    {
      maxX  = width;
      initX = window;
    }
    else
    {
      maxX  = initX = (width + window) >> 1;
    }
    if(x == 0)
    {
      *cnt += 1;
      if(*cnt > cfg_player_scroll_delay)
      {
        x += cfg_player_scroll_speed;
        *cnt = 0;
      }
    }
    else
    {
      if(x < maxX)
      {
        int newX = x + cfg_player_scroll_speed;
        if((x < 0) && (newX > 0)) x = 0;
        else x = newX;
      }
      else x = cfg_player_scroll_speed - initX;
    }
    return x;
  }
  return 0;
}

void UpdatePositions(void)
{
  static int cntX1 = 0, cntX2 = 0, cntX3 = 0,  cntX4 = 0;
  static int cntX5 = 0, cntX6 = 0, cntX7 = 0, cntX8 = 0;
  static int cntX9 = 0, cntX10 = 0;

  nameX      = UpdateX(nameX, nameWidth, cfg_player_title_align, cfg_player_title_scroll, &cntX1, cfg_player_title_rc.x2 - cfg_player_title_rc.x1);
  albumX     = UpdateX(albumX, albumWidth, cfg_player_album_align, cfg_player_album_scroll, &cntX2, cfg_player_album_rc.x2 - cfg_player_album_rc.x1);
  artistX    = UpdateX(artistX, artistWidth, cfg_player_artist_align, cfg_player_artist_scroll, &cntX3, cfg_player_artist_rc.x2 - cfg_player_artist_rc.x1);
  yearX      = UpdateX(yearX, yearWidth, cfg_player_year_align, cfg_player_year_scroll, &cntX4, cfg_player_year_rc.x2 - cfg_player_year_rc.x1);
  genreX     = UpdateX(genreX, genreWidth, cfg_player_genre_show, cfg_player_genre_scroll, &cntX5, cfg_player_genre_rc.x2 - cfg_player_genre_rc.x1);
  frequencyX = UpdateX(frequencyX, frequencyWidth, cfg_player_frequency_align, cfg_player_frequency_scroll, &cntX6, cfg_player_frequency_rc.x2 - cfg_player_frequency_rc.x1);
  bitrateX   = UpdateX(bitrateX, bitrateWidth, cfg_player_bitrate_align, cfg_player_bitrate_scroll, &cntX7, cfg_player_bitrate_rc.x2 - cfg_player_bitrate_rc.x1);
  channelsX  = UpdateX(channelsX, channelsWidth, cfg_player_channels_align, cfg_player_channels_scroll, &cntX8, cfg_player_channels_rc.x2 - cfg_player_channels_rc.x1);
  fulltimeX  = UpdateX(fulltimeX, fulltimeWidth, cfg_player_fulltime_align, cfg_player_fulltime_scroll, &cntX9, cfg_player_fulltime_rc.x2 - cfg_player_fulltime_rc.x1);
  curtimeX   = UpdateX(curtimeX, curtimeWidth, cfg_player_curtime_align, cfg_player_curtime_scroll, &cntX10, cfg_player_curtime_rc.x2 - cfg_player_curtime_rc.x1);
}

void UpdateMediaInfo(void)
{
  if (!FindBook(isAudioPlayerBook()))
  {
    ResetMP3Data();
  }
}

void ShowMediaInfo(int y, int location)
{
   if(RadioIsStarted)
   {
       if (cfg_fm_show)
       {
          if(cfg_fm_align == RIGHT)
                myDrawString(cfg_fm_font, fmID, cfg_fm_align, 1, cfg_fm_y - y, cfg_fm_x, DisplayHeight, cfg_fm_border, cfg_fm_color, cfg_fm_highlight);
          else
                myDrawString(cfg_fm_font, fmID, cfg_fm_align, cfg_fm_x, cfg_fm_y - y, DisplayWidth, DisplayHeight, cfg_fm_border, cfg_fm_color, cfg_fm_highlight);
       }
       if (cfg_fm_mode_show)
       {
          if(cfg_fm_mode_align == RIGHT)
                myDrawString(cfg_fm_mode_font, modeID, cfg_fm_mode_align, 1, cfg_fm_mode_y - y, cfg_fm_mode_x, DisplayHeight, cfg_fm_mode_border, cfg_fm_mode_color, cfg_fm_mode_highlight);
          else
                myDrawString(cfg_fm_mode_font, modeID, cfg_fm_mode_align, cfg_fm_mode_x, cfg_fm_mode_y - y, DisplayWidth, DisplayHeight, cfg_fm_mode_border, cfg_fm_mode_color, cfg_fm_mode_highlight);
       }
       if (cfg_fm_rdsdata_show)
       {
          if(cfg_fm_rdsdata_align == RIGHT)
                myDrawString(cfg_fm_rdsdata_font, rdsdataID, cfg_fm_rdsdata_align, 1, cfg_fm_rdsdata_y - y, cfg_fm_rdsdata_x, DisplayHeight, cfg_fm_rdsdata_border, cfg_fm_rdsdata_color, cfg_fm_rdsdata_highlight);
          else
                myDrawString(cfg_fm_rdsdata_font, rdsdataID, cfg_fm_rdsdata_align, cfg_fm_rdsdata_x, cfg_fm_rdsdata_y - y, DisplayWidth, DisplayHeight, cfg_fm_rdsdata_border, cfg_fm_rdsdata_color, cfg_fm_rdsdata_highlight);
       }
   }
   if(PlayerIsStarted)
   {
       GC *gc=get_DisplayGC();
       RECT tmp_rc, txt_rc;
       get_GC_RECT(gc, &tmp_rc);
       txt_rc.y1=tmp_rc.y1;
       txt_rc.y2=tmp_rc.y2;

       if(mp3fulltime)
       {
         if(cfg_player_timebar_show)
         {
            if(cfg_player_timebar_highlight)
            {
                DrawOwnRect(cfg_player_timebar.x1, cfg_player_timebar.y1-y, cfg_player_timebar.x2, cfg_player_timebar.y2-y, cfg_player_timebar_border,cfg_player_timebar_rem_color);
                DrawOwnRect(player_curbar.x1, player_curbar.y1-y, player_curbar.x2, player_curbar.y2-y, cfg_player_timebar_border,cfg_player_timebar_cur_color);
            }
            else
            {
                DrawOwnRect(cfg_player_timebar.x1, cfg_player_timebar.y1-y, cfg_player_timebar.x2, cfg_player_timebar.y2-y, cfg_player_timebar_rem_color,cfg_player_timebar_rem_color);
                DrawOwnRect(player_curbar.x1, player_curbar.y1-y, player_curbar.x2, player_curbar.y2-y, cfg_player_timebar_cur_color,cfg_player_timebar_cur_color);
            }
         }
         if(cfg_player_fulltime_show)
         {
                txt_rc.x1=cfg_player_fulltime_rc.x1;
                txt_rc.x2=cfg_player_fulltime_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_fulltime_align == RIGHT)
                    myDrawString(cfg_player_fulltime_font, fulltimeID, cfg_player_fulltime_align, cfg_player_fulltime_rc.x1 - fulltimeX, cfg_player_fulltime_rc.y1 - y, cfg_player_fulltime_rc.x2 - fulltimeX, cfg_player_fulltime_rc.y2, cfg_player_fulltime_border, cfg_player_fulltime_color, cfg_player_fulltime_highlight);
                else if(cfg_player_fulltime_align == LEFT)
                    myDrawString(cfg_player_fulltime_font, fulltimeID, cfg_player_fulltime_align, cfg_player_fulltime_rc.x1 - fulltimeX, cfg_player_fulltime_rc.y1 - y, cfg_player_fulltime_rc.x2, cfg_player_fulltime_rc.y2, cfg_player_fulltime_border, cfg_player_fulltime_color, cfg_player_fulltime_highlight);
                else
                    myDrawString(cfg_player_fulltime_font, fulltimeID, cfg_player_fulltime_align, cfg_player_fulltime_rc.x1 - fulltimeX - (fulltimeWidth >> 1), cfg_player_fulltime_rc.y1 - y, cfg_player_fulltime_rc.x2 - fulltimeX + (fulltimeWidth >> 1), cfg_player_fulltime_rc.y2, cfg_player_fulltime_border, cfg_player_fulltime_color, cfg_player_fulltime_highlight);
         }
         if(cfg_player_curtime_show)
         {
                txt_rc.x1=cfg_player_curtime_rc.x1;
                txt_rc.x2=cfg_player_curtime_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_curtime_align == RIGHT)
                    myDrawString(cfg_player_curtime_font, curtimeID, cfg_player_curtime_align, cfg_player_curtime_rc.x1 - curtimeX, cfg_player_curtime_rc.y1 - y, cfg_player_curtime_rc.x2 - curtimeX, cfg_player_curtime_rc.y2, cfg_player_curtime_border, cfg_player_curtime_color, cfg_player_curtime_highlight);
                else if(cfg_player_curtime_align == LEFT)
                    myDrawString(cfg_player_curtime_font, curtimeID, cfg_player_curtime_align, cfg_player_curtime_rc.x1 - curtimeX, cfg_player_curtime_rc.y1 - y, cfg_player_curtime_rc.x2, cfg_player_curtime_rc.y2, cfg_player_curtime_border, cfg_player_curtime_color, cfg_player_curtime_highlight);
                else
                    myDrawString(cfg_player_curtime_font, curtimeID, cfg_player_curtime_align, cfg_player_curtime_rc.x1 - curtimeX - (curtimeWidth >> 1), cfg_player_curtime_rc.y1 - y, cfg_player_curtime_rc.x2 - curtimeX + (curtimeWidth >> 1), cfg_player_curtime_rc.y2, cfg_player_curtime_border, cfg_player_curtime_color, cfg_player_curtime_highlight);
         }
       }
       if (cfg_player_title_show)
       {
            txt_rc.x1=cfg_player_title_rc.x1;
            txt_rc.x2=cfg_player_title_rc.x2;
            GC_validate_RECT(gc, &txt_rc);
            if(cfg_player_title_align == RIGHT)
                myDrawString(cfg_player_title_font, nameID, cfg_player_title_align, cfg_player_title_rc.x1 - nameWidth, cfg_player_title_rc.y1 - y, cfg_player_title_rc.x2 - nameX, cfg_player_title_rc.y2, cfg_player_title_border, cfg_player_title_color, cfg_player_title_highlight);
            else if(cfg_player_title_align == LEFT)
                myDrawString(cfg_player_title_font, nameID, cfg_player_title_align, cfg_player_title_rc.x1 - nameX, cfg_player_title_rc.y1 - y, cfg_player_title_rc.x2, cfg_player_title_rc.y2, cfg_player_title_border, cfg_player_title_color, cfg_player_title_highlight);
            else
                myDrawString(cfg_player_title_font, nameID, cfg_player_title_align, cfg_player_title_rc.x1 - nameX - (nameWidth >> 1), cfg_player_title_rc.y1 - y, cfg_player_title_rc.x2 - nameX + (nameWidth >> 1), cfg_player_title_rc.y2, cfg_player_title_border, cfg_player_title_color, cfg_player_title_highlight);
       }
       if(hastags)
       {
           if (cfg_player_album_show)
           {
                txt_rc.x1=cfg_player_album_rc.x1;
                txt_rc.x2=cfg_player_album_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_album_align == RIGHT)
                    myDrawString(cfg_player_album_font, albumID, cfg_player_album_align, cfg_player_album_rc.x1 - albumWidth, cfg_player_album_rc.y1 - y, cfg_player_album_rc.x2 - albumX, cfg_player_album_rc.y2, cfg_player_album_border, cfg_player_album_color, cfg_player_album_highlight);
                else if(cfg_player_album_align == LEFT)
                    myDrawString(cfg_player_album_font, albumID, cfg_player_album_align, cfg_player_album_rc.x1 - albumX, cfg_player_album_rc.y1 - y, cfg_player_album_rc.x2, cfg_player_album_rc.y2, cfg_player_album_border, cfg_player_album_color, cfg_player_album_highlight);
                else
                    myDrawString(cfg_player_album_font, albumID, cfg_player_album_align, cfg_player_album_rc.x1 - albumX - (albumWidth >> 1), cfg_player_album_rc.y1 - y, cfg_player_album_rc.x2 - albumX + (albumWidth >> 1), cfg_player_album_rc.y2, cfg_player_album_border, cfg_player_album_color, cfg_player_album_highlight);
           }
           if (cfg_player_artist_show)
           {
                txt_rc.x1=cfg_player_artist_rc.x1;
                txt_rc.x2=cfg_player_artist_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_artist_align == RIGHT)
                    myDrawString(cfg_player_artist_font, artistID, cfg_player_artist_align, cfg_player_artist_rc.x1 - artistWidth, cfg_player_artist_rc.y1 - y, cfg_player_artist_rc.x2 - artistX, cfg_player_artist_rc.y2, cfg_player_artist_border, cfg_player_artist_color, cfg_player_artist_highlight);
                else if(cfg_player_artist_align == LEFT)
                    myDrawString(cfg_player_artist_font, artistID, cfg_player_artist_align, cfg_player_artist_rc.x1 - artistX, cfg_player_artist_rc.y1 - y, cfg_player_artist_rc.x2, cfg_player_artist_rc.y2, cfg_player_artist_border, cfg_player_artist_color, cfg_player_artist_highlight);
                else
                    myDrawString(cfg_player_artist_font, artistID, cfg_player_artist_align, cfg_player_artist_rc.x1 - artistX - (artistWidth >> 1), cfg_player_artist_rc.y1 - y, cfg_player_artist_rc.x2 - artistX + (artistWidth >> 1), cfg_player_artist_rc.y2, cfg_player_artist_border, cfg_player_artist_color, cfg_player_artist_highlight);
           }
           if (cfg_player_year_show)
           {
                txt_rc.x1=cfg_player_year_rc.x1;
                txt_rc.x2=cfg_player_year_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_year_align == RIGHT)
                    myDrawString(cfg_player_year_font, yearID, cfg_player_year_align, cfg_player_year_rc.x1 - yearX, cfg_player_year_rc.y1 - y, cfg_player_year_rc.x2 - yearX, cfg_player_year_rc.y2, cfg_player_year_border, cfg_player_year_color, cfg_player_year_highlight);
                else if(cfg_player_year_align == LEFT)
                    myDrawString(cfg_player_year_font, yearID, cfg_player_year_align, cfg_player_year_rc.x1 - yearX, cfg_player_year_rc.y1 - y, cfg_player_year_rc.x2, cfg_player_year_rc.y2, cfg_player_year_border, cfg_player_year_color, cfg_player_year_highlight);
                else
                    myDrawString(cfg_player_year_font, yearID, cfg_player_year_align, cfg_player_year_rc.x1 - yearX - (yearWidth >> 1), cfg_player_year_rc.y1 - y, cfg_player_year_rc.x2 - yearX + (yearWidth >> 1), cfg_player_year_rc.y2, cfg_player_year_border, cfg_player_year_color, cfg_player_year_highlight);
           }
           if (cfg_player_genre_show)
           {
                txt_rc.x1=cfg_player_genre_rc.x1;
                txt_rc.x2=cfg_player_genre_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_genre_align == RIGHT)
                    myDrawString(cfg_player_genre_font, genreID, cfg_player_genre_align, cfg_player_genre_rc.x1 - genreX, cfg_player_genre_rc.y1 - y, cfg_player_genre_rc.x2 - genreX, cfg_player_genre_rc.y2, cfg_player_genre_border, cfg_player_genre_color, cfg_player_genre_highlight);
                else if(cfg_player_genre_align == LEFT)
                    myDrawString(cfg_player_genre_font, genreID, cfg_player_genre_align, cfg_player_genre_rc.x1 - genreX, cfg_player_genre_rc.y1 - y, cfg_player_genre_rc.x2, cfg_player_genre_rc.y2, cfg_player_genre_border, cfg_player_genre_color, cfg_player_genre_highlight);
                else
                    myDrawString(cfg_player_genre_font, genreID, cfg_player_genre_align, cfg_player_genre_rc.x1 - genreX - (genreWidth >> 1), cfg_player_genre_rc.y1 - y, cfg_player_genre_rc.x2 - genreX + (genreWidth >> 1), cfg_player_genre_rc.y2, cfg_player_genre_border, cfg_player_genre_color, cfg_player_genre_highlight);
           }
       }
       if(hasheaders)
       {
           if (cfg_player_frequency_show)
           {
                txt_rc.x1=cfg_player_frequency_rc.x1;
                txt_rc.x2=cfg_player_frequency_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_frequency_align == RIGHT)
                    myDrawString(cfg_player_frequency_font, frequencyID, cfg_player_frequency_align, cfg_player_frequency_rc.x1 - frequencyX, cfg_player_frequency_rc.y1 - y, cfg_player_frequency_rc.x2 - frequencyX, cfg_player_frequency_rc.y2, cfg_player_frequency_border, cfg_player_frequency_color, cfg_player_frequency_highlight);
                else if(cfg_player_frequency_align == LEFT)
                    myDrawString(cfg_player_frequency_font, frequencyID, cfg_player_frequency_align, cfg_player_frequency_rc.x1 - frequencyX, cfg_player_frequency_rc.y1 - y, cfg_player_frequency_rc.x2, cfg_player_frequency_rc.y2, cfg_player_frequency_border, cfg_player_frequency_color, cfg_player_frequency_highlight);
                else
                    myDrawString(cfg_player_frequency_font, frequencyID, cfg_player_frequency_align, cfg_player_frequency_rc.x1 - frequencyX - (frequencyWidth >> 1), cfg_player_frequency_rc.y1 - y, cfg_player_frequency_rc.x2 - frequencyX + (frequencyWidth >> 1), cfg_player_frequency_rc.y2, cfg_player_frequency_border, cfg_player_frequency_color, cfg_player_frequency_highlight);
           }
           if (cfg_player_bitrate_show)
           {
                txt_rc.x1=cfg_player_bitrate_rc.x1;
                txt_rc.x2=cfg_player_bitrate_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_bitrate_align == RIGHT)
                    myDrawString(cfg_player_bitrate_font, bitrateID, cfg_player_bitrate_align, cfg_player_bitrate_rc.x1 - bitrateX, cfg_player_bitrate_rc.y1 - y, cfg_player_bitrate_rc.x2 - bitrateX, cfg_player_bitrate_rc.y2, cfg_player_bitrate_border, cfg_player_bitrate_color, cfg_player_bitrate_highlight);
                else if(cfg_player_bitrate_align == LEFT)
                    myDrawString(cfg_player_bitrate_font, bitrateID, cfg_player_bitrate_align, cfg_player_bitrate_rc.x1 - bitrateX, cfg_player_bitrate_rc.y1 - y, cfg_player_bitrate_rc.x2, cfg_player_bitrate_rc.y2, cfg_player_bitrate_border, cfg_player_bitrate_color, cfg_player_bitrate_highlight);
                else
                    myDrawString(cfg_player_bitrate_font, bitrateID, cfg_player_bitrate_align, cfg_player_bitrate_rc.x1 - bitrateX - (bitrateWidth >> 1), cfg_player_bitrate_rc.y1 - y, cfg_player_bitrate_rc.x2 - bitrateX + (bitrateWidth >> 1), cfg_player_bitrate_rc.y2, cfg_player_bitrate_border, cfg_player_bitrate_color, cfg_player_bitrate_highlight);
           }
           if (cfg_player_channels_show)
           {
                txt_rc.x1=cfg_player_channels_rc.x1;
                txt_rc.x2=cfg_player_channels_rc.x2;
                GC_validate_RECT(gc, &txt_rc);
                if(cfg_player_channels_align == RIGHT)
                    myDrawString(cfg_player_channels_font, channelsID, cfg_player_channels_align, cfg_player_channels_rc.x1 - channelsX, cfg_player_channels_rc.y1 - y, cfg_player_channels_rc.x2 - channelsX, cfg_player_channels_rc.y2, cfg_player_channels_border, cfg_player_channels_color, cfg_player_channels_highlight);
                else if(cfg_player_channels_align == LEFT)
                    myDrawString(cfg_player_channels_font, channelsID, cfg_player_channels_align, cfg_player_channels_rc.x1 - channelsX, cfg_player_channels_rc.y1 - y, cfg_player_channels_rc.x2, cfg_player_channels_rc.y2, cfg_player_channels_border, cfg_player_channels_color, cfg_player_channels_highlight);
                else
                    myDrawString(cfg_player_channels_font, channelsID, cfg_player_channels_align, cfg_player_channels_rc.x1 - channelsX - (channelsWidth >> 1), cfg_player_channels_rc.y1 - y, cfg_player_channels_rc.x2 - channelsX + (channelsWidth >> 1), cfg_player_channels_rc.y2, cfg_player_channels_border, cfg_player_channels_color, cfg_player_channels_highlight);
           }
       }
       GC_validate_RECT(gc, &tmp_rc);
   }
}

void ShowMediaImages(int y, int location)
{
   GC *GC_DISP=get_DisplayGC ();
   if(RadioIsStarted)
   {
       if (cfg_fm_image_show && (radio_image != NOIMAGE))
       {
          putchar(GC_DISP, cfg_fm_image_x, cfg_fm_image_y - y, 0, 0, radio_image);
       }
   }
   if(PlayerIsStarted)
   {
     if(cfg_player_cover_under_image)
     {
       if (cfg_player_cover_show && (cover_image != NOIMAGE))
       {
          putchar(GC_DISP, cfg_player_cover.x1, cfg_player_cover.y1 - y, coverWidth, coverHeight, cover_image);
       }
       if (cfg_player_image_show && (player_image != NOIMAGE))
       {
          putchar(GC_DISP, cfg_player_image_x, cfg_player_image_y - y, 0, 0, player_image);
       }
     }
     else
     {
       if (cfg_player_image_show && (player_image != NOIMAGE))
       {
          putchar(GC_DISP, cfg_player_image_x, cfg_player_image_y - y, 0, 0, player_image);
       }
       if (cfg_player_cover_show && (cover_image != NOIMAGE))
       {
          putchar(GC_DISP, cfg_player_cover.x1, cfg_player_cover.y1 - y, coverWidth, coverHeight, cover_image);
       }
     }
   }
}

void UpdateShowParams(void)
{
  if(PlayerIsStartedPrev != PlayerIsStarted)
  {
    UpdateImagesShowParams();
    UpdateTimeDateShowParams();
    UpdateHeapShowParams();
    UpdateBattTempShowParams();
    UpdateMemShowParams();
    UpdateOperatorShowParams();
    UpdateCustomMsgShowParams();
    UpdateSystemShowParams();
    UpdateNetworkShowParams();
    UpdateAlarmShowParams();
    UpdateAppShowParams();
    UpdateStatusShowParams();
    UpdateCallsShowParams();

    PlayerIsStartedPrev = PlayerIsStarted;
  }
}

void onTimer_media (u16 timerID, LPARAM lparam)
{
    UpdateRadioInfo();
    UpdateMediaInfo();
//    UpdatePlayerInfo();
    UpdateShowParams();
    Timer_ReSet(&timer_media,cfg_refresh_media,onTimer_media,0);
}

void UpdateMediaInfoParams(void)
{
    FSTAT _fstat;

    win12512unicode(cover_file,cfg_player_cover_file,MAXELEMS(cover_file)-1);

    if(radio_image != NOIMAGE)
    {
      ImageID_Free(radio_image);
      radio_image = NOIMAGE;
    }
    if(player_image != NOIMAGE)
    {
      ImageID_Free(player_image);
      player_image = NOIMAGE;
    }

    extractdir(tmppath, tmpname, cfg_fm_file);
    if(fstat(tmppath,tmpname,&_fstat)!=0)
    {
        if(cfg_fm_image_show)
        {
          MessageBox(EMPTY_TEXTID, STR("Картинка FM радио не найдена"), NOIMAGE, 1 ,5000, 0);
        }
    }
    else if(ImageID_Get(tmppath,tmpname,&radio_image)<0) radio_image = NOIMAGE;

    if(stereoID != EMPTY_TEXTID) { TextID_Destroy(stereoID); }
    stereoID = STR("Stereo");
    if(rdsID != EMPTY_TEXTID) { TextID_Destroy(rdsID); }
    rdsID = STR("RDS");

    extractdir(tmppath, tmpname, cfg_player_file);
    if(fstat(tmppath,tmpname,&_fstat)!=0)
    {
        if(cfg_player_image_show)
        {
          MessageBox(EMPTY_TEXTID, STR("Картинка плеера не найдена"), NOIMAGE, 1 ,5000, 0);
        }
    }
    else if(ImageID_Get(tmppath,tmpname,&player_image)<0) player_image = NOIMAGE;

    UpdateRadioInfo();
    UpdatePlayerInfo();
    Timer_ReSet(&timer_media,cfg_refresh_media,onTimer_media,0);

    PlayerIsStartedPrev = !PlayerIsStarted;
    UpdateShowParams();
}

void FreeMediaInfoParams(void)
{
  if(timer_media) Timer_Kill(&timer_media);

  if(radio_image != NOIMAGE)
  {
    ImageID_Free(radio_image);
    radio_image = NOIMAGE;
  }
  if(player_image != NOIMAGE)
  {
    ImageID_Free(player_image);
    player_image = NOIMAGE;
  }
  if(cover_image != NOIMAGE)
  {
    ImageID_Free(cover_image);
    cover_image = NOIMAGE;
  }

  if(fmID != EMPTY_TEXTID)
  {
    TextID_Destroy(fmID);
    fmID = EMPTY_TEXTID;
  }

  if(nameID != EMPTY_TEXTID)
  {
    TextID_Destroy(nameID);
    nameID = EMPTY_TEXTID;
  }
  if(albumID != EMPTY_TEXTID)
  {
    TextID_Destroy(albumID);
    albumID = EMPTY_TEXTID;
  }
  if(artistID != EMPTY_TEXTID)
  {
    TextID_Destroy(artistID);
    artistID = EMPTY_TEXTID;
  }
  if(yearID != EMPTY_TEXTID)
  {
    TextID_Destroy(yearID);
    yearID = EMPTY_TEXTID;
  }
  if(genreID != EMPTY_TEXTID)
  {
    TextID_Destroy(genreID);
    genreID = EMPTY_TEXTID;
  }
  if(frequencyID != EMPTY_TEXTID)
  {
    TextID_Destroy(frequencyID);
    frequencyID = EMPTY_TEXTID;
  }
  if(bitrateID != EMPTY_TEXTID)
  {
    TextID_Destroy(bitrateID);
    bitrateID = EMPTY_TEXTID;
  }
  if(channelsID != EMPTY_TEXTID)
  {
    TextID_Destroy(channelsID);
    channelsID = EMPTY_TEXTID;
  }
  if(fulltimeID != EMPTY_TEXTID)
  {
    TextID_Destroy(fulltimeID);
    fulltimeID = EMPTY_TEXTID;
  }
  if(curtimeID != EMPTY_TEXTID)
  {
    TextID_Destroy(curtimeID);
    curtimeID = EMPTY_TEXTID;
  }
  if(stereoID != EMPTY_TEXTID)
  {
    TextID_Destroy(stereoID);
    stereoID = EMPTY_TEXTID;
  }
  if(rdsID != EMPTY_TEXTID)
  {
    TextID_Destroy(rdsID);
    rdsID = EMPTY_TEXTID;
  }
  if(rdsdataID != EMPTY_TEXTID)
  {
    TextID_Destroy(rdsdataID);
    rdsdataID = EMPTY_TEXTID;
  }
}


