//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "MyFUNC.h"
#include "Game.h"
#include "Pages.h"


int onAccept_DB(void * data, BOOK * book)
{
  wstrcpy(MyBK()->name,FILEITEM_GetFname((FILEITEM*)data));
  wstrcpy(MyBK()->path,FILEITEM_GetPath((FILEITEM*)data));
  BookObj_GotoPage((BOOK *)(MyBK()),&bk_actionlist);
  return (0);
};

int onCancelDB(void *data, BOOK * book)
{
  BookObj_GotoPage((BOOK *)(MyBK()),&bk_gm);
  return(1);
};

int MyFilter(const wchar_t *ExtList, const wchar_t *ItemPath, const wchar_t *ItemName)
{
  if(DataBrowser_isFileInListExt(ExtList,ItemPath,ItemName)) return 1;
  return 0;
}
int CreateDB(void *, BOOK * book)
{
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[2];
  folder_list[0]=GetConfigPath();
  folder_list[1]=0;
  //DataBrowserDesc_Menu_AddFSFunctions(DB_Desc, 1);
  DataBrowserDesc_SetHeaderText(DB_Desc,Str2ID(L"Выбрать сохранение", ENC_UCS2, TEXTID_ANY_LEN));
  DataBrowserDesc_SetBookID(DB_Desc,BOOK_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetFileExtList(DB_Desc,L"*.stg");
  DataBrowserDesc_SetItemFilter(DB_Desc,MyFilter);
  DataBrowser_Create(DB_Desc);
  return(0);
};
