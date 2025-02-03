#include "..\include\Lib_Clara.h"
#include "TimeDateInfo.h"
#include "config_data.h"
#include "main.h"
#include "MediaInfo.h"
#include "binclock.h"

DATETIME dt;
TIME tm;
char  ClockFormat = 1;
char  Clock2Format = 1;

wchar_t* DAYS[7]={_T("MONDAY_TXT"),_T("TUESDAY_TXT"),_T("WEDNESDAY_TXT"),_T("THURSDAY_TXT"),_T("FRIDAY_TXT"),_T("SATURDAY_TXT"),_T("SUNDAY_TXT")};
wchar_t* DAYS_SH[7]={_T("MON_TXT"),_T("TUE_TXT"),_T("WED_TXT"),_T("THU_TXT"),_T("FRI_TXT"),_T("SAT_TXT"),_T("SUN_TXT")};

wchar_t * days[7]={
  L"Monday",
  L"Tuesday",
  L"Wednesday",
  L"Thursday",
  L"Friday",
  L"Saturday"
  L"Sunday",
};

const wchar_t *custom_days[7] = {
    cfg_mon,
    cfg_tue,
    cfg_wed,
    cfg_thu,
    cfg_fri,
    cfg_sat,
    cfg_sun
};

TEXTID Clock0ID = EMPTY_TEXTID;
TEXTID ClockID = EMPTY_TEXTID;
TEXTID SecondID = EMPTY_TEXTID;
TEXTID AmPmID = EMPTY_TEXTID;
TEXTID DateID = EMPTY_TEXTID;
TEXTID DayID = EMPTY_TEXTID;

unsigned int clock_x1 = 0;
unsigned int clock_x2 = 0;
unsigned int second_x1 = 0;
unsigned int second_x2 = 0;
unsigned int ampm_x1 = 0;
unsigned int ampm_x2 = 0;
unsigned int second_y = 0;
unsigned int clock2_x1 = 0;
unsigned int clock2_x2 = 0;
unsigned int second2_x1 = 0;
unsigned int second2_x2 = 0;
unsigned int second2_y = 0;
unsigned int ampm2_x1 = 0;
unsigned int ampm2_x2 = 0;

u16 timer_clock = 0;
int cfg_refresh_clock = 1000;  // 1 sec

u16 timer_remover2 = 0;     // clock, date
int cfg_refresh_time2 = 10; // 10 msec

int clock_show = 0;
int clock2_show = 0;
int date_show = 0;
int day_show = 0;

int orig_clock_remove = 0;
int orig_date_remove = 0;

int GetDay(DATE *dt)
{
   char WeekDay;
   DATE_GetWeekDay(dt, &WeekDay);

   wchar_t msgstr[30];

   if(!cfg_custom_day)
   {
     int strID;
     if (LANG)
     {
       if (cfg_day_mode)
       {
         textidname2id((DAYS[WeekDay]),TEXTID_ANY_LEN,&strID);
         TextID_GetWString(strID,msgstr,TEXTID_ANY_LEN);
       }
       else
       {
         textidname2id((DAYS_SH[WeekDay]),TEXTID_ANY_LEN,&strID);
         TextID_GetWString(strID,msgstr,TEXTID_ANY_LEN);
       }
     }
     else
     {
      wstrncpy(msgstr, days[WeekDay],MAXELEMS(msgstr));
     }
   }
   else
   {
      wstrncpy(msgstr, custom_days[WeekDay],MAXELEMS(msgstr));
   }

   return(TextID_Create(convert_first_symbol(msgstr, cfg_day_format),ENC_UCS2,MAXELEMS(msgstr)));
}

