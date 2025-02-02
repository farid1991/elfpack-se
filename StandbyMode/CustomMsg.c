#include "..\include\Lib_Clara.h"
#include "CustomMsg.h"
#include "config_data.h"
#include "main.h"
#include "MediaInfo.h"

const wchar_t *custom_msg[CUSTOM_MSG_COUNT] = {
  cfg_msg1,
  cfg_msg2,
  cfg_msg3,
  cfg_msg4,
  cfg_msg5,
  cfg_msg6
};

TEXTID custom_msgID[CUSTOM_MSG_COUNT] = {EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID};
int msg_show[CUSTOM_MSG_COUNT] = {0,0,0,0,0,0};


const int *cfg_msg_show[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_show,
  &cfg_msg2_show,
  &cfg_msg3_show,
  &cfg_msg4_show,
  &cfg_msg5_show,
  &cfg_msg6_show
};

const unsigned int *cfg_msg_font[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_font,
  &cfg_msg2_font,
  &cfg_msg3_font,
  &cfg_msg4_font,
  &cfg_msg5_font,
  &cfg_msg6_font
};

const int *cfg_msg_highlight[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_highlight,
  &cfg_msg2_highlight,
  &cfg_msg3_highlight,
  &cfg_msg4_highlight,
  &cfg_msg5_highlight,
  &cfg_msg6_highlight
};

const unsigned int *cfg_msg_color[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_color,
  &cfg_msg2_color,
  &cfg_msg3_color,
  &cfg_msg4_color,
  &cfg_msg5_color,
  &cfg_msg6_color
};

const unsigned int *cfg_msg_border[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_border,
  &cfg_msg2_border,
  &cfg_msg3_border,
  &cfg_msg4_border,
  &cfg_msg5_border,
  &cfg_msg6_border
};

const int *cfg_msg_align[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_align,
  &cfg_msg2_align,
  &cfg_msg3_align,
  &cfg_msg4_align,
  &cfg_msg5_align,
  &cfg_msg6_align
};

const unsigned int *cfg_msg_x[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_x,
  &cfg_msg2_x,
  &cfg_msg3_x,
  &cfg_msg4_x,
  &cfg_msg5_x,
  &cfg_msg6_x
};

const unsigned int *cfg_msg_y[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_y,
  &cfg_msg2_y,
  &cfg_msg3_y,
  &cfg_msg4_y,
  &cfg_msg5_y,
  &cfg_msg6_y
};

const int *cfg_msg_not_show_if_player[CUSTOM_MSG_COUNT] = {
  &cfg_msg1_not_show_if_player,
  &cfg_msg2_not_show_if_player,
  &cfg_msg3_not_show_if_player,
  &cfg_msg4_not_show_if_player,
  &cfg_msg5_not_show_if_player,
  &cfg_msg6_not_show_if_player
};

void ShowCustomMsgInfo(int y, int location)
{
  for(int i=0; i<CUSTOM_MSG_COUNT; i++)
  {
   if (msg_show[i])
   {
     if(*cfg_msg_align[i] == RIGHT)
        myDrawString(*cfg_msg_font[i], custom_msgID[i], *cfg_msg_align[i], 1, *cfg_msg_y[i] - y, *cfg_msg_x[i], DisplayHeight, *cfg_msg_border[i], *cfg_msg_color[i], *cfg_msg_highlight[i]);
     else
        myDrawString(*cfg_msg_font[i], custom_msgID[i], *cfg_msg_align[i], *cfg_msg_x[i], *cfg_msg_y[i] - y, DisplayWidth, DisplayHeight, *cfg_msg_border[i], *cfg_msg_color[i], *cfg_msg_highlight[i]);
   }
  }
}

void UpdateCustomMsgInfoParams(void)
{
  FreeCustomMsgInfoParams();
  for(int i=0; i<CUSTOM_MSG_COUNT; i++)
  {
    custom_msgID[i] = TextID_Create(custom_msg[i],ENC_UCS2,TEXTID_ANY_LEN);
  }
}

void UpdateCustomMsgShowParams(void)
{
    for(int i=0; i<CUSTOM_MSG_COUNT; i++)
      msg_show[i] = *cfg_msg_show[i] && (!(PlayerIsStarted && *cfg_msg_not_show_if_player[i]));
}

void FreeCustomMsgInfoParams(void)
{
    for(int i=0; i < CUSTOM_MSG_COUNT; i++)
    {
      if(custom_msgID[i] != EMPTY_TEXTID) TextID_Destroy(custom_msgID[i]);
      custom_msgID[i] = EMPTY_TEXTID;
    }
}


