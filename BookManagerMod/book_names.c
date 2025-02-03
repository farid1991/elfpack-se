#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "shortcuts.h"
#include "main.h"

void CreateEditorMenu(BOOK*book,int x);

char * GetCurrentName(MyBOOK * mbk)
{
  BOOK_LIST_ITEM* elem = GetBookListItem(mbk,mbk->ActiveTAB);
  return(elem->book_name);
}

void onCancel_InputName(BOOK* book, void* )
{
  MyBOOK* mbk = (MyBOOK*)book;
  GUIObject_Destroy(mbk->StringInput);
  mbk->StringInput=0;
  if(mbk->GUIEditor) GUIObject_Destroy(mbk->GUIEditor);
  mbk->GUIEditor=0;
  CreateEditorMenu(book,0);
}

void onAccept_InputName( BOOK* book, wchar_t* string, int len )
{
  MyBOOK* mbk = (MyBOOK*)book;
  wstrcpy(mbk->BPEditor.name,string);
  GUIObject_Destroy(mbk->StringInput);
  mbk->StringInput=0;
  if(mbk->GUIEditor) GUIObject_Destroy(mbk->GUIEditor);
  mbk->GUIEditor=0;
  CreateEditorMenu(book,0);
}
void onCancel_InputIcon(BOOK* book, void* )
{
  MyBOOK* mbk = (MyBOOK*)book;
  GUIObject_Destroy(mbk->StringInput);
  mbk->StringInput=0;
  if(mbk->GUIEditor) GUIObject_Destroy(mbk->GUIEditor);
  mbk->GUIEditor=0;
  CreateEditorMenu(book,1);
}

void onAccept_InputIcon( BOOK* book, wchar_t* string, int len )
{
  MyBOOK* mbk = (MyBOOK*)book;
  wstrcpy(mbk->BPEditor.i_name,string);
  GUIObject_Destroy(mbk->StringInput);
  mbk->StringInput=0;
  if(mbk->GUIEditor) GUIObject_Destroy(mbk->GUIEditor);
  mbk->GUIEditor=0;
  CreateEditorMenu(book,1);
}

int CreateInputName(BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  int editable_strID = TextID_Create(mbk->BPEditor.name,ENC_UCS2,TEXTID_ANY_LEN);
  mbk->StringInput = CreateStringInputVA( 0,
                                VAR_PREV_ACTION_PROC( onCancel_InputName ),
                                VAR_LONG_BACK_PROC( onCancel_InputName ),
                                VAR_BOOK( book ),
                                VAR_OK_PROC( onAccept_InputName ),
                                VAR_STRINP_TEXT( editable_strID ),
                                VAR_STRINP_MIN_LEN( 0 ),
                                VAR_STRINP_MAX_LEN( 63 ),
                                0 );
  return 0;
}

int CreateInputIcon(BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  int editable_strID = TextID_Create(mbk->BPEditor.i_name,ENC_UCS2,TEXTID_ANY_LEN);
  mbk->StringInput = CreateStringInputVA( 0,
                                VAR_PREV_ACTION_PROC( onCancel_InputIcon ),
                                VAR_LONG_BACK_PROC( onCancel_InputIcon ),
                                VAR_BOOK( book ),
                                VAR_OK_PROC( onAccept_InputIcon ),
                                VAR_STRINP_TEXT( editable_strID ),
                                VAR_STRINP_MIN_LEN( 0 ),
                                VAR_STRINP_MAX_LEN( 63 ),
                                0 );
  return 0;
}

int ExitChangeName( void* data, BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  if (mbk->GUIEditor)
  {
    GUIObject_Destroy(mbk->GUIEditor);
    mbk->GUIEditor=0;
  }
  return 0;
}

void CloseEditor(BOOK*book,GUI*)
{
  BookObj_ReturnPage( book, ACCEPT_EVENT );
}


void onSave(BOOK*book,GUI*)
{
  MyBOOK* mbk=(MyBOOK*)book;
  WritePARAM(&(mbk->BPEditor),mbk->NEditor);
  GUIObject_Destroy(mbk->blist);
  GUIObject_Destroy(mbk->elist);
  mbk->blist=0;
  mbk->elist=0;
  BookObj_ReturnPage( book, ACCEPT_EVENT );
}

