#include "..\include\Lib_Clara.h"
#include "MemInfo.h"
#include "config_data.h"
#include "main.h"
#include "MediaInfo.h"
#include "conf_loader.h"

wchar_t card_mem_msg[96] = {0,};
wchar_t phone_mem_msg[96] = {0,};

TEXTID card_mem_msgID  = EMPTY_TEXTID;
TEXTID phone_mem_msgID = EMPTY_TEXTID;

RECT card_used_bar;
RECT phone_used_bar;

int phone_show = 0;
int card_show = 0;

u16 timer_memInfo = 0;

void UpdateMemInfo(void)
{
    wchar_t buf[32], *fmt;
    float Full,Free;
    float k1,k2,n,m;

    /*--------[ Card ]------------------*/
    switch( ChipID )
    {
      case CHIPID_DB2000:
      case CHIPID_DB2010:
      case CHIPID_DB2020:
      case CHIPID_PNX5230:
    {
      VOLUMESIZE vol;
      GetVolumeSize( L"/card", &vol );
      //Full = (unsigned int) vol.TotalClusters;
      //Free = (unsigned int) vol.TotalFreeClusters;
	  Full = (long long)vol.TotalSpace;
      Free = (long long)vol.FreeSpace;
      break;
    }
      case CHIPID_DB3150:
    {
      if(phoneType == K850_R1FA035 || phoneType == W890_R1FA035 || phoneType == W910_R1FA037)
      {
        VOLUMESIZE vol;
        GetVolumeSize( L"/card", &vol );
        Full = (unsigned int) vol.TotalClusters;
        Free = (unsigned int) vol.TotalFreeClusters;
      }
       else
      {
        VOLUMESIZE_A2 vol;
        GetVolumeSize( L"/card", (VOLUMESIZE*)&vol );
        Full = (long long)vol.TotalSpace;
        Free = (long long)vol.FreeSpace;
      }
      break;
    }
      default:
      VOLUMESIZE_A2 vol;
      GetVolumeSize( L"/card", (VOLUMESIZE*)&vol );
      Full = (long long)vol.TotalSpace;
      Free = (long long)vol.FreeSpace;
      break;
    }
    k1 = Free/Full;
    k2 = (Full - Free)/Full;
    if((Free == 0) && (Full == 0)) {k1=1; k2=1;}

    switch(cfg_card_fractional){
      case ZERO_CHAR:
        fmt = L"%.0f";
        break;
      case ONE_CHAR:
        fmt = L"%.1f";
        break;
      case TWO_CHAR:
        fmt = L"%.2f";
        break;
      default:
        fmt = L"%.3f";
    }

    switch(cfg_card_format){
        case FREE_PERCENT:
            if(Full == 0) Full = 1.0;
            snwprintf(buf,MAXELEMS(buf)-1,L"%d%%", (int)((Free/Full)*100.0));
            break;
        case FREE_MB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, Free/1024.0/1024.0);
            wstrcat(buf,L"MB");
            break;
        case FREE_GB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, Free/1024.0/1024.0/1024.0);
            wstrcat(buf,L"GB");
            break;
        case USED_PERCENT:
            if(Full == 0) Full = 1.0;
            snwprintf(buf,MAXELEMS(buf)-1,L"%d%%", (int)(((Full - Free)/Full)*100.0));
            break;
        case USED_MB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, (Full - Free)/1024.0/1024.0);
            wstrcat(buf,L"MB");
            break;
        case USED_GB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, (Full - Free)/1024.0/1024.0/1024.0);
            wstrcat(buf,L"GB");
            break;
        default: // NOT_SHOW
            buf[0] = 0;
    }
    wstrcpy(card_mem_msg,cfg_card_msg);
    wstrcat(card_mem_msg,buf);
    if(card_mem_msgID != EMPTY_TEXTID)  TextID_Destroy(card_mem_msgID);
    card_mem_msgID = TextID_Create(card_mem_msg,ENC_UCS2,MAXELEMS(card_mem_msg));

    n = cfg_card_bar.x2 - cfg_card_bar.x1;
    m = cfg_card_bar.y2 - cfg_card_bar.y1;
    if(n < m)
    {
      card_used_bar.x1 = cfg_card_bar.x1;
      card_used_bar.y1 = cfg_card_bar.y1 + m*k1;
      card_used_bar.x2 = cfg_card_bar.x2;
      card_used_bar.y2 = cfg_card_bar.y2;
    }
    else
    {
      card_used_bar.x1 = cfg_card_bar.x1;
      card_used_bar.y1 = cfg_card_bar.y1;
      card_used_bar.x2 = cfg_card_bar.x1 + n*k2;
      card_used_bar.y2 = cfg_card_bar.y2;
    }

    /*--------[ Phone ]------------------*/
    switch( ChipID )
    {
      case CHIPID_DB2000:
      case CHIPID_DB2010:
      case CHIPID_DB2020:
      case CHIPID_PNX5230:
    {
      VOLUMESIZE vol;
      GetVolumeSize( L"/tpa", &vol );
      //Full = (unsigned int) vol.TotalClusters;
      //Free = (unsigned int) vol.TotalFreeClusters;
	  Full = (long long)vol.TotalSpace;
      Free = (long long)vol.FreeSpace;
      break;
    }
      case CHIPID_DB3150:
    {
      if(phoneType == K850_R1FA035 || phoneType == W890_R1FA035 || phoneType == W910_R1FA037)
      {
        VOLUMESIZE vol;
        GetVolumeSize( L"/tpa", &vol );
        Full = (unsigned int) vol.TotalClusters;
        Free = (unsigned int) vol.TotalFreeClusters;
      }
       else
      {
        VOLUMESIZE_A2 vol;
        GetVolumeSize( L"/tpa", (VOLUMESIZE*)&vol );
        Full = (long long)vol.TotalSpace;
        Free = (long long)vol.FreeSpace;
      }
      break;
    }
      default:
      VOLUMESIZE_A2 vol;
      GetVolumeSize( L"/tpa", (VOLUMESIZE*)&vol );
      Full = (long long)vol.TotalSpace;
      Free = (long long)vol.FreeSpace;
      break;
    }
    k1 = Free/Full;
    k2 = (Full - Free)/Full;
    if((Free == 0) && (Full == 0)) {k1=1; k2=1;}

    switch(cfg_phone_fractional){
      case ZERO_CHAR:
        fmt = L"%.0f";
        break;
      case ONE_CHAR:
        fmt = L"%.1f";
        break;
      case TWO_CHAR:
        fmt = L"%.2f";
        break;
      default:
        fmt = L"%.3f";
    }

    switch(cfg_phone_format){
        case FREE_PERCENT:
            if(Full == 0) Full = 1.0;
            snwprintf(buf,MAXELEMS(buf)-1,L"%d%%", (int)((Free/Full)*100.0));
            break;
        case FREE_MB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, Free/1024.0/1024.0);
            wstrcat(buf,L"MB");
            break;
        case FREE_GB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, Free/1024.0/1024.0/1024.0);
            wstrcat(buf,L"GB");
            break;
        case USED_PERCENT:
            if(Full == 0) Full = 1.0;
            snwprintf(buf,MAXELEMS(buf)-1,L"%d%%", (int)(((Full - Free)/Full)*100.0));
            break;
        case USED_MB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, (Full - Free)/1024.0/1024.0);
            wstrcat(buf,L"MB");
            break;
        case USED_GB:
            snwprintf(buf,MAXELEMS(buf)-1, fmt, (Full - Free)/1024.0/1024.0/1024.0);
            wstrcat(buf,L"GB");
            break;
        default: // NOT_SHOW
            buf[0] = 0;
    }
    wstrcpy(phone_mem_msg,cfg_phone_msg);
    wstrcat(phone_mem_msg,buf);
    if(phone_mem_msgID != EMPTY_TEXTID)  TextID_Destroy(phone_mem_msgID);
    phone_mem_msgID = TextID_Create(phone_mem_msg,ENC_UCS2,MAXELEMS(phone_mem_msg));

    n = cfg_phone_bar.x2 - cfg_phone_bar.x1;
    m = cfg_phone_bar.y2 - cfg_phone_bar.y1;
    if(n < m)
    {
      phone_used_bar.x1 = cfg_phone_bar.x1;
      phone_used_bar.y1 = cfg_phone_bar.y1 + m*k1;
      phone_used_bar.x2 = cfg_phone_bar.x2;
      phone_used_bar.y2 = cfg_phone_bar.y2;
    }
    else
    {
      phone_used_bar.x1 = cfg_phone_bar.x1;
      phone_used_bar.y1 = cfg_phone_bar.y1;
      phone_used_bar.x2 = cfg_phone_bar.x1 + n*k2;
      phone_used_bar.y2 = cfg_phone_bar.y2;
    }
}


