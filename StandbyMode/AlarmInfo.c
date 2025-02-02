#include "..\include\Lib_Clara.h"
#include "AlarmInfo.h"
#include "MediaInfo.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"

TEXTID alarmID = EMPTY_TEXTID;
u16 timer_alarmInfo = 0;
int alarm_show = 0;

IMAGEID alarm_img = NOIMAGE;
IMAGEID alarm_image = NOIMAGE;

int alarm_height = 0;
int alarm_width = 0;
unsigned int alarm_x = 0;
unsigned int alarm_y = 0;
unsigned int alarm_img_x = 0;
unsigned int alarm_img_y = 0;

/*
u16 timer_remover3 = 0;       // alarm
int cfg_refresh_time3 = 10; // 10 msec

int orig_alarm_remove = 0;
const int cfg_orig_alarm_not_show_if_player = 1;
const int cfg_remove_alarm=1;
*/

int timecmp(const TIME *t1, const TIME *t2)
{
    if(t1->hour > t2->hour) return 1;
    if(t1->hour < t2->hour) return -1;
    if(t1->min > t2->min) return 1;
    if(t1->min < t2->min) return -1;
    return 0;
}

int Alarm_GetCurrentAlarmTime(TIME *AlarmTime)
{
  int status = -1;
  wchar_t ID;
  char type;
  WEEK wk;
  DATETIME dt;

  if (ChipID == CHIPID_DB2010) //Для дб2010:
  {
    char alarm_status;
    REQUEST_ALARMCLOCKSTATUS_GET(SYNC, &alarm_status);
    if(alarm_status)
    {
      REQUEST_ALARMCLOCKTIME_GET(SYNC, AlarmTime);
      return 2;
    }
    return -1;
  }

  if((status = Alarm_GetCurrentTypeAndAlarmID(&type, &ID)) >= 0)
  {
    if((status = Alarm_GetTIME(AlarmTime, ID)) >= 0)
    {
      if(type == 0)
      {
        if((status = Alarm_GetWeekSettings(&wk, ID)) >= 0)
        {
          char WeekDay, nextWeekDay;
          REQUEST_DATEANDTIME_GET(SYNC,&dt);

          DATE_GetWeekDay(&dt.date, &WeekDay);

          nextWeekDay = WeekDay + 1;
          if(nextWeekDay > 6) nextWeekDay = 0;

          if(wk.days[WeekDay])
          {
            if(!wk.days[nextWeekDay])
            {
              if(timecmp(&dt.time, AlarmTime) >= 0) status = -1;
            }
          }
          else
          {
            if(wk.days[nextWeekDay])
            {
              if(timecmp(&dt.time, AlarmTime) < 0) status = -1;
            }
            else status = -1;
          }
        }
      }
      else if(type != 1)
      {
        status = -1;
      }
    }
  }
  return status;
}

void UpdateAlarmInfo(void)
{
  TIME alarm;
  if(Alarm_GetCurrentAlarmTime(&alarm) >= 0)
  {
    wchar_t alarm_msg[32];
    snwprintf(alarm_msg,MAXELEMS(alarm_msg)-1,L"%02d:%02d", alarm.hour, alarm.min);
    if(alarmID != EMPTY_TEXTID) TextID_Destroy(alarmID);
    alarmID = TextID_Create(alarm_msg,ENC_UCS2,MAXELEMS(alarm_msg));
    return;
  }
  if(alarmID != EMPTY_TEXTID)
  {
    TextID_Destroy(alarmID);
    alarmID = EMPTY_TEXTID;
  }
}

void ShowAlarmInfo(int y, int location)
{
   if (alarm_show)
   {
     if(alarmID != EMPTY_TEXTID)
     {
       GC *GC_DISP=get_DisplayGC ();
       if (cfg_alarm_image_show && (alarm_image != NOIMAGE))
       {
          putchar(GC_DISP, cfg_alarm_image_x, cfg_alarm_image_y - y, 0, 0, alarm_image);
       }

       if(cfg_alarm_align == RIGHT)
          myDrawString(cfg_alarm_font, alarmID, cfg_alarm_align, 1, alarm_y - y, alarm_x, DisplayHeight, cfg_alarm_border, cfg_alarm_color, cfg_alarm_highlight);
       else
          myDrawString(cfg_alarm_font, alarmID, cfg_alarm_align, alarm_x, alarm_y - y, DisplayWidth, DisplayHeight, cfg_alarm_border, cfg_alarm_color, cfg_alarm_highlight);

       if(cfg_alarm_icon_show && (alarm_img != NOIMAGE)) putchar(GC_DISP, alarm_img_x, alarm_img_y - y, alarm_height, alarm_height, alarm_img);
     }
   }
}

void onTimer_alarmInfo (u16 timerID, LPARAM lparam)
{
    UpdateAlarmInfo();
    Timer_ReSet(&timer_alarmInfo,cfg_refresh_alarm,onTimer_alarmInfo,0);
}

