#include "..\include\Lib_Clara.h"
//#include "..\\include\cfg_items.h"
#include "StatusInfo.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"
#include "MediaInfo.h"
#include "AlarmInfo.h"

#pragma swi_number=0x8366
__swi __arm   int addrof_IrDa_GetState();

#pragma swi_number=0x832B
__swi __arm int addrof_USB_isConnected(void);

/*
IR_ENABLED_ICN (EC08)
TIMER_ACTIVE_ICN (EE90)

RSS_STATUSBAR_UPDATED_ICN (EE29)
RSS_STATUSBAR_UPDATING_ICN (EE2A)

*/

#define STATUS_ICN_TMP(num, name, file) \
  const CFG_HDR cfghdr_status_##num={CFG_LEVEL, name, 1, 0}; \
  const int cfg_status_custom##num = 0; \
  const wchar_t cfg_status_file##num[256] = L"%INI_INT%/Standby Mode/status/" file

#define TIMER_MSG "Таймер"
//const int cfg_status_timer_show = 1;
//STATUS_ICN_TMP(xx, TIMER_MSG, L"timer.png");
//  TIMER_ICN,

typedef struct
{
  char    num;
  bool    embeded;
  IMAGEID ImageID;
  TEXTID   StrID;
}STATUS_ICON;

typedef struct
{
  STATUS_ICON icon;
}STATUS_ITEM;

enum STATUS_ICONS {
  LOCK_ICN = 0,
  SILENT_ICN,
  MEDIAPLAYER_ICN,
  FMRADIO_ICN,
  VIDEO_ICN,
  JAVA_ICN,
  CALLS_MISSED_ICN,
  SMS_MISSED_ICN,
  MMS_MISSED_ICN,
  EMAIL_MISSED_ICN,
  MEETING_ICN,
  INCAR_ICN,
  OUTDOORS_ICN,
  PHF_PR_ICN,
  HOME_ICN,
  OFFICE_ICN,
  BT_VIS_ICN,
  BT_UNVIS_ICN,
  BT_ACTIV_ICN,
  USB_ICN,
  PHF_ICN,
  CHF_ICN,
  SES_ACTIVE_ICN,
  ALARM_ICN,
  SES_ACTIVITY_ICN,
  SES_SUSPENDED_ICN,
  IRDA_ATTACHED_ICN,
  STOPWATCH_ICN,
  CALLERSFROMLIST_ICN,
  NONECALLERS_ICN,
  RESSIGNMENT_ICN,
  SMS_ICN,
  MMS_ICN,
  DOWNLOAD_ICN,
  BY_NET_NUM_ICN,
  HIDE_NUM_ICN,
  SHOW_NUM_ICN,
  MAX_STATUS_ICONS
};

