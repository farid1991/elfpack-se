#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "check.h"
#include "main.h"
#include "conf_loader.h"
#include "config_data.h"

u16 APtimer;

void elf_exit(void)
{
  	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onMyBookClose(BOOK * book)
{
	MyBOOK * b = (MyBOOK*)book;
	if (b->Ttimer)
	{
		Timer_Kill(&(b->Ttimer));
	}
	Timer_Kill(&APtimer);
	if (((MyBOOK*)book)->key)
	{
		MODIFYKEYHOOK(NewKey, KEY_HOOK_REMOVE, NULL);
	}
  	SUBPROC(elf_exit);
}

int isTT(BOOK * book)
{
  	if(!strcmp(book->xbook->name,BOOK_NAME))
	{
		return(1);
	}
  	return(0);
}

int TerminateElf(void *v,BOOK *book)
{
  	FreeBook(book);
  	return(1);
}

int ShowAuthorInfo(void *mess ,BOOK *book)
{
  	MSG * msg = (MSG *)mess;
	wchar_t * wstr = new wchar_t[256];
	snwprintf(wstr, 256, L"%ls\n ver %ls\n (c) %ls",ELFNAME,VERSION,AUTHOR);
  	MessageBox(0x6fffffff,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),0, 1 ,5000,msg->book);
	delete(wstr);
  	return(1);
}


int  time[14] = {0, 60, 120, 300, 600, 900, 1200, 1800, 3600, 7200, 10800, 14400, 21600, 43200};

void timer(MyBOOK * book)
{
	if (APtimer)
	{
		Timer_Kill(&APtimer);
	}
	if (REPEAT)
	{
		DATETIME dt;
		REQUEST_DATEANDTIME_GET(0,&dt);
		int t = (dt.time.hour*60+dt.time.min)*60+dt.time.sec;
		int tt = time[REPEAT] - ( t % time[REPEAT]);
		APtimer=TIMER_SET(tt*1000,onAPtimer,book);
	}
}

static int ReconfigElf(void *mess ,BOOK *book)
{
  	RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  	int result=0;
  	if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  	{
		MyBOOK* b = (MyBOOK*)book;
    		InitConfig();
		timer(b);
		if ((KEYUSE1 || KEYUSE2) && !b->key)
		{
			b->key=1;
			MODIFYKEYHOOK(NewKey, KEY_HOOK_ADD, NULL);
		}
		else if (!KEYUSE1 && !KEYUSE2 && b->key)
		{
			b->key=0;
			MODIFYKEYHOOK(NewKey, KEY_HOOK_REMOVE, NULL);
		}
    		result=1;
  	}
	return(result);
}


const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  	ELF_TERMINATE_EVENT,     TerminateElf,
  	ELF_SHOW_INFO_EVENT,     ShowAuthorInfo,
  	ELF_RECONFIG_EVENT,      ReconfigElf,
  	NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"TimeTalker_Base_Page",0,bk_msglst_base};


int main (void)
{
  	if (MyBOOK * b = (MyBOOK *)FindBook(isTT))
  	{
    		b->talk(b);
		SUBPROC(elf_exit);
  	}
  	else
  	{
    		MyBOOK * book = new MyBOOK;
    		memset(book,0,sizeof(MyBOOK));
    		if (!CreateBook(book,onMyBookClose,&bk_base,BOOK_NAME,-1,0))
    		{
      			delete book;
      			SUBPROC(elf_exit);
      			return (0);
    		}
    		InitConfig();
		book->talk = (TALK)TalkTime;
		if (KEYUSE1 || KEYUSE2)
		{
			book->key=1;
			MODIFYKEYHOOK(NewKey, KEY_HOOK_ADD, NULL);
		}
		timer(book);
  	}
  	return(0);
}
