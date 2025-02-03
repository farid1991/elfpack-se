#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\cfg_items.h"
#include "..\include\var_arg.h"
#include "shortcuts.h"
#include "main.h"
#include "config_data.h"
#include "conf_loader.h"


const wchar_t *books_tab_actions[MaxModeNum][MAX_BOOKS_ELFS_SH_NUM] = {
  {
    books_short_action_0,
    books_short_action_1,
    books_short_action_2,
    books_short_action_3,
    books_short_action_4,
    books_short_action_5,
    books_short_action_6,
    books_short_action_7,
    books_short_action_8,
    books_short_action_9
  },
  {
    books_long_action_0,
    books_long_action_1,
    books_long_action_2,
    books_long_action_3,
    books_long_action_4,
    books_long_action_5,
    books_long_action_6,
    books_long_action_7,
    books_long_action_8,
    books_long_action_9
  }
};

const wchar_t *elfs_tab_actions[MaxModeNum][MAX_BOOKS_ELFS_SH_NUM] = {
  {
    elfs_short_action_0,
    elfs_short_action_1,
    elfs_short_action_2,
    elfs_short_action_3,
    elfs_short_action_4,
    elfs_short_action_5,
    elfs_short_action_6,
    elfs_short_action_7,
    elfs_short_action_8,
    elfs_short_action_9
  },
  {
    elfs_long_action_0,
    elfs_long_action_1,
    elfs_long_action_2,
    elfs_long_action_3,
    elfs_long_action_4,
    elfs_long_action_5,
    elfs_long_action_6,
    elfs_long_action_7,
    elfs_long_action_8,
    elfs_long_action_9
  }
};

const wchar_t *shortcuts_tab_actions[MAX_OTHER_SH_NUM] = {
    shortcuts_action_1,
    shortcuts_action_2,
    shortcuts_action_3,
    shortcuts_action_4,
    shortcuts_action_5,
    shortcuts_action_6,
    shortcuts_action_7,
    shortcuts_action_8,
    shortcuts_action_9
};

wchar_t **getShortcutActions(MyBOOK* mbk, int *maxNum)
{
  int tab = mbk->ActiveTAB;
  if(tab == books_tab)
  {
    if(maxNum) *maxNum = MAX_BOOKS_ELFS_SH_NUM;
    return (wchar_t **)books_tab_actions[ListMenu_GetSelectedItem(mbk->mode_list)];
  }
  else if(tab == elfs_tab)
  {
    if(maxNum) *maxNum = MAX_BOOKS_ELFS_SH_NUM;
    return (wchar_t **)elfs_tab_actions[ListMenu_GetSelectedItem(mbk->mode_list)];
  }
  else if(tab = shortcuts_tab)
  {
    if(maxNum) *maxNum = MAX_OTHER_SH_NUM;
    return (wchar_t **)shortcuts_tab_actions;
  }
  if(maxNum) *maxNum = 0;
  return 0;
}

int getShortcutsNum(int tab)
{
  if(tab == books_tab)
  {
    return MAX_BOOKS_ELFS_SH_NUM;
  }
  else if(tab == elfs_tab)
  {
    return MAX_BOOKS_ELFS_SH_NUM;
  }
  else if(tab == shortcuts_tab)
  {
    for(int i = 0; i < MAX_OTHER_SH_NUM; i++)
    {
      if(wstrlen(shortcuts_tab_actions[i]) == 0) return i;
    }
    return MAX_OTHER_SH_NUM;
  }
  return 0;
}

// Вставка со сдвигом всех вниз
int InsertAction(MyBOOK* mbk, wchar_t *action, int pos)
{
  int maxPos;
  wchar_t **actions = getShortcutActions(mbk, &maxPos);

  if(!actions) return (-1);

  maxPos--;
  if(pos > maxPos) return (-1);
  if(wstrlen(action) >= MAX_SHORTCUTS_ACTION_LEN) return (-1);

  for(int i = maxPos; i > pos; i--)
  {
    wstrcpy(actions[i],actions[i - 1]);
  }
  wstrcpy(actions[pos],action);
  SaveConfigData(successed_config_path,successed_config_name);
  return 0;
}

