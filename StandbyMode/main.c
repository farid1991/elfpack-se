#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
//#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"
#include "main.h"
#include "misc_util.h"
#include "TimeDateInfo.h"
#include "HeapInfo.h"
#include "BattTempInfo.h"
#include "MemInfo.h"
#include "CustomMsg.h"
#include "Images.h"
#include "MediaInfo.h"
#include "OperatorInfo.h"
#include "NetworkInfo.h"
#include "AlarmInfo.h"
#include "AppInfo.h"
#include "StatusInfo.h"
#include "CallsInfo.h"

#define ELF_BCFG_CONFIG_EVENT 994

#define ELFNAME "Standby Mode"
#define LELFNAME L"Standby Mode\nv3b"
#define LELFVERSION L"\n\n� Ploik & BigHercules\n(r) arban, 2007KrasH"

#pragma swi_number=0x8286
__swi __arm int addrof_iconidname2id();

#define USB_CONNECTED_CUSTOM_EVENT  0x7C1

void onTimer_remover (u16 timerID, LPARAM lparam);

BOOK * StMBook = 0;

DISP_OBJ_ONREDRAW_METHOD Status_oldReDraw = 0;
DISP_OBJ ** GUI_status = 0;
DISP_DESC * Status_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Display_oldReDraw = 0;
DISP_OBJ * GUI_display = 0;
DISP_DESC * Display_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Soft_oldReDraw = 0;
DISP_OBJ * GUI_soft = 0;
DISP_DESC * Soft_desc = 0;

int DisplayWidth  = 0; //������ ������� � ��������
int DisplayHeight = 0; //������ ������� � ��������
int SoftBarY   = 0;
int StatusBarY = 0;

int isA2 = 0;
int ChipID = 0;
phone_t phoneType = UNKNOWN;

static char myappname[]=ELFNAME;

bool LANG=false;
bool SleepModeIsDeactivated = true;
bool IsInStandby = false;

wchar_t tmppath[256] = {0,};
wchar_t tmpname[256] = {0,};

u16 timer = 0;
u16 timer_disable = 0;

int soft_key_show = 0;

int isBookmanDaemonBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"BookManagerDaemon")) return(1);
  return(0);
}

int isTimerBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"TimerBook")) return(1);
  return(0);
}

int isStopWatchBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"StopWatchBook")) return(1);
  return(0);
}

int isImageViewerBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"ImageViewerBook")) return(1);
  return(0);
}

int isJavaBook(BOOK *bk)
{
  if(strstr(bk->xbook->name,"CUIDisplayableBook")) return 1;
  if(strstr(bk->xbook->name,"SessionBook")) return 0;
  wchar_t ws[100];
  TextID_GetWString(Book_GetSession(bk)->name,ws,100);
  if (!wstrncmp(ws,L"Foreign app",11)) return 1;
  if (!wstrcmp(ws,L"Java")) return 1;
  return 0;
};

int myModifyUIHook(int event1 , int event2 , PAGEHOOKPROC PROC,int mode)
{
  if((event1) && (event1 != 0xFFFFFFFF)) return ModifyUIPageHook(event1, PROC, 0, mode);
  return ModifyUIPageHook(event2, PROC, 0, mode);
}

int StandbyModeActivatedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  IsInStandby = true;
  return(0);
}

int StandbyModeDeactivatedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  IsInStandby = false;
  return(0);
}

int isStMBook(BOOK * book)
{
    if(!strcmp(book->xbook->name,myappname)) return true;
    return false;
}

GUI *GetStandbyBookGui(void)
{
/*
  int ChipID = GetChipID();
  if ((ChipID == 0x7100) || (ChipID ==0x8000) || (ChipID ==0x8040)) //��� ��2010:
  {
    BOOK * b2k;
    b2k=Find_StandbyBook();
//    return b2k->gui;
    return (GUI*)(*(int*)((int)b2k+0x18));
  }
*/
  return SBY_GetStatusIndication(Find_StandbyBook());
}

void win12512unicode(wchar_t *ws, const char *s, int len)
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c==0xA8) c=0x401;
    if (c==0xAA) c=0x404;
    if (c==0xAF) c=0x407;
    if (c==0xB8) c=0x451;
    if (c==0xBA) c=0x454;
    if (c==0xBF) c=0x457;
    if ((c>=0xC0)&&(c<0x100)) c+=0x350;
    *ws++=c;
  }
  *ws=0;
}

wchar_t *convert_first_symbol(wchar_t *str, int format)
{
  if(format == UPPER)
  {
    str[0] = towupper(str[0]);
  }
  else if(format == LOWER)
  {
    str[0] = towlower(str[0]);
  }
  return str;
}

