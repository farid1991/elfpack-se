#include "mcl.h"

int CGuiFolderList::OnMessage(CGUIMessage*msg)
{
  FUNCTION
  int item;
  int TextID,IconID;
  switch(msg->GetMsg())
  {
  case LISTMSG_GetItem:
    item=msg->GetCreatedItem();
    switch(item)
    {
    case 0:
      textidname2id(L"DB_DATABROWSER_TXT",-1,&TextID);
      iconidname2id(L"DB_BOOKMARKS_DIRECTORY_ICN",-1,&IconID);
      break;
    case 1:
      textidname2id(L"CALE_DELETE_ALL_EVENTS_TXT",-1,&TextID);
      iconidname2id(L"DB_TITLE_ALL_ICN",-1,&IconID);
      break;
    case 2:
      textidname2id(L"CAMI_SAVE_TO_MS_TXT",-1,&TextID);
      iconidname2id(L"DB_TITLE_MS_ICN",-1,&IconID);
      break;
    case 3:
      textidname2id(L"CAMI_SAVE_TO_PHONE_TXT",-1,&TextID);
      iconidname2id(L"DB_TITLE_PHONE_ICN",-1,&IconID);
      break;
    }
    msg->SetItemText(TextID);
    msg->SetItemIcon(0,IconID);
  }
  return 0;
}

CGuiFolderList::CGuiFolderList(CBook*bk)
:CGuiListMenu(bk)
{
  FUNCTION
  int TextID;
  SetItemCount(4);
  SetItemStyle(0);
  SetFocused(0);
  textidname2id(L"GUI_SELECT_SK",-1,&TextID);
  SetTitleText(TextID);
};

int CMyBook::OnFolderPage(void*,CMyBook*mbk)
{
  FUNCTION
  if(mbk->GetFolderList()->FirstFree)
  {
    mbk->GotoPage(&mbk->find_page);
    return 1;
  }
  mbk->gui_folder=new CGuiFolderList(mbk);
  mbk->gui_folder->SoftKeys_SetAction(ACTION_SELECT1,mbk->OnEnterPress_GuiFolder);
  mbk->gui_folder->SoftKeys_SetAction(ACTION_BACK,mbk->OnBackPress_GuiFolder);
  mbk->gui_folder->Show();
  return 1;
};

void CMyBook::OnBackPress_GuiFolder(CBookBase*bk,CGuiBase*gui)
{
  FUNCTION
  MYBOOK(bk);
  gui->Free();
  mbk->gui_folder=0;
  delete[] mbk->findstr;
  mbk->findstr=0;
  mbk->GotoPage(&mbk->input_page);
};

void CMyBook::OnEnterPress_GuiFolder(CBookBase*bk,CGuiBase*gui)
{
  FUNCTION
  MYBOOK(bk);
  int item=((CGuiListMenu*)gui)->GetSelectedItem();
  gui->Free();
  wchar_t*s;
  void*dir;
  switch(item)
  {
  case 0:
    s=new wchar_t[wstrlen(GetDir(DIR_USER|MEM_EXTERNAL))+1];
    wstrcpy(s,GetDir(DIR_USER|MEM_EXTERNAL));
    mbk->AddFolderItem(L"",s);
    delete[] s;
    s=new wchar_t[wstrlen(GetDir(DIR_USER|MEM_INTERNAL))+1];
    wstrcpy(s,GetDir(DIR_USER|MEM_INTERNAL));
    mbk->AddFolderItem(L"",s);
    delete[] s;
    break;
  case 1:
    mbk->AddFolderItem(SLASH,L"");
    break;
  case 2:
    mbk->AddFolderItem(SLASH,L"card");
    break;
  case 3:
    dir=w_diropen(SLASH);
    if(dir)
    {
      while((s=w_dirread(dir)))
        if(wstrcmp(s,L"card") && isDir(SLASH,s)) mbk->AddFolderItem(SLASH,s);
      w_dirclose(dir);
    }
    break;
  }
  mbk->GotoPage(&mbk->find_page);
}
