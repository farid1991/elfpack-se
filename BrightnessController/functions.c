#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "globals.h"
#include "books.h"
#include "config_data.h"

extern int Platform;
extern int RemainingCapacityInPercent;
extern u16 timer;


int DISPLAY_SetBr(int param1, int param2)
{
	if (Platform==A1)
		Display_SetBrightness(param1,param2);
	else if (Platform==A2)
		Display_SetBrightness(param2,param1);

	return 0;
}

int GetBatteryLevel (void)
{
	BATT Battery;
	GetBatteryState(NULL , &Battery);
	RemainingCapacityInPercent = Battery.RemainingCapacityInPercent;
	return (RemainingCapacityInPercent);
}


void CheckConfig (void)
{
	if ( EndSecInt <= EndFirstInt || EndThirdInt <= EndSecInt || EndThirdInt <= EndFirstInt )
	{
		FileDelete (GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL) ,L"BrightnessController.bcfg" , 0 );
		FileDelete (GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL) ,L"BrightnessController.bcfg" , 0 );
		MessageBox(EMPTY_TEXTID,STR("Error occurs in .bcfg file! Brightness Controller closed."),NOIMAGE, 1 ,1000,0);
		FreeBook(FindBook(isBrightnessControllerBook));
		Timer_Kill( &timer );
	}
}

void BrightnessControll (int param)
{
	int current = Display_GetBrightness(0);
	switch (param)
	{
	case BR_INCREASE: DISPLAY_SetBr(0, current+10); if(IS_ENABLED == 1) ShowMsg("Brightness increased.",1000); break;
	case BR_DECREASE: DISPLAY_SetBr(0, current-10); if(IS_ENABLED == 1) ShowMsg("Brightness decreased.",1000); break;
	}
}

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
	if(!SmartAB && !EnergySaver)
	{
		if (key==(KEY1) && mode==(KEYMODE1) && Display_GetBrightness(0) < 100)
			BrightnessControll (BR_INCREASE);
		else if (key==(KEY2) && mode==(KEYMODE2) && Display_GetBrightness(0) > 10)
			BrightnessControll (BR_DECREASE);
	}
	else if(!SmartAB && EnergySaver)
	{
		GetBatteryLevel();
		if(RemainingCapacityInPercent > (BattPercent))
		{
			if (key==(KEY1) && mode==(KEYMODE1) && Display_GetBrightness(0) < 100)
				BrightnessControll (BR_INCREASE);
			else if (key==(KEY2) && mode==(KEYMODE2) && Display_GetBrightness(0) > 10)
				BrightnessControll (BR_DECREASE);
		}
	}
	return 0;
}

int ManualMode(void)
{
	if(DISABLE_CONTROL==1)
	{
		BOOK * MP = FindBook(get_IsAudioPlayerBook());
		BOOK * FM = FindBook(get_IsFmRadioBook());
		BOOK * CS = FindBook(isCamBook);
		BOOK * VD = FindBook(get_IsMediaPlayerVideoBook());
		BOOK * SS = FindBook(get_IsScreenSaverBook());
		BOOK * IMV = FindBook(isImageViewerBook);

		if(MP || FM || CS || VD || SS || IMV)
			ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
		else
			ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
	}
	else
		ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);

	return 0;
}

void InitInteligenceAutoBacklight (void)
{
	int val, hour;
	DATETIME dt;
	REQUEST_DATEANDTIME_GET (0, &dt);
	hour = dt.time.hour;
	if ( hour>=0 && hour<EndFirstInt )
		val = ValueOne;
	else if ( hour >= EndFirstInt && hour < EndSecInt )
		val = ValueTwo;
	else if ( hour >= EndSecInt && hour < EndThirdInt )
		val = ValueThree;
	else if ( hour >= EndThirdInt && hour > 0 )
		val = ValueFour;

	DISPLAY_SetBr(0, (val));
}
