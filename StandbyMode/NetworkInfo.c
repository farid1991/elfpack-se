#include "..\include\Lib_Clara.h"
#include "NetworkInfo.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"
#include "MediaInfo.h"


/*
//C901
#define Book_A2 0x4BA13A56;
//W760
//#define Book_A2 0x4BB91582;

if ((GetChipID()&CHIPID_MASK)==CHIPID_DB2020)
{
STANDBY_DB2020 *st = (STANDBY_DB2020 *)Find_StandbyBook();
return st->Level;
}
STANDBY_DBA2 *st = (STANDBY_DBA2 *)Book_A2;
return st->Level;
}

*/

NetInfoAddr_t NetInfoAddr = {0};

IMAGEID net_image = NOIMAGE;
u16 timer_networkInfo = 0;
int network_show = 0;

TEXTID rssiID = EMPTY_TEXTID;
int network_rssi_show = 0;

TEXTID rssi_percentID = EMPTY_TEXTID;
int network_rssi_percent_show = 0;

const wchar_t *net_none_file[NET_IMAGE_COUNT] = {
  cfg_net_none_0_file,
  cfg_net_none_1_file,
  cfg_net_none_2_file,
  cfg_net_none_3_file,
  cfg_net_none_4_file,
  cfg_net_none_5_file
};

const wchar_t *net_edge_file[NET_IMAGE_COUNT] = {
  cfg_net_edge_0_file,
  cfg_net_edge_1_file,
  cfg_net_edge_2_file,
  cfg_net_edge_3_file,
  cfg_net_edge_4_file,
  cfg_net_edge_5_file
};

const wchar_t *net_act_edge_file[NET_IMAGE_COUNT] = {
  cfg_net_act_edge_0_file,
  cfg_net_act_edge_1_file,
  cfg_net_act_edge_2_file,
  cfg_net_act_edge_3_file,
  cfg_net_act_edge_4_file,
  cfg_net_act_edge_5_file
};

const wchar_t *net_gprs_file[NET_IMAGE_COUNT] = {
  cfg_net_gprs_0_file,
  cfg_net_gprs_1_file,
  cfg_net_gprs_2_file,
  cfg_net_gprs_3_file,
  cfg_net_gprs_4_file,
  cfg_net_gprs_5_file
};

const wchar_t *net_act_gprs_file[NET_IMAGE_COUNT] = {
  cfg_net_act_gprs_0_file,
  cfg_net_act_gprs_1_file,
  cfg_net_act_gprs_2_file,
  cfg_net_act_gprs_3_file,
  cfg_net_act_gprs_4_file,
  cfg_net_act_gprs_5_file
};

