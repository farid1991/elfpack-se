#include "mybook.h"

#define ELF_BCFG_CONFIG_EVENT 994
extern void elf_exit(void);

typedef struct
{
	BOOK * bookmanbook;
}BOOKMANMSG;

typedef struct
{
	BOOK* book;
	wchar_t *bcfg_p;
	wchar_t *bcfg_n;
}MSG_BCFG;

DISP_OBJ_ONREDRAW_METHOD SoftKeysOnRedraw;

int KeyHook(int key,int,int type,CMyBook *cmb, DISP_OBJ*)
{
	if(key==KEY_START)
	{
		if(type==KEY_START_MODE)
		{
			if(!cmb->gui) cmb->UI_Event(ACCEPT_EVENT);
			else cmb->UI_Event(CANCEL_EVENT);
		}
		return(-1);
	}
	if(ONPLAYERCONTROL)
	{
		BOOK *topbook = Display_GetTopBook(0);
		if(topbook!=cmb && cmb->gui && ONTOP)
		{
			if(key==KEY_LEFT || key==KEY_DIGITAL_0+4)
			{
				if(type==KBD_SHORT_RELEASE) UI_Event(UI_MEDIAPLAYER_PREV_TRACK_EVENT);
				return -1;
			}
			if(key==KEY_RIGHT || key==KEY_DIGITAL_0+6)
			{
				if(type==KBD_SHORT_RELEASE) UI_Event(UI_MEDIAPLAYER_NEXT_TRACK_EVENT);
				return -1;
			}
			if(key==KEY_UP || key==KEY_DIGITAL_0+2)
			{
				if(type==KBD_SHORT_RELEASE) UI_Event(VOLUMEUPKEY_SHORT_PRESS_EVENT);
				return -1;
			}
			if(key==KEY_DOWN || key==KEY_DIGITAL_0+8)
			{
				if(type==KBD_SHORT_RELEASE) UI_Event(VOLUMEDOWNKEY_SHORT_PRESS_EVENT);
				return -1;
			}
			if(key==KEY_ENTER || key==KEY_DIGITAL_0+5)
			{
				if(type==KBD_SHORT_RELEASE) BookObj_SetFocus(FindBook(get_IsAudioPlayerBook()),0);
				return -1;
			}
		}
	}
	return 0;
}

int TerminateElf(BOOKMANMSG *msg, CMyBook* book)
{
	book->Free();
	return 1;
}

void InfoOnClose(BOOK *book,GUI *gui)
{
	GUIObject_Destroy(gui);
}

int ShowAuthorInfo(BOOKMANMSG *msg, CBook* book)
{
	wchar_t buff[64];
	snwprintf(buff,63,L"PlayerPanel 2.0\n© MaPkiZzz\n\nCompile time:\n%s\n%s\n",__DATE__,__TIME__);
	GUI *fb=TextFeedbackWindow(msg->bookmanbook,0);
	GUIObject_SetStyle(fb, UI_OverlayStyle_InternalFrameOutput);
	Feedback_SetTextExtended(fb,TextID_Create(buff,ENC_UCS2,TEXTID_ANY_LEN),0);
	GUIObject_SoftKeys_SetAction(fb,ACTION_OK,InfoOnClose);
	GUIObject_SoftKeys_SetAction(fb,ACTION_BACK,InfoOnClose);
	GUIObject_Show(fb);
	return 1;
}


void SoftKeysMyRedraw(DISP_OBJ *disp,int a,int b,int c)
{
	if(strcmp(Display_GetTopBook(0)->xbook->name,"PlayerPanel")) SoftKeysOnRedraw(disp,a,b,c);
}

void Send_REDRAW_RELEASE()
{
	REDRAW_RELEASE_MESSAGE * sbm= new REDRAW_RELEASE_MESSAGE;
	memset(sbm,0,sizeof(REDRAW_RELEASE_MESSAGE));
	sbm->SK_OldOnRedraw=SoftKeysOnRedraw;
	sbm->SK_NewOnRedraw=SoftKeysMyRedraw;
	UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,sbm,(void (*)(void*))mfree_adr());
}

int SK_ELF_Killed(void *mess ,BOOK* book)
{
	REDRAW_RELEASE_MESSAGE * sbm=(REDRAW_RELEASE_MESSAGE*)mess;
	if (sbm->SK_NewOnRedraw==SoftKeysOnRedraw)
	{
		REDRAW_RELEASE_MESSAGE *res=new REDRAW_RELEASE_MESSAGE;
		memcpy(res,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
		if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) SoftKeysOnRedraw=sbm->SK_OldOnRedraw;
		DISP_DESC_SetOnRedraw(DispObject_GetDESC(DispObject_SoftKeys_Get()),SoftKeysMyRedraw);
		res->SK_OldOnRedraw=EMPTY_REDRAW_METHOD;
		res->SK_NewOnRedraw=SoftKeysMyRedraw;
		UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,res,(void (*)(void*))mfree_adr());
		return BLOCK_EVENT_GLOBALLY;
	}
	return 0;
}

