#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

wchar_t * SKIN;

int DestroySkinGui()
{
  FREE_GUI(mbk->skn_input);
  return(0);
}

void OnOkSkin(BOOK *,wchar_t * string,int len)
{
  //History_Get();
  //сохраняем меню
  Menu_Save();
  //очищаем историю
  History_Free();
  //создаём историю заново
  History_Create();
  //меняем скин
  DELETE(SKIN);
  kickbadsymbols(string,len);
  SKIN=new wchar_t[len+3+1];
  wstrcpy(SKIN,string);
  wstrcat(SKIN,L".mm");
  //кешируем путь для скина
  Cache_SkinPath();
  //создаем папку для него
  mkdir(skin_path_int);
  //создаем меню
  Menu_Create();
  //обновляем гуй
  Menu_Refresh();
  //сохраняем конфиг
  Config_Save();
  //сносим гуи для смены скина
  FREE_GUI(mbk->skin);
  FREE_GUI(mbk->skn_input);
}

void CreateSkinInput()
{
  mbk->skn_input=CreateStringInputVA(0,
                                     VAR_PREV_ACTION_PROC(DestroySkinGui),
                                     VAR_LONG_BACK_PROC(DestroySkinGui),
                                     VAR_BOOK((BOOK*)mbk),
                                     VAR_OK_PROC(OnOkSkin),
                                     VAR_STRINP_MIN_LEN(1),
                                     VAR_STRINP_MAX_LEN(128),
                                     0);
}

int onAcceptSkinDB(void * data, BOOK * book)
{
  FILEITEM* fi = (FILEITEM*)data;
  wchar_t*fname=FILEITEM_GetFname(fi);

  //сохраняем меню
  Menu_Save();
  //очищаем историю
  History_Free();
  //создаем историю
  History_Create();
  //меняем скин
  DELETE(SKIN);
  SKIN=new wchar_t[wstrlen(fname)+1];
  wstrcpy(SKIN,fname);
  //кешируем путь
  Cache_SkinPath();
  //читаем скин
  Menu_Read();
  //обновляем гуй
  Menu_Refresh();
  //сохраняем конфиг
  Config_Save();
  //сносим гуи для смены скина
  FREE_GUI(mbk->skin);
  return(0);
}

int CreateSkinDB(void *data, BOOK * book)
{
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  wchar_t*tmp=get_path(0);
  wchar_t*tmp2=get_path(1);
  folder_list[0]=tmp;
  folder_list[1]=tmp2;
  folder_list[2]=0;
  DataBrowserDesc_SetHeaderText(DB_Desc,TextID_Create(names[31],ENC_UCS2,TEXTID_ANY_LEN));
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFileExtList(DB_Desc,L"*.mm");
  DataBrowserDesc_SetItemFilter(DB_Desc,DB_Filter);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetSelectAction(DB_Desc,1);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);
  DELETE(tmp);
  DELETE(tmp2);
  return(0);
}

const PAGE_MSG SelectSkin_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateSkinDB,
  ACCEPT_EVENT_TAG,onAcceptSkinDB,
  PREVIOUS_EVENT_TAG,onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG,onCancel_MainMenu_DB,
  NIL_EVENT_TAG,0
};

PAGE_DESC SelectSkin_page ={"BookManager_ChangeShortcuts_SelectSkin_Page",0,SelectSkin_PageEvents};

void SkinBack(BOOK *bk, GUI*lt)
{
  FREE_GUI(mbk->skin);
}

void SkinOnEnterPressed(BOOK *bk, GUI*lt)
{
  int item=ListMenu_GetSelectedItem(mbk->skin);
  switch(item)
  {
  case 0:
    BookObj_CallPage(bk,&SelectSkin_page);
    break;
  case 1:
    CreateSkinInput();
    break;
  case 2:
    Create_SelectImageDB(2);
    break;
  }
}

int SkinOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    switch(item)
    {
    case 0:

      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[32],ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"DB_LIST_THEME_ICN"));
      break;
    case 1:

      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[33],ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"MESSAGING_MENU_NEW_MESSAGE_ICN"));
      break;
    case 2:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[44],ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"JOYSTICK_MORE_MENU_ICN"));
      break;
    }
  }
  return(1);
};

void SkinCreateGuiList(void * r0, BOOK * bk)
{
  mbk->skin=CreateListMenu(bk,0);
  wchar_t buf[256];
  wchar_t *str_d=new wchar_t[wstrlen(SKIN)+1];
  wstrcpy(str_d,SKIN);
  *(wstrrchr(str_d,L'.'))=0;
  wchar_t *title=new wchar_t[wstrlen(str_d)+1];
  wstrcpy(title,str_d);

  snwprintf(buf,255,L"%ls: %ls",names[31],title);
  GUIObject_SetTitleText(mbk->skin,TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN));
  if(Menu_Get()->style_type==1)
  ListMenu_SetItemCount(mbk->skin,3);
  else ListMenu_SetItemCount(mbk->skin,2);
  ListMenu_SetOnMessage(mbk->skin,SkinOnLBMessage);
  ListMenu_SetCursorToItem(mbk->skin,0);
  ListMenu_SetItemStyle(mbk->skin,0);
  GUIObject_SoftKeys_SetAction(mbk->skin,ACTION_BACK, SkinBack);
  GUIObject_SoftKeys_SetAction(mbk->skin,ACTION_LONG_BACK, SkinBack);
  GUIObject_SoftKeys_SetAction(mbk->skin,ACTION_SELECT1,SkinOnEnterPressed);
  GUIObject_Show(mbk->skin);
  DELETE(str_d);
  DELETE(title);
};