void UpdateTimeDateInfo(void)
{
    wchar_t uClock[8];
    wchar_t uSecond[4];
    int current_hour;

    REQUEST_DATEANDTIME_GET(SYNC,&dt);
    {
         TEXTID SID = GetDay(&dt.date);
         if(DayID != EMPTY_TEXTID) TextID_Destroy(DayID);
         DayID = SID;
    }
    {
         char DF;
         if(cfg_date_format == 0)
              REQUEST_DATEFORMAT_GET(SYNC, &DF);
         else
              DF = (char)cfg_date_format;

         TEXTID SID = Date2ID(&dt.date,DF,1);
//   0          1             2          3            4              5            6              7
//AUTO_MSG,"DD-MM-YYYY","MM/DD/YYYY","YYYYMMDD","DD-strMM-YYYY","DD/MM/YYYY","DD.MM.YYYY","YYYY-MM-DD"};
//AUTO_MSG,"DD-MM-YY",  "MM/DD/YY",  "YYMMDD",  "DD-strMM-YY",  "DD/MM/YY",  "DD.MM.YY",  "YY-MM-DD"};
         if(cfg_remove_year)
         {
             wchar_t date[32];
             TextID_GetWString(SID,date,MAXELEMS(date));
             TextID_Destroy(SID);
             int len = wstrlen(date);
             if(DF == 3)
             {
               if(len == 6) { SID = TextID_Create(&date[2],ENC_UCS2,TEXTID_ANY_LEN); }
               else         { SID = TextID_Create(&date[4],ENC_UCS2,TEXTID_ANY_LEN); }
             }
             else if(DF == 4)
             {
               date[7] = 0;
               SID = TextID_Create(date,ENC_UCS2,TEXTID_ANY_LEN);
             }
             else if(DF == 7)
             {
               if(len == 8) { SID = TextID_Create(&date[3],ENC_UCS2,TEXTID_ANY_LEN); }
               else         { SID = TextID_Create(&date[5],ENC_UCS2,TEXTID_ANY_LEN); }
             }
             else
             {
               date[5] = 0;
               SID = TextID_Create(date,ENC_UCS2,TEXTID_ANY_LEN);
             }
         }
         if(DateID != EMPTY_TEXTID) TextID_Destroy(DateID);
         DateID = SID;
    }
    {
      if ((ClockFormat == 0) || (Clock2Format == 0))
      {
          if (dt.time.hour > 12)
          {
              current_hour=dt.time.hour-12;
              TEXTID SID = TextID_Create(L" pm",ENC_UCS2,3);
              if(AmPmID != EMPTY_TEXTID) TextID_Destroy(AmPmID);
              AmPmID = SID;
          }
          else
          {
              current_hour=dt.time.hour;
              if (dt.time.hour==0)
              {
                  current_hour=12;
              }
              if (dt.time.hour==12)
              {
                  TEXTID SID = TextID_Create(L" pm",ENC_UCS2,3);
                  if(AmPmID != EMPTY_TEXTID) TextID_Destroy(AmPmID);
                  AmPmID = SID;
              }
              else
              {
                  TEXTID SID = TextID_Create(L" am",ENC_UCS2,3);
                  if(AmPmID != EMPTY_TEXTID) TextID_Destroy(AmPmID);
                  AmPmID = SID;
              }
          }
          snwprintf(uClock,MAXELEMS(uClock),_T("%02d:%02d"),current_hour,dt.time.min);
          TEXTID SID = TextID_Create(uClock,ENC_UCS2,MAXELEMS(uClock));
          if(Clock0ID != EMPTY_TEXTID) TextID_Destroy(Clock0ID);
          Clock0ID = SID;

      }

      snwprintf(uClock,MAXELEMS(uClock),_T("%02d:%02d"),dt.time.hour,dt.time.min);
      TEXTID SID = TextID_Create(uClock,ENC_UCS2,MAXELEMS(uClock));
      if(ClockID != EMPTY_TEXTID) TextID_Destroy(ClockID);
      ClockID = SID;

      if(cfg_seconds || cfg_seconds2)
      {
          snwprintf(uSecond,MAXELEMS(uSecond),_T(":%02d"),dt.time.sec);
          TEXTID SID = TextID_Create(uSecond,ENC_UCS2,MAXELEMS(uSecond));
          if(SecondID != EMPTY_TEXTID) TextID_Destroy(SecondID);
          SecondID = SID;
      }
    }

    UpdateBinClock(&dt.time);
}