wchar_t *icons_name[MAX_STATUS_ICONS] = {        // K790
    L"KEYLOCK_ICN",                              //(EC0E) LOCK_ICN
    L"SILENT_MODE_ICN",                          //(EE35) SILENT_ICN
    L"MEDIAPLAYER_RUNNING_ICN",                  //(EC3A) MEDIAPLAYER_ICN
    L"FM_RADIO_STATUSBAR_RADIO_ICN",             //(EAF9) FMRADIO_ICN
    L"DB_LIST_VIDEO_FILE_ICN",                   //(EA29) VIDEO_ICN
    L"JAVA_RUNNING_ICN",                         //(EC09) JAVA_ICN
    L"CALLS_MISSED_STATUS_ICN",                  //(E5F4) CALLS_MISSED_ICN
    L"MSG_UI_TEXT_MESSAGE_STATUS_BAR_ICN",       //(ED59) SMS_MISSED_ICN
    L"MSG_UI_MULTIMEDIA_MESSAGE_STATUS_BAR_ICN", //(ED1D) MMS_MISSED_ICN
    L"MSG_UI_EMAIL_MESSAGE_STATUS_BAR_ICN",      //(ED02) EMAIL_MISSED_ICN
    L"PROFILE_MEETING_STATUS_BAR_ICN",           //(EDB9) MEETING_ICN
    L"PROFILE_INCAR_STATUS_BAR_ICN",             //(EDB6) INCAR_ICN
    L"PROFILE_OUTDOORS_STATUS_BAR_ICN",          //(EDC2) OUTDOORS_ICN
    L"PROFILE_PHF_STATUS_BAR_ICN",               //(EDC5) PHF_PR_ICN
    L"PROFILE_HOME_STATUS_BAR_ICN",              //(EDB3) HOME_ICN
    L"PROFILE_OFFICE_STATUS_BAR_ICN",            //(EDBF) OFFICE_ICN
    L"BT_AD_HOC_ICN",                            //(E578) BT_VIS_ICN
    L"BT_PORT_ACTIVATED_ICN",                    //(E58E) BT_UNVIS_ICN
    L"BT_CONNECTION_ACTIVE_ICN",                 //(E57B) BT_ACTIV_ICN
    L"MEDIAPLAYER_CONNECTING_ICN",               //(EC21) USB_ICN
    L"PHF_STATUS_BAR_ICN",                       //(ED81) PHF_ICN
    L"BT_ACTIVE_HANDSFREE_CONNECTED_ICN",        //(E576) CHF_ICN
    L"GLOBE_SMALL_ICN",                          //(EB3D) SES_ACTIVE_ICN
    L"ALARM_ON_ICN",                             //(E568) ALARM_ICN
    L"GLOBE_SMALL_ACTIVITY_ICN",                 //(EB3C) SES_ACTIVITY_ICN
    L"GLOBE_SMALL_SUSPENDED_ICN",                //(EB3E) SES_SUSPENDED_ICN
    L"IR_ATTACHED_ICN",                          //(EC07) IRDA_ATTACHED_ICN
    L"UI_STOPWATCH_STATUSBAR_ICN",               //(EEA7) STOPWATCH_ICN
    L"CALLS_SCREENED_ICN",                       //(E5FC) CALLERSFROMLIST_ICN
    L"DIVERTED_ICN",                             //(EA8F) NONECALLERS_ICN
    L"SYNCH_STATUSBAR_ICN",                      //(EE8D) RESSIGNMENT_ICN
    L"MSG_UI_UPLOAD_ACTIVITY_STATUS_BAR_ICN",    //(ED60) SMS_ICN
    L"MSG_UI_UPLOAD_ACTIVITY_STATUS_BAR_ICN",    //(ED60) MMS_ICN
    L"STATUSBAR_DOWNLOADING_ICN",                //(EE71) DOWNLOAD_ICN
    L"UNKNOWN_ICN",                              //(FFFF) BY_NET_NUM_ICN
    L"CIPHERING_OFF_ICN",                        //(E628) HIDE_NUM_ICN
    L"UNKNOWN_ICN"                               //(FFFF) SHOW_NUM_ICN
};
//    iconidname2id( L"TIMER_ICTIVE_ICN", -1, &icn[TIMER_ICN].StrID);

const CFG_HDR *cfghdr_status_[MAX_STATUS_ICONS] = {
  &cfghdr_status_0,
  &cfghdr_status_1,
  &cfghdr_status_2,
  &cfghdr_status_3,
  &cfghdr_status_4,
  &cfghdr_status_5,
  &cfghdr_status_6,
  &cfghdr_status_7,
  &cfghdr_status_8,
  &cfghdr_status_9,
  &cfghdr_status_10,
  &cfghdr_status_11,
  &cfghdr_status_12,
  &cfghdr_status_13,
  &cfghdr_status_14,
  &cfghdr_status_15,
  &cfghdr_status_16,
  &cfghdr_status_17,
  &cfghdr_status_18,
  &cfghdr_status_19,
  &cfghdr_status_20,
  &cfghdr_status_21,
  &cfghdr_status_22,
  &cfghdr_status_23,
  &cfghdr_status_24,
  &cfghdr_status_25,
  &cfghdr_status_26,
  &cfghdr_status_27,
  &cfghdr_status_28,
  &cfghdr_status_29,
  &cfghdr_status_30,
  &cfghdr_status_31,
  &cfghdr_status_32,
  &cfghdr_status_33,
  &cfghdr_status_34,
  &cfghdr_status_35,
  &cfghdr_status_36
};

const wchar_t *cfg_status_file[MAX_STATUS_ICONS] = {
  cfg_status_file0,
  cfg_status_file1,
  cfg_status_file2,
  cfg_status_file3,
  cfg_status_file4,
  cfg_status_file5,
  cfg_status_file6,
  cfg_status_file7,
  cfg_status_file8,
  cfg_status_file9,
  cfg_status_file10,
  cfg_status_file11,
  cfg_status_file12,
  cfg_status_file13,
  cfg_status_file14,
  cfg_status_file15,
  cfg_status_file16,
  cfg_status_file17,
  cfg_status_file18,
  cfg_status_file19,
  cfg_status_file20,
  cfg_status_file21,
  cfg_status_file22,
  cfg_status_file23,
  cfg_status_file24,
  cfg_status_file25,
  cfg_status_file26,
  cfg_status_file27,
  cfg_status_file28,
  cfg_status_file29,
  cfg_status_file30,
  cfg_status_file31,
  cfg_status_file32,
  cfg_status_file33,
  cfg_status_file34,
  cfg_status_file35,
  cfg_status_file36
};