IMAGEID none_images[NET_IMAGE_COUNT]     = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
IMAGEID edge_images[NET_IMAGE_COUNT]     = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
IMAGEID act_edge_images[NET_IMAGE_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
IMAGEID gprs_images[NET_IMAGE_COUNT]     = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
IMAGEID act_gprs_images[NET_IMAGE_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
IMAGEID flight_image = NOIMAGE;

static char getLevel(void)
{
  if(phoneType == W710_R1JC002)
  {
    return NetInfoAddr.W710->Level;
  }
  if(phoneType == W300_R4EA031)
  {
    return NetInfoAddr.W300->Level;
  }
  if(phoneType == K750_R1CA021)
  {
    return NetInfoAddr.K750->Level;
  }
  if (ChipID == CHIPID_DB2010) //Для дб2010:
  {
    return NetInfoAddr.DB2010->Level;
  }
  if (ChipID == CHIPID_DB2020)
  {
    return NetInfoAddr.DB2020->Level;
  }
  return NetInfoAddr.DBA2->Level;;
}

static wchar_t getCurrentIconID(void)
{
  if(phoneType == W710_R1JC002)
  {
    return NetInfoAddr.W710->CurrentIconID;
  }
  if(phoneType == W300_R4EA031)
  {
    return NetInfoAddr.W300->CurrentIconID;
  }
  if(phoneType == K750_R1CA021)
  {
    return NetInfoAddr.K750->CurrentIconID;
  }
  if (ChipID == CHIPID_DB2010) //Для дб2010:
  {
    return NetInfoAddr.DB2010->CurrentIconID;
  }
  if (ChipID == CHIPID_DB2020)
  {
    return NetInfoAddr.DB2020->CurrentIconID;
  }
  return NetInfoAddr.DBA2->CurrentIconID;;
}

static char getTypeActive(void)
{
  if(phoneType == W710_R1JC002)
  {
    return NetInfoAddr.W710->TypeActive;
  }
  if(phoneType == W300_R4EA031)
  {
    return NetInfoAddr.W300->TypeActive;
  }
  if(phoneType == K750_R1CA021)
  {
    return NetInfoAddr.K750->TypeActive;
  }
  if (ChipID == CHIPID_DB2010) //Для дб2010:
  {
    return NetInfoAddr.DB2010->TypeActive;
  }
  if (ChipID == CHIPID_DB2020)
  {
    return NetInfoAddr.DB2020->TypeActive;
  }
  return NetInfoAddr.DBA2->TypeActive;
}

static char isFlightNotActive(void)
{
  if(phoneType == W710_R1JC002)
  {
    return NetInfoAddr.W710->isFlightNotActive;
  }
  if(phoneType == W300_R4EA031)
  {
    return NetInfoAddr.W300->isFlightNotActive;
  }
  if((phoneType == K750_R1CA021) || (phoneType == W800_R1BD001))
  {
    return 1;
  }
  if (ChipID == CHIPID_DB2010) //Для дб2010:
  {
    return NetInfoAddr.DB2010->isFlightNotActive;
  }
  if (ChipID == CHIPID_DB2020)
  {
    return NetInfoAddr.DB2020->isFlightNotActive;
  }
  return 1;
}

static char getType(void)
{
  if(phoneType == W710_R1JC002)
  {
    return NetInfoAddr.W710->Type;
  }
  if(phoneType == W300_R4EA031)
  {
    return NetInfoAddr.W300->Type;
  }
  if((phoneType == K750_R1CA021) || (phoneType == W800_R1BD001))
  {
    return 1;
  }
  if (ChipID == CHIPID_DB2010) //Для дб2010:
  {
    return NetInfoAddr.DB2010->Type;
  }
  if (ChipID == CHIPID_DB2020)
  {
    return NetInfoAddr.DB2020->Type;
  }
  return NetInfoAddr.DBA2->Type;
}

void UpdateNetworkInfo(void)
{
  if(cfg_network_rssi_show || cfg_network_rssi_percent_show)
  {
    char rssi, ber;
    GetSignalQuality(&rssi, &ber);

    if(rssi < 99)
    {
      TEXTID rssi_id[2];
      int len = 1;
      int inp = rssi-113;
      rssi_id[0] = TextID_CreateIntegerID(inp);

      if(cfg_network_rssi_dbm_show)
      {
        rssi_id[1] = STR(" dBm");
        len = 2;
      }
      if(rssiID != EMPTY_TEXTID) TextID_Destroy(rssiID);
      rssiID = TextID_Create(rssi_id, ENC_TEXTID, len);

      TEXTID rssi_percent_id[2];
      int inp_percent = (5650 + inp*50)/31;
      if(inp_percent > 100) inp_percent = 100;
      rssi_percent_id[0] = TextID_CreateIntegerID(inp_percent);
      rssi_percent_id[1] = STR("%");

      if(rssi_percentID != EMPTY_TEXTID) TextID_Destroy(rssi_percentID);
      rssi_percentID = TextID_Create(rssi_percent_id, ENC_TEXTID, 2);
    }
    else
    {
      if(rssiID != EMPTY_TEXTID) TextID_Destroy(rssiID);
      rssiID = EMPTY_TEXTID;

      if(rssi_percentID != EMPTY_TEXTID) TextID_Destroy(rssi_percentID);
      rssi_percentID = EMPTY_TEXTID;
    }
  }

  if(cfg_network_custom_images)
  {
    if(isFlightNotActive() == 0)
    {
	 switch( ChipID )
	 {
	  case CHIPID_DB2000:
	  case CHIPID_DB2010:
	  case CHIPID_DB2020:
      case CHIPID_PNX5230:
	  {
       net_image = flight_image | 0x78000000;
	   break;
	  }
	  default:
	   net_image = flight_image;
	   break;
	 }
     return;
    }
    char TypeActive = getTypeActive();
    char Type = getType();

    if(TypeActive == 2)
    {
      if(Type == 1)
      {
	   switch( ChipID )
	   {
	    case CHIPID_DB2000:
	    case CHIPID_DB2010:
	    case CHIPID_DB2020:
        case CHIPID_PNX5230:
	    {
         net_image = act_gprs_images[getLevel()] | 0x78000000;
         break;
		}
		default:
		 net_image = act_gprs_images[getLevel()];
         break;
	   }
	   return;
      }
      if(Type == 2)
      {
	   switch( ChipID )
	   {
	    case CHIPID_DB2000:
	    case CHIPID_DB2010:
	    case CHIPID_DB2020:
        case CHIPID_PNX5230:
	    {
         net_image = act_edge_images[getLevel()] | 0x78000000;
		 break;
		}
		default:
		 net_image = act_edge_images[getLevel()];
		 break;
	   }
       return;
      }
      return;
    }

    if(TypeActive == 3)
    {
      if(Type == 1)
      {
	   switch( ChipID )
	   {
	    case CHIPID_DB2000:
	    case CHIPID_DB2010:
	    case CHIPID_DB2020:
        case CHIPID_PNX5230:
	    {
         net_image = gprs_images[getLevel()] | 0x78000000;
		 break;
		}
		default:
		 net_image = gprs_images[getLevel()];
		 break;
	   }
       return;
      }
      if(Type == 2)
      {
	   switch( ChipID )
	   {
	    case CHIPID_DB2000:
	    case CHIPID_DB2010:
	    case CHIPID_DB2020:
        case CHIPID_PNX5230:
	    {
         net_image = edge_images[getLevel()] | 0x78000000;
		 break;
		}
		default:
		 net_image = edge_images[getLevel()];
		 break;
	   }
       return;
      }
      return;
    }

   switch( ChipID )
   {
    case CHIPID_DB2000:
    case CHIPID_DB2010:
    case CHIPID_DB2020:
    case CHIPID_PNX5230:
    {
     net_image = none_images[getLevel()] | 0x78000000;
	 break;
	}
	default:
	 net_image = none_images[getLevel()];
	 break;
   }
  }
}

void ShowNetworkInfo(int y, int location)
{
   if(network_show)
   {
     if(cfg_network_custom_images)
     {/*
	  switch( ChipID )
	  {
	   case CHIPID_DB2000:
	   case CHIPID_DB2010:
	   case CHIPID_DB2020:
	   case CHIPID_PNX5230:
	   {
		DrawString(net_image, 0, cfg_network_x, cfg_network_y - y, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
		break;
	   }
	   default:*/
	    GC *GC_DISP=get_DisplayGC ();
        if(net_image != NOIMAGE)
        {
		 putchar(GC_DISP, cfg_network_x, cfg_network_y - y, 0, 0, net_image);
        }
		else
        {
         putchar(GC_DISP, cfg_network_x, cfg_network_y - y, 0, 0, getCurrentIconID());
        }/*
		break;
	  }	*/
     }
     else
     {/*
	  switch( ChipID )
	  {
	   case CHIPID_DB2000:
	   case CHIPID_DB2010:
	   case CHIPID_DB2020:
	   case CHIPID_PNX5230:
	   {
		DrawString(net_image, 0, cfg_network_x, cfg_network_y - y, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
		break;
	   }
	   default:*/
	    GC *GC_DISP=get_DisplayGC ();
        putchar(GC_DISP, cfg_network_x, cfg_network_y - y, 0, 0, getCurrentIconID());/*
		break;
	  }	*/
     }
   }
   if(network_rssi_show)
   {
     if(cfg_network_rssi_align == RIGHT)
        myDrawString(cfg_network_rssi_font, rssiID, cfg_network_rssi_align, 1, cfg_network_rssi_y - y, cfg_network_rssi_x, DisplayHeight, cfg_network_rssi_border, cfg_network_rssi_color, cfg_network_rssi_highlight);
     else
        myDrawString(cfg_network_rssi_font, rssiID, cfg_network_rssi_align, cfg_network_rssi_x, cfg_network_rssi_y - y, DisplayWidth, DisplayHeight, cfg_network_rssi_border, cfg_network_rssi_color, cfg_network_rssi_highlight);
   }
   if(network_rssi_percent_show)
   {
     if(cfg_network_rssi_percent_align == RIGHT)
        myDrawString(cfg_network_rssi_percent_font, rssi_percentID, cfg_network_rssi_percent_align, 1, cfg_network_rssi_percent_y - y, cfg_network_rssi_percent_x, DisplayHeight, cfg_network_rssi_percent_border, cfg_network_rssi_percent_color, cfg_network_rssi_percent_highlight);
     else
        myDrawString(cfg_network_rssi_percent_font, rssi_percentID, cfg_network_rssi_percent_align, cfg_network_rssi_percent_x, cfg_network_rssi_percent_y - y, DisplayWidth, DisplayHeight, cfg_network_rssi_percent_border, cfg_network_rssi_percent_color, cfg_network_rssi_percent_highlight);
   }
}

void ShowNonStandbyNetworkInfo(int y, int location)
{
  ShowNetworkInfo(y, location);
}

void onTimer_networkInfo (u16 timerID, LPARAM lparam)
{
    UpdateNetworkInfo();
    Timer_ReSet(&timer_networkInfo,cfg_refresh_network,onTimer_networkInfo,0);
}
/*
void UpdateNetworkInfoImages(wchar_t *title, IMAGEID *images, const char **file, int cnt)
{
    FSTAT _fstat;

    if((cfg_network_show) && (cfg_network_custom_images))
    {
      for (int i=0;i<cnt;i++)
      {
        extractdir(tmppath, tmpname, file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          #ifdef ENG
            snwprintf(temp, MAXELEMS(temp), _T("Image for %d not found"),i);
          #else
            snwprintf(temp, MAXELEMS(temp), _T("Картинка для %d не найдена"),i);
          #endif
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&images[i])<0) images[i] = NOIMAGE;
      }
    }
}
*/
void UpdateNetworkInfoParams(void)
{
    FSTAT _fstat;

    FreeNetworkInfoImages();
/*
    #ifdef ENG
      UpdateNetworkInfoImages(_T("Network"), none_images, net_none_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Network"), edge_images, net_edge_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Network"), act_edge_images, net_act_edge_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Network"), gprs_images, net_gprs_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Network"), act_gprs_images, net_act_gprs_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Network"), &flight_image, (const char **)&cfg_net_flight_file, 1);
    #else
      UpdateNetworkInfoImages(_T("Сеть"), none_images, net_none_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Сеть"), edge_images, net_edge_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Сеть"), act_edge_images, net_act_edge_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Сеть"), gprs_images, net_gprs_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Сеть"), act_gprs_images, net_act_gprs_file, NET_IMAGE_COUNT);
      UpdateNetworkInfoImages(_T("Сеть"), &flight_image, (const char **)&cfg_net_flight_file, 1);
    #endif
*/
    if(cfg_network_show && cfg_network_custom_images)
    {
      for (int i=0;i<NET_IMAGE_COUNT;i++)
      {
        extractdir(tmppath, tmpname, net_none_file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("Нет GPRS и EDGE\nКартинка для %d не найдена"),i);
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&none_images[i])<0) none_images[i] = NOIMAGE;
      }
      if((phoneType != K750_R1CA021) && (phoneType != W800_R1BD001))
      {
        for (int i=0;i<NET_IMAGE_COUNT;i++)
        {
          extractdir(tmppath, tmpname, net_edge_file[i]);
          if(fstat(tmppath,tmpname,&_fstat)!=0)
          {
            wchar_t temp[128];
            snwprintf(temp, MAXELEMS(temp), _T("EDGE\nКартинка для %d не найдена"),i);
            MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
          }
          else if(ImageID_Get(tmppath,tmpname,&edge_images[i])<0) edge_images[i] = NOIMAGE;
        }
        for (int i=0;i<NET_IMAGE_COUNT;i++)
        {
          extractdir(tmppath, tmpname, net_act_edge_file[i]);
          if(fstat(tmppath,tmpname,&_fstat)!=0)
          {
            wchar_t temp[128];
            snwprintf(temp, MAXELEMS(temp), _T("EDGE активный\nКартинка для %d не найдена"),i);
            MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
          }
          else if(ImageID_Get(tmppath,tmpname,&act_edge_images[i])<0) act_edge_images[i] = NOIMAGE;
        }
      }
      for (int i=0;i<NET_IMAGE_COUNT;i++)
      {
        extractdir(tmppath, tmpname, net_gprs_file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("GPRS\nКартинка для %d не найдена"),i);
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&gprs_images[i])<0) gprs_images[i] = NOIMAGE;
      }
      for (int i=0;i<NET_IMAGE_COUNT;i++)
      {
        extractdir(tmppath, tmpname, net_act_gprs_file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("GPRS активный\nКартинка для %d не найдена"),i);
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&act_gprs_images[i])<0) act_gprs_images[i] = NOIMAGE;
      }
      if((phoneType != K750_R1CA021) && (phoneType != W800_R1BD001))
      {
        extractdir(tmppath, tmpname, cfg_net_flight_file);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("Картинка для flight не найдена"));
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&flight_image)<0) flight_image = NOIMAGE;
      }
    }

    if((cfg_network_show && cfg_network_custom_images) || cfg_network_rssi_show || cfg_network_rssi_percent_show)
    {
      UpdateNetworkInfo();
      Timer_ReSet(&timer_networkInfo,cfg_refresh_network,onTimer_networkInfo,0);
    }
    else if(timer_networkInfo)
    {
      Timer_Kill(&timer_networkInfo);
      timer_networkInfo = 0;
    }
}

