#include "..\include\Lib_Clara.h"
#include "OperatorInfo.h"
#include "config_data.h"
#include "main.h"
#include "MediaInfo.h"

const int cfg_operator_name_show = 0;
const unsigned int cfg_operator_name_font = 0xD;
const int cfg_operator_name_highlight = 0;
const unsigned int cfg_operator_name_color=0xFFFFFFFF;
const unsigned int cfg_operator_name_border=0xFF000000;
const int cfg_operator_name_align = 0;
const unsigned int cfg_operator_name_x = 5;
const unsigned int cfg_operator_name_y = 200;

const int cfg_refresh_operator_name = 2000;

const int cfg_operator_name_not_show_if_player = 1;

TEXTID operator_nameID = EMPTY_TEXTID;
int operator_name_show = 0;

u16 timer_operator = 0;
u16 timer_remover = 0;

bool remove_operator_1st_line = false;

int operator_1st_line_remove = 0;
int operator_2nd_line_remove = 0;
int operator_3rd_line_remove = 0;

//TEXTID (*getOperatorName)(void *hz)=(TEXTID (*)(void *hz))(0x44DACC98 + 1);

void UpdateOperatorInfo(void)
{
/*
  TEXTID buf[1];

  char hz[1];
  buf[0] = getOperatorName(hz);

  if(operator_nameID != EMPTY_TEXTID) TextID_Destroy(operator_nameID);
  operator_nameID = TextID_Create(buf,ENC_TEXTID,1);
*/
}

void ShowOperatorInfo(int y, int location)
{
  if (operator_name_show)
  {
    if(cfg_operator_name_align == RIGHT)
      myDrawString(cfg_operator_name_font, operator_nameID, cfg_operator_name_align, 1, cfg_operator_name_y - y, cfg_operator_name_x, DisplayHeight, cfg_operator_name_border, cfg_operator_name_color, cfg_operator_name_highlight);
    else
      myDrawString(cfg_operator_name_font, operator_nameID, cfg_operator_name_align, cfg_operator_name_x, cfg_operator_name_y - y, DisplayWidth, DisplayHeight, cfg_operator_name_border, cfg_operator_name_color, cfg_operator_name_highlight);
  }
}

void onTimer_operator (u16 timerID, LPARAM lparam)
{
    UpdateOperatorInfo();
    Timer_ReSet(&timer_operator,cfg_refresh_operator_name,onTimer_operator,0);
}

void onTimer_remover (u16 timerID, LPARAM lparam)
{
  if(SleepModeIsDeactivated && IsInStandby)
  {
    GUI *sb = GetStandbyBookGui();
    if(cfg_remove_operator_1st_line || operator_1st_line_remove)
    {
        //0x01 - the first operator line
        StatusIndication_SetItemText(sb, 0x01, TextID_Create(L" ", ENC_UCS2, TEXTID_ANY_LEN ));
    }
    if(cfg_remove_operator_2nd_line || operator_2nd_line_remove)
    {
        //0x06 - the second operator line
        StatusIndication_SetItemText(sb, 0x06, EMPTY_TEXTID);
    }
    if(cfg_remove_operator_3rd_line || operator_3rd_line_remove)
    {
        //0x00 - the third operator line
        StatusIndication_SetItemText(sb, 0x00, EMPTY_TEXTID);
    }
  }
  Timer_ReSet(&timer_remover,cfg_refresh_time,onTimer_remover,0);
}

void UpdateOperatorInfoParams(void)
{
    if(cfg_operator_name_show)
    {
      UpdateOperatorInfo();
      Timer_ReSet(&timer_operator,cfg_refresh_operator_name,onTimer_operator,0);
    }
    else if(timer_operator)
    {
      Timer_Kill(&timer_operator);
      timer_operator = 0;
    }

    if(cfg_remove_operator_1st_line || operator_1st_line_remove)
    {
      remove_operator_1st_line = true;
    }
    else
    {
      if(remove_operator_1st_line)
      {
        //0x01 - the first operator line
        StatusIndication_SetItemText(GetStandbyBookGui(), 0x01, EMPTY_TEXTID);
      }
      remove_operator_1st_line = false;
    }

    if(cfg_remove_operator_1st_line || cfg_remove_operator_2nd_line || cfg_remove_operator_3rd_line ||
       operator_1st_line_remove || operator_2nd_line_remove || operator_3rd_line_remove)
    {
      Timer_ReSet(&timer_remover,cfg_refresh_time,onTimer_remover,0);
    }
    else if(timer_remover)
    {
      Timer_Kill(&timer_remover);
      timer_remover = 0;
    }
}

void UpdateOperatorShowParams(void)
{
    operator_name_show = cfg_operator_name_show && (!(PlayerIsStarted && cfg_operator_name_not_show_if_player));

    operator_1st_line_remove  = PlayerIsStarted && cfg_remove_operator_1st_line_if_player;
    operator_2nd_line_remove  = PlayerIsStarted && cfg_remove_operator_2nd_line_if_player;
    operator_3rd_line_remove  = PlayerIsStarted && cfg_remove_operator_3rd_line_if_player;

    if(cfg_remove_operator_1st_line || operator_1st_line_remove)
    {
      remove_operator_1st_line = true;
    }
    else
    {
      if(remove_operator_1st_line)
      {
        //0x01 - the first operator line
        StatusIndication_SetItemText(GetStandbyBookGui(), 0x01, EMPTY_TEXTID);
      }
      remove_operator_1st_line = false;
    }

    if(operator_1st_line_remove || operator_2nd_line_remove || operator_3rd_line_remove)
    {
      if(!timer_remover) Timer_ReSet(&timer_remover,cfg_refresh_time,onTimer_remover,0);
    }
    else if(timer_remover)
    {
      if(!(cfg_remove_operator_1st_line || cfg_remove_operator_2nd_line || cfg_remove_operator_3rd_line))
      {
        Timer_Kill(&timer_remover);
        timer_remover = 0;
      }
    }
}

void FreeOperatorInfoParams(void)
{
    if(timer_operator)
    {
      Timer_Kill(&timer_operator);
      timer_operator = 0;
    }
    if(operator_nameID != EMPTY_TEXTID)
    {
      TextID_Destroy(operator_nameID);
      operator_nameID = EMPTY_TEXTID;
    }

    if(timer_remover)
    {
      Timer_Kill(&timer_remover);
      timer_remover = 0;
    }
    if(remove_operator_1st_line)
    {
        //0x01 - the first operator line
        StatusIndication_SetItemText(GetStandbyBookGui(), 0x01, EMPTY_TEXTID);
        remove_operator_1st_line = false;
    }
}


