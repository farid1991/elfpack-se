//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\dll.h"
//#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
//#include "..\\include\var_arg.h"
#include "main.h"

#define DLL_DATA FILE_DIALOG
DLL_DATA * FILE_DIALOG_THIS;

//Select icon file/////////////////////////////////////////////////////

int ExtEditor_ExtSettings_SelectIconFile_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  wchar_t wstr[100];
  FILE_DIALOG_THIS = (DLL_DATA *)LoadDLL(L"FileDialogDll.dll");
  if ((INVALID(FILE_DIALOG_THIS)))
  {
    BookObj_ReturnPage(book, PREVIOUS_EVENT);
    snwprintf(wstr,99,L"Error Load FileDialogDll.dll : %d",FILE_DIALOG_THIS);
    MessageBox(EMPTY_TEXTID,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1,0,0);
    return 1;
  }
  FILE_DIALOG_Create(FILE_DIALOG_THIS,
                     book,
                     OFD_OPEN_FILE_DIALOG,
                     L"Config",
                     L"*.gif;*.jpg;*.jpeg;*.png;*.bmp;*.wbmp;*.svg;*.svgz;*.mng;*.dcf;*.dm;",
                     DIR_ELFS_CONFIG|MEM_INTERNAL,
                     DIR_ELFS_CONFIG|MEM_EXTERNAL,
                     -1);
  return 1;
}

int ExtEditor_ExtSettings_SelectIconFile_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  FILEITEM * fi = (FILEITEM *)mess;
  eebook->Ext_SetIconFile(eebook->ext, FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectIconFile_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}
int ExtEditor_ExtSettings_SelectIconFile_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  UnLoadDLL(FILE_DIALOG_THIS);
  return 1;
}

const PAGE_MSG ExtEditor_SelectIcon_msg[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_SelectIconFile_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtSettings_SelectIconFile_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtSettings_SelectIconFile_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_SelectIconFile_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_SelectIconFile_Page={"ExtEditor_ExtSettings_SelectIconFile_Page",0,ExtEditor_SelectIcon_msg};

//IconID input/////////////////////////////////////////////////////////

void InputIconID_ACTION_BACK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void InputIconID_ACTION_LONG_BACK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

char h2d(wchar_t c)
{FUNCTION
  if (c>='0' && c<='9') return c-'0';
  if (c>='A' && c<='F') return c-'A'+10;
  if (c>='a' && c<='f') return c-'a'+10;
  return 0;        // not Hex digit
}

int Hex2Dec(wchar_t * hex, int l)
{FUNCTION
  if (*hex==0) return(l);
  return Hex2Dec(hex+1, l*16+h2d(*hex)); // hex+1?
}

int h2i(wchar_t * w)
{FUNCTION
  return Hex2Dec(w, 0);
}

void InputIconID_ACTION_OK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int i = (len)?h2i(string):NOIMAGE;
  eebook->Ext_SetIconId(eebook->ext, i);
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}

int ExtEditor_ExtSettings_InputIconID_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int index=ListMenu_GetSelectedItem(eebook->extension);
  wchar_t iconid, *path, *name;
  int i = (eebook->Ext_GetIcon(eebook->ext, &iconid, &path, &name)==1)?iconid:0;
  wchar_t w[5];
  if (i == NOIMAGE) i = 0;
  if (i)
  {
    snwprintf(w,4,L"%X",i);
    i = TextID_Create(w,ENC_UCS2,TEXTID_ANY_LEN);
  }
  eebook->input = CreateStringInputVA(0,
                                           VAR_HEADER_TEXT(TextID_Create(ItemName[index],ENC_UCS2,TEXTID_ANY_LEN)),
                                           //VAR_STRINP_MIN_LEN(4),
                                           VAR_STRINP_MAX_LEN(4),
                                           VAR_STRINP_MODE(7),
                                           VAR_BOOK(eebook),
                                           //VAR_STRINP_ENABLE_EMPTY_STR(0),
                                           VAR_STRINP_TEXT(i),
                                           VAR_PREV_ACTION_PROC(InputIconID_ACTION_BACK),
                                           VAR_LONG_BACK_PROC(InputIconID_ACTION_LONG_BACK),
                                           VAR_OK_PROC(InputIconID_ACTION_OK),
                                           0);
  return 1;
}

int ExtEditor_ExtSettings_InputIconID_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->input)
  {
    GUIObject_Destroy(eebook->input);
    eebook->input=0;
  }
  return 1;
}

const PAGE_MSG ExtEditor_ExtSettings_InputIconID_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_InputIconID_PAGE_ENTER_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_InputIconID_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_InputIconID_Page={"ExtEditor_ExtSettings_InputIconID_Page",0,ExtEditor_ExtSettings_InputIconID_EventsList};

//Select icon type/////////////////////////////////////////////////////

void SelectIconType_ACTION_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void SelectIconType_ACTION_LONG_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void SelectIconType_ACTION_SELECT1(BOOK * book, GUI * v)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int index=ListMenu_GetSelectedItem(eebook->icontype);
  switch(index)
  {
  case 0:
    BookObj_CallPage(book,&ExtEditor_ExtSettings_InputIconID_Page);
    break;
  case 1:
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectIconFile_Page);
    break;
  }
}

int SelectIconType_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    switch(index)
    {
    case 0:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Internal (iconID)",ENC_UCS2,TEXTID_ANY_LEN));
      break;
    case 1:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"External (file)",ENC_UCS2,TEXTID_ANY_LEN));
      break;
    }
  }
  return(1);
}

int ExtEditor_ExtSettings_SelectIconType_PAGE_ENTER_EVENT(void *mess ,BOOK *book)
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
  GUIObject_SetTitleText(icontype,TextID_Create(L"Icon type",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(icontype, 2);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)icontype, SelectIconType_onGuiMessage);
  ListMenu_SetCursorToItem(icontype, SelectedItem);
  GUIObject_SoftKeys_SetAction(icontype,ACTION_BACK, SelectIconType_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(icontype,ACTION_LONG_BACK, SelectIconType_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(icontype,ACTION_SELECT1, SelectIconType_ACTION_SELECT1);
  GUIObject_Show(icontype);
  return 1;
}

int ExtEditor_ExtSettings_SelectIconType_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectIconType_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectIconType_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  return 1;
}

int ExtEditor_ExtSettings_SelectIconType_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->icontype)
  {
    GUIObject_Destroy(eebook->icontype);
    eebook->icontype=0;
  }
  return 1;
}

const PAGE_MSG ExtEditor_ExtSettings_SelectIconType_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_SelectIconType_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtSettings_SelectIconType_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtSettings_SelectIconType_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    ExtEditor_ExtSettings_SelectIconType_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_SelectIconType_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_SelectIconType_Page={"ExtEditor_ExtSettings_SelectIconType_Page",0,ExtEditor_ExtSettings_SelectIconType_EventsList};
