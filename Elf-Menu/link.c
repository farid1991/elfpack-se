#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

GUI_LIST * java_list_menu;
LIST * java_list;

int MainMenuID=-1;

extern GUI_LIST * java_list_menu;
extern LIST * java_list;
extern int MainMenuID;
bool is_pic_changed=false;
char * buffer;
int buf_size;

typedef struct
{
  wchar_t * name;
  wchar_t * fullpath;
  int appID;
  IMAGEID imageID;
}java_list_elem;


//
//ввод номера
int DestroyVcGui()
{
  FREE_GUI(mbk->vc_input);

  if((Menu_CheckStyle()&&(Menu_Get()->style_type==0))||((Menu_Get()->style_type==1)&&(Menu_Get()->st.is_selected)))
  {
      RefreshEdList(mbk->edit,5);
  } else {
      RefreshEdList(mbk->edit,4);
  }

  return(0);
}

void OnOkVcAction(BOOK *,wchar_t * string,int len)
{
  ITEM *it=Menu_GetSelected();
  DELETE(it->link);
  kickbadsymbols(string,len);
  it->link=new wchar_t[len+1];
  wstrncpy(it->link,string,len);
  it->link[len]=L'\0';
  DestroyVcGui();
}

void CreateVcInput()
{
  ITEM *it=Menu_GetSelected();
  mbk->vc_input=CreateStringInputVA(0,
                                  VAR_PREV_ACTION_PROC(DestroyVcGui),
                                  VAR_LONG_BACK_PROC(DestroyVcGui),
                                  VAR_BOOK((BOOK*)mbk),
                                  VAR_OK_PROC(OnOkVcAction),
                                  VAR_STRINP_MODE(3),
                                  VAR_STRINP_TEXT(TextID_Create(it->link,ENC_UCS2,TEXTID_ANY_LEN)),
                                  VAR_STRINP_MIN_LEN(1),
                                  VAR_STRINP_MAX_LEN(128),
                                  0);
}


//ввод ивента
int DestroyEvGui()
{
  FREE_GUI(mbk->ev_input);
  SetRefresh();
  return(0);
}

void OnOkEvAction(BOOK *,wchar_t * string,int len)
{
  ITEM *it=Menu_GetSelected();
  DELETE(it->link);
  kickbadsymbols(string,len);
  it->link=new wchar_t[len+1];
  wstrncpy(it->link,string,len);
  it->link[len]=L'\0';
  DestroyEvGui();
}

void CreateEvInput()
{
  ITEM *it=Menu_GetSelected();
  int str;
  if(wstrwstr(it->link,L"0x"))str=TextID_Create(it->link,ENC_UCS2,TEXTID_ANY_LEN);
  else str=STR("0x");
  mbk->ev_input=CreateStringInputVA(0,
                                  VAR_PREV_ACTION_PROC(DestroyEvGui),
                                  VAR_LONG_BACK_PROC(DestroyEvGui),
                                  VAR_BOOK((BOOK*)mbk),
                                  VAR_OK_PROC(OnOkEvAction),
                                  VAR_STRINP_TEXT(str),
                                  VAR_STRINP_MIN_LEN(1),
                                  VAR_STRINP_MAX_LEN(128),
                                  0);
}

//выбор ссылки

