//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\dll.h"
//#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "main.h"


FILE_DIALOG * FILE_DIALOG_ELF;

int ExtEditor_ExtSettings_SelectElf_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  wchar_t wstr[100];
  FILE_DIALOG_ELF = (FILE_DIALOG *)LoadDLL(L"FileDialogDll.dll");
  if ((INVALID(FILE_DIALOG_ELF)))
  {
    BookObj_ReturnPage(book, PREVIOUS_EVENT);
    snwprintf(wstr,99,L"Error Load FileDialogDll.dll : %d",FILE_DIALOG_ELF);
    MessageBox(EMPTY_TEXTID,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1,0,0);
    return 1;
  }
  FILE_DIALOG_Create(FILE_DIALOG_ELF,
                     book,
                     OFD_SHOW_FILES|OFD_SHOW_FOLDERS|OFD_ACT_INFO_ENABLE,
                     L"Elfs",
                     L"*.elf",
                     DIR_ELFS|MEM_INTERNAL,
                     DIR_ELFS|MEM_EXTERNAL,
                     -1);
  return 1;
}

int ExtEditor_ExtSettings_SelectElf_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  FILEITEM * fi = (FILEITEM *)mess;
  wchar_t * name = MakeFullName(FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));
  eebook->Ext_SetElf(eebook->ext, name);
  delete name;
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectElf_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectElf_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  UnLoadDLL(FILE_DIALOG_ELF);
  return 1;
}

const PAGE_MSG ExtEditor_ExtSettings_SelectElf_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_SelectElf_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtSettings_SelectElf_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtSettings_SelectElf_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_SelectElf_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_SelectElf_Page={"ExtEditor_ExtSettings_SelectElf_Page",0,ExtEditor_ExtSettings_SelectElf_EventsList};