void DrawStr(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  SetFont(Font);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void DrawHighlightStrV1(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  SetFont(Font);
  DrawString(StrID,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void DrawHighlightStrV2(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  SetFont(Font);
  DrawString(StrID,CEN,XPos+1,YPos,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos+1,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos-1,YPos,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos-1,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void DrawShadowStr(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  SetFont(Font);
  DrawString(StrID,CEN,XPos+1,YPos+1,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void myDrawString(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor, int type)
{
  //��� ������� (Overlay type)
  switch(type){
  case 1: //������ (Full) v1
    DrawHighlightStrV1(Font, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  case 2: //������ (Full) v2
    DrawHighlightStrV2(Font, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  case 3: //���� (Shadow)
    DrawShadowStr(Font, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  default: //��� (No)
    DrawStr(Font, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
  }
}

void DrawOwnRect(int _x1, int _y1,int _x2 ,int _y2,int pen_color,int brush_color)
{
  int x1,x2,y1,y2;
  if (_x1>_x2){ x1=_x2;  x2=_x1;} else { x1=_x1;  x2=_x2;}
  if (_y1>_y2){ y1=_y2;  y2=_y1;} else { y1=_y1;  y2=_y2;}
  if (y1==y2 || x1==x2)
  {
    GC *gc=get_DisplayGC();
    GC_SetPenColor(gc,pen_color);
    GC_SetBrushColor(gc,brush_color);
    GC_DrawLine(gc,x1,y1,x2,y2);
  }
  else
    DrawRect(x1,y1,x2,y2,pen_color,brush_color);
}

void DrawParams(int y, int location)
{
   ShowMediaImages(y, location);
   ShowImages(y, location);
   ShowHeapInfo(y, location);
   ShowBattTempInfo(y, location);
   ShowTimeDateInfo(y, location);
   ShowMemInfo(y, location);
   ShowNetworkInfo(y, location);
   ShowCustomMsgInfo(y, location);
   ShowAlarmInfo(y, location);
   ShowAppInfo(y, location);
   ShowOperatorInfo(y, location);
   ShowStatusInfo(y, location);
   ShowCallsInfo(y, location);
   ShowMediaInfo(y, location);
}

void NonStandbyDrawParams(int y, int location)
{
  ShowNonStandbyTimeInfo(y, location);
  ShowNonStandbyStatusInfo(y, location);
  ShowNonStandbyBattTempInfo(y, location);
  ShowNonStandbyNetworkInfo(y, location);
}


void UpdateSystemShowParams(void)
{
    soft_key_show = cfg_soft_key_show && (!(PlayerIsStarted && cfg_soft_key_not_show_if_player));
}

void onTimer_disable (u16 timerID, LPARAM lparam)
{
  IsInStandby = false;
}

void Display_ReDraw(DISP_OBJ* DO,int a,int b,int c)
{
  IsInStandby = true;
  Timer_ReSet(&timer_disable,cfg_refresh_time << 1, onTimer_disable,0);
  if(IsInStandby) DrawParams(StatusBarY, SCREEN);
  if(Display_oldReDraw) Display_oldReDraw(DO,a,b,c);
}

void Status_ReDraw(DISP_OBJ * d, int a, int b, int c)
{
  if(cfg_orig_status_show) Status_oldReDraw(d, a, b, c);
  if(IsInStandby || (!cfg_standby_only))DrawParams(0, STATUS);
  if(!IsInStandby) NonStandbyDrawParams(0, STATUS);
}

void Soft_ReDraw(DISP_OBJ* DO,int a,int b,int c)
{
  BOOK *bk1 = FindBook(isImageViewerBook);
  BOOK *bk2 = FindBook(isCameraBook());
  bool flag = (!bk1) && (!bk2);

  if(IsInStandby)
  {
    DrawParams(SoftBarY, SOFT);
    if(soft_key_show) Soft_oldReDraw(DO,a,b,c);
  }
  else
  {
    if(flag) NonStandbyDrawParams(SoftBarY, SOFT);
    Soft_oldReDraw(DO,a,b,c);
  }
}

void InvalidateAll(void)
{
    if(GUI_display) DispObject_InvalidateRect(GUI_display,0);
    if(GUI_status) DispObject_InvalidateRect(*GUI_status,0);
    if(GUI_soft) DispObject_InvalidateRect(GUI_soft,0);
}

void onTimer_display (u16 timerID, LPARAM lparam)
{
  if(SleepModeIsDeactivated)
  {
    if(PlayerIsStarted && (IsInStandby || (!cfg_standby_only))) UpdatePositions();

    if(GUI_display && IsInStandby) DispObject_InvalidateRect(GUI_display,0);
    if(GUI_status) DispObject_InvalidateRect(*GUI_status,0);
    if(GUI_soft && IsInStandby) DispObject_InvalidateRect(GUI_soft,0);
  }
  Timer_ReSet(&timer,cfg_refresh_time,onTimer_display,0);
}

int SleepModeActivatedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
    SleepModeIsDeactivated = false;
    return(0);
}

int SleepModeDeactivatedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
    SleepModeIsDeactivated = true;
    return(0);
}

#pragma segment="ELFBEGIN"
void elf_exit(void)
{
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}

void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE *sbm = new REDRAW_RELEASE_MESSAGE;

  sbm->SI_OldOnRedraw = Status_oldReDraw;
  sbm->SI_NewOnRedraw = Status_ReDraw;

  sbm->SB_OldOnRedraw = Display_oldReDraw;
  sbm->SB_NewOnRedraw = Display_ReDraw;

  sbm->SK_OldOnRedraw = Soft_oldReDraw;
  sbm->SK_NewOnRedraw = Soft_ReDraw;

  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, sbm, (void (*)(void*))mfree_adr());
}

int SB_ELF_Killed(void *mess, BOOK *book)
{
  REDRAW_RELEASE_MESSAGE * sbm =(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE * ms = 0;

  if (sbm->SI_NewOnRedraw == Status_oldReDraw)
  {
    ms = new REDRAW_RELEASE_MESSAGE;
    memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));

    if (sbm->SI_OldOnRedraw!=EMPTY_REDRAW_METHOD) Status_oldReDraw = sbm->SI_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(*GUI_status), Status_ReDraw);

    ms->SI_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SI_NewOnRedraw = Status_ReDraw;
  }

  if (sbm->SB_NewOnRedraw == Display_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) Display_oldReDraw = sbm->SB_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_display), Display_ReDraw);

    ms->SB_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SB_NewOnRedraw = Display_ReDraw;
  }

  if (sbm->SK_NewOnRedraw == Soft_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) Soft_oldReDraw = sbm->SK_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_soft), Soft_ReDraw);

    ms->SK_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SK_NewOnRedraw = Soft_ReDraw;
  }
  if(ms)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return(BLOCK_EVENT_GLOBALLY);
  }
  return(0);
}

