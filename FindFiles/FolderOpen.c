#include "mcl.h"
int CMyBook::OnFolderOpenPage(void*,CMyBook*mbk)
{
  FUNCTION
  int item=mbk->gui_find->GetSelectedItem();
  ELEM_*elem=(ELEM_*)List_Get(mbk->GetFindList(),item);
  FILEITEM*fitem=FILEITEM_Create();
  FILEITEM_SetFnameAndContentType(fitem,elem->name);
  FILEITEM_SetPath(fitem,elem->path);
  mbk->DB=DataBrowserDesc_Create();
  const wchar_t * folder_list[2];
  folder_list[0]=elem->path;
  folder_list[1]=0;
  DataBrowserDesc_SetHeaderText(mbk->DB,Str2ID(elem->path, ENC_UCS2, TEXTID_ANY_LEN));
  DataBrowserDesc_SetBookID(mbk->DB,mbk->GetBookID());
  DataBrowserDesc_SetFolders(mbk->DB,folder_list);
  DataBrowserDesc_SetSelectActionOnFolders(mbk->DB,1);
  DataBrowserDesc_Menu_AddFSFunctions(mbk->DB,0);
  DataBrowserDesc_SetFoldersNumber(mbk->DB,1);
  DataBrowserDesc_SetFocusToFILEITEM(mbk->DB,fitem);
  DataBrowser_Create(mbk->DB);
  FILEITEM_Destroy(fitem);
  return 1;
};

int CMyBook::OnExitPress_Folder(void*,CMyBook*mbk)
{
  FUNCTION
  int i=0;
  delete mbk->DB;
  mbk->DB=0;
  ELEM_*elem;
  FSTAT _fs;
  LIST*list=mbk->GetFindList();
  while(i<list->FirstFree)
  {
    elem=(ELEM_*)List_Get(list,i);
    if(fstat(elem->path,elem->name,&_fs)!=0)
    {
      delete (ELEM_*)List_RemoveAt(list,i);
      continue;
    }
    i++;
  };
  mbk->GotoPage(&mbk->findshow_page);
  return 1;
}
