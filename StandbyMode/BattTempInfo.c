#include "..\include\Lib_Clara.h"
#include "BattTempInfo.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"
#include "MediaInfo.h"
#include "conf_loader.h"

#pragma swi_number=0x82DC
__swi __arm  int addrof_OrangeLED_Control(void);


// Глобальные переменные
unsigned int found_full_empty[3]={0,0,0}; //0-found, 1-full, 3-EMPTY_TEXTID
unsigned int levels_up[9]   ={0,0,0,0,0,0,0,0};//0-10,...,8-90
unsigned int levels_down[10]={0,0,0,0,0,0,0,0,0};//0-5,...,9-90
unsigned int flag=0;

wchar_t charge_snd_path[256] = {0};

bool haveOrangeLEDFunc = false;

/*
BATTERY_CAPACITY_CHANGED_EVENT
BATTERY_CHARGING_ALERT_EVENT
*/

#define BATTERY_LEV0 0
#define BATTERY_LEV1 5
#define BATTERY_LEV2 18
#define BATTERY_LEV3 27
#define BATTERY_LEV4 35
#define BATTERY_LEV5 45
#define BATTERY_LEV6 55
#define BATTERY_LEV7 65
#define BATTERY_LEV8 75
#define BATTERY_LEV9 86
#define BATTERY_LEV10 100

