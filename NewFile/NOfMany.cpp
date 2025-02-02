#include "NOfMany.h"

wchar_t* GetLocation(wchar_t* path)
{
	if(!wstrncmp(path,L"/card",5)) return L"Карта памяти";
	else if(!wstrncmp(path,L"/tpa",4)) return L"Телефон(/tpa)";
	else if(!wstrncmp(path,L"/usb",4)) return L"Телефон(/usb)";
	return path;
}

void CMyGuiNOfMany::OnBack( CBookBase* book, CGuiBase* gui )
{
	CMyGuiNOfMany* mygui = (CMyGuiNOfMany*)gui;
	mygui->Destroy();
}

void CMyGuiNOfMany::OnLongBack( CBookBase* book, CGuiBase* gui )
{
	CMyGuiNOfMany* mygui = (CMyGuiNOfMany*)gui;
	mygui->Destroy();
	UI_Event(RETURN_TO_STANDBY_EVENT);
}

void CMyGuiNOfMany::OnSelect( CBookBase* book, CGuiBase* gui )
{
	CMyGuiNOfMany* mygui = (CMyGuiNOfMany*)gui;
	mygui->GetChecked( mygui->checked_table );
	mygui->CallBack(mygui->itemlst,mygui->checked_table,mygui->GetCheckedCount(),mygui->lparam);
	mygui->Destroy();
}

CMyGuiNOfMany::CMyGuiNOfMany( CBook* book, int display,LIST *lst,void(*OnAccept)(LIST *lst,u16 *checked,int count,unsigned long ClientData),unsigned long ClientData): CGuiNOfMany( book, display )
{
	itemlst=lst;
	lparam=ClientData;
	CallBack=OnAccept;
	strids = new int[lst->FirstFree];
	checked_table = new u16[lst->FirstFree];
	SetItemCount(lst->FirstFree);
	for(int i=0;i<lst->FirstFree;i++)
	{
		strids[i]=TextID_Create(GetLocation((wchar_t*)List_Get(lst,i)),ENC_UCS2,TEXTID_ANY_LEN);
		checked_table[i]=i;
	}
	SetTexts(strids,lst->FirstFree);
	SetChecked(checked_table, lst->FirstFree );
	SetFocused(0);
	SetTitleText(STR("Сохранить в..."));
	SoftKeys_SetAction( ACTION_BACK, OnBack );
	SoftKeys_SetAction( ACTION_LONG_BACK, OnLongBack );
	SoftKeys_SetAction( ACTION_ACCEPT, OnSelect );
	Show();
}


CMyGuiNOfMany::~CMyGuiNOfMany()
{
}
