#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

int exit=0;

BOOK * SCBook;

typedef struct
{
	BOOK * book;
}MSG;

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
	BOOK * bk = Display_GetTopBook(0);
	BOOK * Audio = FindBook(get_IsAudioPlayerBook());
	if(Audio && (bk==Find_StandbyBook() || isKeylocked()))
	{
		if (mode==KBD_SHORT_RELEASE)
		{
			if(key==KEY_LEFT)
				UI_Event(VOLUMEUPKEY_LONG_PRESS_EVENT);

			else if (key==KEY_RIGHT)
				UI_Event(VOLUMEDOWNKEY_LONG_PRESS_EVENT);

			else if (key==KEY_UP)
				UI_Event(VOLUMEUPKEY_SHORT_PRESS_EVENT);

			else if (key==KEY_DOWN)
				UI_Event(VOLUMEDOWNKEY_SHORT_PRESS_EVENT);
		}
	}
	if (!Audio && (bk==Find_StandbyBook()))
	{
		if (mode==KBD_SHORT_RELEASE)
		{
			if (key==KEY_LEFT)
				Shortcut_Run (L"FileManager");
			else if (key==KEY_RIGHT)
				Shortcut_Run (L"Msg_WriteNew");
			else if (key==KEY_UP)
				Shortcut_Run (L"MediaPlayer");
			else if (key==KEY_DOWN)
				Shortcut_Run (L"Phonebook");
		}
	}
	return (0);
}




int TerminateElf(void * ,BOOK* book)
{
	ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
	FreeBook(book);
	return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
	MSG * msg = (MSG*)mess;
	MessageBox(EMPTY_TEXTID,STR("A2 Song Changer\n(c)2010 mc_kibel"),NOIMAGE, 1 ,5000,msg->book);
	return(1);
}

const PAGE_MSG SC_PageEvents[]@ "DYN_PAGE" ={
	ELF_TERMINATE_EVENT , TerminateElf,
	ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
	NIL_EVENT_TAG,           NULL
};

PAGE_DESC base_page ={"SC_BasePage",0,SC_PageEvents};


void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onCloseSCBook(BOOK * book)
{
	if (book)
	{
		SUBPROC(elf_exit);
	}
}

BOOK * CreateSCBook()
{
	SCBook= new BOOK;
	CreateBook(SCBook,onCloseSCBook,&base_page,"A2 Song Changer",-1,0);
	return(SCBook);
}

int main (void)
{
	ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
	CreateSCBook();
	return(0);
}
