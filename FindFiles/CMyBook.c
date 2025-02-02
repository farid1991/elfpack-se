#include "mcl.h"

int CMyBook::ShowAuthorInfo(CBook**bm_book,CMyBook*mbk)
{
  FUNCTION
  MESSAGE(L"Find Files\nver. 0.1\nfix3\n(c) mmcorp\n");
  return 1;
};

int CMyBook::TerminateElf(CBook**bm_book,CMyBook*mbk)
{
  FUNCTION
  mbk->Free();
  elf_exit();
  return 1;
};

CMyBook::~CMyBook()
{
  FUNCTION
  GUIFREE(gui_input);
  GUIFREE(gui_find);
  GUIFREE(gui_folder);
  if(findstr) delete[] findstr;
  if(DB) delete DB;
  while(folder_list->FirstFree) delete[] (wchar_t*)List_RemoveAt(folder_list,0);
  List_Destroy(folder_list);
  while(find_list->FirstFree) delete (ELEM_*)List_RemoveAt(find_list,0);
  List_Destroy(find_list);
  if(TimerID) Timer_Kill(&TimerID);
};

CMyBook::CMyBook(wchar_t*_findstr,wchar_t**_findfolders,int nfolder)
:CBook(NAME_ELF,&base_page)
{
  FUNCTION
  DB=0;
  gui_input=0;
  gui_folder=0;
  gui_del=0;
  gui_addfolder=0;
  open_folder=0;
  find_list=List_Create();
  folder_list=List_Create();
  TimerID=0;
  gui_find=0;
  state=false;
  if(_findstr)
  {
    findstr=new wchar_t[wstrlen(_findstr)+1];
    wstrcpy(findstr,_findstr);
  }
  else findstr=0;
  wchar_t*dir;
  int i;
  if(nfolder) for(int j=0;j<nfolder;j++)
  {
    dir=new wchar_t[wstrlen(_findfolders[j])+1];
    wstrcpy(dir,_findfolders[j]);
    i=FolderInList(GetFolderList(),dir);
    switch(i)
    {
    case -2:
      AddFolderItem(L"",dir);
      break;
    case -1:
      delete[] dir;
      break;
    default:
      delete[] (wchar_t*)List_Get(GetFolderList(),i);
      List_Insert(GetFolderList(),i,dir);
      break;
    }
  }
  GotoPage(&input_page);
};

void CMyBook::OnTimer(u16 unk,LPARAM data)
{
  FUNCTION
  MYBOOK(data);
  bool refresh=false;
  wchar_t*folder=mbk->GetFindFolder();
  if(folder)
  {
    void*dir=w_diropen(folder);
    wchar_t*dir_item;
    while((dir_item=w_dirread(dir)))
    {
      if(equ(dir_item,mbk->GetFindStr()))
      {
        mbk->AddFindItem(folder,dir_item);
        refresh=true;
      }
      if(isDir(folder,dir_item)) mbk->AddFolderItem(folder,dir_item);
    }
    delete folder;
    w_dirclose(dir);
    mbk->QuickRefresh();
    Timer_ReSet(&mbk->TimerID,1,mbk->OnTimer,data);
    if(refresh) mbk->GotoPage(&mbk->find_page);
  }
  else
  {
    Timer_Kill(&mbk->TimerID);
    mbk->GotoPage(&mbk->findshow_page);
  }
}

LIST*CMyBook::GetFolderList()
{
  FUNCTION
  return folder_list;
}

LIST*CMyBook::GetFindList()
{
  FUNCTION
  return find_list;
}

void CMyBook::OnFindStopPress_GuiFind(CBookBase*bk,CGuiBase*gui)
{
  MYBOOK(bk);
  mbk->GotoPage(&mbk->findshow_page);
}

void CMyBook::AddFindItem(wchar_t*dir,wchar_t*name)
{
  FUNCTION
  ELEM_*elem=new ELEM_;
  wstrcpy(elem->path,dir);
  wstrcpy(elem->name,name);
  elem->isDir=isDir(dir,name);
  List_InsertLast(find_list,elem);
};

bool DirAsSubDir(wchar_t*dir,wchar_t*sub_dir)
{
  FUNCTION
  if(wstrncmp2(dir,sub_dir)==0) return true;
  return false;
}

int CMyBook::FolderInList(LIST*list,wchar_t*dir)
{
  FUNCTION
  int len=wstrlen(dir);
  int len2;
  wchar_t*dir2;
  for(int i=0;i<=list->FirstFree;i++)
  {
    dir2=(wchar_t*)List_Get(list,i);
    len2=wstrlen(dir2);
    if(!wstrcmp(dir,dir2)) return -1;
    if(len>len2) if(DirAsSubDir(dir2,dir)) return -1;
    else if(DirAsSubDir(dir,dir2)) return i;
  }
  return -2;
};

void CMyBook::AddFolderItem(wchar_t*dir,wchar_t*sub_dir)
{
  FUNCTION
  wchar_t*folder=new wchar_t[wstrlen(dir)+wstrlen(sub_dir)+10];
  if(wstrlen(dir))
  {
    wstrcpy(folder,dir);
    if(folder[wstrlen(folder)-1]!='/' && sub_dir[0]!='/') wstrcat(folder,SLASH);
    wstrcat(folder,sub_dir);
  }
  else wstrcpy(folder,sub_dir);
  List_InsertFirst(folder_list,folder);
};

wchar_t*CMyBook::GetFindFolder()
{
  FUNCTION
  if(folder_list->FirstFree) return (wchar_t*)List_RemoveAt(folder_list,0);
  else return 0;
};


void CMyBook::SetFindStr(wchar_t*str)
{
  FUNCTION
  if(findstr) delete[] findstr;
  findstr=new wchar_t[wstrlen(str)+1];
  wstrcpy(findstr,str);
};

wchar_t*CMyBook::GetFindStr()
{
  FUNCTION
  return findstr;
};

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "FF_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END
DECLARE_PAGE_DESC_BEGIN(CMyBook::input_page, "FF_InputPage")
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnInputPage )
DECLARE_PAGE_DESC_END
DECLARE_PAGE_DESC_BEGIN(CMyBook::find_page, "FF_FindPage")
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnFindPage )
DECLARE_PAGE_DESC_END
DECLARE_PAGE_DESC_BEGIN(CMyBook::findshow_page, "FF_FindShowPage")
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnFindShowPage )
DECLARE_PAGE_DESC_END
DECLARE_PAGE_DESC_BEGIN(CMyBook::folder_page, "FF_FolderPage")
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnFolderPage )
DECLARE_PAGE_DESC_END
DECLARE_PAGE_DESC_BEGIN(CMyBook::folderopen_page, "FF_FolderOpenPage")
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnFolderOpenPage )
DECLARE_PAGE_DESC_MSG( CANCEL_EVENT_TAG,     CMyBook::OnExitPress_Folder)
DECLARE_PAGE_DESC_MSG( PREVIOUS_EVENT_TAG,   CMyBook::OnExitPress_Folder)
DECLARE_PAGE_DESC_END
