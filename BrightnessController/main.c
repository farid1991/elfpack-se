/* -----------------------------------
Brightness Controller v4.21 - The most advanced brightness and energy manager for Sony Ericsson's elfpack-enabled phone!
(c)2009 mc_kibel from CentrumSE.pl ! Visit us!
Thanks to:
-Edgpaez (for disabling control in mediaplayer and radio)
-Zylka
-G29 (bugreport)
-SE-Developers.net and CentrumSE.pl communities for ideas

Changelog:
v1.0:
-Ability to change brightness by pressing volume up / down keys

v1.1 / v1.2:
-Disabled control, when CameraBook, AudioPlayerBook, MediaPlayerVideoBook or FMRadioBook are on.
-Small fixes

v1.3 to v3.01 not published due to a many bugs!

v3.02:
-BCFG Support!
-Inteligence AutoBacklight and EnergySaver system added!

v3.03:
-Fixed bug with manual brightness control with InteligenceAutoBacklight and EnergySaver systems

v3.04 (private):
-Big code optimization

v3.051:
-Based on v3.04
-Added ability to automatically change brightness level in CameraBook
-Now it's possible to run only one copy of elf at the same time (security).
-Support for Bookman v3 mod

v4.00
-Configurable intervals!!
--from 00:00 to xx  default: to 7:00
--from xx to yy     default: to 11:00
--from yy to zz     default: to 20:00
--from zz to 00:00
-CheckConfig System added
--If CheckConfig system will find errors in .bcfg file, elf will be closed and default settings will be restored.

v.4.2
-Changed BCFG Structure
-Changed BCFG name from "BacklightController" to "BrightnessController" (!)
-Changed default settings in BCFG
-Small optimizations

v4.3
-Very important update! Fixed work of all systems together

v4.4
-Added A2 support :)

v4.5
-Small code improvements
-No reboot when closing elf (a2)

------------------------------------*/

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "globals.h"
#include "books.h"
#include "functions.h"

u16 timer;
BOOK * BCBook;

int Platform;
int RemainingCapacityInPercent;

int ShowAuthorInfo(void *mess ,BOOK* book)
{
	ShowMsg(__AUTHOR_INFO__,7000);
	return(1);
}

int TerminateElf(void * ,BOOK* book)
{
	ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
	Timer_Kill(&timer);
	FreeBook(book);
	return(1);
}

static int ReconfigElf(void *mess ,BOOK *book)
{
	RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
	if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
	{
		InitConfig();
		return 1;
	}
	return 0;
}

int onBcfgConfig(void* mess,BOOK* b)
{
	FSTAT _fstat;
	wchar_t path[256];

	if(fstat(GetDir(DIR_ELFS|MEM_INTERNAL),L"BcfgEdit.elf",&_fstat)==0)
		wstrcpy(path,GetDir(DIR_ELFS|MEM_INTERNAL));
	else if(fstat(GetDir(DIR_ELFS|MEM_EXTERNAL),L"BcfgEdit.elf",&_fstat)==0)
		wstrcpy(path,GetDir(DIR_ELFS|MEM_EXTERNAL));
	else
	{
		ShowMsg("BcfgEdit.elf not found!",2000);
		return (1);
	}
	wstrcat(path,L"/BcfgEdit.elf");
	elfload(path,(void*)successed_config_path,(void*)successed_config_name,0);
	return (1);
}

const PAGE_MSG BC_PageEvents[]@ "DYN_PAGE" ={
	ELF_TERMINATE_EVENT , TerminateElf,
	ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
	ELF_RECONFIG_EVENT,    ReconfigElf,
	ELF_BCFG_CONFIG_EVENT,   onBcfgConfig,
	NIL_EVENT_TAG,           NULL
};
PAGE_DESC base_page ={"BC_BasePage",0,BC_PageEvents};


void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onCloseBCBook(BOOK * book)
{
	SUBPROC(elf_exit);
}

BOOK * CreateBCBook()
{
	BCBook= new BOOK;
	CreateBook(BCBook,onCloseBCBook,&base_page,"BrightnessController",-1,0);
	return(BCBook);
}

void onTimer (u16 unk , LPARAM )
{
	BOOK * CamBook = FindBook(isCamBook);
	if (SmartAB && EnergySaver && CamChange) //111
	{
		GetBatteryLevel();
		if (!CamBook)
		{
			if (RemainingCapacityInPercent <= (BattPercent))
				DISPLAY_SetBr ( 0, (DecreaseTo) );
			else
				InitInteligenceAutoBacklight();
		}
		else
			DISPLAY_SetBr ( 0, (CamLevel) );
	}
	else if (!SmartAB && EnergySaver && CamChange) // 011
	{
		GetBatteryLevel();
		if (!CamBook)
		{
			if (RemainingCapacityInPercent <= (BattPercent))
				DISPLAY_SetBr ( 0, (DecreaseTo) );
		}
		else
			DISPLAY_SetBr ( 0, (CamLevel) );
	}
	else if (!SmartAB && !EnergySaver && CamChange) //001
	{
		if (CamBook)
			DISPLAY_SetBr ( 0, (CamLevel) );
	}
	else if (SmartAB && !EnergySaver && !CamChange) //100
	{
		InitInteligenceAutoBacklight();
	}
	else if (SmartAB && !EnergySaver && CamChange) //101
	{
		if (!CamBook)
			InitInteligenceAutoBacklight();
		else
			DISPLAY_SetBr ( 0, (CamLevel) );
	}
	else if (SmartAB && EnergySaver && !CamChange) //110
	{
		GetBatteryLevel();

		if (RemainingCapacityInPercent <= (BattPercent))
			DISPLAY_SetBr ( 0, (DecreaseTo) );
		else
			InitInteligenceAutoBacklight();
	}
	else if (!SmartAB && EnergySaver && !CamChange) //010
	{
		if (RemainingCapacityInPercent <= (BattPercent))
			DISPLAY_SetBr ( 0, (DecreaseTo) );
	}
	else if (!SmartAB && !EnergySaver && !CamChange) //000
		ManualMode();

	Timer_ReSet(&timer,3000,onTimer,0);
}

void GetPlatform (int *ptr)
{
	int chipID = GetChipID() & CHIPID_MASK;
	if (chipID==CHIPID_DB2000 || chipID==CHIPID_DB2010 || chipID==CHIPID_DB2020)
		*ptr=A1;
	else
		*ptr=A2;
}

int main (void)
{
	if (FindBook(isBrightnessControllerBook))
		ShowMsg("BrightnessController is already working!",5000);
	else
	{
		CreateBCBook();
		InitConfig();
		GetPlatform (&Platform);
		timer=Timer_Set(3000,onTimer,0);
	}
	return(0);
}