int TerminateElf(void * ,BOOK* book)
{
    Send_REDRAW_RELEASE();
    FreeBook(book);
    return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
    MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,5000, *(BOOK**)mess);
    return 1;
}

int USB_ConnectedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
    onUSB_Connected(0,0);
    return(0);
}

void onCloseStMBook(BOOK * book)
{
 if (book)
 {
    myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,0);
    myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,0);
    ModifyUIPageHook(UI_SLEEPMODE_ACTIVATED_EVENT,SleepModeActivatedHook,0,PAGE_HOOK_REMOVE);
    ModifyUIPageHook(UI_SLEEPMODE_DEACTIVATED_EVENT,SleepModeDeactivatedHook,0,PAGE_HOOK_REMOVE);

    ModifyUIPageHook(USB_CONNECTED_CUSTOM_EVENT,USB_ConnectedHook,0,PAGE_HOOK_REMOVE);

    ModifyUIPageHook(DOWNLOAD_PROGRESS_EVENT,onDownloadProgressHook,0,PAGE_HOOK_REMOVE);
    ModifyUIPageHook(DOWNLOAD_COMPLETED_EVENT,onDownloadDoneHook,0,PAGE_HOOK_REMOVE);
    ModifyUIPageHook(DOWNLOAD_ERROR_EVENT,onDownloadDoneHook,0,PAGE_HOOK_REMOVE);

    if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
    if(Status_desc)  DISP_DESC_SetOnRedraw(Status_desc, Status_oldReDraw);
    if(Soft_desc)    DISP_DESC_SetOnRedraw(Soft_desc, Soft_oldReDraw);

    Timer_Kill(&timer);
    if(timer_disable) Timer_Kill(&timer_disable);

    FreeTimeDateInfoParams();
    FreeMemInfoParams();
    FreeHeapInfoParams();
    FreeBattTempInfoParams();
    FreeMediaInfoParams();
    FreeImagesParams();
    FreeCustomMsgInfoParams();
    FreeOperatorInfoParams();
    FreeNetworkInfoParams();
    FreeAlarmInfoParams();
    FreeAppInfoParams();
    FreeStatusInfoParams();
    FreeCallsInfoParams();

    SUBPROC(elf_exit);
 }
}