const int *cfg_status_custom[MAX_STATUS_ICONS] = {
  &cfg_status_custom0,
  &cfg_status_custom1,
  &cfg_status_custom2,
  &cfg_status_custom3,
  &cfg_status_custom4,
  &cfg_status_custom5,
  &cfg_status_custom6,
  &cfg_status_custom7,
  &cfg_status_custom8,
  &cfg_status_custom9,
  &cfg_status_custom10,
  &cfg_status_custom11,
  &cfg_status_custom12,
  &cfg_status_custom13,
  &cfg_status_custom14,
  &cfg_status_custom15,
  &cfg_status_custom16,
  &cfg_status_custom17,
  &cfg_status_custom18,
  &cfg_status_custom19,
  &cfg_status_custom20,
  &cfg_status_custom21,
  &cfg_status_custom22,
  &cfg_status_custom23,
  &cfg_status_custom24,
  &cfg_status_custom25,
  &cfg_status_custom26,
  &cfg_status_custom27,
  &cfg_status_custom28,
  &cfg_status_custom29,
  &cfg_status_custom30,
  &cfg_status_custom31,
  &cfg_status_custom32,
  &cfg_status_custom33,
  &cfg_status_custom34,
  &cfg_status_custom35,
  &cfg_status_custom36
};

STATUS_ICON icn[MAX_STATUS_ICONS];

int status_show = 0;

TEXTID statusrowId[STATUS_ICN_ROW_MAX] = {EMPTY_TEXTID,};
TEXTID statuscolumnId[STATUS_ICN_COLUMN_MAX] = {EMPTY_TEXTID,};
static int rowcnt = 0;
static int image_height = 0;

LIST * status_list = 0;

static int PrevProfileIcn = MAX_STATUS_ICONS;
static char *ressignment_addr = 0;

int m_call, m_sms, m_email, m_mms;

u16 timer_StatusInfo = 0;

static int FirstRun = 1;

bool haveIrdaFunc = false;
bool haveUsbFunc = false;

static char BluetoothActivity = 0;
char USB_state = 0;

void FreeStatusInfoImages(void);
void FreeStatusRow(void);
void UpdateStatusInfo(void);

void GetMissed()
{
  switch(ChipID)
   {
     case CHIPID_DB2000:
     case CHIPID_DB2010:
         m_call  = 0x01;
         m_sms   = 0x02;
         m_email = 0x04;
         m_mms   = 0x08;
      break;
      default:
         m_call  = 0x04;
         m_sms   = 0x08;
         m_email = 0x10;
         m_mms   = 0x20;
      break;
    }
}

int status_item_cmp(void *r0,void *r1)
{
  if((!r0) || (!r1)) return 1;
  if (((STATUS_ITEM*)r0)->icon.num == ((STATUS_ITEM*)r1)->icon.num) return 0;
  return 1;
}

void status_item_free(void * r)
{
  if(r) delete(r);
}

int status_item_filter(void * r0)
{
  if (r0) return(1);
  return(0);
}

int status_item_find(int num)
{
  if(!status_list) return LIST_ERROR;

  STATUS_ITEM temp_si;
  temp_si.icon.num = num;
  return ListElement_Find(status_list, &temp_si, status_item_cmp);
}

int status_item_set(int num, char mode)
{
  STATUS_ITEM temp_si;

  if(!status_list) return LIST_ERROR;

  temp_si.icon.num = num;
  int pos = ListElement_Find(status_list, &temp_si, status_item_cmp);
  if(mode)
  {
    if(pos != LIST_ERROR) return pos;
    STATUS_ITEM *si = new STATUS_ITEM;
    si->icon = icn[num];
    ListElement_Insert(status_list, 0, si);
    return 0;
  }

  if(pos != LIST_ERROR)
  {
    STATUS_ITEM *si = (STATUS_ITEM*)ListElement_Remove(status_list, pos);
    status_item_free(si);
  }
  return pos;
}

