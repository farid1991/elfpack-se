#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"

#define ELF_BCFG_CONFIG_EVENT 994

#define ELFNAME "Indication v.3"
#define LELFNAME L"Indication"
#define LELFVERSION L"\nv3\n\n(c)Ploik & BigHercules\n\nRespect: Slawwan"

void (*LEDControl_W580)(int,int id,int RED,int GREEN,int BLUE, int br, int delay)=(void (*)(int,int id,int RED,int GREEN,int BLUE,int br,int delay))(0x4529BFA9);

#define COLOR_GET_A(x) ((unsigned int)x>>24)
#define COLOR_GET_R(x) (((unsigned int)x>>16)&0xFF)
#define COLOR_GET_G(x) (((unsigned int)x>>8)&0xFF)
#define COLOR_GET_B(x) ((unsigned int)x&0xFF)

static char myappname[]=ELFNAME;

int lamp = 0;
int screen=0;
u16 timerFlash = 0;
u16 timerREDLED = 0;
u16 timerLED = 0;
u16 timerScreen = 0;
u16 vibraminutetimer = 0;
u16 vibraNminutetimer = 0;
u16 offtimer = 0;
u16 offtimerREDLED = 0;
u16 offtimerLED = 0;
u16 offtimerScreen = 0;

u16 rejectTimer = 0;
u16 muteTimer = 0;

int skipevents;
int checkevents;
int REDLED=0;
int LED=0;
int LED580=1;
int LEDnum=2;
int incom=0;
int connect=0;

void offTimerFlash(u16 timerID, LPARAM lparam);
void offTimerLED(u16 timerID, LPARAM lparam);
void offTimerREDLED(u16 timerID, LPARAM lparam);
void offTimerScreen(u16 timerID, LPARAM lparam);

int isMTCallBook(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"MTCallBook")==0) { return 1; }
  else { return 0; }
};

void onRejectTimer(u16 timerID, LPARAM lparam)
{
  BOOK *bk = FindBook(isMTCallBook);
  if(bk)
  {
    IncommingCall_Reject(bk);
  }
}

void onMuteTimer(u16 timerID, LPARAM lparam)
{
  BOOK *bk = FindBook(isMTCallBook);
  if(bk)
  {
    IncommingCall_Mute(bk);
  }
}

int DisableUP(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM, u16)
{
  if((incom==1) && (cfg_flash_silent==0))
  {
     offTimerFlash(0,0);
  }
  if((incom==1) && (cfg_led_silent==0))
  {
     offTimerLED(0,0);
  }
  if((incom==1) && (cfg_redled_silent==0))
  {
     offTimerREDLED(0,0);
  }
  if((incom==1) && (cfg_screen_silent==0))
  {
     offTimerScreen(0,0);
  }
  return(0);
}

int DisableDOWN(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM, u16)
{
  if((incom==1) && (cfg_flash_silent==0))
  {
     offTimerFlash(0,0);
  }
  if((incom==1) && (cfg_led_silent==0))
  {
     offTimerLED(0,0);
  }
  if((incom==1) && (cfg_redled_silent==0))
  {
     offTimerREDLED(0,0);
  }
  if((incom==1) && (cfg_screen_silent==0))
  {
     offTimerScreen(0,0);
  }
  return(0);
}

void offTimerFlash(u16 timerID, LPARAM lparam)
{
  if(timerFlash)
  {
    Timer_Kill(&timerFlash);
    timerFlash = 0;
    SetLampLevel(0);
  }
}

void offTimerREDLED(u16 timerID, LPARAM lparam)
{
   if(timerREDLED)
   {
     Timer_Kill(&timerREDLED);
     timerREDLED = 0;
     RedLED_Off(0);
   }
   REDLED = 0;
}

void offTimerLED(u16 timerID, LPARAM lparam)
{
   if(timerLED)
   {
     Timer_Kill(&timerLED);
     timerLED = 0;
   }
   if(cfg_phone_type == 1)
   {
     LEDControl_W580(1,0,0,0,0,0,1);
     LEDControl_W580(1,1,0,0,0,0,1);
   }
   else if(cfg_phone_type == 2)
   {
     for(int i=0;i<3;i++)
     OrangeLED_Control(1,i,0,50);
   }
   LED580 = 1;
   LEDnum=0;
   LED = 0;
}