// Вставка без сдвига всех вниз
int AddAction(MyBOOK* mbk, wchar_t *action, int pos)
{
  int maxPos;
  wchar_t **actions = getShortcutActions(mbk, &maxPos);

  if(!actions) return (-1);

  maxPos--;
  if(pos > maxPos) return (-1);
  if(wstrlen(action) >= MAX_SHORTCUTS_ACTION_LEN) return (-1);

  wstrcpy(actions[pos],action);
  SaveConfigData(successed_config_path,successed_config_name);
  return 0;
}

// Удаление со сдвигом всех вверх
int RemoveAction(MyBOOK* mbk, int pos)
{
  int maxPos;
  wchar_t **actions = getShortcutActions(mbk, &maxPos);

  if(!actions) return (-1);

  maxPos--;
  if(pos > maxPos) return (-1);

  for(int i = pos; i < maxPos; i++)
  {
    wstrcpy(actions[i],actions[i + 1]);
  }
  actions[maxPos][0] = 0;
  SaveConfigData(successed_config_path,successed_config_name);
  return 0;
}

// Удаление без сдвига всех вверх
int EraseAction(MyBOOK* mbk, int pos)
{
  int maxPos;
  wchar_t **actions = getShortcutActions(mbk, &maxPos);

  if(!actions) return (-1);

  maxPos--;
  if(pos > maxPos) return (-1);

  actions[pos][0] = 0;
  SaveConfigData(successed_config_path,successed_config_name);
  return 0;
}

void onShortcutSet( BOOK* MainMenu, GUI* )
{
  wchar_t* name_buf = MenuBook_Desktop_GetSelectedItemID( MainMenu );
  MyBOOK* mbk = (MyBOOK*)FindBook( isBookManager );
  if ( name_buf )
  {
    if(mbk->ActiveTAB == shortcuts_tab)
    {
      InsertAction(mbk, name_buf, ListMenu_GetSelectedItem(mbk->slist));
    }
    else
    {
      AddAction(mbk, name_buf, ListMenu_GetSelectedItem(mbk->but_list));
    }
    mfree( name_buf );
  }
  FreeBook( MainMenu );
  BookObj_ReturnPage( mbk, ACCEPT_EVENT );
}


int onPrevious_MainMenu_DB( void* data, BOOK* book )
{
  BookObj_ReturnPage( book, NIL_EVENT );
  return 1;
}


int onCancel_MainMenu_DB( void* data, BOOK* book )
{
  BookObj_ReturnPage( book, CANCEL_EVENT );
  return 1;
}


int CreateMainMenu( void* data, BOOK* book )
{
  BOOK* MainMenu = MenuBook_Desktop( 1, BookObj_GetBookID( book ) );
  if ( MainMenu )
  {
    ( (MyBOOK*) book )->MainMenuID = BookObj_GetBookID( MainMenu );
    BookObj_SoftKeys_SetAction( MainMenu, 0x0, onShortcutSet );
    int str_id;
    textidname2id( L"SHC_SET_SHORTCUT_SK", TEXTID_ANY_LEN, &str_id );
    BookObj_SoftKeys_SetText( MainMenu, 0x0, str_id );
    return 1;
  }
  BookObj_ReturnPage( book, ACCEPT_EVENT );
  return 0;
}

int DB_Filter( const wchar_t* ext_table, const wchar_t* path , const wchar_t* name )
{
  FSTAT _fstat;

  if ( DataBrowser_isFileInListExt( ext_table, path, name ) )
    return 1;

  fstat( path, name, &_fstat );

  return 0 != ( _fstat.st_mode & FSX_S_IFDIR );
}


int onAccept_DB( void* data, BOOK* book )
{
  MyBOOK * mbk = (MyBOOK*)book;
  wchar_t* path_ptr = wstrwstr( FILEITEM_GetPath( ( FILEITEM* )data ), L"ZBin/" );
  wchar_t* elf_buf = new wchar_t[wstrlen( path_ptr + 4 ) + wstrlen( FILEITEM_GetFname( ( FILEITEM* )data ) ) + 1];
  elf_buf[0] = 0;

  if ( path_ptr )
    wstrcpy( elf_buf, path_ptr + 4 );

  wstrcat( elf_buf, L"/" );
  wstrcat( elf_buf, FILEITEM_GetFname( ( FILEITEM* )data ) );
  AddAction(mbk, elf_buf, ListMenu_GetSelectedItem(mbk->but_list));
  delete( elf_buf );
  BookObj_ReturnPage( book, ACCEPT_EVENT );
  return 0;
}