IMAGEID batt_images[BATT_IMAGE_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
IMAGEID batt_charge = NOIMAGE;

TEXTID batt_icons[BATT_IMAGE_COUNT] = {EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID,EMPTY_TEXTID};
TEXTID batt_charge_icon = EMPTY_TEXTID;

TEXTID batt_icn;
TEXTID batt_charge_icn;

const wchar_t *batt_file[BATT_IMAGE_COUNT] = {
  cfg_batt_0_file,
  cfg_batt_5_file,
  cfg_batt_15_file,
  cfg_batt_25_file,
  cfg_batt_35_file,
  cfg_batt_45_file,
  cfg_batt_55_file,
  cfg_batt_65_file,
  cfg_batt_75_file,
  cfg_batt_85_file,
  cfg_batt_100_file
};

const wchar_t* batt_icon_names[BATT_IMAGE_COUNT] = {
    L"C_UI_BATTERY_LEV0_ICN",
    L"C_UI_BATTERY_LEV1_ICN",
    L"C_UI_BATTERY_LEV2_ICN",
    L"C_UI_BATTERY_LEV3_ICN",
    L"C_UI_BATTERY_LEV4_ICN",
    L"C_UI_BATTERY_LEV5_ICN",
    L"C_UI_BATTERY_LEV6_ICN",
    L"C_UI_BATTERY_LEV7_ICN",
    L"C_UI_BATTERY_LEV8_ICN",
    L"C_UI_BATTERY_LEV9_ICN",
    L"C_UI_BATTERY_LEV10_ICN"
};

unsigned char percents[BATT_IMAGE_COUNT] = {0,5,15,25,35,45,55,65,75,85,100};

RECT batt_add_bar;
RECT batt_capacity_bar;

unsigned int batt_add_color = 0xFFFFFFFF;
unsigned int batt_capacity_color = 0xFFFFFFFF;

TEXTID battID = EMPTY_TEXTID;
TEXTID batt_percentID = EMPTY_TEXTID;
TEXTID batt_tempID = EMPTY_TEXTID;
TEXTID currentID = EMPTY_TEXTID;
TEXTID sys_tempID = EMPTY_TEXTID;

unsigned int batt_percent_y = 0;
unsigned int battWidth = 0;
unsigned int percentWidth = 0;

int MeasRemainingCapacity;
int CurRemainingCapacity;

bool isCharging = false;
bool showBattTemp = false;

int batt_show = 0;
int batt_bar_show = 0;
int current_show = 0;
int batt_temp_show = 0;
int temp_show = 0;
int batt_charge_show = 0;

u16 timer_batt_tempInfo = 0;
u16 timer_batt_anim = 0;

u16 timer_batt_redled = 0;
u16 timer_batt_led = 0;

static int FirstRun = 1;

void BeginTimerRedLed(u16 timerID, LPARAM lparam);
void BeginTimerLed(u16 timerID, LPARAM lparam);

//=============================Проигрывание звука===============================
void Play(const wchar_t *filename)
{
  if((!GetSilent() || (cfg_charge_silent==1)))
  {
    char vol = cfg_charge_volume;
//    GetAudioSettings(2,&vol);
    PlayFileV(charge_snd_path, filename, vol);
  }
}

void SayUp(int num)
{
  wchar_t level[16];
  snwprintf(level, 15, L"up_%d.wav", num);
  Play(level);
}

void SayDown(int num)
{
  wchar_t level[16];
  snwprintf(level, 15, L"down_%d.wav", num);
  Play(level);
}

int IsMediaActive(void)
{
  return 0;
}

int IsCalling(void)
{
  return 0;
}

void Check(bool isCharge, int cap_akku)
{
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC,&dt);
  if(cfg_crc == REF_CRC32)
  if(((dt.time.hour*60 + dt.time.min) <= (cfg_charge_TimeTo.hour*60+cfg_charge_TimeTo.min)) &&
     ((dt.time.hour*60 + dt.time.min) >= (cfg_charge_TimeFrom.hour*60+cfg_charge_TimeFrom.min)) &&
     (!IsCalling()))
  {
      //state 0-разрядка
      //      1-зарядка
      //      2-полный
      int state = 0;
      if(isCharge) state = 1;
      if(cap_akku == 100) state = 2;

      //unsigned int found_full_empty[3] //0-found, 1-full, 2-EMPTY_TEXTID

      switch (state)
      {
        case 0:
          if (found_full_empty[0]||found_full_empty[1])
            {
              if (!IsMediaActive()) Play(L"out.wav");

              found_full_empty[0]=0;
              found_full_empty[1]=0;

              unsigned int i;
              for (i=0; i<=8; i++) {levels_up[i]=0;}
              break;
            }

          if ((cfg_charge_say_levels_down) && (cap_akku <= cfg_charge_begin_from))
            {//Говорить промежуточные уовни разрядки
              //unsigned int levels_down[10]={0,0,0,0,0,0,0,0,0};//0-5,...,9-90
              if((cap_akku > 5) && (cap_akku <= 10))
              {
                  if (!levels_down[1]) {levels_down[1]=1; SayDown(10);}
              }
              else if(cap_akku <= 20)
              {
                  if (!levels_down[2]) {levels_down[2]=1; SayDown(20);}
              }
              else if(cap_akku <= 30)
              {
                  if (!levels_down[3]) {levels_down[3]=1; SayDown(30);}
              }
              else if(cap_akku <= 40)
              {
                  if (!levels_down[4]) {levels_down[4]=1; SayDown(40);}
              }
              else if(cap_akku <= 50)
              {
                  if (!levels_down[5]) {levels_down[5]=1; SayDown(50);}
              }
              else if(cap_akku <= 60)
              {
                  if (!levels_down[6]) {levels_down[6]=1; SayDown(60);}
              }
              else if(cap_akku <= 70)
              {
                  if (!levels_down[7]) {levels_down[7]=1; SayDown(70);}
              }
              else if(cap_akku <= 80)
              {
                  if (!levels_down[8]) {levels_down[8]=1; SayDown(80);}
              }
              else if(cap_akku <= 90)
              {
                  if (!levels_down[9]) {levels_down[9]=1; SayDown(90);}
              }
            } //Говорить промежуточные уовни разрядки


          if ((!levels_down[0]) && (cap_akku > 0) && (cap_akku <= 5))
            {//-5% Говорить в любом случае
              levels_down[0]=1;
              SayDown(5);
            }

          if ((!found_full_empty[2]) && (cap_akku == 0))
            {//0% Говорить в любом случае
              found_full_empty[2]=1;
              Play(L"0.wav");
            }

        break;

        case 1:
          if (!found_full_empty[0])
            {// Найден источник

              if (!IsMediaActive()) Play(L"found.wav");

              found_full_empty[0]=1;

              //Обнуление флагов
              found_full_empty[1]=0;

              unsigned int i;
              for (i=0; i<=8; i++) {levels_up[i]=0;}
              for (i=0; i<=9; i++) {levels_down[i]=0;}
              break;
            }// Найден источник
          if ((found_full_empty[0]) && (cfg_charge_say_levels_up))
            {//Говорить промежуточные уровни зарядки
              if(cap_akku >= 90)
              {
                  if (!levels_up[8]) {levels_up[8]=1; SayUp(90);}
              }
              else if(cap_akku >= 80)
              {
                  if (!levels_up[7]) {levels_up[7]=1; SayUp(80);}
              }
              else if(cap_akku >= 70)
              {
                  if (!levels_up[6]) {levels_up[6]=1; SayUp(70);}
              }
              else if(cap_akku >= 60)
              {
                  if (!levels_up[5]) {levels_up[5]=1; SayUp(60);}
              }
              else if(cap_akku >= 50)
              {
                  if (!levels_up[4]) {levels_up[4]=1; SayUp(50);}
              }
              else if(cap_akku >= 40)
              {
                  if (!levels_up[3]) {levels_up[3]=1; SayUp(40);}
              }
              else if(cap_akku >= 30)
              {
                  if (!levels_up[2]) {levels_up[2]=1; SayUp(30);}
              }
              else if(cap_akku >= 20)
              {
                  if (!levels_up[1]) {levels_up[1]=1; SayUp(20);}
              }
              else if(cap_akku >= 10)
              {
                  if (!levels_up[0]) {levels_up[0]=1; SayUp(10);}
              }
            }//Говорить промежуточные уровни зарядки
        break;

        case 2:
          if (!found_full_empty[0]) found_full_empty[0]=1;
          if ((!found_full_empty[1])&&(found_full_empty[0]))
            {// Энергия восстановлена
              found_full_empty[1]=1;

              if (!IsMediaActive()) Play(L"full.wav");

              //Обнуление флагов
              found_full_empty[0]=0;
              found_full_empty[2]=0;

              unsigned int i;
              for (i=0; i<=9; i++) {levels_down[i]=0;}
            }// Энергия восстановлена
        break;
      }

  }//if ((time.hour>=from_h)&&(time.hour<=to_h))
}

