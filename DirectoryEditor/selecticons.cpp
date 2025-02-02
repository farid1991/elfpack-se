//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\dll.h"
//#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
//#include "..\\include\var_arg.h"
#include "main.h"

#define DLL_DATA FILE_DIALOG
DLL_DATA * FILE_DIALOG_THIS;

//Select icon file/////////////////////////////////////////////////////

int DirEditor_DirSettings_SelectIconFile_PAGE_ENTER_EVENT(void * r0, BOOK * book)
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

int DirEditor_DirSettings_SelectIconFile_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  FILEITEM * fi = (FILEITEM *)mess;
  debook->Dir_SetIconFile(debook->dir, FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int DirEditor_DirSettings_SelectIconFile_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}
int DirEditor_DirSettings_SelectIconFile_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  UnLoadDLL(FILE_DIALOG_THIS);
  return 1;
}

const PAGE_MSG DirEditor_SelectIcon_msg[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirSettings_SelectIconFile_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirSettings_SelectIconFile_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirSettings_SelectIconFile_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirSettings_SelectIconFile_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirSettings_SelectIconFile_Page={"DirEditor_DirSettings_SelectIconFile_Page",0,DirEditor_SelectIcon_msg};

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
  DEBOOK * debook=(DEBOOK*)book;
  int i = (len)?h2i(string):NOIMAGE;
  debook->Dir_SetIconId(debook->dir, i);
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}

int DirEditor_DirSettings_InputIconID_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int index=ListMenu_GetSelectedItem(debook->gui_dirsettings);
  wchar_t iconid, *path, *name;
  int i = (debook->Dir_GetIcon(debook->dir, &iconid, &path, &name)==1)?iconid:0;
  wchar_t w[5];
  if (i == NOIMAGE) i = 0;
  if (i)
  {
    snwprintf(w,4,L"%X",i);
    i = TextID_Create(w,ENC_UCS2,TEXTID_ANY_LEN);
  }
  debook->gui_input = CreateStringInputVA(0,
                                           VAR_HEADER_TEXT(TextID_Create(ItemName[index],ENC_UCS2,TEXTID_ANY_LEN)),
                                           //VAR_STRINP_MIN_LEN(1),
                                           VAR_STRINP_MAX_LEN(4),
                                           VAR_STRINP_MODE(7),
                                           VAR_BOOK(debook),
                                           //VAR_STRINP_ENABLE_EMPTY_STR(0),
                                           VAR_STRINP_TEXT(i),
                                           VAR_PREV_ACTION_PROC(InputIconID_ACTION_BACK),
                                           VAR_LONG_BACK_PROC(InputIconID_ACTION_LONG_BACK),
                                           VAR_OK_PROC(InputIconID_ACTION_OK),
                                           0);
  return 1;
}

int DirEditor_DirSettings_InputIconID_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_input)
  {
    GUIObject_Destroy(debook->gui_input);
    debook->gui_input=0;
  }
  return 1;
}

const PAGE_MSG DirEditor_DirSettings_InputIconID_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirSettings_InputIconID_PAGE_ENTER_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirSettings_InputIconID_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirSettings_InputIconID_Page={"DirEditor_DirSettings_InputIconID_Page",0,DirEditor_DirSettings_InputIconID_EventsList};

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
  DEBOOK * debook=(DEBOOK*)book;
  int index=ListMenu_GetSelectedItem(debook->gui_icontype);
  switch(index)
  {
  case 0:
    BookObj_CallPage(book,&DirEditor_DirSettings_InputIconID_Page);
    break;
  case 1:
    BookObj_CallPage(book,&DirEditor_DirSettings_SelectIconFile_Page);
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

int DirEditor_DirSettings_SelectIconType_PAGE_ENTER_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int SelectedItem = 0;
  GUI_LIST * gui_icontype = debook->gui_icontype;
  if (gui_icontype)
  {
    SelectedItem=ListMenu_GetSelectedItem(gui_icontype);
    GUIObject_Destroy(gui_icontype);
  }
  debook->gui_icontype = gui_icontype = CreateListMenu(book,0);
  GUIObject_SetTitleText(gui_icontype,TextID_Create(L"Icon type",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(gui_icontype, 2);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)gui_icontype, SelectIconType_onGuiMessage);
  ListMenu_SetCursorToItem(gui_icontype, SelectedItem);
  GUIObject_SoftKeys_SetAction(gui_icontype,ACTION_BACK, SelectIconType_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(gui_icontype,ACTION_LONG_BACK, SelectIconType_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(gui_icontype,ACTION_SELECT1, SelectIconType_ACTION_SELECT1);
  GUIObject_Show(gui_icontype);
  return 1;
}

int DirEditor_DirSettings_SelectIconType_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int DirEditor_DirSettings_SelectIconType_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int DirEditor_DirSettings_SelectIconType_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  return 1;
}

int DirEditor_DirSettings_SelectIconType_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_icontype)
  {
    GUIObject_Destroy(debook->gui_icontype);
    debook->gui_icontype=0;
  }
  return 1;
}

const PAGE_MSG DirEditor_DirSettings_SelectIconType_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirSettings_SelectIconType_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirSettings_SelectIconType_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirSettings_SelectIconType_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    DirEditor_DirSettings_SelectIconType_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirSettings_SelectIconType_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirSettings_SelectIconType_Page={"DirEditor_DirSettings_SelectIconType_Page",0,DirEditor_DirSettings_SelectIconType_EventsList};