void offTimerScreen(u16 timerID, LPARAM lparam)
{
  if(timerScreen)
  {
    Timer_Kill(&timerScreen);
    timerScreen = 0;
    IndicationDevice_Backlight_FadeToLevel(0,cfg_screen_level);
  }
}

void onVibraMinuteTimer(u16 timerID, LPARAM lparam)
{
   PAudioControl pAC = AudioControl_Init();
   if( !pAC ) pAC = *GetAudioControlPtr();
   AudioControl_Vibrate(pAC, 400, 50, cfg_vibra_at_minute_end_time);
   Timer_ReSet(&vibraminutetimer,60000,onVibraMinuteTimer,0);
}

void onVibraNMinuteTimer(u16 timerID, LPARAM lparam)
{
   PAudioControl pAC = AudioControl_Init();
   if( !pAC ) pAC = *GetAudioControlPtr();
   AudioControl_Vibrate(pAC, 400, 50, cfg_vibra_at_n_minute_end_time);
}

void onTimerFlash(u16 timerID, LPARAM lparam)
{
    switch(GetChipID()&CHIPID_MASK)
    {
        case CHIPID_DB2000:
        case CHIPID_DB2010:
        {
            SetLampLevel(lamp^=0x50);
        }
        break;
        case CHIPID_DB2020:
        case CHIPID_DB3150:
        case CHIPID_DB3200:
        case CHIPID_DB3210:
        case CHIPID_DB3350:
        {
            SetLampLevel(lamp^=1);
        }
        break;
    }
    Timer_ReSet(&timerFlash,cfg_flash_blink_speed,onTimerFlash,0);
}

void onTimerREDLED(u16 timerID, LPARAM lparam)
{
  if(REDLED == 0)
  {
    REDLED = 1;
    RedLED_On(0);
    Timer_ReSet(&timerREDLED,cfg_redled_ontime,onTimerREDLED,0);
  }
  else
  {
    REDLED = 0;
    RedLED_Off(0);
    Timer_ReSet(&timerREDLED,cfg_redled_offtime,onTimerREDLED,0);
  }
}

void onTimerLED(u16 timerID, LPARAM lparam)
{
  if(cfg_phone_type == 1)
  {
    Timer_ReSet(&timerLED,cfg_led_blink_speed,onTimerLED,0);
    if (cfg_led580_blink_mode == 2)
    {
      LEDControl_W580(1, 0, COLOR_GET_R(cfg_led580_color),COLOR_GET_G(cfg_led580_color),COLOR_GET_B(cfg_led580_color),0,LED580 ^= 1);
      LEDControl_W580(1, 1, COLOR_GET_R(cfg_led580_color),COLOR_GET_G(cfg_led580_color),COLOR_GET_B(cfg_led580_color),0,LED580);
    }
    else
    {
      LEDControl_W580(1, cfg_led580_blink_mode, COLOR_GET_R(cfg_led580_color),COLOR_GET_G(cfg_led580_color),COLOR_GET_B(cfg_led580_color),0,LED580 ^= 1);
    }
  }
  else if(cfg_phone_type == 2)
  {
    Timer_ReSet(&timerLED,100,onTimerLED,0);
    if (cfg_led_blink_mode)
    {
      OrangeLED_Control(1,LEDnum,0,cfg_led_blink_speed);
      if (LEDnum == 2) LEDnum=0;
      else LEDnum++;
      OrangeLED_Control(1,LEDnum,100,cfg_led_blink_speed);
    }
    else
      OrangeLED_Control(1,0,LED^=0x64,cfg_led_blink_speed);
  }
}

void onTimerScreen(u16 timerID, LPARAM lparam)
{
    IndicationDevice_Backlight_FadeToLevel(0,screen ^= cfg_screen_level);
    Timer_ReSet(&timerScreen,cfg_screen_blink_speed,onTimerScreen,0);
}

