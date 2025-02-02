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
	RECORD*_recs;//Таблица рекордов
	int ipos;//Номер рекорда для загрузки
	CGame*game;//Указатель на игру
	CGuiBase*gui;//GUI игры
	CGuiMainMenu*guimenu;//GUI главного меню
	CGuiRecordMenu*guirecord;//GUI рекордов
	static int ShowAuthorInfo(CBook** bm_book,CMyBook* mbk);//Просмотр инфы в букмане
	static int TerminateElf(CBook** bm_book,CMyBook* mbk);//Закрытие через букман
	static int OnGamePage(void*,CMyBook* mbk);//Создание игры
	static int OnMainPage(void*,CMyBook* mbk);//Создание главного меню
	static int OnRecordPage(void*,CMyBook* mbk);//Создание меню рекордов
	static int OnGameBeatPage(void*,CMyBook* mbk);//Загрузка игры из рекордов
	static void onBackActionGuiGame(CBookBase* mbk,CGuiBase*gui);//"Назад" в игре
	static void onEnterActionGuiGame(CBookBase* mbk,CGuiBase*gui);//"Выбрать" в игре
	static void onExitActionGuiMenu(CBookBase* mbk,CGuiBase*gui);//"Выход" в главном меню
	static void onEnterActionGuiMenu(CBookBase* mbk,CGuiBase*gui);//"Выбрать" в главном меню
	static void onBackActionGuiRecord(CBookBase* mbk,CGuiBase*gui);//"Назад" в меню рекордов
	static void onEnterActionGuiRecord(CBookBase* mbk,CGuiBase*gui);//"Выбрать" в меню рекордов
	static void onGameOver(void* lparam);//CallBack при конце игры
	bool EquColor(RECORD*r1,RECORD*r2);//Сравнение матриц цветов
	void GetRecords(RECORD*);//Загрузить таблицу рекордов
	void SetRecords(RECORD*);//Сохранить таблицу рекордов
	~CMyBook(void);//Деструктор (при закрытии буки)
	DECLARE_PAGE_DESC_MEMBER(base_page);//Базовая страница
	DECLARE_PAGE_DESC_MEMBER(main_page);//Страница главного меню
	DECLARE_PAGE_DESC_MEMBER(game_page);//Страница игры
	DECLARE_PAGE_DESC_MEMBER(gamebeat_page);//Страница загрузки игры из рекорда
	DECLARE_PAGE_DESC_MEMBER(record_page);//Страница рекордов
public:
	CMyBook(void);//Создание книги
};

#endif