void onTimerRedLed (u16 timerID, LPARAM lparam)
{
  RedLED_Off(0);
  Timer_ReSet(&timer_batt_redled, cfg_charge_RedLed_OffTime, BeginTimerRedLed, 0);
}

void BeginTimerRedLed(u16 timerID, LPARAM lparam)
{
  BATT batt;
  GetBattaryState(SYNC,&batt);
  if(cfg_charge_RedLed_show && ((batt.ChargingState == 2) || (batt.ChargingState == 8)) && (batt.RemainingCapacityInPercent < 100))
  {
    RedLED_On(0);
    Timer_ReSet(&timer_batt_redled, cfg_charge_RedLed_OnTime, onTimerRedLed, 0);
  }
  else
  {
    if(timer_batt_redled)
    {
      Timer_Kill(&timer_batt_redled);
      timer_batt_redled = 0;
    }
  }
}

void onTimerLed (u16 timerID, LPARAM lparam)
{
  if(haveOrangeLEDFunc) OrangeLED_Control(0,0,0,100);
  Timer_ReSet(&timer_batt_led, cfg_charge_Led_OffTime, BeginTimerLed, 0);
}

void BeginTimerLed(u16 timerID, LPARAM lparam)
{
  BATT batt;
  GetBattaryState(SYNC,&batt);
  if(cfg_charge_Led_show && ((batt.ChargingState == 2) || (batt.ChargingState == 8)) && (batt.RemainingCapacityInPercent < 100))
  {
    if(haveOrangeLEDFunc) OrangeLED_Control(1,2,100,100);
    Timer_ReSet(&timer_batt_led, cfg_charge_Led_OnTime, onTimerLed, 0);
  }
  else
  {
    if(timer_batt_led)
    {
      Timer_Kill(&timer_batt_led);
      timer_batt_led = 0;
    }
  }
}

int onBatteryCapacityChanged(void* mess,BOOK* b)
{

  return (0);
}

int onBatteryChargingState(void* mess,BOOK* b)
{
//  MessageBox(EMPTY_TEXTID, STR("Battery Charging State Event!"), NOIMAGE, 1, 0, 0);
  UpdateBattTempInfo();
  BeginTimerRedLed(0,0);
  BeginTimerLed(0,0);
  return (0);
}

extern char USB_state;
int onBatteryChargerDisconnected(void* mess,BOOK* b)
{
//  MessageBox(EMPTY_TEXTID, STR("Battery Charger Disconnected Event!"), NOIMAGE, 1, 0, 0);
  UpdateBattTempInfo();
  USB_state = 0;
  return (0);
}

int onBatteryChargerConnected(void* mess,BOOK* b)
{
  UpdateBattTempInfo();
  return (0);
}