void elf_exit(void)
{
    kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void bookOnDestroy(BOOK * book)
{
    if(timerFlash)
    {
        Timer_Kill(&timerFlash);
        offTimerFlash(0,0);
    }
    if(offtimer) Timer_Kill(&offtimer);
    if(offtimerREDLED)
    {
        Timer_Kill(&offtimerREDLED);
        offTimerREDLED(0,0);
    }
    if(offtimerLED)
    {
        Timer_Kill(&offtimerLED);
        offTimerLED(0,0);
    }
    {
        PAudioControl pAC = AudioControl_Init();
        if( !pAC ) pAC = *GetAudioControlPtr();
        AudioControl_Vibrate(pAC, 0, 0, 0);
    }
    if(timerScreen) offTimerScreen(0,0);
    if(offtimerScreen) Timer_Kill(&offtimerScreen);
    if(vibraminutetimer) Timer_Kill(&vibraminutetimer);
    if(vibraNminutetimer) Timer_Kill(&vibraNminutetimer);

    if(rejectTimer) Timer_Kill(&rejectTimer);
    if(muteTimer) Timer_Kill(&muteTimer);

    ModifyUIPageHook(VOLUMEUPKEY_SHORT_PRESS_EVENT,DisableUP,0,PAGE_HOOK_REMOVE);
    ModifyUIPageHook(VOLUMEDOWNKEY_SHORT_PRESS_EVENT,DisableDOWN,0,PAGE_HOOK_REMOVE);
    StatusIndication_ShowNotes(EMPTY_TEXTID);
    SUBPROC(elf_exit);
}


int onExit(void* r0,BOOK* b)
{
    FreeBook(b);
    return 1;
}

int onAbout(void* mess, BOOK* b)
{
    MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,5000, *(BOOK**)mess);
    return 1;
}

enum
{
    CALLMANAGER_IDLE            =0,
    CALLMANAGER_CALL_CONNECTED  =1, /*Поднятие трубки*/
    CALLMANAGER_CALL_SETUP      =2, /*Нажатие вызвать*/
    CALLMANAGER_CALL_ALERT      =4, /*Входящий вызов*/
    CALLMANAGER_CALL_WAITING    =5,
    CALLMANAGER_CALL_TERMINATED =6  /*Завершение соединения*/
};

/*Входящее сообщение*/
int incomingSms(void* r0,BOOK* b)
{
    if(cfg_vibraSms==1)
    {
        PAudioControl pAC = AudioControl_Init();
        if( !pAC ) pAC = *GetAudioControlPtr();
        AudioControl_Vibrate(pAC, 400, 50, cfg_vibraSms_time);
    }
    if(cfg_flashSms==1)
    {
        timerFlash=Timer_Set(70,onTimerFlash,0);
        offtimer=Timer_Set(cfg_flashSms_time*1000,offTimerFlash,0);
    }
    if(cfg_redledSms==1)
    {
        timerREDLED=Timer_Set(cfg_redled_ontime,onTimerREDLED,0);
        offtimerREDLED=Timer_Set(cfg_redledSms_time*1000,offTimerREDLED,0);
    }
    if(cfg_ledSms==1)
    {
        timerLED=Timer_Set(cfg_led_blink_speed,onTimerLED,0);
        offtimerLED=Timer_Set(cfg_ledSms_time*1000,offTimerLED,0);
    }
    if(cfg_screenSms==1)
    {
        timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
        offtimerScreen=Timer_Set(cfg_screenSms_time*1000,offTimerScreen,0);
    }
    return(0);
}