void ShowTimeDateInfo(int y, int location)
{
   ShowBinClock(y, location);

   if (clock_show)
   {
      if(ClockFormat)
      {
        myDrawString(cfg_clock_font, ClockID, cfg_clock_align, clock_x1, cfg_clock_y - y, clock_x2, DisplayHeight, cfg_clock_border, cfg_clock_color, cfg_clock_highlight);
      }
      else
      {
        myDrawString(cfg_clock_font, Clock0ID, cfg_clock_align, clock_x1, cfg_clock_y - y, clock_x2, DisplayHeight, cfg_clock_border, cfg_clock_color, cfg_clock_highlight);
        myDrawString(cfg_clock_font, AmPmID, cfg_clock_align, ampm_x1, cfg_clock_y - y, ampm_x2, DisplayHeight, cfg_clock_border, cfg_clock_color, cfg_clock_highlight);
      }
      if(cfg_seconds) myDrawString(cfg_seconds_font, SecondID, cfg_clock_align, second_x1, second_y - y, second_x2, DisplayHeight, cfg_clock_border, cfg_clock_color, cfg_seconds_highlight);
   }
   if (clock2_show && ((!cfg_clock2_nonstandby_show) || (cfg_clock2_mediapleyer_show && PlayerIsStarted)))
   {
      if(Clock2Format)
      {
        myDrawString(cfg_clock2_font, ClockID, cfg_clock2_align, clock2_x1, cfg_clock2_y - y, clock2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_clock2_highlight);
      }
      else
      {
        myDrawString(cfg_clock2_font, Clock0ID, cfg_clock2_align, clock2_x1, cfg_clock2_y - y, clock2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_clock2_highlight);
        myDrawString(cfg_clock2_font, AmPmID, cfg_clock2_align, ampm2_x1, cfg_clock2_y - y, ampm2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_clock2_highlight);
      }
      if(cfg_seconds2) myDrawString(cfg_seconds2_font, SecondID, cfg_clock2_align, second2_x1, second2_y - y, second2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_seconds2_highlight);
   }
   if (date_show)
   {
    if(cfg_date_align == RIGHT)
        myDrawString(cfg_date_font, DateID, cfg_date_align, 1, cfg_date_y - y, cfg_date_x, DisplayHeight, cfg_date_border, cfg_date_color, cfg_date_highlight);
    else
        myDrawString(cfg_date_font, DateID, cfg_date_align, cfg_date_x, cfg_date_y - y, DisplayWidth, DisplayHeight, cfg_date_border, cfg_date_color, cfg_date_highlight);
   }
   if (day_show)
   {
     if(cfg_day_align == RIGHT)
        myDrawString(cfg_day_font, DayID, cfg_day_align, 1, cfg_day_y - y, cfg_day_x, DisplayHeight, cfg_day_border, cfg_day_color, cfg_day_highlight);
     else
        myDrawString(cfg_day_font, DayID, cfg_day_align, cfg_day_x, cfg_day_y - y, DisplayWidth, DisplayHeight, cfg_day_border, cfg_day_color, cfg_day_highlight);
   }
}

void ShowNonStandbyTimeInfo(int y, int location)
{
   if (clock2_show && cfg_clock2_nonstandby_show)
   {
      if(Clock2Format)
      {
        myDrawString(cfg_clock2_font, ClockID, cfg_clock2_align, clock2_x1, cfg_clock2_y - y, clock2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_clock2_highlight);
      }
      else
      {
        myDrawString(cfg_clock2_font, Clock0ID, cfg_clock2_align, clock2_x1, cfg_clock2_y - y, clock2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_clock2_highlight);
        myDrawString(cfg_clock2_font, AmPmID, cfg_clock2_align, ampm2_x1, cfg_clock2_y - y, ampm2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_clock2_highlight);
      }
      if(cfg_seconds2) myDrawString(cfg_seconds2_font, SecondID, cfg_clock2_align, second2_x1, second2_y - y, second2_x2, DisplayHeight, cfg_clock2_border, cfg_clock2_color, cfg_seconds2_highlight);
   }
}

void onTimer_clock (u16 timerID, LPARAM lparam)
{
    UpdateTimeDateInfo();
    Timer_ReSet(&timer_clock,cfg_refresh_clock,onTimer_clock,0);
}

