//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\dll.h"
//#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "main.h"

wchar_t * GetUserPath(int mem)
{
  wchar_t * path_other = GetDir(DIR_OTHER|mem);
  int len = wstrwstr(path_other, L"/other") - path_other;
  wchar_t * path_user = new wchar_t[len+1];
  wstrncpy(path_user, path_other, len);
  path_user[len] = 0;
  return path_user;
}
FILE_DIALOG * FILE_DIALOG_FOLDER;

int ExtEditor_ExtSettings_SelectFolder_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  wchar_t wstr[100];
  FILE_DIALOG_FOLDER = (FILE_DIALOG *)LoadDLL(L"FileDialogDll.dll");
  if ((INVALID(FILE_DIALOG_FOLDER)))
  {
    BookObj_ReturnPage(book, PREVIOUS_EVENT);
    snwprintf(wstr,99,L"Error Load FileDialogDll.dll : %d",FILE_DIALOG_FOLDER);
    MessageBox(EMPTY_TEXTID,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1,0,0);
    return 1;
  }
  EEBOOK * eebook=(EEBOOK*)book;
  wchar_t * path = GetUserPath(eebook->path);
  FILE_DIALOG_Create(FILE_DIALOG_FOLDER,
                     book,
                     OFD_OPEN_FOLDER_DIALOG,
                     L"user",
                     L"*.elf",
                     path,
                     -1);
  delete path;
  return 1;
}

int ExtEditor_ExtSettings_SelectFolder_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  FILEITEM * fi = (FILEITEM *)mess;
  wchar_t * name = MakeFullName(FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));
  eebook->Ext_SetPath(eebook->ext, name);
  delete name;
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectFolder_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_SelectFolder_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  UnLoadDLL(FILE_DIALOG_FOLDER);
  return 1;
}

const PAGE_MSG ExtEditor_ExtSettings_SelectFolder_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_SelectFolder_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtSettings_SelectFolder_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtSettings_SelectFolder_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_SelectFolder_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_SelectFolder_Page={"ExtEditor_ExtSettings_SelectFolder_Page",0,ExtEditor_ExtSettings_SelectFolder_EventsList};