int CreateDB( void* data, BOOK* book )
{
  void* DB_Desc = DataBrowserDesc_Create();
  const wchar_t* folder_list[3];
  folder_list[0] = GetDir( DIR_ELFS | MEM_INTERNAL );
  folder_list[1] = GetDir( DIR_ELFS | MEM_EXTERNAL );
  folder_list[2] = 0;
  DataBrowserDesc_SetHeaderText( DB_Desc, STR( "ZBin" ) );
  DataBrowserDesc_SetBookID( DB_Desc, BookObj_GetBookID( book ) );
  DataBrowserDesc_SetFolders( DB_Desc, folder_list );
  DataBrowserDesc_SetFileExtList( DB_Desc, L"*.elf" );
  DataBrowserDesc_SetItemFilter( DB_Desc, DB_Filter );
  DataBrowserDesc_SetFoldersNumber( DB_Desc, 2 );
  DataBrowserDesc_SetSelectAction( DB_Desc, 1 );
  DataBrowser_Create( DB_Desc );
  DataBrowserDesc_Destroy( DB_Desc );
  return 0;
}


void onEnter_JavaList( BOOK* book, GUI* )
{
  MyBOOK* mbk = (MyBOOK*) book;
  java_list_elem* elem = (java_list_elem*) List_Get( mbk->java_list, ListMenu_GetSelectedItem( mbk->java_list_menu ) );
  int java_buf_len = wstrlen( elem->name ) + 40;
  wchar_t* java_buf = new wchar_t[java_buf_len];
  snwprintf( java_buf, java_buf_len, L"java:%ls//ID=%d", elem->name, elem->appID );
  AddAction(mbk, java_buf, ListMenu_GetSelectedItem(mbk->but_list));
  delete( java_buf );
  BookObj_ReturnPage( book, ACCEPT_EVENT );
}


void JavaList_Cancel_Action( BOOK* book, GUI* )
{
  BookObj_ReturnPage( book, CANCEL_EVENT );
}


void JavaList_Prev_Action( BOOK* book, GUI* )
{
  BookObj_ReturnPage( book, NIL_EVENT );
}


void elem_free( void* elem )
{
  java_list_elem* lm = (java_list_elem*) elem;

  if ( lm->name )
    delete( lm->name );

  if ( lm->fullpath )
    delete( lm->fullpath );

  if ( lm->imageID )
    ImageID_Free( lm->imageID );

  delete( lm );
}

int elem_filter( void* elem )
{
  return elem!= NULL;
}


int onExit_JavaList( void* data, BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;

  if ( mbk->java_list_menu )
  {
    GUIObject_Destroy( mbk->java_list_menu );
    mbk->java_list_menu = 0;
  }

  if ( mbk->java_list )
  {
    List_DestroyElements( mbk->java_list, elem_filter, elem_free );
    List_Destroy( mbk->java_list );
    mbk->java_list = 0;
  }
  return 0;
}


int java_list_callback( GUI_MESSAGE* msg )
{
  MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );

  switch( GUIonMessage_GetMsg( msg ) )
  {
  case LISTMSG_GetItem:
    java_list_elem* elem = (java_list_elem*) List_Get( mbk->java_list, GUIonMessage_GetCreatedItemIndex( msg ) );
    GUIonMessage_SetMenuItemText( msg, TextID_Create( elem->name, ENC_UCS2, TEXTID_ANY_LEN ) );
    JavaApp_LogoImageID_Get( elem->fullpath, &elem->imageID );
    GUIonMessage_SetMenuItemIcon( msg, 0, elem->imageID );
  }
  return 1;
}


java_list_elem* CreateElem( void* JavaDesc )
{
  java_list_elem* elem = new java_list_elem;
  wchar_t* sp;
  JavaAppDesc_GetJavaAppInfo( JavaDesc, 0, &sp );
  elem->name = sp;
  JavaAppDesc_GetJavaAppInfo( JavaDesc, 5, &sp );
  elem->fullpath = sp;
  elem->appID = JavaAppDesc_GetJavaAppID( JavaDesc );
  return elem;
}


