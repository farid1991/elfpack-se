//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\var_arg.h"
#include "main.h"

wchar_t * GetExtFilter()
{
  wchar_t ** ext_list;
  int count = methods->GetExtList(&ext_list);
  if (!count) return 0;
  int len=0;
  for (int i=0; i<count; i++)
  {
    len+=wstrlen(ext_list[i]);
  }
  wchar_t * ext_filter =  new wchar_t[len+count*3];
  wstrcpy(ext_filter,L"*.");
  wstrcat(ext_filter,ext_list[0]);
  for (int i=1; i<count; i++)
  {
    wstrcat(ext_filter, L";*.");
    wstrcat(ext_filter, ext_list[i]);
  }
  return ext_filter;
}
//Select icon file/////////////////////////////////////////////////////
FILE_DIALOG * FILE_DIALOG_FILES;
int ExtEditor_SelectFile_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  wchar_t * ext_filter = GetExtFilter();
  if (!ext_filter)
  {
    BookObj_GotoPage(book,&ExtEditor_ExtAdd_Page);
    MessageBox(EMPTY_TEXTID, TextID_Create(L"Create an association first...",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
    return 1;
  }
  wchar_t wstr[100];
  FILE_DIALOG_FILES = (FILE_DIALOG *)LoadDLL(L"FileDialogDll.dll");
  if ((INVALID(FILE_DIALOG_FILES)))
  {
    BookObj_ReturnPage(book, PREVIOUS_EVENT);
    snwprintf(wstr,99,L"Error Load FileDialogDll.dll : %d",FILE_DIALOG_FILES);
    MessageBox(EMPTY_TEXTID,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1,0,0);
    return 1;
  }
  wchar_t * path_int = GetUserPath(MEM_INTERNAL);
  wchar_t * path_ext = GetUserPath(MEM_EXTERNAL);
  FILE_DIALOG_Create(FILE_DIALOG_FILES,
                     book,
                     OFD_OPEN_FILE_DIALOG,
                     L"Other",
                     ext_filter,
                     path_int,
                     path_ext,
                     -1);
  delete path_int;
  delete path_ext;
  delete ext_filter;
  return 1;
}

int ExtEditor_SelectFile_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  FILEITEM * fi = (FILEITEM *)mess;
  if (!eebook->ext)
  {
    eebook->ext = new wchar_t[wstrlen(FILEITEM_GetFname(fi))+1];
    wstrcpy(eebook->ext, FILEITEM_GetFname(fi));
    wstrnlwr(eebook->ext, TEXTID_ANY_LEN);
    methods->Ext_Add(eebook->ext);
    BookObj_GotoPage(book,&ExtEditor_ExtSettings_Page);
    return 1;
  }
  wchar_t * e = eebook->ext;
  eebook->ext = new wchar_t[wstrlen(FILEITEM_GetFname(fi))+1];
  wstrcpy(eebook->ext, FILEITEM_GetFname(fi));
  wstrnlwr(eebook->ext, TEXTID_ANY_LEN);
  methods->Ext_Rename(e, eebook->ext);
  delete e;
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int ExtEditor_SelectFile_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}
int ExtEditor_SelectFile_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  UnLoadDLL(FILE_DIALOG_FILES);
  return 1;
}

const PAGE_MSG ExtEditor_SelectIcon_msg[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_SelectFile_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_SelectFile_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_SelectFile_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_SelectFile_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_SelectFile_Page={"ExtEditor_SelectFile_Page",0,ExtEditor_SelectIcon_msg};

//ext input/////////////////////////////////////////////////////////
void ExtAdd_ACTION_BACK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void ExtAdd_ACTION_LONG_BACK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void ExtAdd_ACTION_OK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->ext)
  {
    wchar_t * ext_old = eebook->ext;
    eebook->ext = new wchar_t[wstrlen(string)+1];
    wstrcpy(eebook->ext, string);
    wstrnlwr(eebook->ext, TEXTID_ANY_LEN);
    methods->Ext_Rename(ext_old, eebook->ext);
    delete ext_old;
    BookObj_ReturnPage(book,ACCEPT_EVENT);
  }
  else
  {
    eebook->ext = new wchar_t[wstrlen(string)+1];
    wstrcpy(eebook->ext, string);
    wstrnlwr(eebook->ext, TEXTID_ANY_LEN);
    methods->Ext_Add(eebook->ext);
    BookObj_GotoPage(book,&ExtEditor_ExtSettings_Page);
  }
}