void UpdateBattBar(int RemainingCapacity)
{
    float n,m;
    //Полоска
    n = cfg_batt_bar.x2 - cfg_batt_bar.x1;
    m = cfg_batt_bar.y2 - cfg_batt_bar.y1;

    if(n < m)
    {
      if(cfg_batt_bar_reverse)
      {
        batt_capacity_bar.y1 = cfg_batt_bar.y1;
        batt_capacity_bar.x1 = cfg_batt_bar.x1;
        batt_capacity_bar.x2 = cfg_batt_bar.x2;
        batt_capacity_bar.y2 = cfg_batt_bar.y1 + (RemainingCapacity * m)/100;

        int l = n/3;
        int k = m/20;
        batt_add_bar.x1 = cfg_batt_bar.x1 + ((l>0)? l : 1);
        batt_add_bar.y1 = cfg_batt_bar.y2;
        batt_add_bar.x2 = cfg_batt_bar.x2 - ((l>0)? l : 1);
        batt_add_bar.y2 = cfg_batt_bar.y2 + ((k>0)? k : 1);
      }
      else
      {
        batt_capacity_bar.y1 = cfg_batt_bar.y1 + ((100 - RemainingCapacity) * m)/100;
        batt_capacity_bar.x1 = cfg_batt_bar.x1;
        batt_capacity_bar.x2 = cfg_batt_bar.x2;
        batt_capacity_bar.y2 = cfg_batt_bar.y2;

        int l = n/3;
        int k = m/20;
        batt_add_bar.x1 = cfg_batt_bar.x1 + ((l>0)? l : 1);
        batt_add_bar.y1 = cfg_batt_bar.y1 - ((k>0)? k : 1);
        batt_add_bar.x2 = cfg_batt_bar.x2 - ((l>0)? l : 1);
        batt_add_bar.y2 = cfg_batt_bar.y1;
      }
    }
    else
    {
      if(cfg_batt_bar_reverse)
      {
        batt_capacity_bar.x1 = cfg_batt_bar.x1;
        batt_capacity_bar.y1 = cfg_batt_bar.y1;
        batt_capacity_bar.y2 = cfg_batt_bar.y2;
        batt_capacity_bar.x2 = cfg_batt_bar.x1 + (RemainingCapacity * n)/100;

        int l = n/20;
        int k = m/3;
        batt_add_bar.x1 = cfg_batt_bar.x2;
        batt_add_bar.y1 = cfg_batt_bar.y1 + ((k>0)? k : 1);
        batt_add_bar.x2 = cfg_batt_bar.x2 + ((l>0)? l : 1);
        batt_add_bar.y2 = cfg_batt_bar.y2 - ((k>0)? k : 1);
      }
      else
      {
        batt_capacity_bar.x1 = cfg_batt_bar.x1 + ((100 - RemainingCapacity) * n)/100;
        batt_capacity_bar.y1 = cfg_batt_bar.y1;
        batt_capacity_bar.y2 = cfg_batt_bar.y2;
        batt_capacity_bar.x2 = cfg_batt_bar.x2;

        int l = n/20;
        int k = m/3;
        batt_add_bar.x1 = cfg_batt_bar.x1 - ((l>0)? l : 1);
        batt_add_bar.y1 = cfg_batt_bar.y1 + ((k>0)? k : 1);
        batt_add_bar.x2 = cfg_batt_bar.x1;
        batt_add_bar.y2 = cfg_batt_bar.y2 - ((k>0)? k : 1);
      }
    }

    if(RemainingCapacity == 100) batt_add_color = cfg_batt_100_color;
    else batt_add_color = cfg_batt_empty_color;
    switch(RemainingCapacity/10){
    case 0:
      batt_capacity_color = cfg_batt_0_color;
      break;
    case 1:
      batt_capacity_color = cfg_batt_10_color;
      break;
    case 2:
      batt_capacity_color = cfg_batt_20_color;
      break;
    case 3:
      batt_capacity_color = cfg_batt_30_color;
      break;
    case 4:
      batt_capacity_color = cfg_batt_40_color;
      break;
    case 5:
      batt_capacity_color = cfg_batt_50_color;
      break;
    case 6:
      batt_capacity_color = cfg_batt_60_color;
      break;
    case 7:
      batt_capacity_color = cfg_batt_70_color;
      break;
    case 8:
      batt_capacity_color = cfg_batt_80_color;
      break;
    case 9:  //85
      batt_capacity_color = cfg_batt_90_color;
      break;
    case 10:
      batt_capacity_color = cfg_batt_100_color;
      break;
    default:
      batt_capacity_color = cfg_batt_100_color;
    }
}

