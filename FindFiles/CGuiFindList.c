#include "mcl.h"
int CMyBook::OnFindShowPage(void*,CMyBook*mbk)
{
  FUNCTION
  int TextID;
  Timer_Kill(&mbk->TimerID);
  mbk->TimerID=0;
  int item=mbk->gui_find->GetSelectedItem();
  if(item>=mbk->GetFindList()->FirstFree) item=mbk->GetFindList()->FirstFree-1;
  mbk->gui_find->Free();
  mbk->state=false;
  mbk->gui_find=new CGuiFindList(mbk,item,mbk->GetFindList());
  textidname2id(L"DB_NEW_FOLDER_TXT",-1,&TextID);
  mbk->gui_find->SoftKeys_SetAction(ACTION_SELECT,OnFindElseWherePress_GuiFind,TextID);
  textidname2id(L"MENU_NEW_SEARCH_TXT",-1,&TextID);
  mbk->gui_find->SoftKeys_SetAction(2,OnFindNewPress_GuiFind,TextID);
  textidname2id(L"GUI_EXIT_SK",-1,&TextID);
  mbk->gui_find->SoftKeys_SetAction(ACTION_BACK,OnExitPress_GuiFind,TextID);
  mbk->gui_find->SoftKeys_SetAction(ACTION_LONG_BACK,OnExitPress_GuiFind);
  textidname2id(L"DB_NO_FILES_TXT",-1,&TextID);
  mbk->gui_find->SetNoItemText(TextID);
  textidname2id(L"MMS_UI_PREVIEW_MESSAGE_TXT",-1,&TextID);
  if(mbk->GetFindList()->FirstFree) mbk->gui_find->SoftKeys_SetAction(ACTION_SELECT1,OnFolderOpenPress_GuiFind,TextID);
  TEXTID str[3];
  str[0]=int2strID(mbk->GetFindList()->FirstFree);
  str[1]=STR(" ");
  textidname2id(L"DB_FILES_TXT",-1,&str[2]);
  mbk->gui_find->SetTitleText(Str2ID(str,ENC_TEXTID,3));
  mbk->gui_find->Show();
  return 1;
}

int CGuiFindList::OnMessage(CGUIMessage*msg)
{
  FUNCTION
  int item;
  ELEM_*elem;
  int icon;
  switch(msg->GetMsg())
  {
  case LISTMSG_GetItem:
    item=msg->GetCreatedItem();
    elem=(ELEM_*)List_Get(list,item);
    msg->SetItemText(Str2ID(elem->name,ENC_UCS2,TEXTID_ANY_LEN));
    msg->SetItemSecondLineText(Str2ID(elem->path,ENC_UCS2,TEXTID_ANY_LEN));
    if(elem->isDir) iconidname2id(L"DB_LIST_FOLDER_ICN",-1,&icon);
    else icon=GetIcon(elem->name);
    msg->SetItemIcon(0,icon);
    if(wstrncmp2(elem->path,L"/card")==0) iconidname2id(L"DB_TITLE_MS_ICN",-1,&icon);
    else iconidname2id(L"DB_TITLE_PHONE_ICN",-1,&icon);
    msg->SetItemIcon(1,icon);
    break;
  }
  return 0;
};

CGuiFindList::CGuiFindList(CBook*bk,int x,LIST*_list)
:CGuiListMenu(bk)
{
  FUNCTION
  list=_list;
  SetItemCount(list->FirstFree);
  SetItemStyle(3);
  SetFocused(x);
};

int CMyBook::OnFindPage(void*,CMyBook*mbk)
{
  FUNCTION
  int TextID;
  int item;
  if(mbk->gui_find)
  {
    item=mbk->gui_find->GetSelectedItem();
    mbk->gui_find->Free();
  }
  else item=0;
  mbk->gui_find=new CGuiFindList(mbk,item,mbk->GetFindList());
  mbk->gui_find->SoftKeys_SetAction(ACTION_BACK,0,STR(""));
  textidname2id(L"GUI_STOP_SK",-1,&TextID);
  mbk->gui_find->SoftKeys_SetAction(ACTION_SELECT1,OnFindStopPress_GuiFind,TextID);
  textidname2id(L"SEARCHING_TXT",-1,&TextID);
  mbk->gui_find->SetNoItemText(TextID);
  mbk->gui_find->SetTitleText(Str2ID((wchar_t*)List_Get(mbk->GetFolderList(),0),ENC_UCS2,TEXTID_ANY_LEN));
  mbk->gui_find->Show();
  Timer_ReSet(&mbk->TimerID,50,mbk->OnTimer,mbk);
  return 0;
};

void CMyBook::OnExitPress_GuiFind(CBookBase*bk,CGuiBase*gui)
{
  FUNCTION
  bk->Free();
  elf_exit();
};

void CMyBook::OnFolderOpenPress_GuiFind(CBookBase*bk,CGuiBase*gui)
{
  FUNCTION
  MYBOOK(bk);
  mbk->GotoPage(&mbk->folderopen_page);
};

void CMyBook::OnFindElseWherePress_GuiFind(CBookBase*bk,CGuiBase*gui)
{
  FUNCTION
  MYBOOK(bk);
  new CMyBook(mbk->GetFindStr(),0,0);
  bk->Free();
};

void CMyBook::OnFindNewPress_GuiFind(CBookBase*bk,CGuiBase*gui)
{
  FUNCTION
  bk->Free();
  new CMyBook(0,0,0);
};

void CMyBook::QuickRefresh()
{
  FUNCTION
  gui_find->SetTitleText(Str2ID((wchar_t*)List_Get(GetFolderList(),0),ENC_UCS2,TEXTID_ANY_LEN));
};