/*Начало гудков*/
int Connect(void* r0,BOOK* b)
{
    if(cfg_vibraring==1)
    {
        PAudioControl pAC = AudioControl_Init();
        if( !pAC ) pAC = *GetAudioControlPtr();
        AudioControl_Vibrate(pAC, 400, 50, cfg_vibraring_time);
    }
    if(cfg_flashring==1)
    {
        timerFlash=Timer_Set(70,onTimerFlash,0);
        offtimer=Timer_Set(cfg_flashring_time*1000,offTimerFlash,0);
    }
    if(cfg_redledring==1)
    {
        timerREDLED=Timer_Set(cfg_redled_ontime,onTimerREDLED,0);
        offtimerREDLED=Timer_Set(cfg_redledring_time*1000,offTimerREDLED,0);
    }
    if(cfg_ledring==1)
    {
        timerLED=Timer_Set(cfg_led_blink_speed,onTimerLED,0);
        offtimerLED=Timer_Set(cfg_ledring_time*1000,offTimerLED,0);
    }
    if(cfg_screenring==1)
    {
        timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
        offtimerScreen=Timer_Set(cfg_screenring_time*1000,offTimerScreen,0);
    }
    return(0);
}

int OnCallManagerEvent(void* r0,BOOK* b)
{
    int eventdata;

    switch (GetChipID()&CHIPID_MASK)
    {
      case CHIPID_DB2000:
      case CHIPID_DB2010:
      case CHIPID_DB2020:
                 eventdata= (((CALLMANAGER_EVENT_DATA*)r0)->CallState);
                 break;
      case CHIPID_DB3150:
      case CHIPID_DB3200:
      case CHIPID_DB3210:
      case CHIPID_DB3350:
                 eventdata=(((CALLMANAGER_EVENT_DATA_A2*)r0)->CallState);
        }
        switch (eventdata)
        {
      /*Входящий вызов*/
        case CALLMANAGER_CALL_ALERT:
        {
            if(GetVibrator(0,0) && (cfg_vibra==0))
            {
                PAudioControl pAC = AudioControl_Init();
                if( !pAC ) pAC = *GetAudioControlPtr();
                AudioControl_Vibrate(pAC, 0, 0, 0);
            }
            if(cfg_flash==1)
            {
                timerFlash=Timer_Set(cfg_flash_blink_speed,onTimerFlash,0);
                if(cfg_flash_time > 0) offtimer=Timer_Set(cfg_flash_time*1000,offTimerFlash,0);
                incom=1;
            }
            if(cfg_redled==1)
            {
                timerREDLED=Timer_Set(cfg_redled_ontime,onTimerREDLED,0);
                if(cfg_redled_time > 0) offtimerREDLED=Timer_Set(cfg_redled_time*1000,offTimerREDLED,0);
            }
            if(cfg_led==1)
            {
                timerLED=Timer_Set(cfg_led_blink_speed,onTimerLED,0);
                if(cfg_led_time > 0) offtimerLED=Timer_Set(cfg_led_time*1000,offTimerLED,0);
            }
            if(cfg_screen==1)
            {
                timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
                if(cfg_screen_time > 0) offtimerScreen=Timer_Set(cfg_screen_time*1000,offTimerScreen,0);
            }
            if(cfg_reject_time)
            {
                rejectTimer=Timer_Set(cfg_reject_time*1000,onRejectTimer,0);
            }
            if(cfg_mute_time && (!GetSilent()))
            {
                muteTimer=Timer_Set(cfg_mute_time*1000,onMuteTimer,0);
            }
            break;
        }
       /*Поднятие трубки*/
        case CALLMANAGER_CALL_CONNECTED:
        {
            if(timerFlash) offTimerFlash(0,0);
            if(timerLED) offTimerLED(0,0);
            if(timerREDLED) offTimerREDLED(0,0);
            if(timerScreen) offTimerScreen(0,0);

            if((cfg_vibracon==1) && (incom==0))
            {
                PAudioControl pAC = AudioControl_Init();
                if( !pAC ) pAC = *GetAudioControlPtr();
                AudioControl_Vibrate(pAC, 400, 50, cfg_vibracon_time);
            }
            if((cfg_flashcon==1) && (incom==0))
            {
                timerFlash=Timer_Set(cfg_flash_blink_speed,onTimerFlash,0);
                offtimer=Timer_Set(cfg_flashcon_time*1000,offTimerFlash,0);
            }
            if((cfg_redledcon==1) && (incom==0))
            {
                timerREDLED=Timer_Set(cfg_redled_ontime,onTimerREDLED,0);
                offtimerREDLED=Timer_Set(cfg_redledcon_time*1000,offTimerREDLED,0);
            }
            if((cfg_ledcon==1) && (incom==0))
            {
                timerLED=Timer_Set(cfg_led_blink_speed,onTimerLED,0);
                offtimerLED=Timer_Set(cfg_ledcon_time*1000,offTimerLED,0);
            }
            if((cfg_screencon==1) && (incom==0))
            {
                timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
                offtimerScreen=Timer_Set(cfg_screencon_time*1000,offTimerScreen,0);
            }
            if(cfg_vibra_at_minute_end==1)
            {
                   vibraminutetimer = Timer_Set((60 - cfg_vibra_at_minute_end_lead) * 1000 ,onVibraMinuteTimer,0);
            }
            if(cfg_vibra_at_n_minute_end > 0)
            {
                   vibraNminutetimer = Timer_Set((cfg_vibra_at_n_minute_end * 60 - cfg_vibra_at_n_minute_end_lead) * 1000 ,onVibraNMinuteTimer,0);
            }
            connect = 1;
            break;
        }
         /*Нажатие вызвать*/
        case CALLMANAGER_CALL_SETUP:
        {
            if(cfg_vibraon==1)
            {
                PAudioControl pAC = AudioControl_Init();
                if( !pAC ) pAC = *GetAudioControlPtr();
                AudioControl_Vibrate(pAC, 400, 50, cfg_vibraon_time);
            }
            if(cfg_flashon==1)
            {
                timerFlash=Timer_Set(cfg_flash_blink_speed,onTimerFlash,0);
                offtimer=Timer_Set(cfg_flashon_time*1000,offTimerFlash,0);
            }
            if(cfg_redledon==1)
            {
                timerREDLED=Timer_Set(cfg_redled_ontime,onTimerREDLED,0);
                offtimerREDLED=Timer_Set(cfg_redledon_time*1000,offTimerREDLED,0);
            }
            if(cfg_ledon==1)
            {
                timerLED=Timer_Set(cfg_led_blink_speed,onTimerLED,0);
                offtimerLED=Timer_Set(cfg_ledon_time*1000,offTimerLED,0);
            }
            if(cfg_screenon==1)
            {
                timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
                offtimerScreen=Timer_Set(cfg_screenon_time*1000,offTimerScreen,0);
            }
            break;
        }
         /*Завершение соединения*/
        case CALLMANAGER_CALL_TERMINATED:
        {
            if(cfg_flash  == 1) offTimerFlash(0,0);
            if(cfg_redled == 1) offTimerREDLED(0,0);
            if(cfg_led    == 1) offTimerLED(0,0);
            if(cfg_screen == 1) offTimerScreen(0,0);

            if((cfg_vibraend == 1) && (connect == 1))
            {
                PAudioControl pAC = AudioControl_Init();
                if( !pAC ) pAC = *GetAudioControlPtr();
                AudioControl_Vibrate(pAC, 400, 50, cfg_vibraend_time);
            }
            if((cfg_flashend == 1) && (connect == 1))
            {
                timerFlash=Timer_Set(cfg_flash_blink_speed,onTimerFlash,0);
                offtimer=Timer_Set(cfg_flashend_time*1000,offTimerFlash,0);
            }
            if((cfg_screenend==1) && (connect == 1))
            {
                timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
                offtimerScreen=Timer_Set(cfg_screenend_time*1000,offTimerScreen,0);
            }
            if((cfg_redledend==1) && (connect == 1))
            {
                timerREDLED=Timer_Set(cfg_redled_ontime,onTimerREDLED,0);
                offtimerREDLED=Timer_Set(cfg_redledend_time*1000,offTimerREDLED,0);
            }
            if((cfg_ledend==1) && (connect == 1))
            {
                timerLED=Timer_Set(cfg_led_blink_speed,onTimerLED,0);
                offtimerLED=Timer_Set(cfg_ledend_time*1000,offTimerLED,0);
            }

            if(vibraminutetimer)
            {
                Timer_Kill(&vibraminutetimer);
                vibraminutetimer = 0;
            }
            if(vibraNminutetimer)
            {
                Timer_Kill(&vibraNminutetimer);
                vibraNminutetimer = 0;
            }
            if(rejectTimer)
            {
              Timer_Kill(&rejectTimer);
              rejectTimer = 0;
            }
            if(muteTimer)
            {
              Timer_Kill(&muteTimer);
              muteTimer = 0;
            }
            incom   = 0;
            connect = 0;
            break;
        }
    }
    return 1;
}