void UpdateBattIcon(int RemainingCapacity)
{
    int batt_indx = 0;
    //Images
    if(RemainingCapacity == BATTERY_LEV0)
      batt_indx = 0;
    else if (RemainingCapacity <= BATTERY_LEV1)
      batt_indx = 1;
    else if (RemainingCapacity <= BATTERY_LEV2)
      batt_indx = 2;
    else if (RemainingCapacity <= BATTERY_LEV3)
      batt_indx = 3;
    else if (RemainingCapacity <= BATTERY_LEV4)
      batt_indx = 4;
    else if (RemainingCapacity <= BATTERY_LEV5)
      batt_indx = 5;
    else if (RemainingCapacity <= BATTERY_LEV6)
      batt_indx = 6;
    else if (RemainingCapacity <= BATTERY_LEV7)
      batt_indx = 7;
    else if (RemainingCapacity <= BATTERY_LEV8)
      batt_indx = 8;
    else if (RemainingCapacity <= BATTERY_LEV9)
      batt_indx = 9;
    else
      batt_indx = 10;

    if(cfg_batt_images_custom)
    {
      if(batt_images[batt_indx] != NOIMAGE)
      {
	    switch( ChipID )
		{
		 case CHIPID_DB2000:
		 case CHIPID_DB2010:
		 case CHIPID_DB2020:
		 case CHIPID_PNX5230:
		 {
		  batt_icn = batt_images[batt_indx] | 0x78000000;
		  break;
		 }
		 default:
		  batt_icn = batt_images[batt_indx];
		  break;
		}
      }
      else
      {
	   batt_icn = batt_icons[batt_indx] | 0x78000000;
      }
    }
    else
    {
	 batt_icn = batt_icons[batt_indx] | 0x78000000;
    }
}

void onTimer_batt_anim (u16 timerID, LPARAM lparam)
{
    UpdateBattBar(CurRemainingCapacity);
    UpdateBattIcon(CurRemainingCapacity);

    CurRemainingCapacity += 10;
    if(CurRemainingCapacity >= 100)
    {
        if((MeasRemainingCapacity <= BATTERY_LEV9) || (MeasRemainingCapacity >= BATTERY_LEV10)) CurRemainingCapacity = MeasRemainingCapacity;
        else CurRemainingCapacity = BATTERY_LEV9;
    }
    Timer_ReSet(&timer_batt_anim,cfg_refresh_time,onTimer_batt_anim,0);
}

void UpdateBattTempInfo(void)
{
    wchar_t batt_temp_msg[10];
    wchar_t curr[10];
    wchar_t sys_temp_msg[10];
    BATT batt;

    GetBattaryState(SYNC,&batt);
    MeasRemainingCapacity = (int)batt.RemainingCapacityInPercent;

    if(battID != EMPTY_TEXTID) TextID_Destroy(battID);
    battID = TextID_CreateIntegerID(MeasRemainingCapacity);

    if(cfg_percent)
    {
      if(batt_percentID == EMPTY_TEXTID) batt_percentID = TextID_Create(L"%",ENC_UCS2,1);
      int font_old = SetFont(cfg_batt_font);
      int battHeight = GetImageHeight(battID);
      battWidth = Disp_GetTextIDWidth(battID,TextID_GetLength(battID));
      SetFont(cfg_percent_font);
      int percentHeight = GetImageHeight(batt_percentID);
      percentWidth = Disp_GetTextIDWidth(batt_percentID,TextID_GetLength(batt_percentID));
      SetFont(font_old);
      batt_percent_y = cfg_batt_y + battHeight - percentHeight + cfg_percent_offset_y;
    }
    else
    {
      battWidth = 0;
      percentWidth = 0;
    }

    float Curr=batt.ChargerCurrent;

    switch(ChipID)
    {
     case CHIPID_DB2000:
     case CHIPID_DB2010:
     case CHIPID_DB2020:
         snwprintf(curr,MAXELEMS(curr),L"%.1f mA",Curr/10.0);
      break;
      default:
         snwprintf(curr,MAXELEMS(curr),L"%.1f mA",Curr);
      break;
    }

    if(currentID != EMPTY_TEXTID) TextID_Destroy(currentID);
    currentID = TextID_Create(curr,ENC_UCS2,MAXELEMS(curr));

    int BattTemp=(int)batt.BatteryTemperature;
    snwprintf(batt_temp_msg,MAXELEMS(batt_temp_msg)-1,L"%d%°C", BattTemp);
    if(batt_tempID != EMPTY_TEXTID) TextID_Destroy(batt_tempID);
    batt_tempID = TextID_Create(batt_temp_msg,ENC_UCS2,MAXELEMS(batt_temp_msg));

    if(!cfg_charging_only) showBattTemp = true;
    if((batt.ChargingState == 2) || (batt.ChargingState == 8)) //из SleepMode
    {
      isCharging = true;
      showBattTemp = true;
    }
    else
    {
      isCharging = false;
      if(cfg_charging_only) showBattTemp = false;
    }

    int Temp=(int)batt.SystemTemperature;
    snwprintf(sys_temp_msg,MAXELEMS(sys_temp_msg)-1,L"%d%°C", Temp);
    if(sys_tempID != EMPTY_TEXTID) TextID_Destroy(sys_tempID);
    sys_tempID = TextID_Create(sys_temp_msg,ENC_UCS2,MAXELEMS(sys_temp_msg));

    Check(isCharging, MeasRemainingCapacity);

    if(isCharging)
    {
       if(!timer_batt_redled) BeginTimerRedLed(0,0);
       if(!timer_batt_led)    BeginTimerLed(0,0);
    }

    if(isCharging && cfg_batt_charge_anim)
    {
      if(!timer_batt_anim)
      {
        CurRemainingCapacity = MeasRemainingCapacity;
        Timer_ReSet(&timer_batt_anim,10,onTimer_batt_anim,0);
      }
    }
    else
    {
      if(timer_batt_anim)
      {
        Timer_Kill(&timer_batt_anim);
        timer_batt_anim = 0;
      }
      UpdateBattBar(MeasRemainingCapacity);
      UpdateBattIcon(MeasRemainingCapacity);
    }
}

