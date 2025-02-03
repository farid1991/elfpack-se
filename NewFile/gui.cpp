#include "gui.h"

GUI *TextInputGui=0;
GUI *SizeInputGui=0;
unsigned int FileSize=0;

void ErrorMessage(int ErrorNum)
{
	switch(ErrorNum)
	{
		case -1: { MessageBox(EMPTY_TEXTID,STR("Ошибка создания файла"),NOIMAGE,1,0,0); break; }
		case -2: { MessageBox(EMPTY_TEXTID,STR("Файл уже существует"),NOIMAGE,1,0,0); break; }
	}
}

void OnLongBackSizeInput(BOOK* book, wchar_t* size, int len)
{
	GUIObject_Destroy(SizeInputGui);
	UI_Event(RETURN_TO_STANDBY_EVENT);
}

void OnBackSizeInput(BOOK* book, wchar_t* size, int len )
{
	GUIObject_Destroy(SizeInputGui);
}

void OnOkSizeInput(BOOK* book, wchar_t* size, int len )
{
	FileSize = wcstoul( size, 0, 10 );
	GUIObject_Destroy(SizeInputGui);
}

void CreateSizeInput( BOOK *book, GUI *)
{
	wchar_t buff[16];
	snwprintf(buff,MAXELEMS(buff),L"%u",FileSize);
	SizeInputGui=CreateStringInputVA( 0,
						VAR_HEADER_TEXT( STR("Размер (в байтах)") ),
						VAR_STRINP_MIN_LEN( 1 ),
						VAR_STRINP_MAX_LEN( 10 ),
						VAR_STRINP_MODE( 9 ),
						VAR_BOOK( book ),
						VAR_STRINP_TEXT( TextID_Create(buff,ENC_UCS2,TEXTID_ANY_LEN) ),
						VAR_PREV_ACTION_PROC( OnBackSizeInput ),
						VAR_LONG_BACK_PROC(OnLongBackSizeInput),
						VAR_OK_PROC( OnOkSizeInput ),
						0 );

}

void OnLongBackTextInput(BOOK* book, wchar_t* name, int len)
{
	GUIObject_Destroy(TextInputGui);
	UI_Event(RETURN_TO_STANDBY_EVENT);
}

void OnBackTextInput(BOOK* book, wchar_t* name, int len )
{
	GUIObject_Destroy(TextInputGui);
}

void OnAccept(LIST *lst,u16 *checked,int count,unsigned long name)
{
	for(int i=0;i<count;i++)
	{
		wchar_t pathname[512];
		snwprintf(pathname,MAXELEMS(pathname),L"%ls/%ls",List_Get(lst,checked[i]),name);
		ErrorMessage( CreateNewFile(pathname,FileSize) );
	}
	GUIObject_Destroy(TextInputGui);
	UI_Event(DB_UPDATE_EVENT);
}

void OnOkTextInput(BOOK* book, wchar_t* name, int len )
{
	LIST *folderlst=DataBrowserBook_GetCurrentFoldersList(book);
	if(folderlst->FirstFree<2)
	{
		ErrorMessage( FolderList_CreateFile(folderlst,name,FileSize) );
		GUIObject_Destroy(TextInputGui);
		UI_Event(DB_UPDATE_EVENT);
	}
	else new CMyGuiNOfMany( (CBook*)book, 0,folderlst,OnAccept,(unsigned long) name);
}

void CreateNameInput( BOOK *book, GUI *)
{
	FileSize=0;
	TextInputGui=CreateStringInputVA( 0,
						VAR_STRINP_FIXED_TEXT( STR("Имя:") ),
						VAR_STRINP_MIN_LEN( 1 ),
						VAR_STRINP_MAX_LEN( 255 ),
						VAR_STRINP_MODE( 1 ),
						VAR_BOOK( book ),
						VAR_STRINP_TEXT( STR("Новый_Файл.txt") ),
						VAR_PREV_ACTION_PROC( OnBackTextInput ),
						VAR_LONG_BACK_PROC(OnLongBackTextInput),
						VAR_OK_PROC( OnOkTextInput ),
						0 );
	GUIObject_SoftKeys_SetAction( TextInputGui, 1, CreateSizeInput );
	GUIObject_SoftKeys_SetText( TextInputGui, 1, STR("Размер") );
	StringInput_MenuItem_SetPriority( TextInputGui, 0, 1 );
}