static int onReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    result=1;
  }
  return(result);
}

int onBcfgConfig(void* mess,BOOK* b)
{
  FSTAT _fstat;
  wchar_t path[256];

  if(fstat(GetDir(DIR_ELFS|MEM_INTERNAL),L"BcfgEdit.elf",&_fstat)==0)
  {
    wstrcpy(path,GetDir(DIR_ELFS|MEM_INTERNAL));
  }
  else if(fstat(GetDir(DIR_ELFS|MEM_EXTERNAL),L"BcfgEdit.elf",&_fstat)==0)
  {
    wstrcpy(path,GetDir(DIR_ELFS|MEM_EXTERNAL));
  }
  else
  {
    #ifndef ENG
      MessageBox(EMPTY_TEXTID, STR("BcfgEdit.elf не найден!"), NOIMAGE, 1 ,5000, 0);
    #else
      MessageBox(EMPTY_TEXTID, STR("BcfgEdit.elf not found!"), NOIMAGE, 1 ,5000, 0);
    #endif
    return (1);
  }
  wstrcat(path,L"/BcfgEdit.elf");
  elfload(path,(void*)successed_config_path,(void*)successed_config_name,0);
  return (1);
}

const PAGE_MSG evtlist[] @ "DYN_PAGE"=
{
  ELF_TERMINATE_EVENT,                   onExit,
  ELF_SHOW_INFO_EVENT,                   onAbout,
  ELF_RECONFIG_EVENT,                    onReconfigElf,
  UI_MESSAGING_SMS_RECEIVED_EVENT_TAG,   incomingSms,
  ON_CALLMANAGER_EVENT_TAG,              OnCallManagerEvent,
  ONGOINGCALL_CALL_CONNECTED_EVENT_TAG , Connect,
  ELF_BCFG_CONFIG_EVENT,                 onBcfgConfig,
  NIL_EVENT_TAG,                         NULL
};