BOOK * MainMenu;
void onShortcutSet(BOOK * bk,GUI*)
{
  wchar_t*name_buf=new wchar_t[wstrlen(MenuBook_Desktop_GetSelectedItemID(MainMenu))+1];
  wstrcpy(name_buf,MenuBook_Desktop_GetSelectedItemID(MainMenu));
  if (name_buf)
  {
    ITEM*it=Menu_GetSelected();
    DELETE(it->link);
    it->link=new wchar_t[wstrlen(name_buf)+1];
    wstrcpy(it->link,name_buf);

    if(Platform != 200)
    {
      SHORTCUT_DESC * w_buf =new SHORTCUT_DESC;
      memset(w_buf,0,0x32);
      w_buf->shortcut_state=0x2;
      if (wstrlen(it->link)<0x32) wstrcpy(w_buf->name,it->link);
      else
      {
        wchar_t temp_buf[0x32];
        wstrncpy(temp_buf,it->link,0x31);
        temp_buf[0x31]=0;
        wstrcpy(w_buf->name,temp_buf);
      }
      int stid=Shortcut_Get_MenuItemName(w_buf);
      DELETE(it->name);
      it->name=new wchar_t[TextID_GetLength(stid)+1];
      TextID_GetWString(stid,it->name,TextID_GetLength(stid)+1);
      GUIObject_SetTitleText(mbk->edit,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
      DELETE(w_buf);
      DELETE(name_buf);
    }
    else
    {
      SHORTCUT_DESC_A2 * w_buf =new SHORTCUT_DESC_A2;
      memset(w_buf,0,sizeof(w_buf));
//      w_buf->shortcut_state = SC_State_Activated;
      wstrcpy(w_buf->name,it->link);
//    }
      int stid=Shortcut_Get_MenuItemName(w_buf);
      DELETE(it->name);
      it->name=new wchar_t[TextID_GetLength(stid)+1];
      TextID_GetWString(stid,it->name,TextID_GetLength(stid)+1);
      GUIObject_SetTitleText(mbk->edit,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
      DELETE(w_buf);
      DELETE(name_buf);
    }
  }
  if (MainMenuID != -1 )
  {
    BOOK* MainMenu = FindBookByID(MainMenuID );

    if ( MainMenu )
      FreeBook( MainMenu );

    MainMenuID = -1;
  }
  SetRefresh();
}


int onPrevious_MainMenu_DB(void *data, BOOK * book)
{
  Book_Refresh();
  return(1);
}


int onCancel_MainMenu_DB(void *data, BOOK * book)
{
  Book_Refresh();
  return(1);
}


int CreateMainMenu(void *data, BOOK * book)
{
  if(MainMenu=MenuBook_Desktop(1,BookObj_GetBookID(book)))
  {
    MainMenuID=BookObj_GetBookID(MainMenu);
    BookObj_SoftKeys_SetAction(MainMenu,0x0,onShortcutSet);
    int str_id;
    textidname2id(L"SHC_SET_SHORTCUT_SK",TEXTID_ANY_LEN,&str_id);
    BookObj_SoftKeys_SetText(MainMenu,0x0,str_id);
    return(1);
  }
  return(0);
}

int DB_Filter(const wchar_t * ext_table,const wchar_t * path ,const wchar_t * name)
{
  return (DataBrowser_isFileInListExt(ext_table, path ,name));
}
int DB_Elf_Filter(const wchar_t * ext_table,const wchar_t * path ,const wchar_t * name)
{
  FSTAT _fstat;
  if (DataBrowser_isFileInListExt(ext_table,path,name)) return(1);
  fstat(path,name,&_fstat);
  if ((_fstat.st_mode&FSX_S_IFDIR)) return(1);
  return(0);
}
int onAccept_DB(void * data, BOOK * book)
{
  FILEITEM* fi = (FILEITEM*)data;
  wchar_t*fname=FILEITEM_GetFname(fi);
  wchar_t*path=FILEITEM_GetPath(fi);

  wchar_t * elf_buf=new wchar_t[wstrlen(path)+wstrlen(fname)+wstrlen(Slash)+1];
  wstrcpy(elf_buf,path);
  wstrcat(elf_buf,Slash);
  wstrcat(elf_buf,fname);
  ITEM*it=Menu_GetSelected();
  DELETE(it->link);
  it->link=new wchar_t[wstrlen(elf_buf)+1];
  wstrcpy(it->link,elf_buf);
  DELETE(it->name);
  wchar_t *str_d=new wchar_t[wstrlen(fname)+1];
  wstrcpy(str_d,fname);
  *(wstrrchr(str_d,L'.'))=0;
  it->name=new wchar_t[wstrlen(str_d)+1];
  wstrcpy(it->name,str_d);
  GUIObject_SetTitleText(mbk->edit,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
  DELETE(elf_buf);
  DELETE(str_d);
  Book_Refresh();
  SetRefresh();
  return(0);
}

int CreateDB(void *data, BOOK * book)
{
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  if (wstrwstr(GetDir(DIR_ELFS_DAEMONS | MEM_EXTERNAL),L"ZBin"))
  {
    folder_list[0]=GetDir(DIR_ELFS | MEM_INTERNAL);
    folder_list[1]=GetDir(DIR_ELFS | MEM_EXTERNAL);
    DataBrowserDesc_SetHeaderText(DB_Desc,STR("ZBin"));
  }
  else
  {
    folder_list[0]=GetDir(DIR_OTHER | MEM_INTERNAL);
    folder_list[1]=GetDir(DIR_OTHER | MEM_EXTERNAL);
    DataBrowserDesc_SetHeaderText(DB_Desc,STR("Other"));
  }
  folder_list[2]=0;
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFileExtList(DB_Desc,L"*.elf");
  DataBrowserDesc_SetItemFilter(DB_Desc,DB_Elf_Filter);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetSelectAction(DB_Desc,1);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);
  return(0);
}

int onExit_JavaList(void *data, BOOK * book);
void onEnter_JavaList(BOOK * book, GUI *)
{
  java_list_elem * elem=(java_list_elem *)List_Get(java_list,ListMenu_GetSelectedItem(java_list_menu));

  ITEM*it=Menu_GetSelected();

  DELETE(it->link);
  it->link=new wchar_t[wstrlen(elem->name)+1];
  wstrcpy(it->link,elem->name);

  DELETE(it->name);
  it->name=new wchar_t[wstrlen(elem->name)+1];
  wstrcpy(it->name,elem->name);
  GUIObject_SetTitleText(mbk->edit,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
  onExit_JavaList(0, book);
  SetRefresh();
}

void ExitJavaList(BOOK * book, GUI *)
{
  onExit_JavaList(0, book);
}

void elem_free(void * elem)
{
  java_list_elem * lm=(java_list_elem *)elem;
  if (lm->name) delete(lm->name);
  if (lm->fullpath) delete(lm->fullpath);
  IDFREE(lm->imageID);
  delete(lm);
}

int elem_filter(void * elem)
{
  if (elem) return(1);
  return(0);
}


int onExit_JavaList(void *data, BOOK * book)
{
  if (java_list_menu)
  {
    FREE_GUI(java_list_menu);
    java_list_menu=0;
  }
  if (java_list)
  {
    List_DestroyElements(java_list,elem_filter,elem_free);
    List_Destroy(java_list);
    java_list=0;
  }
  return(0);
}


int java_list_callback(GUI_MESSAGE * msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    java_list_elem * elem=(java_list_elem*)List_Get(java_list,GUIonMessage_GetCreatedItemIndex(msg));
    GUIonMessage_SetMenuItemText(msg,TextID_Create(elem->name,ENC_UCS2,TEXTID_ANY_LEN));
    JavaApp_LogoImageID_Get(elem->fullpath,&elem->imageID);
    GUIonMessage_SetMenuItemIcon(msg,0,elem->imageID);
  }
  return(1);
}


java_list_elem * CreateElem(void * JavaDesc)
{
  java_list_elem * elem=new java_list_elem;
  wchar_t * sp;
  JavaAppDesc_GetJavaAppInfo(JavaDesc,0,&sp);
  elem->name=sp;
  JavaAppDesc_GetJavaAppInfo(JavaDesc,5,&sp);
  elem->fullpath=sp;
  elem->appID=JavaAppDesc_GetJavaAppID(JavaDesc);
  return(elem);
}

int JavaName_GetID(wchar_t * name)
{
  int res=-1;
  char sp1;
  void * JavaDesc;
  JavaDialog_Open(0,&sp1,&JavaDesc);
  if (!JavaAppDesc_GetFirstApp(JavaDesc))
  {
    int result=0;
    while (!result)
    {
      java_list_elem * je=CreateElem(JavaDesc);
      if(wstrcmp(name,je->name)==0)
      {
        res=je->appID;
        DELETE(je);
        break;
      }
      result=JavaAppDesc_GetNextApp(JavaDesc);
      DELETE(je);
    }
  }
  JavaDialog_Close(sp1);
  return(res);
}

int CreateJavaList(void *data, BOOK * book)
{
  if (java_list)
  {
    List_DestroyElements(java_list,elem_filter,elem_free);
    List_Destroy(java_list);
  }
  java_list=List_Create();
  char sp1;
  void * JavaDesc;
  JavaDialog_Open(0,&sp1,&JavaDesc);
  if (!JavaAppDesc_GetFirstApp(JavaDesc))
  {
    int result=0;
    while (!result)
    {
      List_InsertLast(java_list,CreateElem(JavaDesc));
      result=JavaAppDesc_GetNextApp(JavaDesc);
    }
  }
  JavaDialog_Close(sp1);
  FREE_GUI(java_list_menu);
  java_list_menu=CreateListMenu(book,0);
  GUIObject_SetTitleText(java_list_menu,STR("Java"));
  ListMenu_SetItemCount(java_list_menu,java_list->FirstFree);
  ListMenu_SetOnMessage(java_list_menu,java_list_callback);
  ListMenu_SetCursorToItem(java_list_menu,0);
  GUIObject_SoftKeys_SetAction(java_list_menu,ACTION_BACK,ExitJavaList);
  GUIObject_SoftKeys_SetAction(java_list_menu,ACTION_LONG_BACK,ExitJavaList);
  GUIObject_SoftKeys_SetAction(java_list_menu,ACTION_SELECT1,onEnter_JavaList);
  GUIObject_Show(java_list_menu);
  return(0);
}


const PAGE_MSG SelectElf_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateDB,
  ACCEPT_EVENT_TAG,onAccept_DB,
  PREVIOUS_EVENT_TAG,onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG,onCancel_MainMenu_DB,
  NIL_EVENT_TAG,0
};

PAGE_DESC SelectElf_page ={"BookManager_ChangeShortcuts_SelectElf_Page",0,SelectElf_PageEvents};