int CreateJavaList( void* data, BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;

  if ( mbk->java_list )
  {
    List_DestroyElements( mbk->java_list, elem_filter, elem_free );
    List_Destroy( mbk->java_list );
  }

  mbk->java_list = List_Create();
  char sp1;
  void* JavaDesc;
  JavaDialog_Open( 0, &sp1, &JavaDesc );

  if ( !JavaAppDesc_GetFirstApp( JavaDesc ) )
  {
    int result = 0;
    while ( !result )
    {
      List_InsertLast( mbk->java_list, CreateElem( JavaDesc ) );
      result = JavaAppDesc_GetNextApp( JavaDesc );
    }
  }

  JavaDialog_Close( sp1 );

  if ( mbk->java_list_menu )
    GUIObject_Destroy( mbk->java_list_menu );

  mbk->java_list_menu = CreateListMenu( book, 0 );
  int str_id;
  textidname2id(L"DB_APPLICATIONS_TXT",TEXTID_ANY_LEN,&str_id); // "Applications"
  GUIObject_SetTitleText( mbk->java_list_menu, str_id );
  ListMenu_SetItemCount( mbk->java_list_menu, mbk->java_list->FirstFree );
  ListMenu_SetOnMessage( mbk->java_list_menu, java_list_callback );
  ListMenu_SetCursorToItem( mbk->java_list_menu, 0 );
  GUIObject_SoftKeys_SetAction( mbk->java_list_menu, ACTION_BACK, JavaList_Prev_Action );
  GUIObject_SoftKeys_SetAction( mbk->java_list_menu, ACTION_LONG_BACK, JavaList_Cancel_Action );
  GUIObject_SoftKeys_SetAction( mbk->java_list_menu, ACTION_SELECT1, onEnter_JavaList );
  GUIObject_Show( mbk->java_list_menu );
  return 0;
}


const PAGE_MSG SelectShortcut_PageEvents[]@ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, CreateMainMenu,
  PREVIOUS_EVENT_TAG, onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG, onCancel_MainMenu_DB,
  NIL_EVENT_TAG, 0
};

PAGE_DESC SelectShortcut_page = { "BookManager_ChangeShortcuts_SelectShortcut_Page", 0, SelectShortcut_PageEvents };


const PAGE_MSG SelectElf_PageEvents[]@ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, CreateDB,
  ACCEPT_EVENT_TAG, onAccept_DB,
  PREVIOUS_EVENT_TAG, onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG, onCancel_MainMenu_DB,
  NIL_EVENT_TAG, 0
};

PAGE_DESC SelectElf_page = { "BookManager_ChangeShortcuts_SelectElf_Page", 0, SelectElf_PageEvents };

const PAGE_MSG SetJava_PageEvents[]@ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, CreateJavaList,
  PAGE_EXIT_EVENT_TAG, onExit_JavaList,
  NIL_EVENT_TAG, 0
};

PAGE_DESC ChangeShortcuts_SetJava_page = { "BookManager_ChangeShortcuts_SetJava_Page", 0, SetJava_PageEvents };


void But_SetJava( BOOK* book, GUI* )
{
  BookObj_CallPage( book, &ChangeShortcuts_SetJava_page );
}


void DestroyButList( BOOK* book, GUI* )
{
  BookObj_ReturnPage( book, PREVIOUS_EVENT );
}


void * SHORTCUT_DESC_Init(char * param)
{
  SHORTCUT_DESC * w_buf = new SHORTCUT_DESC;
  memset( w_buf, 0, sizeof(w_buf) );
  w_buf->shortcut_state = SC_State_Activated;

  if ( strlen( param ) < 0x32 )
  {
    str2wstr( w_buf->name, param );
  }
  else
  {
    char temp_buf[0x32];
    strncpy( temp_buf, param, MAXELEMS(temp_buf) - 1 );
    temp_buf[ MAXELEMS(temp_buf) - 1 ] = 0;
    str2wstr( w_buf->name, temp_buf );
  }
  if (!wstrcmp(w_buf->name,L"MainMenu")) w_buf->shortcut_state = SC_State_MainMenu;
  return(w_buf);
}


void * SHORTCUT_DESC_A2_Init(char * param)
{
  SHORTCUT_DESC_A2 * w_buf = new SHORTCUT_DESC_A2;
  memset( w_buf, 0, sizeof(w_buf) );
  str2wstr( w_buf->name, param );
  if (!wstrcmp(w_buf->name,L"MainMenu")) w_buf->item_type = SC_Type_MainMenu;
  else w_buf->item_type = SC_Type_Standard;

  return(w_buf);
}