void ShowBattInfo(int y, int location)
{
   if(batt_bar_show)
   {
     if(cfg_batt_bar_images)
     {
	  if(cfg_batt_images_custom)
      {
	    switch( ChipID )
		{
		 case CHIPID_DB2000:
		 case CHIPID_DB2010:
		 case CHIPID_DB2020:
		 case CHIPID_PNX5230:
		 {
		  DrawString(batt_icn, 0, cfg_batt_bar_x, cfg_batt_bar_y - y, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
		  break;
		 }
		 default:
          if(batt_icn != NOIMAGE)
          {
		   GC *GC_DISP=get_DisplayGC ();
           putchar(GC_DISP, cfg_batt_bar_x, cfg_batt_bar_y - y, 0, 0, batt_icn);
          }
		  break;
		}
      }
	  else
	  {
       DrawString(batt_icn, 0, cfg_batt_bar_x, cfg_batt_bar_y - y, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
	  }
     }
     else
     {
      if(cfg_batt_bar_highlight)
      {
          DrawOwnRect(batt_add_bar.x1, batt_add_bar.y1-y, batt_add_bar.x2, batt_add_bar.y2-y, cfg_batt_bar_border,batt_add_color);
          DrawOwnRect(cfg_batt_bar.x1, cfg_batt_bar.y1-y, cfg_batt_bar.x2, cfg_batt_bar.y2-y, cfg_batt_bar_border,cfg_batt_empty_color);
          DrawOwnRect(batt_capacity_bar.x1, batt_capacity_bar.y1-y, batt_capacity_bar.x2, batt_capacity_bar.y2-y, cfg_batt_bar_border,batt_capacity_color);
      }
      else
      {
          DrawOwnRect(batt_add_bar.x1, batt_add_bar.y1-y, batt_add_bar.x2, batt_add_bar.y2-y, batt_add_color,batt_add_color);
          DrawOwnRect(cfg_batt_bar.x1, cfg_batt_bar.y1-y, cfg_batt_bar.x2, cfg_batt_bar.y2-y, cfg_batt_empty_color,cfg_batt_empty_color);
          DrawOwnRect(batt_capacity_bar.x1, batt_capacity_bar.y1-y, batt_capacity_bar.x2, batt_capacity_bar.y2-y, batt_capacity_color,batt_capacity_color);
      }
     }
   }
   if(isCharging && batt_charge_show)
   {
    if(cfg_batt_charge_custom)
    {
	 switch( ChipID )
	 {
		 case CHIPID_DB2000:
		 case CHIPID_DB2010:
		 case CHIPID_DB2020:
		 case CHIPID_PNX5230:
		 {
		  DrawString(batt_charge_icn, 0, cfg_batt_charge_x, cfg_batt_charge_y - y, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
		  break;
		 }
		 default:
          if(batt_charge_icn != NOIMAGE)
          {
		   GC *GC_DISP=get_DisplayGC ();
           putchar(GC_DISP, cfg_batt_charge_x, cfg_batt_charge_y - y, 0, 0, batt_charge_icn);
          }
		  break;
	 }
    }
	else
	{
     DrawString(batt_charge_icn, 0, cfg_batt_charge_x, cfg_batt_charge_y - y, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
	}
   }
   if (batt_show)
   {
     if(cfg_percent)
     {
       if(cfg_batt_align == RIGHT)
       {
          myDrawString(cfg_batt_font, battID, cfg_batt_align, 1, cfg_batt_y - y, cfg_batt_x - percentWidth, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_highlight);
          myDrawString(cfg_percent_font, batt_percentID, cfg_batt_align, 1, batt_percent_y - y, cfg_batt_x, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_percent_highlight);
       }
       else if(cfg_batt_align == LEFT)
       {
          myDrawString(cfg_batt_font, battID, cfg_batt_align, cfg_batt_x, cfg_batt_y - y, DisplayWidth, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_highlight);
          myDrawString(cfg_percent_font, batt_percentID, cfg_batt_align, cfg_batt_x + battWidth, batt_percent_y - y, DisplayWidth, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_percent_highlight);
       }
       else
       {
          myDrawString(cfg_batt_font, battID, cfg_batt_align, cfg_batt_x, cfg_batt_y - y, DisplayWidth - percentWidth, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_highlight);
          myDrawString(cfg_percent_font, batt_percentID, cfg_batt_align, cfg_batt_x + battWidth , batt_percent_y - y, DisplayWidth, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_percent_highlight);
       }
     }
     else
     {
       if(cfg_batt_align == RIGHT)
          myDrawString(cfg_batt_font, battID, cfg_batt_align, 1, cfg_batt_y - y, cfg_batt_x, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_highlight);
       else
          myDrawString(cfg_batt_font, battID, cfg_batt_align, cfg_batt_x, cfg_batt_y - y, DisplayWidth, DisplayHeight, cfg_batt_border, cfg_batt_color, cfg_batt_highlight);
     }
   }
}

void ShowBattTempInfo(int y, int location)
{
   if (temp_show)
   {
     if(cfg_temp_align == RIGHT)
        myDrawString(cfg_temp_font, sys_tempID, cfg_temp_align, 1, cfg_temp_y - y, cfg_temp_x, DisplayHeight, cfg_temp_border, cfg_temp_color, cfg_temp_highlight);
     else
        myDrawString(cfg_temp_font, sys_tempID, cfg_temp_align, cfg_temp_x, cfg_temp_y - y, DisplayWidth, DisplayHeight, cfg_temp_border, cfg_temp_color, cfg_temp_highlight);
   }

   ShowBattInfo(y, location);

   if (batt_temp_show && showBattTemp)
   {
     if(cfg_batt_temp_align == RIGHT)
        myDrawString(cfg_batt_temp_font, batt_tempID, cfg_batt_temp_align, 1, cfg_batt_temp_y - y, cfg_batt_temp_x, DisplayHeight, cfg_batt_temp_border, cfg_batt_temp_color, cfg_batt_temp_highlight);
     else
        myDrawString(cfg_batt_temp_font, batt_tempID, cfg_batt_temp_align, cfg_batt_temp_x, cfg_batt_temp_y - y, DisplayWidth, DisplayHeight, cfg_batt_temp_border, cfg_batt_temp_color, cfg_batt_temp_highlight);
   }
   if (current_show && isCharging)
   {
     if(cfg_current_align == RIGHT)
        myDrawString(cfg_current_font, currentID, cfg_current_align, 1, cfg_current_y - y, cfg_current_x, DisplayHeight, cfg_current_border, cfg_current_color, cfg_current_highlight);
     else
        myDrawString(cfg_current_font, currentID, cfg_current_align, cfg_current_x, cfg_current_y - y, DisplayWidth, DisplayHeight, cfg_current_border, cfg_current_color, cfg_current_highlight);
   }
}

void ShowNonStandbyBattTempInfo(int y, int location)
{
   ShowBattInfo(y, location);
}

void onTimer_batt_tempInfo (u16 timerID, LPARAM lparam)
{
    UpdateBattTempInfo();
    Timer_ReSet(&timer_batt_tempInfo,cfg_refresh_batt_temp,onTimer_batt_tempInfo,0);
}

void UpdateBattTempInfoParams(void)
{
    FSTAT _fstat;

    FreeBattTempInfoImages();
    if(FirstRun)
    {
      for(int i=0;i<BATT_IMAGE_COUNT;i++)
      {
        iconidname2id( batt_icon_names[i], -1, &batt_icons[i]);
      }

      iconidname2id( L"C_UI_BATTERY_CHARGING_ICN", -1, &batt_charge_icon);

      FirstRun = 0;
    }

    int addr = addrof_OrangeLED_Control();
    if (addr > 0 && addr < 0xFFFFFFFF)
    {
       haveOrangeLEDFunc = true;
    }
    else haveOrangeLEDFunc = false;

    getdir(charge_snd_path, cfg_charge_folder_path);

    if((cfg_batt_bar_show) && (cfg_batt_bar_images) && (cfg_batt_images_custom))
    {
      for (int i=0;i<BATT_IMAGE_COUNT;i++)
      {
        extractdir(tmppath, tmpname, batt_file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("Картинка для %d%% не найдена"),percents[i]);
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&batt_images[i])<0) batt_images[i] = NOIMAGE;
      }
    }

    if((cfg_batt_charge_show) && (cfg_batt_charge_custom))
    {
        extractdir(tmppath, tmpname, cfg_batt_charge_file);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("Картинка для режима зарядки не найдена"));
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&batt_charge)<0) batt_charge = NOIMAGE;
    }

    if(cfg_batt_charge_custom)
    {
      if(batt_charge != NOIMAGE)
      {
	   switch( ChipID )
	   {
	    case CHIPID_DB2000:
	    case CHIPID_DB2010:
	    case CHIPID_DB2020:
	    case CHIPID_PNX5230:
	    {
         batt_charge_icn = batt_charge | 0x78000000;
		 break;
        }
		default:
		 batt_charge_icn = batt_charge;
		 break;
	   }
	  }
      else
      {
	   batt_charge_icn = batt_charge_icon + 0x78000000;
	  }
    }
    else
    {
	 batt_charge_icn = batt_charge_icon + 0x78000000;
    }

    if(timer_batt_anim)
    {
      Timer_Kill(&timer_batt_anim);
      timer_batt_anim = 0;
    }

    if(cfg_batt_show || cfg_batt_bar_show || cfg_current_show ||
       cfg_batt_charge_show || cfg_batt_temp_show || cfg_temp_show ||
       cfg_charge_say_levels_up || cfg_charge_say_levels_down ||
       cfg_charge_RedLed_show || cfg_charge_Led_show)
    {
      UpdateBattTempInfo();
      Timer_ReSet(&timer_batt_tempInfo,cfg_refresh_batt_temp,onTimer_batt_tempInfo,0);
    }
    else if(timer_batt_tempInfo)
    {
      Timer_Kill(&timer_batt_tempInfo);
      timer_batt_tempInfo = 0;
    }

    BeginTimerRedLed(0,0);
    BeginTimerLed(0,0);
}