void OnDestroy(CMyDisp *cmd)
{
	if(cmd->timer) Timer_Kill(&cmd->timer);
  	Send_REDRAW_RELEASE();
	DISP_OBJ *softkeys=DispObject_SoftKeys_Get();
	DISP_DESC_SetOnRedraw(DispObject_GetDESC(softkeys),SoftKeysOnRedraw);
	CMyBook *book=(CMyBook *)cmd->GetGUI()->GetBook();
	BookObj_StayOnTop(book,1);
	BookObj_WindowSetWantsFocus(book,0,1);
	book->gui->Free();
	book->Hide(0);
	book->gui=NULL;
}

int CMyBook::Cancel(void *msg, CMyBook* book)
{
	if(book->gui) ((CMyDisp*)book->gui->GetDISPObj())->DestroyAnimation(ANIMATION_HIDE<8?ANIMATION_HIDE:rand()%7,BACKGROUND_RC,OnDestroy);
	return 1;
}

int CMyBook::Accept(void *msg, CMyBook* book)
{
	if(ONTOP)
	{
		BookObj_StayOnTop(book,2);
		BookObj_WindowSetWantsFocus(book,0,0);
	}
	book->gui = new CGuiT<CMyDisp>(book, 0);
	book->gui->SetStyle(UI_OverlayStyle_TrueFullScreen);
	book->gui->SetTitleType(UI_TitleMode_None);
	book->gui->SoftKeys_Hide();
	book->gui->Show();
	DISP_OBJ *softkeys=DispObject_SoftKeys_Get();
	SoftKeysOnRedraw=DispObject_GetOnRedraw(softkeys);
	DISP_DESC_SetOnRedraw(DispObject_GetDESC(softkeys),SoftKeysMyRedraw);
	book->Show(0);
	((CMyDisp*)book->gui->GetDISPObj())->CreateAnimation(ANIMATION_SHOW<8 ? ANIMATION_SHOW : rand()%7,BACKGROUND_RC);
	return 1;
}

static int ReconfigElf(void *msg, void*)
{
	RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)msg;
	int result=0;
	if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
	{
		InitConfig();
		result=1;
	}
	return(result);
}

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

int onPlayingTime(void *mess, CMyBook* book)
{
	if(book->gui) ((CMyDisp*)book->gui->GetDISPObj())->OnPlayingTime((PLAY_TIME*)mess);
	return 1;
}

int OnPlay(void *mess, CMyBook* book)
{
	if(book->gui)
	{
		CMyDisp * cmd = ((CMyDisp*)book->gui->GetDISPObj());
		cmd->OnPlay();
		if(AUTO_HIDE_TIME)
		{
			DispObject_KillRefreshTimer(cmd);
			DispObject_SetRefreshTimer(cmd,AUTO_HIDE_TIME);
		}
	}
	else if(ON_TRACK_CHANGE)
	{
		BOOK *topbook=Display_GetTopBook(0);
		if(!IsAudioPlayerBook(topbook) && !IsScreenSaverBook(topbook))
			book->UI_Event(ACCEPT_EVENT);
	}
	return 1;
}

int OnPlayerDestroyed(void *mess, CMyBook* book)
{
	if(book->gui) book->UI_Event(CANCEL_EVENT);
	ModifyKeyHook(MKKEYHOOKPROC(KeyHook),KEY_HOOK_REMOVE,0);
	return 1;
}

int OnPlayerCreated(void *mess, CMyBook* book)
{
	ModifyKeyHook(MKKEYHOOKPROC(KeyHook),KEY_HOOK_ADD,book);
	return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "PlayerPanel_BasePage")
	DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, TerminateElf )
	DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, ShowAuthorInfo )
	DECLARE_PAGE_DESC_MSG( CANCEL_EVENT_TAG, CMyBook::Cancel )
	DECLARE_PAGE_DESC_MSG( ACCEPT_EVENT_TAG, CMyBook::Accept)
	DECLARE_PAGE_DESC_MSG( ELF_RECONFIG_EVENT, ReconfigElf )
	DECLARE_PAGE_DESC_MSG( ELF_BCFG_CONFIG_EVENT, onBcfgConfig )
	DECLARE_PAGE_DESC_MSG( UI_MEDIAPLAYER_AUDIO_PLAYING_TIME_EVENT_TAG, onPlayingTime)
	DECLARE_PAGE_DESC_MSG( UI_MEDIAPLAYER_PLAY_PRESSED_EVENT_TAG,OnPlay)
	DECLARE_PAGE_DESC_MSG( UI_MEDIAPLAYER_DESTROYED_EVENT_TAG, OnPlayerDestroyed)
	DECLARE_PAGE_DESC_MSG( UI_MEDIAPLAYER_CREATED_EVENT_TAG, OnPlayerCreated)
	DECLARE_PAGE_DESC_MSG( SBY_REDRAW_RELEASE_EVENT,SK_ELF_Killed)
DECLARE_PAGE_DESC_END

CMyBook::CMyBook()	:CBook("PlayerPanel",&base_page)
{
	if(FindBook(get_IsAudioPlayerBook())) ModifyKeyHook(MKKEYHOOKPROC(KeyHook),KEY_HOOK_ADD,this);
	gui=NULL;
}

CMyBook::~CMyBook()
{
	if(gui) OnDestroy((CMyDisp*)GUIObject_GetDispObject(gui));
	ModifyKeyHook(MKKEYHOOKPROC(KeyHook),KEY_HOOK_REMOVE,0);
	MMIPROC(elf_exit);
}