void UpdateNetworkShowParams(void)
{
    network_show = cfg_network_show && (!(PlayerIsStarted && cfg_network_not_show_if_player));
    network_rssi_show = cfg_network_rssi_show && (!(PlayerIsStarted && cfg_network_rssi_not_show_if_player));
    network_rssi_percent_show = cfg_network_rssi_percent_show && (!(PlayerIsStarted && cfg_network_rssi_percent_not_show_if_player));
}

void FreeNetworkInfoImages(void)
{
    for (int i=0;i<NET_IMAGE_COUNT;i++)
    {
        if(none_images[i] != NOIMAGE)
        {
          ImageID_Free(none_images[i]);
          none_images[i] = NOIMAGE;
        }
        if(edge_images[i] != NOIMAGE)
        {
          ImageID_Free(edge_images[i]);
          edge_images[i] = NOIMAGE;
        }
        if(act_edge_images[i] != NOIMAGE)
        {
          ImageID_Free(act_edge_images[i]);
          act_edge_images[i] = NOIMAGE;
        }
        if(gprs_images[i] != NOIMAGE)
        {
          ImageID_Free(gprs_images[i]);
          gprs_images[i] = NOIMAGE;
        }
        if(act_gprs_images[i] != NOIMAGE)
        {
          ImageID_Free(act_gprs_images[i]);
          act_gprs_images[i] = NOIMAGE;
        }
    }
    if(flight_image != NOIMAGE)
    {
      ImageID_Free(flight_image);
      flight_image = NOIMAGE;
    }
}

void FreeNetworkInfoParams(void)
{
  if(timer_networkInfo)
  {
      Timer_Kill(&timer_networkInfo);
      timer_networkInfo = 0;
  }

  if(rssiID != EMPTY_TEXTID) TextID_Destroy(rssiID);
  rssiID = EMPTY_TEXTID;

  if(rssi_percentID != EMPTY_TEXTID) TextID_Destroy(rssi_percentID);
  rssi_percentID = EMPTY_TEXTID;

  FreeNetworkInfoImages();
}