void UpdateParams(void)
{
    if(!GUI_status)
    {
      GUI_status = StatusRow_p();
      Status_desc = DispObject_GetDESC (* GUI_status);
      Status_oldReDraw = DispObject_GetOnRedraw (* GUI_status);
      DISP_DESC_SetOnRedraw (Status_desc, (DISP_OBJ_ONREDRAW_METHOD)Status_ReDraw);
    }
    if(!GUI_soft)
    {
      GUI_soft = DispObject_SoftKeys_Get();
      Soft_desc = DispObject_GetDESC (GUI_soft);
      Soft_oldReDraw = DispObject_GetOnRedraw(GUI_soft);
      DISP_DESC_SetOnRedraw(Soft_desc, (DISP_OBJ_ONREDRAW_METHOD)Soft_ReDraw);
    }
    if(!GUI_display)
    {
      GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
      Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
      Display_desc = DispObject_GetDESC (GUI_display);
      DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
    }

    UpdateMediaInfoParams();
    UpdateTimeDateInfoParams();
    UpdateCustomMsgInfoParams();
    UpdateMemInfoParams();
    UpdateImagesParams();
    UpdateBattTempInfoParams();
    UpdateOperatorInfoParams();
    UpdateHeapInfoParams();
    UpdateNetworkInfoParams();
    UpdateAlarmInfoParams();
    UpdateAppInfoParams();
    UpdateStatusInfoParams();
    UpdateCallsInfoParams();
}

static int onReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    UpdateParams();
    result=1;
  }
  return(result);
}