int but_list_callback( GUI_MESSAGE* msg )
{
  int str_id=EMPTY_TEXTID;
  int icon_id=NOIMAGE;
  wchar_t * param=0;
  MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );

  switch( GUIonMessage_GetMsg( msg ) )
  {
  case LISTMSG_GetItem:
    int item_num = GUIonMessage_GetCreatedItemIndex( msg );
    textidname2id(L"SHC_NONE_NAME_TXT",TEXTID_ANY_LEN,&str_id);
    icon_id=NOIMAGE;
    wchar_t **actions = getShortcutActions(mbk, 0);
    param = actions[item_num];
    int len = wstrlen(param);
    if (len)
    {
      if (wstrwstr(param,L"java:"))
      {
         str_id=TextID_Create(param+5,ENC_UCS2,wstrwstr(param,L"//")-(param+5));
         iconidname2id(L"DB_LIST_JAVA_ICN",TEXTID_ANY_LEN,&icon_id);
      }
      else
      {
         void * w_buf;
         char *cparam = new char[len+1];
         wstr2strn(cparam, param, len);
         if (mbk->isA2) w_buf = SHORTCUT_DESC_A2_Init(cparam);
         else w_buf = SHORTCUT_DESC_Init(cparam);

         str_id = Shortcut_Get_MenuItemName( w_buf );
         icon_id = Shortcut_Get_MenuItemIconID( w_buf );

         if ( icon_id == NOIMAGE ) iconidname2id( L"RN_VERT_MY_SHORTCUTS_ICN", TEXTID_ANY_LEN, &icon_id );

         delete( w_buf );
         delete( cparam );

         if ( str_id == EMPTY_TEXTID ) str_id = TextID_Create( param, ENC_UCS2, TEXTID_ANY_LEN );
      }
    }
    if (str_id==EMPTY_TEXTID) str_id=TextID_Create(param,ENC_UCS2,TEXTID_ANY_LEN);

    GUIonMessage_SetMenuItemText( msg, str_id );
    GUIonMessage_SetMenuItemIcon(msg, 0, mbk->digs_image[item_num].ImageID );
    GUIonMessage_SetMenuItemIcon( msg, 2, icon_id );
    break;
  }

  return 1;
}

void But_SetMM( BOOK* book, GUI* )
{
  wchar_t name_buf[20];
  wstrcpy( name_buf, L"MainMenu" );
  AddAction((MyBOOK*)book, name_buf, ListMenu_GetSelectedItem(((MyBOOK*)book)->but_list));
  CreateButtonList( 0, book );
}

void But_onEnter( BOOK* book, GUI* )
{
  if ( ((MyBOOK*)book)->ActiveTAB == books_tab )
    BookObj_CallPage( book, &SelectShortcut_page );
  else
    BookObj_CallPage( book, &SelectElf_page );
}


int ExitButtonList( void* data, BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  if ( mbk->but_list )
  {
    GUIObject_Destroy( mbk->but_list );
    mbk->but_list = 0;
  }
  return 0;
}


void CancelButtonList( BOOK* book, GUI* )
{
  BookObj_ReturnPage( book, CANCEL_EVENT );
}



void But_onDelete( BOOK* book, GUI* )
{
  MyBOOK* mbk = (MyBOOK*) book;
  EraseAction(mbk, ListMenu_GetSelectedItem(mbk->but_list));
  CreateButtonList(0, book);
}

