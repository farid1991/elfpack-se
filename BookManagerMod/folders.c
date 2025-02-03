#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "main.h"

int CreateFolder(void*,BOOK* book)
{
  MyBOOK*mbk=(MyBOOK*)book;

  const wchar_t* folder_list[2];
  folder_list[0]=mbk->folder[0];
  folder_list[1]=mbk->folder[1];
  mbk->folder[0]=0;
  mbk->folder[1]=0;

  void * DB_Desc=DataBrowserDesc_Create();
  DataBrowserDesc_SetHeaderText(DB_Desc,mbk->folname);
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);

  if(filter_on ==1)
  {
    DataBrowserDesc_SetItemFilter(DB_Desc, Folders_Filter);
  }
  DataBrowserDesc_SetFoldersNumber(DB_Desc,mbk->fol);
  DataBrowserDesc_SetSelectAction(DB_Desc,0);
  DataBrowserDesc_Menu_AddFSFunctions(DB_Desc, 0);
  DataBrowserDesc_Menu_AddNewFolder(DB_Desc, 1);
  DataBrowserDesc_Menu_AddMarkFiles(DB_Desc, 1);

  if(mbk->set_2020 == 1)
  {
    DataBrowserDesc_SetViewModeAndSortOrder(DB_Desc, 0x8);
    DataBrowserDesc_SetItemStyle(DB_Desc, mbk->fmod);
  } else {
    DataBrowserDesc_SetViewModeAndSortOrder(DB_Desc, 0);
    DataBrowserDesc_SetItemStyle(DB_Desc, 1);
  }
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);
  return 0;
}

int onCancelDB(void*,BOOK* book)
{
  BookObj_ReturnPage( book, ACCEPT_EVENT );
  return 0;
}

const PAGE_MSG OpenFolder_PageEvents[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,     CreateFolder,
  PREVIOUS_EVENT_TAG,       onCancelDB,
  CANCEL_EVENT_TAG,         onCancelDB,
  NIL_EVENT_TAG,            NULL
};

const PAGE_DESC OpenFolder_Page = {"BookManager_OpenFolder_Page",0,OpenFolder_PageEvents};

void FolderCreate(BOOK*bk)
{
  BookObj_CallPage( bk, &OpenFolder_Page );
}