typedef struct
{
  BOOK* book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

int onBcfgConfig(void *mess, BOOK *book)
{
  MSG_BCFG *msg = (MSG_BCFG *)mess;
  wchar_t tmp[256];
  wstrcpy(tmp,msg->bcfg_p);
  wstrcat(tmp,L"/");
  wstrcat(tmp,msg->bcfg_n);
  elfload(tmp,(void *)successed_config_path,(void *)successed_config_name,0);
  return(1);
}

#if 0
//K790
#define FMRADIO_RDS_STATE_EVENT    0x190D
//W300
#define FMRADIO_RDS_STATE_EVENT    0x1395
#endif

const PAGE_MSG HW_PageEvents[]@ "DYN_PAGE" ={
     BATTERY_CAPACITY_CHANGED_EVENT_TAG,               onBatteryCapacityChanged,
     BATTERY_CHARGER_CONNECTED_EVENT_TAG,              onBatteryChargerConnected,
     BATTERY_CHARGER_DISCONNECTED_EVENT_TAG,           onBatteryChargerDisconnected,
     BATTERY_CHARGING_STATE_EVENT_TAG,                 onBatteryChargingState,
     UI_BLUETOOTH_ACTIVITY_CHANGED_EVENT_TAG,          onBluetoothActivityChanged,
     UI_BLUETOOTH_STATE_CHANGED_EVENT_TAG,             onBluetoothStateChanged,
     //UI_MEDIAPLAYER_CREATED_EVENT_TAG,                 onMediaplayerCreated,
     //UI_MEDIAPLAYER_DESTROYED_EVENT_TAG,               onMediaplayerDestroyed,
     //UI_MEDIAPLAYER_PLAY_PRESSED_EVENT_TAG,            onMediaplayerPlayPressed,
     //UI_MEDIAPLAYER_PAUSE_PRESSED_EVENT_TAG,           onMediaplayerPausePressed,
     //UI_MEDIAPLAYER_AUDIO_PLAYING_TIME_EVENT_TAG,      onMediaplayerAudioPlayingTime,
     FMRADIO_MODE_CHANGED_EVENT_TAG,                   onFmRadioModeUpdate,
     FMRADIO_RDS_DATA_RECEIVED_EVENT_TAG,              onFmRadioRdsNewData,
     UI_CONNECTIONMANAGER_SESSION_ACTIVE_EVENT_TAG,    onSessionActive,
     UI_CONNECTIONMANAGER_SESSION_ACTIVITY_EVENT_TAG,  onSessionActivity,
     UI_CONNECTIONMANAGER_SESSION_SUSPENDED_EVENT_TAG, onSessionSuspended,
     CHF_CONNECTED_EVENT_TAG,                          onCHF_Connected,
     CHF_DISCONNECTED_EVENT_TAG,                       onCHF_DisConnected,
     ON_CALLMANAGER_EVENT_TAG,                         onCallManager,
     UI_MESSAGING_SMS_SENDING_EVENT_TAG,               onSmsSending,
     UI_MESSAGING_SMS_SENT_EVENT_TAG,                  onSmsSent,
     UI_MESSAGING_SMS_SENDING_ERROR_EVENT_TAG,         onSmsSent,
     BOOK_DESTROYED_EVENT_TAG,                         onRootListChanged,
     ELF_TERMINATE_EVENT,                              TerminateElf,
     ELF_SHOW_INFO_EVENT,                              ShowAuthorInfo,
     ELF_RECONFIG_EVENT,                               onReconfigElf,
     ELF_BCFG_CONFIG_EVENT,                            onBcfgConfig,
     SBY_REDRAW_RELEASE_EVENT,                         SB_ELF_Killed,
//     FMRADIO_RDS_STATE_EVENT,    onFmRadioRdsState,
     NIL_EVENT_TAG,              NULL
};

PAGE_DESC base_page ={"Standby_Mode_Base_Page",0,HW_PageEvents};

int init_var(void)
{
  if (addrof_iconidname2id() > 0 && addrof_iconidname2id() < 0xFFFFFFFF)
  {
     LANG=true;
  }

  DisplayWidth  = Display_GetWidth(0);
  DisplayHeight = Display_GetHeight(0);

  ChipID = GetChipID()&CHIPID_MASK;
  phoneType = GetPhoneType();
  switch(ChipID)
   {
      case CHIPID_DB3150:
      case CHIPID_DB3200:
      case CHIPID_DB3210:
           isA2      = 1;
           break;
       default:
           isA2      = 0;
           break;
   }

  NetInfoAddr.addr   = GetNetInfoAddr(phoneType);

  StatusBarY    = DISP_OBJ_GetWindowHeight(*StatusRow_p());
  SoftBarY      = DisplayHeight - DISP_OBJ_GetWindowHeight(DispObject_SoftKeys_Get());

/*
  switch(DisplayHeight){
    case 128:
        StatusBarY = STATUS_HEIGHT_128;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_128;
        break;
    case 160:
        StatusBarY = STATUS_HEIGHT_160;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_160;
        break;
    case 208:
        StatusBarY = STATUS_HEIGHT_208;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_208;
        break;
    case 220:
        StatusBarY = STATUS_HEIGHT_220;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_220;
        break;
    case 320:
        StatusBarY = STATUS_HEIGHT_320;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_320;
        break;
    default:
        #ifdef ENG
          MessageBox(EMPTY_TEXTID, STR("Oops!\nUnkown display resolution.\nPlease contact with elf's authors."), NOIMAGE, 1 ,5000, 0);
        #else
          MessageBox(EMPTY_TEXTID, STR("Oops!\n����������� ���������� ������.\n���������� ��������� � �������� ����� �����."), NOIMAGE, 1 ,5000, 0);
        #endif
  }
*/
  return 0;
}

int main(void)
{
    init_var();
    if(FindBook(isStMBook))
    {
        MessageBox(EMPTY_TEXTID, STR("Standby Mode\n��� �������."), NOIMAGE, 1, 5000, 0);
        SUBPROC(elf_exit);
    }
    else
    {
        InitConfig();
        StMBook = new BOOK;
        if(!CreateBook(StMBook, onCloseStMBook, &base_page, ELFNAME,-1,0))
        {
          delete StMBook;
          SUBPROC(elf_exit);
          return 0;
        }
        UpdateParams();

        if(!GUI_status)
    {
      GUI_status = StatusRow_p();
      Status_desc = DispObject_GetDESC (* GUI_status);
      Status_oldReDraw = DispObject_GetOnRedraw (* GUI_status);
      DISP_DESC_SetOnRedraw (Status_desc, (DISP_OBJ_ONREDRAW_METHOD)Status_ReDraw);
    }
    if(!GUI_soft)
    {
      GUI_soft = DispObject_SoftKeys_Get();
      Soft_desc = DispObject_GetDESC (GUI_soft);
      Soft_oldReDraw = DispObject_GetOnRedraw(GUI_soft);
      DISP_DESC_SetOnRedraw(Soft_desc, (DISP_OBJ_ONREDRAW_METHOD)Soft_ReDraw);
    }
    if(!GUI_display)
    {
      GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
      Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
      Display_desc = DispObject_GetDESC (GUI_display);
      DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
    }

        myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,1);
        myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,1);

        ModifyUIPageHook(UI_SLEEPMODE_ACTIVATED_EVENT,SleepModeActivatedHook,0,PAGE_HOOK_ADD_BEFORE);
        ModifyUIPageHook(UI_SLEEPMODE_DEACTIVATED_EVENT,SleepModeDeactivatedHook,0,PAGE_HOOK_ADD_BEFORE);

        ModifyUIPageHook(USB_CONNECTED_CUSTOM_EVENT,USB_ConnectedHook,0,PAGE_HOOK_ADD_BEFORE);

        ModifyUIPageHook(DOWNLOAD_PROGRESS_EVENT,onDownloadProgressHook,0,PAGE_HOOK_ADD_BEFORE);
        ModifyUIPageHook(DOWNLOAD_COMPLETED_EVENT,onDownloadDoneHook,0,PAGE_HOOK_ADD_BEFORE);
        ModifyUIPageHook(DOWNLOAD_ERROR_EVENT,onDownloadDoneHook,0,PAGE_HOOK_ADD_BEFORE);

        timer=Timer_Set(cfg_refresh_time,onTimer_display,0);
    }
    return(0);
}