void onTimer_remover2 (u16 timerID, LPARAM lparam)
{
  if(SleepModeIsDeactivated && IsInStandby)
  {
    GUI *sb = GetStandbyBookGui();
    if(cfg_remove_clock || orig_clock_remove)
    {
        //0x0B - small-screen phones: left bottom (place of time)large-screen phones: in place of big clock
        StatusIndication_SetItemText(sb, 0x0B, EMPTY_TEXTID);
        //0x0C - One line above the right bottom (only on large-screen phones - default place of small clock)
        StatusIndication_SetItemText(sb, 0x0C, EMPTY_TEXTID);
    }
    if(cfg_remove_date || orig_date_remove)
    {
        //0x0D - At the right bottom (place of date)
        StatusIndication_SetItemText(sb, 0x0D, EMPTY_TEXTID);
    }
  }
  Timer_ReSet(&timer_remover2,cfg_refresh_time2,onTimer_remover2,0);
}

void UpdateTimeDateInfoParams(void)
{
    TEXTID strID;
    int font_old;
    int clockHeight, secondHeight;
    int clockdWidth, secondWidth, ampmWidth;

    if (cfg_clock_show)
    {
      font_old = SetFont(cfg_clock_font);
      strID = TextID_Create(L"00:00",ENC_UCS2,5);
      clockHeight = GetImageHeight(strID);
      clockdWidth = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
      TextID_Destroy(strID);

      if(cfg_clock_format == 0)
        REQUEST_TIMEFORMAT_GET(SYNC, &ClockFormat);
      else
        ClockFormat = cfg_clock_format - 1;

      if(cfg_seconds)
      {
        SetFont(cfg_seconds_font);
        strID = TextID_Create(L":00",ENC_UCS2,3);
        secondHeight = GetImageHeight(strID);
        secondWidth = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
        TextID_Destroy(strID);
        second_y = cfg_clock_y + clockHeight - secondHeight + cfg_seconds_offset_y;
      }
      else
      {
        second_y = cfg_clock_y;
        secondWidth = 0;
      }

      if(ClockFormat == 0)
      {
        SetFont(cfg_clock_font);
        strID = TextID_Create(L" pm",ENC_UCS2,3);
        ampmWidth = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
        TextID_Destroy(strID);
      }
      else
      {
        ampmWidth = 0;
      }
      SetFont(font_old);

      if(cfg_clock_align == RIGHT)
      {
        clock_x1 = second_x1 = ampm_x1 = 1;
        ampm_x2 = cfg_clock_x;
        second_x2 = ampm_x2 - ampmWidth;
        clock_x2 = second_x2 - secondWidth;
      }
      else if(cfg_clock_align == LEFT)
      {
        clock_x1 = cfg_clock_x;
        second_x1 = clock_x1 + clockdWidth;
        ampm_x1 = second_x1 + secondWidth;
        clock_x2 = second_x2 = ampm_x2 = DisplayWidth;
      }
      else
      {
        clock_x1 = cfg_clock_x;
        second_x1 = clock_x1 + clockdWidth;
        ampm_x1 = second_x1 + secondWidth;
        ampm_x2 = DisplayWidth;
        second_x2 = ampm_x2 - ampmWidth;
        clock_x2 = second_x2 - secondWidth;
      }
    }

    if (cfg_clock2_show)
    {
      font_old = SetFont(cfg_clock2_font);
      strID = TextID_Create(L"00:00",ENC_UCS2,5);
      clockHeight = GetImageHeight(strID);
      clockdWidth = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
      TextID_Destroy(strID);

      if(cfg_clock2_format == 0)
        REQUEST_TIMEFORMAT_GET(SYNC, &Clock2Format);
      else
        Clock2Format = cfg_clock2_format - 1;

      if(cfg_seconds2)
      {
        SetFont(cfg_seconds2_font);
        strID = TextID_Create(L":00",ENC_UCS2,3);
        secondHeight = GetImageHeight(strID);
        secondWidth = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
        TextID_Destroy(strID);
        second2_y = cfg_clock2_y + clockHeight - secondHeight + cfg_seconds2_offset_y;
      }
      else
      {
        second2_y = cfg_clock2_y;
        secondWidth = 0;
      }

      if(Clock2Format == 0)
      {
        SetFont(cfg_clock2_font);
        strID = TextID_Create(L" pm",ENC_UCS2,3);
        ampmWidth = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
        TextID_Destroy(strID);
      }
      else
      {
        ampmWidth = 0;
      }
      SetFont(font_old);

      if(cfg_clock2_align == RIGHT)
      {
        clock2_x1 = second2_x1 = ampm2_x1 = 1;
        ampm2_x2 = cfg_clock2_x;
        second2_x2 = ampm2_x2 - ampmWidth;
        clock2_x2 = second2_x2 - secondWidth;
      }
      else if(cfg_clock2_align == LEFT)
      {
        clock2_x1 = cfg_clock2_x;
        second2_x1 = clock2_x1 + clockdWidth;
        ampm2_x1 = second2_x1 + secondWidth;
        clock2_x2 = second2_x2 = ampm2_x2 = DisplayWidth;
      }
      else
      {
        clock2_x1 = cfg_clock2_x;
        second2_x1 = clock2_x1 + clockdWidth;
        ampm2_x1 = second2_x1 + secondWidth;
        ampm2_x2 = DisplayWidth;
        second2_x2 = ampm2_x2 - ampmWidth;
        clock2_x2 = second2_x2 - secondWidth;
      }
    }

    UpdateBinClockParams();

    if(cfg_remove_clock || cfg_remove_date || orig_clock_remove || orig_date_remove)
    {
      Timer_ReSet(&timer_remover2,cfg_refresh_time2,onTimer_remover2,0);
    }
    else if(timer_remover2)
    {
      Timer_Kill(&timer_remover2);
      timer_remover2 = 0;
    }

    if(cfg_clock_show || cfg_clock2_show || cfg_clock3_show ||
       cfg_date_show || cfg_day_show)
    {
      Timer_ReSet(&timer_clock,cfg_refresh_clock,onTimer_clock,0);
    }
    else if(timer_clock)
    {
      Timer_Kill(&timer_clock);
      timer_clock = 0;
    }
}