/*
void onTimer_remover3 (u16 timerID, LPARAM lparam)
{
  if(SleepModeIsDeactivated && IsInStandby)
  {
    GUI *sb = GetStandbyBookGui();
    if(cfg_remove_alarm)
    {
        //0x0E - At the left bottom of large-screen phones
        StatusIndication_SetItemText(sb, 0x0E, EMPTY_TEXTID);
        //0x0F - One line above the left bottom (only on large-screen phones)
        StatusIndication_SetItemText(sb, 0x0F, EMPTY_TEXTID);
    }
  }
  Timer_ReSet(&timer_remover3,cfg_refresh_time3,onTimer_remover3,0);
}
*/
void UpdateAlarmShowParams(void)
{
   alarm_show = cfg_alarm_show && (!(PlayerIsStarted && cfg_alarm_not_show_if_player));
/*
   orig_alarm_remove = PlayerIsStarted && cfg_orig_alarm_not_show_if_player;

    if(orig_alarm_remove)
    {
      if(!timer_remover3) Timer_ReSet(&timer_remover3,cfg_refresh_time3,onTimer_remover3,0);
    }
    else if(timer_remover3)
    {
      if(!cfg_remove_alarm)
      {
        Timer_Kill(&timer_remover3);
        timer_remover3 = 0;
      }
    }
*/
}

void FreeAlarmImages(void)
{
  if(alarm_img != NOIMAGE)
  {
    ImageID_Free(alarm_img);
    alarm_img = NOIMAGE;
  }
  if(alarm_image != NOIMAGE)
  {
    ImageID_Free(alarm_image);
    alarm_image = NOIMAGE;
  }
}

void UpdateAlarmInfoParams(void)
{
    FSTAT _fstat;

    FreeAlarmImages();

    if(cfg_alarm_show && cfg_alarm_icon_show)
    {
        extractdir(tmppath, tmpname, cfg_alarm_file);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
            MessageBox(EMPTY_TEXTID, STR("Иконка для будильника не найдена"),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&alarm_img)<0) alarm_img = NOIMAGE;
    }

    int font_old;
    TEXTID strID;

    font_old = SetFont(cfg_alarm_font);
    strID = TextID_Create(L"00:00",ENC_UCS2,5);
    alarm_height = GetImageHeight(strID);
    alarm_width = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
    TextID_Destroy(strID);
    SetFont(font_old);

    if(cfg_alarm_icon_show)
    {
      if(cfg_alarm_align == RIGHT)
      {
        alarm_img_x = cfg_alarm_x - alarm_width - alarm_height - 1;
        alarm_x = cfg_alarm_x;
      }
      else if(cfg_alarm_align == LEFT)
      {
        alarm_img_x = cfg_alarm_x;
        alarm_x = cfg_alarm_x + alarm_height + 1;
      }
      else
      {
        alarm_img_x = cfg_alarm_x + ((DisplayWidth - alarm_width - alarm_height) >> 1) - 1;
        alarm_x = cfg_alarm_x + (alarm_height >> 1) + 2;
      }
      alarm_y = alarm_img_y = cfg_alarm_y;
    }
    else
    {
      alarm_x = cfg_alarm_x;
      alarm_y = cfg_alarm_y;
    }

    if(cfg_alarm_image_show)
    {
      extractdir(tmppath, tmpname, cfg_alarm_image_file);
      if(fstat(tmppath,tmpname,&_fstat)!=0)
      {
          MessageBox(EMPTY_TEXTID, STR("Картинка будильника не найдена"), NOIMAGE, 1 ,5000, 0);
      }
      else if(ImageID_Get(tmppath,tmpname,&alarm_image)<0) alarm_image = NOIMAGE;
    }

/*
    if(cfg_remove_alarm || orig_alarm_remove)
    {
      Timer_ReSet(&timer_remover3,cfg_refresh_time3,onTimer_remover3,0);
    }
    else if(timer_remover3)
    {
      Timer_Kill(&timer_remover3);
      timer_remover3 = 0;
    }
*/
    if(cfg_alarm_show)
      UpdateAlarmInfo();

    if(cfg_alarm_show)
    {
      UpdateAlarmInfo();
      Timer_ReSet(&timer_alarmInfo,cfg_refresh_alarm,onTimer_alarmInfo,0);
    }
    else if(timer_alarmInfo)
    {
      Timer_Kill(&timer_alarmInfo);
      timer_alarmInfo = 0;
    }
}

void FreeAlarmInfoParams(void)
{
  if(timer_alarmInfo)
  {
      Timer_Kill(&timer_alarmInfo);
      timer_alarmInfo = 0;
  }
/*
  if(timer_remover3)
  {
      Timer_Kill(&timer_remover3);
      timer_remover3 = 0;
  }
*/
  if(alarmID != EMPTY_TEXTID)
  {
     TextID_Destroy(alarmID);
     alarmID = EMPTY_TEXTID;
  }
  FreeAlarmImages();
}