int status_item_replace(int old, int num, int mode)
{
  STATUS_ITEM temp_si;

  if(!status_list) return 0xFFFF;

  temp_si.icon.num = old;
  int pos = ListElement_Find(status_list, &temp_si, status_item_cmp);
  if(pos != LIST_ERROR)
  {
    STATUS_ITEM *si = (STATUS_ITEM*)ListElement_Remove(status_list, pos);
    status_item_free(si);
  }
  else pos = 0;

  temp_si.icon.num = num;
  int pos_new = ListElement_Find(status_list, &temp_si, status_item_cmp);
  if(mode)
  {
    if(pos_new == LIST_ERROR)
    {
      STATUS_ITEM *si = new STATUS_ITEM;
      si->icon = icn[num];
      ListElement_Insert(status_list, pos, si);
    }
  }
  else
  {
    if(pos_new != LIST_ERROR)
    {
      STATUS_ITEM *si = (STATUS_ITEM*)ListElement_Remove(status_list, pos_new);
      status_item_free(si);
    }
  }
  return num;
}

int status_item_replace2(int old1, int old2, int num, int mode)
{
  STATUS_ITEM temp_si;
  int pos = 0;

  if(!status_list) return 0xFFFF;

  temp_si.icon.num = old1;
  int pos1 = ListElement_Find(status_list, &temp_si, status_item_cmp);
  if(pos1 != LIST_ERROR)
  {
    STATUS_ITEM *si = (STATUS_ITEM*)ListElement_Remove(status_list, pos1);
    status_item_free(si);
    pos = pos1;
  }

  temp_si.icon.num = old2;
  int pos2 = ListElement_Find(status_list, &temp_si, status_item_cmp);
  if(pos2 != LIST_ERROR)
  {
    STATUS_ITEM *si = (STATUS_ITEM*)ListElement_Remove(status_list, pos2);
    status_item_free(si);
    pos = pos2;
  }

  temp_si.icon.num = num;
  int pos_new = ListElement_Find(status_list, &temp_si, status_item_cmp);

  if(mode)
  {
    if(pos_new == LIST_ERROR)
    {
      STATUS_ITEM *si = new STATUS_ITEM;
      si->icon = icn[num];
      ListElement_Insert(status_list, pos, si);
    }
  }
  else
  {
    if(pos_new != LIST_ERROR)
    {
      STATUS_ITEM *si = (STATUS_ITEM*)ListElement_Remove(status_list, pos_new);
      status_item_free(si);
    }
  }
  return num;
}