/*
Revision history:
    2.18
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.17.x
        + ������� ������������ �� ����� �������. ��� 100% ������� ������������.
        + ����������� �������� ��������/�������.
        + ����������� ������� ������ ������
        + �������� ����
    2.17.1
        + ������ ��������� �� ���������� ������ �� ��������
        + ���������� ��������� ������ � ����������� ������� ������������ ����������
    2.17
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.16.
        + ��������� ������� ���������.
        + ��������� � ������� ��������.
        + ��������� � ������� �������� SMS
        + ����������� ��������� ����������� ������ ����������.
        + ����������� ����������� ������� (� ��������) ������� ������� �� ���������� ������ ������.
        + ����������� ���������� ������� ������� �������
        + ���������/�����������/�����������
    2.16
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.15.
        + ��������� � ������� ����������� ������ ����������� ������ �������
        + ��������� � ������� ����������� ������ �������������� ����������
        + ����������� ��������� ������� ����
        + ��������� ��������� �������
        + ����������� ������� ��� ���� ���������
        + ���������/�����������/�����������
    2.15.1
        + ��� ����������� ������ ���������, ������ ������� � ������� �� ������������.
        + ���������� ��������� ������ ������ ������ � ������.
        + ���������� ������ ��-�� ������� ����� �� ����������� ������ ����.
        + ���������� ��������� ������ � ��������� ������� �������� ��� 100% ������.
        + ��������� ����������� �� USB �� K750. (��������� ����!)
    2.15
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.14.
        + ����������� � ������� ����������� �� �����.
        + ����������� � ������� �����������.
        + ��������� ��������� � ������� ���������� ��������.
        + ��������� ��������� � ������� ���������� bluetooth.
        + ������������ ������ � ������� �� ������, ������� ���� ��� �� ������
        + ������������ ������ ���������� ���������� �� ������, ������� ���� ��� �� ������
        + ��������� ����������� ������� ������������ ���������� � ���. ������� "������" ��� db2020
        + ��������� ����������� ����� �������� ��� ����������.
        + ���������� ����������� ��������� ��� ��� k750 � w800
        + ���� ��� �������� 6 ���������� ����� "��������", �� ��������� "�������� 6 �� �������" �� ��������� � ����� ������.
        + ���������/�����������/�����������
    2.14
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.13.
        + ����������� ������� ������������ ����������. ���� ������ � ����. ������� "������"
        + ����������� ������ ���������� � �������.
        + ���������� ����������� �������� ��� k750 � w800
        + ���������/�����������/�����������
    2.13
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.12.
        + ����������� ������ ��������� � �������.
        + ����������� ������ ������������ ��������� � �������.
        + ����������� ������ ��������� � �������.
        + ������� "�����" ������ ��� ��������� ������� ������� � ������
          ����������� ������ ���� ������ �������������� �����..
        + ���������/�����������/�����������
    2.12
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.11.
        + ����������� ������������ ������ �� �������� ��� ��������� ������� �������.
        + ����������� ������������ ������ �� �������� ��� ��������� ������ �������.
        + ����������� ������� �����, � �� ������ � ������ ��������.
        + ��������� �������� ������ �������.
        + ����������� ������ ������� �����, � �� ������ � ������ ��������.
        + ����������� ���������� ������ ���� � ������ �������� ���� ������� �����.
        + ��������� ������ ������.
        + ���������/�����������/�����������
    2.11
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.10.
        + ��������� ���������� ����� "AltStatusBar"
        + ����������� ������ ��� ���������� ������ ����:
            ������ � ������ ��������;
            ������ �� � ������ ��������.
        + ���������/�����������/�����������
    2.10
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.9.
        + ����� ���������� ���������� ����������, ����, ������.
        + ����������� ������ �� ������ �������� ������������ ������� � ���������
        + ���������� ����� ����� ��� ����� � �������
        + ���������/�����������/�����������
    2.9
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x - v2.8.x.
        + ����������� ������ �� ������ �������� ������������ ������� � dBm
        + ����������� ������ �� ������ ���������� ���������� (�� ������!!!)
    2.8
        + ��������� � ��������� ����������������� �����.
        + ������ ����������������� ����� �� v2.7.x.
        + ����������� ������ ������ ��� �����: �������, �������+���, ���
        + ������� �������� ��������� ���������� � �����
        + ��������� VBR
        + ��������� ������ ������.
        + ���������/�����������/�����������
    2.7.2
        + ������� �������� ��������� ���������� � �����
        + ������� �������� ��������� SBY_REDRAW_RELEASE_EVENT (������ ������������ REDRAW_RELEASE_MESSAGE)
    2.7.1
        + ���������� ������ � ������������ ����� � ������ �� ����������.
    2.7
        + ��������� � ��������� ����������������� �����.
        + ����� ������ 'Stereo' ��� �����.
        + ����� RDS ������.
        + ����� ����������������� �����
        + ����� ��������/����������� ������� �����
        + ����� ����������������� � ��������/����������� ������� ����� � ���� �������.
        + ����������� ���/���� ��������� ��������� ��� ������� ��������
        + ��� ��������� � ������� ������ ����� ��� CFG_UTF16_STRING, ������ �� ����� ���� ���������
        + ��������� ������ ������.
        + ���������/�����������/�����������
    2.6.3
        + ���������� ������. ��� ������������ �������� ����������� ������������ �������, ������� � ������.
    2.6.2
        + ���������� ��������� ������. ��� �������� ���� �� ���� ��������� ������ ������.
    2.6.1
        + ���������� ��������� ������. ��� ������������ �������� ���������� �������� ��� �� ����.
    2.6
        + ��������� � ��������� ����������������� �����.
        + ���������� ������. ��� ��������������� ����������� ���������� ������.
        + ���������� ������. ��� ���������� ������, ���������� ������������ ���������� � ������ � ���������.
        + ����������� �� ���������� ��� � ����.
        + ����������� ������ ������ ��������� �������� ID3 ���������� � �����.
        + ����� �������� ��� ������� (��������� �������)
        + ���������/�����������/�����������
    2.5
        + ��������� � ��������� ����������������� �����.
        + ���������� ������. � ������ ��������� ���� title, �� �������������� ��� �����.
        + ����������� ����������� ������� ��� ��������� � ���� ����� ��������
        + ����������� ������ ������ �������� �6 �������� ����� �������� (*.png)
        + ����� ������� ����
        + ���������/�����������/�����������
    2.4
        + ��������� � ��������� ����������������� �����.
        + ��������� SBY_REDRAW_RELEASE_EVENT
        + ����������� ������ ���� ����� ���� ������
        + �������������� ����������� ������ �����
        + ����� ����������� ������� ����� ������ �������� ���� ��������
        + ����������� ��������� ����� ����������� �����, ����, ����� �� ������ �
          ���� ��������� ��� ��������� ������ � ������ �����-���� ���������� � �����.
        + ��������� ������ ����
        + ����� ������ ��� ������ � ����������� � ��������� ��������
          ��������� �� ��������� �� ��������� (Y)
        + ����� ������ ��� ����� % � ������� �������  � ����������� � ���������
          �������� ��������� ��� ��������� �� ��������� (Y)
        + ���������/�����������/�����������
    2.3.3
        + ���������� ������ � ���������� ������� ��� ���������� �����.
        + ���������/�����������/�����������
    2.3.2
        + ��������� � ��������� ����������������� �����.
        + ��������� ������� ��� ���������.
    2.3.1
        + ��������� ��������� �������������� ELF_BCFG_CONFIG_EVENT
    2.3
        + ��������� � ��������� ����������������� �����.
        + ��������� ID3v2
        + ����� ������� ����������� �� �����. ���� ���� �� �������� ���������� �������,
          �� ������������ ������� ������������ �� ����������, ��� ��������� ������������� ����.
        + ���/���� ��������� �������� � ������������� �������� ���������
        + ����������� ��������� ����� �������, ���� � �.�. ��� ���������
          ������ � ������ �����-���� ���������� � �����. ���� ����� �����-����
          ���������� � ����� ��������, �� ��� ���������� �������� ����� ��������������
          ��� ����������� �� ���� ������ ������ ��� ���.
        + ��������� ����������� ������ ���� � ������ � GB, � ����� �����
          ���������� ������ ����� ������� � ��������� ��������
        + ��������� ������ ������.
        + ������ ���������/�����������/�����������
    2.2.4
        + ��������� �� ������ � ������ ��������, �� �  ����� ��������
          ������ ����� ������ �������� ������ (����� ������)
    2.2.3
        + ��������� � ��������� ����������������� �����.
        + ����� �������� �� ����� ����������� ������
        + ����������� ���������� � ������� ����� (������� �������������, �������, ������).
        + ������ ���������� ���������� � ����.
        + ������ ���������� ���������� � ����������� � ���������.
        + ������ ���������� ���������� � ����� � �����.
        + ����� ���� ������ �� ����� ������.
        + ����� ����������� ��������� ��� ������ ��� ������ �� ����� ������.
        + ����� ��������� ������ � ������ �������� ��� ���.
    2.2.2
        + ���������� ������ � ��������������� ������� ��� ������ � ����
    2.2.1
        + ���������/�����������/�����������
    2.2
        + ����� �������� ��������� (������ "�������" ���� ����� �������� �
          ������� � ����� ������� � �������)
        + ������ ���������/�����������/�����������
    2.1
        + ��������� �� ������ ������ � ������ ��������
    2.0.9
        + ��������� � ��������� ����������������� �����.
        + ���������� ���������������� ��������� ��������� �� 6.
        + ������ ����� � �������� "Ok"
        + ������� ��������� ���������� � ������ � ���� �������
        + ������ ���������/�����������/�����������
    2.0.8
        + ��������� � ��������� ����������������� �����.
        + ����� �������� � ������ �� ����� ����������� �����
        + ����������� ���������� � ������� �����.
          ���� ������ ID3v1.0 (�������� �����, ������, �����������, ���, ����)
    2.0.7
        + ��������� � ��������� ����������������� �����.
        + ������ ��������� � ������ �� ���������� � ������. ��� ������� �� ����� ������ � ������ ������������.
        + �������������� ����� ��������� (����+���) �������� � �������.
        + �������������� ������ �������� � �� ����� (����+���) �������� � �������.
    2.0.6
        + ���������� ������ ����������������� �����. (���������� ������������� ������� Release_eng)
        + ��������� � ��������� ����������������� �����.
        + �������� ���� ���������
        + ������ ���������� ������
    2.0.5
        + ������� �������� ��������� �� ������.
    2.0.4
        + ��������� � ��������� ����������������� �����.
        + ���������� ����������� ����� � ����
        + ����������� ������������� ���� � �������/��������� ����� ��� �������� ��� ����
    2.0.3
        + ��������� � ����������� ����� �����������
        + ����� ������� �� ����� ���� � ��������� (���������� ��������, ������� ����)
        + ����� ������ �������� �� 2010 ��� ������
    2.0.2
        + ��������� � ��������� ����������������� �����.
        + ����� �������� ������ ������
        + ����� �������� ����� ���������.
          ��� ������������ �� ������ ���� ��� �� ������ - �������� ������� ����� ����.
          ��� ������������ �� ������� ���� - �������� ������� ������ ����.
        + ���������� ���������������� ��������� ��������� �� ����.
    2.0.1
        + ��������� � ��������� ����������������� �����.
        + ����� �������� ������������. ������ ����� �����->�������->������
        + ������ ����� �� ���������� ����� �������� ��� �� �����, ��� � � ������, � �� ����.
        + ���������� ��������� ������, ��-�� ������� �� ��������� ����� � ����������� Y=0.
        + ������ ���������/�����������/�����������
    2.0 beta 2
        + ��������� � ��������� ����������������� �����.
        + ���������� �������� ��������� �� 6.
        + ����������� ������ �������� ��� �� �����, ��� � � ������, � �� ����.
        + ����������� ����. ����������� ����� "�������" � ������ �������.
        + ����������� ���������� �������
        + ��� ������ ������������ �� ������� ����, ���������� x - ������ �� ������� ����
        + ������ �����������
    2.0 beta
        + ������� �� bcfg
        + �������� �� ������
        + ������� FM �� ������.
    1.0.9
        + ����� ����� ������� "Ok" �� ������
        + ����� ��������� � ���-�����
    1.0.8
        + ����������, ������������ � ����� ������� "Ok" �� ������
        + ������ ���� ������������ ���� ����������� �������� ���� �������
        + ����� ����� ������������ ���� ����������� �������� ���� �������
        + ��������� ���-�����
    1.0.7
        + ������ ���������� ���������� � ������
        + �������� ���������� ����� �������� ��� � hex ��� � � dec
          ����� �������� ������ � hex.

*/