void UpdateTimeDateShowParams(void)
{
    clock_show = cfg_clock_show && (!(PlayerIsStarted && cfg_clock_not_show_if_player));
    clock2_show = cfg_clock2_show && (!(PlayerIsStarted && cfg_clock2_not_show_if_player));
    date_show = cfg_date_show && (!(PlayerIsStarted && cfg_date_not_show_if_player));
    day_show = cfg_day_show && (!(PlayerIsStarted && cfg_day_not_show_if_player));

    UpdateBinClockShowParams();

    orig_clock_remove = PlayerIsStarted && cfg_orig_clock_not_show_if_player;
    orig_date_remove  = PlayerIsStarted && cfg_orig_date_not_show_if_player;

    if(orig_clock_remove || orig_date_remove)
    {
      if(!timer_remover2) Timer_ReSet(&timer_remover2,cfg_refresh_time2,onTimer_remover2,0);
    }
    else if(timer_remover2)
    {
      if(!(cfg_remove_clock || cfg_remove_date))
      {
        Timer_Kill(&timer_remover2);
        timer_remover2 = 0;
      }
    }
}

void FreeTimeDateInfoParams(void)
{
    if(timer_clock)
    {
      Timer_Kill(&timer_clock);
      timer_clock = 0;
    }
    if(timer_remover2)
    {
      Timer_Kill(&timer_remover2);
      timer_remover2 = 0;
    }

    if(Clock0ID != EMPTY_TEXTID)
    {
      TextID_Destroy(Clock0ID);
      Clock0ID = EMPTY_TEXTID;
    }
    if(ClockID != EMPTY_TEXTID)
    {
      TextID_Destroy(ClockID);
      ClockID = EMPTY_TEXTID;
    }
    if(SecondID != EMPTY_TEXTID)
    {
      TextID_Destroy(SecondID);
      SecondID = EMPTY_TEXTID;
    }
    if(AmPmID != EMPTY_TEXTID)
    {
      TextID_Destroy(AmPmID);
      AmPmID = EMPTY_TEXTID;
    }

    if(DateID != EMPTY_TEXTID)
    {
      TextID_Destroy(DateID);
      DateID = EMPTY_TEXTID;
    }
    if(DayID != EMPTY_TEXTID)
    {
      TextID_Destroy(DayID);
      DayID = EMPTY_TEXTID;
    }

    FreeBinClockParams();
}


