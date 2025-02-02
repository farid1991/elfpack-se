#include "CMyBook.h"
#include "Code_Huffman.h"
void elf_exit(void)
{
	trace_done();
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

void CMyBook::onBackActionGuiRecord(CBookBase* bk,CGuiBase*gui)
{
	CMyBook*mbk=(CMyBook*)bk;
	mbk->GotoPage(&mbk->main_page);
	mbk->guirecord->Free();
	mbk->guirecord=0;
};

void CMyBook::onEnterActionGuiRecord(CBookBase* bk,CGuiBase*gui)
{
	CMyBook*mbk=(CMyBook*)bk;
	mbk->ipos=mbk->guirecord->GetSelectedItem();
	if(!mbk->_recs[mbk->ipos].point) mbk->GotoPage(&mbk->game_page);
	else mbk->GotoPage(&mbk->gamebeat_page);
	mbk->guirecord->Free();
	mbk->guirecord=0;
};

int CMyBook::OnRecordPage(void*,CMyBook* mbk)
{
	mbk->SetDisplayOrientation(0);
	mbk->guirecord=new CGuiRecordMenu(mbk,mbk->_recs);
	mbk->guirecord->SoftKeys_SetAction(ACTION_LONG_BACK,mbk->onBackActionGuiRecord);
	mbk->guirecord->SoftKeys_SetAction(ACTION_BACK,mbk->onBackActionGuiRecord,TextID_Create(CGUIRECORDMENU_BACK_TXT,ENC_UCS2,TEXTID_ANY_LEN));
	mbk->guirecord->SoftKeys_SetAction(ACTION_SELECT1,mbk->onEnterActionGuiRecord,TextID_Create(CGUIRECORDMENU_BEAT_TXT,ENC_UCS2,TEXTID_ANY_LEN));
	mbk->guirecord->Show();
	return 0;
};

int CMyBook::OnMainPage(void*,CMyBook* mbk)
{
	mbk->SetDisplayOrientation(0);
	mbk->guimenu=new CGuiMainMenu(mbk);
	mbk->guimenu->SoftKeys_SetAction(ACTION_LONG_BACK,mbk->onExitActionGuiMenu);
	mbk->guimenu->SoftKeys_SetAction(ACTION_BACK,mbk->onExitActionGuiMenu,TextID_Create(CGUIMAINMENU_EXIT_TXT,ENC_UCS2,TEXTID_ANY_LEN));
	mbk->guimenu->SoftKeys_SetAction(ACTION_SELECT1,mbk->onEnterActionGuiMenu,TextID_Create(CGUIMAINMENU_SELECT_TXT,ENC_UCS2,TEXTID_ANY_LEN));
	mbk->guimenu->Show();
	return 0;
};

void CMyBook::onEnterActionGuiMenu(CBookBase* bk,CGuiBase* gui)
{
	CMyBook*mbk=(CMyBook*)bk;
	int item=mbk->guimenu->GetSelectedItem();
	switch(item)
	{
	case 0:
		mbk->GotoPage(&mbk->game_page);
		mbk->guimenu->Free();
		mbk->guimenu=0;
		break;
	case 1:
		mbk->GotoPage(&mbk->record_page);
		mbk->guimenu->Free();
		mbk->guimenu=0;
		break;
	case 2:
		mbk->Free();
	};
};

void CMyBook::onExitActionGuiMenu(CBookBase* bk,CGuiBase* gui)
{
	bk->Free();
};

int CMyBook::OnGameBeatPage(void*,CMyBook* mbk)
{
	mbk->SetDisplayOrientation(1);
	mbk->gui=new CGuiT<CGameDisp>(mbk,0);
	mbk->game=((CGameDisp*)mbk->gui->GetDISPObj())->GetGame();
	mbk->game->onSetGameOver((void*)mbk,mbk->onGameOver);
	mbk->game->SetRecord(&mbk->_recs[mbk->ipos]);
	mbk->gui->SoftKeys_SetAction(ACTION_LONG_BACK,mbk->onBackActionGuiGame);
	mbk->gui->SoftKeys_SetAction(ACTION_BACK,mbk->onBackActionGuiGame);
	mbk->gui->SoftKeys_SetAction(ACTION_SELECT1,mbk->onEnterActionGuiGame);
	mbk->gui->SetStyle(4);
	mbk->gui->SetTitleType(1);
	mbk->gui->SoftKeys_Hide();
	mbk->gui->Show();
	return 0;
};

int CMyBook::OnGamePage(void*,CMyBook* mbk)
{
	mbk->SetDisplayOrientation(1);
	mbk->gui=new CGuiT<CGameDisp>(mbk,0);
	mbk->game=((CGameDisp*)mbk->gui->GetDISPObj())->GetGame();
	mbk->game->onSetGameOver((void*)mbk,mbk->onGameOver);
	mbk->gui->SoftKeys_SetAction(ACTION_LONG_BACK,onBackActionGuiGame);
	mbk->gui->SoftKeys_SetAction(ACTION_BACK,mbk->onBackActionGuiGame);
	mbk->gui->SoftKeys_SetAction(ACTION_SELECT1,onEnterActionGuiGame);
	mbk->gui->SetStyle(4);
	mbk->gui->SetTitleType(1);
	mbk->gui->SoftKeys_Hide();
	mbk->gui->Show();
	return 0;
};

bool CMyBook::EquColor(RECORD*r1,RECORD*r2)
{
	for(int i=0;i<15;i++)
		for(int j=0;j<20;j++)
			if(r1->colors[i][j]!=r2->colors[i][j]) return false;
	return true;
};

void CMyBook::GetRecords(RECORD*recs)
{
	BITRECORD* bitrecs=new BITRECORD[10];
	wchar_t*cpath=game->GetConfigPath(CONFIGPATH);
	FSTAT _fstat;
	if(fstat(cpath,RECORDSFILENAME,&_fstat)!=0)
	{
		for(int i=0;i<10;i++)
		{
			recs[i].point=0;
			for(int j=0;j<15;j++)
				for(int k=0;k<20;k++)
					recs[i].colors[j][k]=COLOR_WHITE;
		};
		SetRecords(recs);
	};
	int f=_fopen(cpath,RECORDSFILENAME,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
	fread(f,bitrecs,sizeof(BITRECORD)*10);
	fclose(f);
	for(int i=0;i<10;i++) BITRECORD2RECORD(&bitrecs[i],&recs[i]);
	delete[] bitrecs;
	delete[] cpath;
};

void CMyBook::SetRecords(RECORD*recs)
{
	BITRECORD* bitrecs=new BITRECORD[10];
	wchar_t*cpath=game->GetConfigPath(CONFIGPATH);
	for(int i=0;i<10;i++) RECORD2BITRECORD(&recs[i],&bitrecs[i]);
	int f=_fopen(cpath,RECORDSFILENAME,FSX_O_WRONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
	fwrite(f,bitrecs,sizeof(BITRECORD)*10);
	fclose(f);
	delete[] bitrecs;
	delete[] cpath;
};

void CMyBook::onGameOver(void*lparam)
{
	RECORD rec;
	CMyBook*mbk=(CMyBook*)lparam;
	mbk->game->GetRecord(&rec);
	int pos=10;
	for(int i=0;i<10;i++)
		if(mbk->EquColor(&rec,&mbk->_recs[i]))
		{
			pos=i;
			break;
		};
	if(pos<10)
	{
		if(rec.point>mbk->_recs[pos].point)
		{
			mbk->_recs[pos]=rec;
			for(int i=pos;i>0;i--)
				if(mbk->_recs[i].point>mbk->_recs[i-1].point)
				{
					rec=mbk->_recs[i-1];
					mbk->_recs[i-1]=mbk->_recs[i];
					mbk->_recs[i]=rec;
				}else break;
		};
	}
	else
	{
		for(int i=0;i<10;i++)
		{
			if(rec.point>mbk->_recs[i].point)
			{
				pos=i;
				for(int j=9;j>pos;j--) mbk->_recs[j]=mbk->_recs[j-1];
				mbk->_recs[pos]=rec;
				break;
			};
		};
	};
	mbk->game->SetPlace(pos);
	mbk->gui->GetDISPObj()->InvalidateRect(NULL);
};

void CMyBook::onEnterActionGuiGame(CBookBase* mbk,CGuiBase*gui)
{
	((CMyBook*)mbk)->game->EnterAction();
	gui->GetDISPObj()->InvalidateRect(NULL);
};

void CMyBook::onBackActionGuiGame(CBookBase* bk,CGuiBase*gui)
{
	CMyBook*mbk=(CMyBook*)bk;
	mbk->GotoPage(&mbk->main_page);
	mbk->gui->Free();
	mbk->gui=0;
};

int CMyBook::TerminateElf(CBook** bm_book,CMyBook* mbk)
{
	mbk->Free();
	return 0;
};

int CMyBook::ShowAuthorInfo(CBook** bm_book,CMyBook* mbk)
{
	MessageBox(EMPTY_TEXTID,TextID_Create(ELFINFO,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,*bm_book);
	return 0;
};

CMyBook::~CMyBook()
{
	SetRecords(_recs);
	delete[] _recs;
	if(guirecord) guirecord->Free();
	if(guimenu) guimenu->Free();
	if(gui) gui->Free();
	elf_exit();
};

CMyBook::CMyBook()
:CBook("Cubes",&base_page)
{
	gui=0;
	game=0;
	guimenu=0;
	guirecord=0;
	_recs=new RECORD[10];
	GetRecords(_recs);
	GotoPage(&main_page);
};

DECLARE_PAGE_DESC_BEGIN(CMyBook::record_page, RECORDPAGE_NAME)
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnRecordPage )
DECLARE_PAGE_DESC_END

DECLARE_PAGE_DESC_BEGIN(CMyBook::main_page, MAINPAGE_NAME)
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnMainPage )
DECLARE_PAGE_DESC_END

DECLARE_PAGE_DESC_BEGIN(CMyBook::game_page, GAMEPAGE_NAME)
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnGamePage )
DECLARE_PAGE_DESC_END

DECLARE_PAGE_DESC_BEGIN(CMyBook::gamebeat_page, GAMEBEATPAGE_NAME)
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnGameBeatPage )
DECLARE_PAGE_DESC_END

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, BASEPAGE_NAME)
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END