int CreateButtonList( void* data, BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  int str_id;
  int but_pos = 0;

  if ( mbk->but_list )
  {
    but_pos = ListMenu_GetSelectedItem( mbk->but_list );
    GUIObject_Destroy( mbk->but_list );
  }

  mbk->but_list = CreateListMenu( book, 0 );
  GUIObject_SetTitleText( mbk->but_list, STR( "Клав. доступа" ) );
  ListMenu_SetItemCount( mbk->but_list, 10 );
  ListMenu_SetOnMessage( mbk->but_list, but_list_callback );
  ListMenu_SetCursorToItem( mbk->but_list, but_pos );
  GUIObject_SoftKeys_SetAction( mbk->but_list, ACTION_BACK, DestroyButList );
  GUIObject_SoftKeys_SetAction( mbk->but_list, ACTION_LONG_BACK, CancelButtonList );
  GUIObject_SoftKeys_SetAction( mbk->but_list, ACTION_SELECT1, But_onEnter );
  GUIObject_SoftKeys_SetAction( mbk->but_list, ACTION_DELETE, But_onDelete );
  GUIObject_SoftKeys_SetVisible( mbk->but_list, ACTION_DELETE, 0 );

  if ( mbk->ActiveTAB == books_tab)
  {
    GUIObject_SoftKeys_SetAction( mbk->but_list, 0, But_SetMM );
    textidname2id( L"SHC_SET_MAINMENU_TXT", TEXTID_ANY_LEN, &str_id );
    GUIObject_SoftKeys_SetText( mbk->but_list, 0, str_id );
    GUIObject_SoftKeys_SetAction( mbk->but_list, 1, But_SetJava );
    textidname2id(L"MENU_JAVA_SETTINGS_TXT",TEXTID_ANY_LEN,&str_id);
    GUIObject_SoftKeys_SetText( mbk->but_list, 1, str_id );
    /*
    GUIObject_SoftKey_SetAction( but_list, 2, But_EditShortcut );
    textidname2id( L"CALE_EDIT_EVENT_TXT", TEXTID_ANY_LEN, &str_id );
    GUIObject_SoftKey_SetText( but_list, 2, str_id );
    */
  }

  GUIObject_Show( mbk->but_list );
  return 0;
}


int ButtonList_Cancel_Event_Action( void* data, BOOK* book )
{
  BookObj_ReturnPage( book, CANCEL_EVENT );
  return(1);
}


const PAGE_MSG ChangeShortcuts_Buttons_PageEvents[]@ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, CreateButtonList,
  ACCEPT_EVENT_TAG, CreateButtonList,
  CANCEL_EVENT_TAG, ButtonList_Cancel_Event_Action,
  PAGE_EXIT_EVENT_TAG, ExitButtonList,
  NIL_EVENT_TAG, 0
};

PAGE_DESC ChangeShortcuts_Buttons_page = { "BookManager_ChangeShortcuts_Buttons_Page", 0, ChangeShortcuts_Buttons_PageEvents };


void ModeList_Cancel_Action( BOOK* book, GUI* )
{
  BookObj_ReturnPage( book, ACCEPT_EVENT );
}


void ModeList_Prev_Action( BOOK* book, GUI* )
{
  BookObj_ReturnPage( book, ACCEPT_EVENT );
}


void onEnter_ModeList( BOOK* book, GUI* )
{
  BookObj_CallPage( book, &ChangeShortcuts_Buttons_page );
}


int list_callback( GUI_MESSAGE* msg )
{
  switch( GUIonMessage_GetMsg( msg ) )
  {
  case LISTMSG_GetItem:
    if ( GUIonMessage_GetCreatedItemIndex( msg ) )
    {
        GUIonMessage_SetMenuItemText(msg,STR("Длинное"));
    }
    else
    {
        GUIonMessage_SetMenuItemText(msg,STR("Короткое"));
    }
  }
  return 1;
}


int CreateModeList( void* data, BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  int mode_list_pos = 0;
  if ( mbk->mode_list )
  {
    mode_list_pos = ListMenu_GetSelectedItem( mbk->mode_list );
    GUIObject_Destroy( mbk->mode_list );
  }
  mbk->mode_list = CreateListMenu( book, 0 );
  GUIObject_SetTitleText(mbk->mode_list,STR("Режим нажатия"));
  ListMenu_SetItemCount( mbk->mode_list, 2 );
  ListMenu_SetOnMessage( mbk->mode_list, list_callback );
  ListMenu_SetCursorToItem( mbk->mode_list, mode_list_pos );
  GUIObject_SoftKeys_SetAction( mbk->mode_list, ACTION_BACK, ModeList_Prev_Action );
  GUIObject_SoftKeys_SetAction( mbk->mode_list, ACTION_LONG_BACK, ModeList_Cancel_Action );
  GUIObject_SoftKeys_SetAction( mbk->mode_list, ACTION_SELECT1, onEnter_ModeList );
  GUIObject_Show( mbk->mode_list );
  return 0;
}