const PAGE_DESC defaultpage = {"Indication_Base_page",0,evtlist};

int isChallengeBook(BOOK * book)
{
        if(!strcmp(book->xbook->name,myappname)) return true;
        return false;
}

int main(wchar_t* filename)
{
        if(FindBook(isChallengeBook))
        {
            #ifndef ENG
                MessageBox(EMPTY_TEXTID, STR("Indication of Call\nуже запущен"), NOIMAGE, 1 ,5000, 0);
            #else
                MessageBox(EMPTY_TEXTID, STR("Indication of Call\nalready runned"), NOIMAGE, 1 ,5000, 0);
            #endif
            SUBPROC(elf_exit);
        }
        else
        {
            InitConfig();

            ModifyUIPageHook(VOLUMEUPKEY_SHORT_PRESS_EVENT,DisableUP,0,PAGE_HOOK_ADD_BEFORE);
            ModifyUIPageHook(VOLUMEDOWNKEY_SHORT_PRESS_EVENT,DisableDOWN,0,PAGE_HOOK_ADD_BEFORE);
            BOOK *myBook = (BOOK*)malloc(sizeof(BOOK));
            memset(myBook,0,sizeof(BOOK));
            if(!CreateBook(myBook,bookOnDestroy,&defaultpage,myappname,-1,0))
            {
              delete myBook;
              SUBPROC(elf_exit);
              return 0;
            }

            if(!wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)))
              StatusIndication_ShowNotes(EMPTY_TEXTID);
        }
        return 0;
}