int SetEditorText( GUI_MESSAGE* msg )
{

  MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );
  int item = GUIonMessage_GetCreatedItemIndex( msg );;
  int icn=0;
  int str_id;
  switch(item)
  {
  case 0:
    textidname2id(L"DB_NAMEP_TXT",-1,&str_id);
    GUIonMessage_SetMenuItemText( msg, str_id);
    GUIonMessage_SetMenuItemSecondLineText( msg, TextID_Create(mbk->BPEditor.name,ENC_UCS2,TEXTID_ANY_LEN));
    iconidname2id(L"DB_LIST_OTHER_ICN",-1,&icn);
    GUIonMessage_SetMenuItemIcon( msg, 0, icn);
    break;
  case 1:
    int str[2];
    textidname2id(L"MSG_UI_PICTURE_TXT",-1,&str[0]);
    str[1]=TextID_Create(L":",ENC_UCS2,1);
    str_id=TextID_Create(str,ENC_TEXTID,2);
    GUIonMessage_SetMenuItemText( msg, str_id);
    GUIonMessage_SetMenuItemSecondLineText( msg, TextID_Create(mbk->BPEditor.i_name,ENC_UCS2,TEXTID_ANY_LEN));
    GetIconByName(mbk->BPEditor.i_name,icn);
    if(icn<=0 || icn==0xFFFF) iconidname2id(L"HPB_LIST_SIM_CATEGORY_ICN",-1,&icn);
      GUIonMessage_SetMenuItemIcon( msg, 0, icn);
    break;

  }
  return 1;
}

void onEnterEditor(BOOK * book, GUI* lt)
{
  MyBOOK* mbk=(MyBOOK*)book;
  int item=ListMenu_GetSelectedItem(mbk->GUIEditor);
  if(item==0) CreateInputName(book);
  if(item==1) CreateInputIcon(book);
}


void CreateEditorMenu(BOOK* book,int x)
{
  MyBOOK*mbk=(MyBOOK*)book;
  if(mbk->GUIEditor) GUIObject_Destroy(mbk->GUIEditor);
  mbk->GUIEditor=CreateListMenu( book, 0 );
  ListMenu_SetItemStyle(mbk->GUIEditor, 3);
  ListMenu_SetOnMessage( mbk->GUIEditor, SetEditorText );
  ListMenu_SetItemCount( mbk->GUIEditor, 2 );
  ListMenu_SetCursorToItem(mbk->GUIEditor,x);
  GUIObject_SetTitleText(mbk->GUIEditor,TextID_Create(mbk->BPEditor.book,ENC_LAT1,strlen(mbk->BPEditor.book)));
  GUIObject_SoftKeys_SetAction( mbk->GUIEditor, ACTION_BACK, CloseEditor );
  GUIObject_SoftKeys_SetAction( mbk->GUIEditor, ACTION_LONG_BACK, CloseEditor );
  GUIObject_SoftKeys_SetAction( mbk->GUIEditor, ACTION_SELECT1, onEnterEditor );
  GUIObject_SoftKeys_SetAction( mbk->GUIEditor, 0, onSave);
  int str_id;
  textidname2id(L"GUI_EDIT_SK",-1,&str_id);
  GUIObject_SoftKeys_SetText( mbk->GUIEditor, ACTION_SELECT1, str_id);
  textidname2id(L"DB_SAVE_TXT",-1,&str_id);
  GUIObject_SoftKeys_SetText( mbk->GUIEditor, 0, str_id);
  GUIObject_Show(mbk->GUIEditor);
}

int CreateSI(void*,BOOK* book)
{
  MyBOOK* mbk=(MyBOOK*)book;
  if(mbk->GUIEditor)
  {
    GUIObject_Destroy(mbk->GUIEditor);
    mbk->GUIEditor=0;
  }
  mbk->NEditor=GetPARAM(GetCurrentName(mbk),&(mbk->BPEditor));
  if(mbk->NEditor==-1)
  {
    strcpy(mbk->BPEditor.book,GetCurrentName(mbk));
    win12512unicode(mbk->BPEditor.name,mbk->BPEditor.book,strlen(mbk->BPEditor.book));
    wstrcpy(mbk->BPEditor.i_name,L"");
  }
  CreateEditorMenu(book,0);
  return 0;
}



const PAGE_MSG ChangeName_PageEvents[]@ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, CreateSI,
  PAGE_EXIT_EVENT_TAG, ExitChangeName,
  NIL_EVENT_TAG, 0
};

PAGE_DESC ChangeName_page = { "BookManager_ChangeName_Page", 0, ChangeName_PageEvents };


void BookNames( BOOK* book, GUI* )
{
  MyBOOK * mbk = (MyBOOK*)book;

  mbk->ActiveTAB=GetActiveTab(mbk);
  BookObj_CallPage( book, &ChangeName_page );
}