int ModeList_CancelEvent_Action( void* data, BOOK* bk )
{
  BookObj_ReturnPage( bk, ACCEPT_EVENT );
  return(0);
}

int ExitShortcutsSet( void* data, BOOK* bk )
{
  MyBOOK* mbk = (MyBOOK*) bk;
  if ( mbk->mode_list )
  {
    GUIObject_Destroy( mbk->mode_list );
    mbk->mode_list = 0;
  }
  if ( mbk->but_list )
  {
    GUIObject_Destroy( mbk->but_list );
    mbk->but_list = 0;
  }
  if ( mbk->MainMenuID!= - 1 )
  {
    BOOK* MainMenu = FindBookByID( mbk->MainMenuID );
    if ( MainMenu ) FreeBook( MainMenu );
    mbk->MainMenuID = - 1;
  }
  return 0;
}


const PAGE_MSG ChangeShortcuts_PageEvents[]@ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, CreateModeList,
  CANCEL_EVENT_TAG, ModeList_CancelEvent_Action,
  PAGE_EXIT_EVENT_TAG, ExitShortcutsSet,
  NIL_EVENT_TAG, 0
};

PAGE_DESC ChangeShortcuts_page = { "BookManager_ChangeShortcuts_Main_Page", 0, ChangeShortcuts_PageEvents };


void Shortcuts( BOOK* book, GUI* )
{
  MyBOOK * mbk = (MyBOOK*)book;
  mbk->ActiveTAB=GetActiveTab(mbk);
  BookObj_CallPage( book, &ChangeShortcuts_page );
}

void AddShortcut(BOOK* book, GUI* lt)
{
   BookObj_CallPage( book, &SelectShortcut_page );
};

void Yes2DelShortcut(BOOK* book, GUI* lt)
{
  MyBOOK * mbk = (MyBOOK*)book;
  if(mbk->YesNoQuestion) GUIObject_Destroy(mbk->YesNoQuestion);
  mbk->YesNoQuestion=0;
  int item = ListMenu_GetSelectedItem(mbk->slist);
  RemoveAction(mbk, item);
  CreateBookManagerGUI(mbk->ActiveTAB, mbk);
};

void Cancel2DelShortcut(BOOK * book, GUI*)
{
  MyBOOK * mbk = (MyBOOK*)book;
  if (mbk->YesNoQuestion)GUIObject_Destroy(mbk->YesNoQuestion);
  mbk->YesNoQuestion = 0;
}

void DelShortcut(BOOK * book , GUI* lt)
{
  MyBOOK * mbk = (MyBOOK*)book;

  int item = ListMenu_GetSelectedItem(mbk->slist);
  TEXTID DeleteQ;
  textidname2id(L"DELETEQ_TXT",-1,&DeleteQ);

  char param[MAX_SHORTCUTS_ACTION_LEN];
  wstr2strn(param,shortcuts_tab_actions[item],MAX_SHORTCUTS_ACTION_LEN-1);
  TEXTID str_id;
  void * w_buf;
  if (mbk->isA2) w_buf = SHORTCUT_DESC_A2_Init(param);
  else w_buf = SHORTCUT_DESC_Init(param);
  str_id = Shortcut_Get_MenuItemName( w_buf );
  delete( w_buf );
  if ( str_id == EMPTY_TEXTID ) str_id = TextID_Create( param, ENC_LAT1, TEXTID_ANY_LEN );
  int icon_id;
  iconidname2id(L"POPUP_WARNING_ICN",TEXTID_ANY_LEN,&icon_id);

  mbk->YesNoQuestion=CreateYesNoQuestionVA(0,
                              VAR_BOOK(book),
                              VAR_YESNO_PRE_QUESTION(str_id),
                              VAR_YESNO_QUESTION(DeleteQ),
                              0);
  YesNoQuestion_SetIcon(mbk->YesNoQuestion,icon_id);
  GUIObject_SoftKeys_SetAction(mbk->YesNoQuestion,ACTION_YES,Yes2DelShortcut);
  GUIObject_SoftKeys_SetAction(mbk->YesNoQuestion,ACTION_NO,Cancel2DelShortcut);
  GUIObject_SoftKeys_SetAction(mbk->YesNoQuestion,ACTION_BACK,Cancel2DelShortcut);
}
