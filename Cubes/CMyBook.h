#include "CGameDisp.h"
#include "CGuiMainMenu.h"
#include "CGuiRecordMenu.h"
#include "..\\include\var_arg.h"
#ifndef CMYBOOK_H_
#define CMYBOOK_H_

#define RECORDPAGE_NAME "Cubes_RecordPage"
#define MAINPAGE_NAME "Cubes_MainPage"
#define GAMEPAGE_NAME "Cubes_GamePage"
#define GAMEBEATPAGE_NAME "Cubes_GameBeatPage"
#define BASEPAGE_NAME "Cubes_BasePage"


class CMyBook:public CBook
{
	RECORD*_recs;//������� ��������
	int ipos;//����� ������� ��� ��������
	CGame*game;//��������� �� ����
	CGuiBase*gui;//GUI ����
	CGuiMainMenu*guimenu;//GUI �������� ����
	CGuiRecordMenu*guirecord;//GUI ��������
	static int ShowAuthorInfo(CBook** bm_book,CMyBook* mbk);//�������� ���� � �������
	static int TerminateElf(CBook** bm_book,CMyBook* mbk);//�������� ����� ������
	static int OnGamePage(void*,CMyBook* mbk);//�������� ����
	static int OnMainPage(void*,CMyBook* mbk);//�������� �������� ����
	static int OnRecordPage(void*,CMyBook* mbk);//�������� ���� ��������
	static int OnGameBeatPage(void*,CMyBook* mbk);//�������� ���� �� ��������
	static void onBackActionGuiGame(CBookBase* mbk,CGuiBase*gui);//"�����" � ����
	static void onEnterActionGuiGame(CBookBase* mbk,CGuiBase*gui);//"�������" � ����
	static void onExitActionGuiMenu(CBookBase* mbk,CGuiBase*gui);//"�����" � ������� ����
	static void onEnterActionGuiMenu(CBookBase* mbk,CGuiBase*gui);//"�������" � ������� ����
	static void onBackActionGuiRecord(CBookBase* mbk,CGuiBase*gui);//"�����" � ���� ��������
	static void onEnterActionGuiRecord(CBookBase* mbk,CGuiBase*gui);//"�������" � ���� ��������
	static void onGameOver(void* lparam);//CallBack ��� ����� ����
	bool EquColor(RECORD*r1,RECORD*r2);//��������� ������ ������
	void GetRecords(RECORD*);//��������� ������� ��������
	void SetRecords(RECORD*);//��������� ������� ��������
	~CMyBook(void);//���������� (��� �������� ����)
	DECLARE_PAGE_DESC_MEMBER(base_page);//������� ��������
	DECLARE_PAGE_DESC_MEMBER(main_page);//�������� �������� ����
	DECLARE_PAGE_DESC_MEMBER(game_page);//�������� ����
	DECLARE_PAGE_DESC_MEMBER(gamebeat_page);//�������� �������� ���� �� �������
	DECLARE_PAGE_DESC_MEMBER(record_page);//�������� ��������
public:
	CMyBook(void);//�������� �����
};

#endif