int onSessionActive(void* mess, BOOK* b)
{
  if(cfg_status_internet_show)
  {
    status_item_replace2(SES_ACTIVITY_ICN, SES_SUSPENDED_ICN, SES_ACTIVE_ICN, ConnectionManager_Connection_GetState());
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onSessionActivity(void* mess, BOOK* b)
{
  if(cfg_status_internet_show)
  {
    status_item_replace2(SES_ACTIVE_ICN, SES_SUSPENDED_ICN, SES_ACTIVITY_ICN, ConnectionManager_Connection_GetState());
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onSessionSuspended(void* mess, BOOK* b)
{
  if(cfg_status_internet_show)
  {
    status_item_replace2(SES_ACTIVE_ICN, SES_ACTIVITY_ICN, SES_SUSPENDED_ICN, ConnectionManager_Connection_GetState());
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onBluetoothActivityChanged(void* mess,BOOK* b)
{
  char activity = ((char *)mess)[0x2A];
  char PrevActivity = BluetoothActivity;

//  MessageBox(EMPTY_TEXTID, STR("Bluetooth Activity Changed"), NOIMAGE, 1, 0, 0);

  if(activity) BluetoothActivity++;
  else if (BluetoothActivity) BluetoothActivity--;

  if(cfg_status_bluetooth_show)
  {
    if(BluetoothActivity == 0)
    {
      if(Bluetooth_GetState())
      {
        int vis = Bluetooth_GetPhoneVisibility();
        status_item_replace2(BT_UNVIS_ICN, BT_ACTIV_ICN, BT_VIS_ICN, vis);
        status_item_replace2(BT_VIS_ICN, BT_ACTIV_ICN, BT_UNVIS_ICN, !vis);
      }
      else
      {
        status_item_replace2(BT_UNVIS_ICN, BT_VIS_ICN, BT_ACTIV_ICN, 0);
      }
      if(status_show)
      {
        UpdateStatusInfo();
        InvalidateAll();
      }
    }
    else if (PrevActivity == 0)
    {
      status_item_replace2(BT_VIS_ICN, BT_UNVIS_ICN, BT_ACTIV_ICN, 1);
      if(status_show)
      {
        UpdateStatusInfo();
        InvalidateAll();
      }
    }
  }
  return (0);
}

int onBluetoothStateChanged(void* mess,BOOK* b)
{
//  MessageBox(EMPTY_TEXTID, STR("Bluetooth State Changed"), NOIMAGE, 1, 0, 0);

  if(cfg_status_bluetooth_show)
  {
    if(BluetoothActivity) return 0;
    char state = ((char *)mess)[0x24];

    switch(state){
    case 1:
      status_item_replace2(BT_VIS_ICN, BT_ACTIV_ICN, BT_UNVIS_ICN, 1);
      break;
    case 2:
    case 3:
      status_item_replace2(BT_ACTIV_ICN, BT_UNVIS_ICN, BT_VIS_ICN, 1);
      break;
    case 4:
      status_item_replace2(BT_VIS_ICN, BT_UNVIS_ICN, BT_ACTIV_ICN, 1);
      break;
    case 0:
    default:
      status_item_replace2(BT_VIS_ICN, BT_UNVIS_ICN, BT_ACTIV_ICN, 0);
    }

    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onUSB_Connected(void* mess,BOOK* b)
{
  USB_state = 1;
  return (0);
}

int onUSB_DisConnected(void* mess,BOOK* b)
{
  USB_state = 0;
  return (0);
}

int onCHF_Connected(void* mess,BOOK* b)
{
  if(cfg_status_chf_show)
  {
    status_item_replace2(BT_VIS_ICN, BT_ACTIV_ICN, BT_UNVIS_ICN, 0);
    status_item_set(CHF_ICN, 1);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onCHF_DisConnected(void* mess,BOOK* b)
{
  if(cfg_status_chf_show)
  {
    status_item_set(CHF_ICN, 0);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onSmsSent(void* mess,BOOK* b)
{
  if(cfg_status_msg_show)
  {
    status_item_set(SMS_ICN, 0);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onSmsSending(void* mess,BOOK* b)
{
  if(cfg_status_msg_show)
  {
    status_item_set(SMS_ICN, 1);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onMmsSent(void* mess,BOOK* b)
{
  if(cfg_status_msg_show)
  {
    status_item_set(MMS_ICN, 0);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onMmsSending(void* mess,BOOK* b)
{
  if(cfg_status_msg_show)
  {
    status_item_set(MMS_ICN, 1);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onDownloadProgressHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  if(cfg_status_download_show)
  {
    status_item_set(DOWNLOAD_ICN, 1);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

int onDownloadDoneHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  if(cfg_status_download_show)
  {
    status_item_set(DOWNLOAD_ICN, 0);
    if(status_show)
    {
      UpdateStatusInfo();
      InvalidateAll();
    }
  }
  return (0);
}

void InitStatusList(void)
{
    if(!status_list) status_list=List_New();

    if(cfg_status_bluetooth_show)
    {
      if(Bluetooth_GetState())
      {
        int vis = Bluetooth_GetPhoneVisibility();
        status_item_replace2(BT_UNVIS_ICN, BT_ACTIV_ICN, BT_VIS_ICN, vis);
        status_item_replace2(BT_VIS_ICN, BT_ACTIV_ICN, BT_UNVIS_ICN, !vis);
      }
      else
      {
        status_item_replace2(BT_UNVIS_ICN, BT_VIS_ICN, BT_ACTIV_ICN, 0);
      }
    }
    else status_item_replace2(BT_UNVIS_ICN, BT_VIS_ICN, BT_ACTIV_ICN, 0);

    if(cfg_status_chf_show)
    {
      status_item_set(CHF_ICN, CHF_GetState());
    }
    else status_item_set(CHF_ICN, 0);

    if(!cfg_status_msg_show)
    {
      status_item_replace(SMS_ICN, MMS_ICN, 0);
    }
}

void UpdateStatusList(void)
{
  if(!status_list) status_list=List_New();

  if(cfg_status_ressignment_show && ressignment_addr)
  {
    status_item_set(RESSIGNMENT_ICN, *ressignment_addr);
  }
  else status_item_set(RESSIGNMENT_ICN, 0);

  if(cfg_status_phonenum_state_show)
  {
    char state;
    Settings_ShowNumber_Get(&state);
    if(state == 1) //1 - Скрыть номер
    {
      status_item_replace2(BY_NET_NUM_ICN, SHOW_NUM_ICN, HIDE_NUM_ICN, 1);
    }
    else if(state == 2) //2 - Показать номер
    {
      status_item_replace(BY_NET_NUM_ICN, HIDE_NUM_ICN, 0);
      if(icn[SHOW_NUM_ICN].StrID != 0xFFFF) status_item_set(SHOW_NUM_ICN, 1);
    }
    else //0 - Задано сетью
    {
      status_item_replace(SHOW_NUM_ICN, HIDE_NUM_ICN, 0);
      if(icn[BY_NET_NUM_ICN].StrID != 0xFFFF) status_item_set(BY_NET_NUM_ICN, 1);
    }

  }
  else status_item_replace2(BY_NET_NUM_ICN, HIDE_NUM_ICN, SHOW_NUM_ICN, 0);

  if(cfg_status_allowedcallers_show)
  {
    char state;
    REQUEST_SETTING_ALLOWEDCALLERS_GET(SYNC,-2,&state);
    switch(state){
    case 1:
      status_item_replace(NONECALLERS_ICN, CALLERSFROMLIST_ICN, 1);
      break;
    case 2:
      status_item_replace(CALLERSFROMLIST_ICN, NONECALLERS_ICN, 1);
      break;
    default:
      status_item_replace(CALLERSFROMLIST_ICN, NONECALLERS_ICN, 0);
    }
  }
  else status_item_replace(CALLERSFROMLIST_ICN, NONECALLERS_ICN, 0);

  if(cfg_status_alarm_show)
  {
    TIME t;
    status_item_set(ALARM_ICN, (Alarm_GetCurrentAlarmTime(&t) >= 0));
  }
  else status_item_set(ALARM_ICN, 0);

  if(cfg_status_profile_show)
  {
    int pr;
    REQUEST_PROFILE_GETACTIVEPROFILE(SYNC, &pr);
    PrevProfileIcn = status_item_replace(PrevProfileIcn, MEETING_ICN + pr - 1, (pr > 0));
  }
  else status_item_set(PrevProfileIcn, 0);

  if(cfg_status_phf_show)
  {
    status_item_set(PHF_ICN, PHF_GetState());
  }
  else status_item_set(PHF_ICN, 0);

  if(cfg_status_silent_show)
  {
    status_item_set(SILENT_ICN, GetSilent());
  }
  else status_item_set(SILENT_ICN, 0);

  if(cfg_status_media_show)
  {
    if(FindBook(isFmRadioBook())) status_item_set(FMRADIO_ICN, 1);
    else status_item_set(FMRADIO_ICN, 0);

    if(FindBook(isMediaPlayerVideoBook()))
    {
      status_item_replace(MEDIAPLAYER_ICN, VIDEO_ICN, 1);
    }
    else if(FindBook(isAudioPlayerBook()))
    {
      status_item_replace(VIDEO_ICN, MEDIAPLAYER_ICN, 1);
    }
    else
    {
      status_item_replace(VIDEO_ICN, MEDIAPLAYER_ICN, 0);
    }
  }
  else status_item_replace2(FMRADIO_ICN, VIDEO_ICN, MEDIAPLAYER_ICN, 0);

  if(cfg_status_java_show)
  {
    status_item_set(JAVA_ICN, (FindBook(isJavaBook) != 0));
  }
  else status_item_set(JAVA_ICN, 0);

  if(cfg_status_lock_show)
  {
    status_item_set(LOCK_ICN, isKeylocked());
  }
  else status_item_set(LOCK_ICN, 0);

  if(cfg_status_missed_show)
  {
    char* me=MissedEvents();
    char mestatus=*me;

    status_item_set(CALLS_MISSED_ICN, mestatus & m_call);
    status_item_set(SMS_MISSED_ICN, mestatus & m_sms);
    status_item_set(EMAIL_MISSED_ICN, mestatus & m_email);
    status_item_set(MMS_MISSED_ICN, mestatus & m_mms);
  }
  else
  {
    status_item_set(CALLS_MISSED_ICN, 0);
    status_item_set(SMS_MISSED_ICN, 0);
    status_item_set(EMAIL_MISSED_ICN, 0);
    status_item_set(MMS_MISSED_ICN, 0);
  }

  if(cfg_status_irda_show && haveIrdaFunc)
  {
    char state = 1;
    IrDa_GetState(&state);
    status_item_set(IRDA_ATTACHED_ICN, (state != 1));
  }
  else status_item_set(IRDA_ATTACHED_ICN, 0);

  if(cfg_status_internet_show)
  {
    int icn1 = (status_item_find(SES_ACTIVE_ICN) != 0xFFFF);
    int icn2 = (status_item_find(SES_ACTIVITY_ICN) != 0xFFFF);
    int icn3 = (status_item_find(SES_SUSPENDED_ICN) != 0xFFFF);
    if((!icn1) && (!icn2) && (!icn3)) status_item_set(SES_ACTIVE_ICN, ConnectionManager_Connection_GetState());
    else
    {
      status_item_set(SES_ACTIVE_ICN, icn1 && ConnectionManager_Connection_GetState());
      status_item_set(SES_ACTIVITY_ICN, icn2 && ConnectionManager_Connection_GetState());
      status_item_set(SES_SUSPENDED_ICN, icn3 && ConnectionManager_Connection_GetState());
    }
  }
  else
  {
    status_item_replace2(SES_ACTIVE_ICN, SES_ACTIVITY_ICN, SES_SUSPENDED_ICN, 0);
  }

  if(cfg_status_stopwatch_show)
  {
    status_item_set(STOPWATCH_ICN, (FindBook(isStopWatchBook) != 0));
  }
  else status_item_set(STOPWATCH_ICN, 0);

  if(cfg_status_usb_show)
  {
    if(haveUsbFunc) USB_state =  USB_isConnected();
    status_item_set(USB_ICN, USB_state);
  }
  else status_item_set(USB_ICN, 0);
}

void UpdateStatusInfo(void)
{
  FreeStatusRow();
  if(status_list->FirstFree)
  {
    STATUS_ITEM * si;
    int indx;
    int i = 0;
    int j = 0;

    if(cfg_status_fill_by_row)
    {
      int rowcnt_tmp = 0;
      for (indx=0; indx<status_list->FirstFree; indx++)
      {
        si = (STATUS_ITEM*)ListElement_GetByIndex(status_list,indx);
        statuscolumnId[i++] = (si->icon.embeded ? si->icon.StrID : si->icon.ImageID) + 0x78000000;
        if(i >= cfg_status_column_max)
        {
          statusrowId[j++] = TextID_Create(statuscolumnId,ENC_TEXTID,i);
          i = 0;
          ++rowcnt_tmp;
          if(j >= cfg_status_row_max) break;
        }
      }
      if(i)
      {
        statusrowId[j++] = TextID_Create(statuscolumnId,ENC_TEXTID,i);
        ++rowcnt_tmp;
      }
      rowcnt = rowcnt_tmp;
    }
    else
    {
      int colcnt = 0;
      for (indx=0; indx<status_list->FirstFree; indx++)
      {
        si = (STATUS_ITEM*)ListElement_GetByIndex(status_list,indx);
        if(indx < cfg_status_row_max)
        {
          statusrowId[j++] = (si->icon.embeded ? si->icon.StrID : si->icon.ImageID) | 0x78000000;
        }
        else
        {
          statuscolumnId[0] = statusrowId[j];
          statuscolumnId[1] = (si->icon.embeded ? si->icon.StrID : si->icon.ImageID) | 0x78000000;
          statusrowId[j++] = TextID_Create(statuscolumnId,ENC_TEXTID,2);
        }
        if(j >= cfg_status_row_max)
        {
          j = 0;
          ++colcnt;
        }
        if(rowcnt < cfg_status_row_max) ++rowcnt;
        else if(colcnt >= cfg_status_column_max) break;
      }
    }
  }
}

void ShowStatusInfo(int y, int location)
{
   if(status_show && rowcnt)
   {
     int oldFont = SetFont(FONT_E_14R);
     int offset = 0 - y;
     for(int i=0; i<rowcnt; i++)
     {
       if(cfg_status_align == RIGHT)
         DrawString(statusrowId[i], cfg_status_align, 1, cfg_status_y + offset, cfg_status_x, DisplayHeight, 0, 1, clRed, clGreen);
       else
         DrawString(statusrowId[i], cfg_status_align, cfg_status_x, cfg_status_y + offset, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);

       offset += image_height;
     }
     SetFont(oldFont);
   }
}

void ShowNonStandbyStatusInfo(int y, int location)
{
  ShowStatusInfo(y, location);
}

void onTimer_StatusInfo (u16 timerID, LPARAM lparam)
{
    UpdateStatusList();
    UpdateStatusInfo();
    Timer_ReSet(&timer_StatusInfo,cfg_status_refresh,onTimer_StatusInfo,0);
}

void UpdateStatusInfoParams(void)
{
  if(FirstRun)
  {
    for(int i=0; i<MAX_STATUS_ICONS; i++)
    {
      icn[i].num = i;
      icn[i].embeded = true;
      iconidname2id( icons_name[i], -1, &icn[i].StrID);
      if(icn[i].StrID == 0) icn[i].StrID = 0xFFFF;
#if 0
      if((icn[i].StrID == 0xFFFF) || (icn[i].StrID == 0))
      {
        wchar_t temp[128];
        wchar_t name[32];
        str2wstr(name,cfghdr_status_[i]->name);
        snwprintf(temp, MAXELEMS(temp), _T("Иконка из прошивки '%ls' не найдена"),name);
        MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
      }
#endif
    }

    FirstRun = 0;
  }

  FreeStatusInfoImages();
  if(cfg_status_show)
  {
    int addr = addrof_IrDa_GetState();
    if (addr > 0 && addr < 0xFFFFFFFF)
    {
       haveIrdaFunc = true;
    }
    else haveIrdaFunc = false;

    addr = addrof_USB_isConnected();
    if (addr > 0 && addr < 0xFFFFFFFF)
    {
       haveUsbFunc = true;
    }
    else haveUsbFunc = false;

    ressignment_addr = (char *)wcstoul(cfg_ressignment_addr,0,0);
    if((ressignment_addr == 0) && cfg_status_ressignment_show)
    {
      MessageBox(EMPTY_TEXTID, TextID_Create(INCORRECTRESSIGNMENTADDR_MSG,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    }

    if(!status_list) status_list = List_New();

    FSTAT _fstat;
    int image_height_int = GetImageHeight(icn[0].StrID) + 1;
    int image_height_ext = 0;
    int image_height_ext_max = 0;
    int image_height_new = 0;
    int use_ext = 0;
    int use_int = 0;
    for(int i=0; i<MAX_STATUS_ICONS; i++)
    {
      if(*cfg_status_custom[i])
      {
        extractdir(tmppath, tmpname, cfg_status_file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          wchar_t name[32];
          str2wstr(name,cfghdr_status_[i]->name);
          snwprintf(temp, MAXELEMS(temp), _T("Иконка '%ls' не найдена"),name);
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else
        {
          icn[i].embeded = true;
          if(ImageID_Get(tmppath,tmpname,&icn[i].ImageID)>=0)
          {
            icn[i].embeded = false;
            image_height_ext = GetImageHeight(icn[i].ImageID) + 1;
            if(image_height_ext_max < image_height_ext) image_height_ext_max = image_height_ext;
            use_ext = 1;
          }
        }
      }
      else use_int = 1;
    }
    if(use_int) image_height_new = image_height_int;
    if(use_ext)
    {
      if(image_height_ext_max > image_height_new) image_height_new = image_height_ext_max;
    }
    image_height = image_height_new;

    GetMissed();
    InitStatusList();
    UpdateStatusList();
    UpdateStatusInfo();

    Timer_ReSet(&timer_StatusInfo,cfg_status_refresh,onTimer_StatusInfo,0);
  }
  else
  {
    if(timer_StatusInfo)
    {
      Timer_Kill(&timer_StatusInfo);
      timer_StatusInfo = 0;
    }
    FreeStatusRow();
    if(status_list)
    {
      List_FreeElements(status_list, status_item_filter, status_item_free);
      List_Free(status_list);
      status_list = 0;
    }
  }
}

void UpdateStatusShowParams(void)
{
  status_show = cfg_status_show && (!(PlayerIsStarted && cfg_status_not_show_if_player));
}

void FreeStatusInfoImages(void)
{
  for(int i=0; i<MAX_STATUS_ICONS; i++ )
  {
    if(!icn[i].embeded)
    {
      ImageID_Free(icn[i].ImageID);
      icn[i].embeded = true;
    }
  }
}

void FreeStatusRow(void)
{
  int row = rowcnt;
  rowcnt = 0;
  if(row)
  {
     for(int i=0; i<row; i++)
     {
       TextID_Destroy(statusrowId[i]);
     }
  }
}

void FreeStatusList()
{
  if(!status_list) return;
  List_FreeElements(status_list, status_item_filter, status_item_free);
  List_Free(status_list);
  status_list = 0;
}

void FreeStatusInfoParams(void)
{
  if(timer_StatusInfo)
  {
    Timer_Kill(&timer_StatusInfo);
    timer_StatusInfo = 0;
  }
  FreeStatusList();
  FreeStatusRow();
  FreeStatusInfoImages();
}