void ShowMemInfo(int y, int location)
{
   if(cfg_crc == REF_CRC32)
   if(card_show && cfg_card_bar_show)
   {
      if(cfg_card_bar_highlight)
      {
          DrawOwnRect(cfg_card_bar.x1, cfg_card_bar.y1-y, cfg_card_bar.x2, cfg_card_bar.y2-y, cfg_card_bar_border,cfg_card_free_color);
          DrawOwnRect(card_used_bar.x1, card_used_bar.y1-y, card_used_bar.x2, card_used_bar.y2-y, cfg_card_bar_border,cfg_card_used_color);
      }
      else
      {
          DrawOwnRect(cfg_card_bar.x1, cfg_card_bar.y1-y, cfg_card_bar.x2, cfg_card_bar.y2-y, cfg_card_free_color,cfg_card_free_color);
          DrawOwnRect(card_used_bar.x1, card_used_bar.y1-y, card_used_bar.x2, card_used_bar.y2-y, cfg_card_used_color,cfg_card_used_color);
      }
   }
   if(cfg_crc == REF_CRC32)
   if(card_show)
   {
      if(cfg_card_align == RIGHT)
          myDrawString(cfg_card_font, card_mem_msgID, cfg_card_align, 1, cfg_card_y - y, cfg_card_x, DisplayHeight, cfg_card_border, cfg_card_color, cfg_card_highlight);
      else
          myDrawString(cfg_card_font, card_mem_msgID, cfg_card_align, cfg_card_x, cfg_card_y - y, DisplayWidth, DisplayHeight, cfg_card_border, cfg_card_color, cfg_card_highlight);
   }
   if(cfg_crc == REF_CRC32)
   if(phone_show && cfg_phone_bar_show)
   {
      if(cfg_phone_bar_highlight)
      {
          DrawOwnRect(cfg_phone_bar.x1, cfg_phone_bar.y1-y, cfg_phone_bar.x2, cfg_phone_bar.y2-y, cfg_phone_bar_border,cfg_phone_free_color);
          DrawOwnRect(phone_used_bar.x1, phone_used_bar.y1-y, phone_used_bar.x2, phone_used_bar.y2-y, cfg_phone_bar_border,cfg_phone_used_color);
      }
      else
      {
          DrawOwnRect(cfg_phone_bar.x1, cfg_phone_bar.y1-y, cfg_phone_bar.x2, cfg_phone_bar.y2-y, cfg_phone_free_color,cfg_phone_free_color);
          DrawOwnRect(phone_used_bar.x1, phone_used_bar.y1-y, phone_used_bar.x2, phone_used_bar.y2-y, cfg_phone_used_color,cfg_phone_used_color);
      }
   }
   if(cfg_crc == REF_CRC32)
   if(phone_show)
   {
      if(cfg_phone_align == RIGHT)
          myDrawString(cfg_phone_font, phone_mem_msgID, cfg_phone_align, 1, cfg_phone_y - y, cfg_phone_x, DisplayHeight, cfg_phone_border, cfg_phone_color, cfg_phone_highlight);
      else
          myDrawString(cfg_phone_font, phone_mem_msgID, cfg_phone_align, cfg_phone_x, cfg_phone_y - y, DisplayWidth, DisplayHeight, cfg_phone_border, cfg_phone_color, cfg_phone_highlight);
   }
}