void UpdateBattTempShowParams(void)
{
    batt_show = cfg_batt_show && (!(PlayerIsStarted && cfg_batt_not_show_if_player));
    batt_bar_show = cfg_batt_bar_show && (!(PlayerIsStarted && cfg_batt_bar_not_show_if_player));
    current_show = cfg_current_show && (!(PlayerIsStarted && cfg_current_not_show_if_player));
    batt_temp_show = cfg_batt_temp_show && (!(PlayerIsStarted && cfg_batt_temp_not_show_if_player));
    temp_show = cfg_temp_show && (!(PlayerIsStarted && cfg_temp_not_show_if_player));
    batt_charge_show = cfg_batt_charge_show && (!(PlayerIsStarted && cfg_batt_charge_not_show_if_player));
}

void FreeBattTempInfoImages(void)
{
    for (int i=0;i<BATT_IMAGE_COUNT;i++)
    {
        if(batt_images[i] != NOIMAGE)
        {
          ImageID_Free(batt_images[i]);
          batt_images[i] = NOIMAGE;
        }
    }
    if(batt_charge != NOIMAGE)
    {
      ImageID_Free(batt_charge);
      batt_charge = NOIMAGE;
    }
}

void FreeBattTempInfoParams(void)
{

    if(timer_batt_redled)
    {
      Timer_Kill(&timer_batt_redled);
      timer_batt_redled = 0;
    }
    RedLED_Off(0);

    if(timer_batt_led)
    {
      Timer_Kill(&timer_batt_led);
      timer_batt_led = 0;
    }
    if(haveOrangeLEDFunc) OrangeLED_Control(0,0,0,100);

    if(timer_batt_anim)
    {
      Timer_Kill(&timer_batt_anim);
      timer_batt_anim = 0;
    }

    if(timer_batt_tempInfo)
    {
      Timer_Kill(&timer_batt_tempInfo);
      timer_batt_tempInfo = 0;
    }

    FreeBattTempInfoImages();

    if(battID != EMPTY_TEXTID)
    {
      TextID_Destroy(battID);
      battID = EMPTY_TEXTID;
    }
    if(batt_percentID != EMPTY_TEXTID)
    {
      TextID_Destroy(batt_percentID);
      batt_percentID = EMPTY_TEXTID;
    }
    if(batt_tempID != EMPTY_TEXTID)
    {
      TextID_Destroy(batt_tempID);
      batt_tempID = EMPTY_TEXTID;
    }
    if(currentID != EMPTY_TEXTID)
    {
      TextID_Destroy(currentID);
      currentID = EMPTY_TEXTID;
    }
    if(sys_tempID != EMPTY_TEXTID)
    {
      TextID_Destroy(sys_tempID);
      sys_tempID = EMPTY_TEXTID;
    }
}