int ExtEditor_ExtAdd_PAGE_ENTER_EVENT(void *, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  eebook->input = CreateStringInputVA(0,
                                           VAR_HEADER_TEXT(STR("Extension")),
                                           VAR_STRINP_MAX_LEN(12),
                                           VAR_STRINP_MODE(1),
                                           VAR_BOOK(eebook),
                                           VAR_STRINP_ENABLE_EMPTY_STR(0),
                                           VAR_STRINP_TEXT(TextID_Create(eebook->ext,ENC_UCS2,TEXTID_ANY_LEN)),
                                           //VAR_STRINP_TEXT(eebook->ext),
                                           VAR_PREV_ACTION_PROC(ExtAdd_ACTION_BACK),
                                           VAR_LONG_BACK_PROC(ExtAdd_ACTION_LONG_BACK),
                                           VAR_OK_PROC(ExtAdd_ACTION_OK),
                                           0);
  return 1;
}

int ExtEditor_ExtAdd_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->input)
  {
    GUIObject_Destroy(eebook->input);
    eebook->input=0;
  }
  return 1;
}

const PAGE_MSG ExtEditor_ExtAdd_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtAdd_PAGE_ENTER_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtAdd_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtAdd_Page={"ExtEditor_ExtAdd_Page",0,ExtEditor_ExtAdd_EventsList};
//Select ext or file/////////////////////////////////////////////////////

void SelectType_ACTION_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void SelectType_ACTION_LONG_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void SelectType_ACTION_SELECT1(BOOK * book, GUI * v)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int index=ListMenu_GetSelectedItem(eebook->icontype);
  switch(index)
  {
  case 0:
    BookObj_GotoPage(book,&ExtEditor_ExtAdd_Page);
    break;
  case 1:
    BookObj_GotoPage(book,&ExtEditor_SelectFile_Page);
    break;
  }
}

int SelectType_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    switch(index)
    {
    case 0:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Extension",ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Adding new extension",ENC_UCS2,TEXTID_ANY_LEN));
      break;
    case 1:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"File",ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Extension of chosing file must exist",ENC_UCS2,TEXTID_ANY_LEN));
      break;
    }
  }
  return(1);
}

int ExtEditor_ExtAdd_SelectType_PAGE_ENTER_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int SelectedItem = 0;
  GUI_LIST * icontype = eebook->icontype;
  if (icontype)
  {
    SelectedItem=ListMenu_GetSelectedItem(icontype);
    GUIObject_Destroy(icontype);
  }
  eebook->icontype = icontype = CreateListMenu(book,0);
  GUIObject_SetTitleText(icontype,TextID_Create(L"Ext or file",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(icontype, 2);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)icontype, SelectType_onGuiMessage);
  ListMenu_SetCursorToItem(icontype, SelectedItem);
  GUIObject_SoftKeys_SetAction(icontype,ACTION_BACK, SelectType_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(icontype,ACTION_LONG_BACK, SelectType_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(icontype,ACTION_SELECT1, SelectType_ACTION_SELECT1);
  GUIObject_Show(icontype);
  return 1;
}

int ExtEditor_ExtAdd_SelectType_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int ExtEditor_ExtAdd_SelectType_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int ExtEditor_ExtAdd_SelectType_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  return 1;
}

int ExtEditor_ExtAdd_SelectType_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->icontype)
  {
    GUIObject_Destroy(eebook->icontype);
    eebook->icontype=0;
  }
  return 1;
}

const PAGE_MSG ExtEditor_ExtAdd_SelectType_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtAdd_SelectType_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtAdd_SelectType_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtAdd_SelectType_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    ExtEditor_ExtAdd_SelectType_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtAdd_SelectType_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtAdd_SelectType_Page={"ExtEditor_ExtAdd_SelectType_Page",0,ExtEditor_ExtAdd_SelectType_EventsList};