void onTimer_memInfo (u16 timerID, LPARAM lparam)
{
    UpdateMemInfo();
    Timer_ReSet(&timer_memInfo,cfg_mem_update*1000,onTimer_memInfo,0);
}

void UpdateMemInfoParams(void)
{

    if(cfg_phone_show || cfg_card_show)
    {
      UpdateMemInfo();
      Timer_ReSet(&timer_memInfo,cfg_mem_update*1000,onTimer_memInfo,0);
    }
    else if(timer_memInfo)
    {
      Timer_Kill(&timer_memInfo);
      timer_memInfo = 0;
    }

}

void UpdateMemShowParams(void)
{
    phone_show = cfg_phone_show && (!(PlayerIsStarted && cfg_phone_not_show_if_player));
    card_show = cfg_card_show && (!(PlayerIsStarted && cfg_card_not_show_if_player));
}

void FreeMemInfoParams(void)
{
    if(timer_memInfo)
    {
      Timer_Kill(&timer_memInfo);
      timer_memInfo = 0;
    }

    if(card_mem_msgID != EMPTY_TEXTID)
    {
      TextID_Destroy(card_mem_msgID);
      card_mem_msgID = EMPTY_TEXTID;
    }
    if(phone_mem_msgID != EMPTY_TEXTID)
    {
      TextID_Destroy(phone_mem_msgID);
      phone_mem_msgID = EMPTY_TEXTID;
    }
}


