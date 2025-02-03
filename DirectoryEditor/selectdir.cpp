//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\dll.h"
//#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "main.h"


FILE_DIALOG * FILE_DIALOG_FOLDER;

int DirEditor_DirAdd_SelectDir_PAGE_ENTER_EVENT(void * r0, BOOK * book)
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
  FILE_DIALOG_Create(FILE_DIALOG_FOLDER,
                     book,
                     OFD_OPEN_FOLDER_DIALOG,
                     L"user",
                     L"*.elf",
                     GetDir(DIR_USER|MEM_EXTERNAL),
                     GetDir(DIR_USER|MEM_INTERNAL),
                     -1);
  return 1;
}

int DirEditor_DirAdd_SelectDir_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION

  DEBOOK * debook=(DEBOOK*)book;
  FILEITEM * fi = (FILEITEM *)mess;
  wchar_t * fullname = MakeFullName(FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));
  wchar_t * shortname = methods->Dir_GetShortPath(fullname);
  wstrnlwr(shortname, TEXTID_ANY_LEN);
  delete fullname;
  if (!debook->dir)
  {
    debook->dir = shortname;
    methods->Dir_Add(debook->dir);
    BookObj_GotoPage(book,&DirEditor_DirSettings_Page);
    return 1;
  }
  else
  {
    methods->Dir_Rename(debook->dir, shortname);
    delete debook->dir;
    debook->dir = shortname;
    BookObj_ReturnPage(book,ACCEPT_EVENT);
  }
  return 1;
}

int DirEditor_DirAdd_SelectDir_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}

int DirEditor_DirAdd_SelectDir_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  UnLoadDLL(FILE_DIALOG_FOLDER);
  return 1;
}

const PAGE_MSG DirEditor_DirAdd_SelectDir_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirAdd_SelectDir_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirAdd_SelectDir_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirAdd_SelectDir_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirAdd_SelectDir_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirAdd_SelectDir_Page={"DirEditor_DirAdd_SelectDir_Page",0,DirEditor_DirAdd_SelectDir_EventsList};

//Select memory/////////////////////////////////////////////////////
/*
void SelectMemory_ACTION_BACK(BOOK * book, void * v)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void SelectMemory_ACTION_LONG_BACK(BOOK * book, void * v)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void SelectMemory_ACTION_SELECT1(BOOK * book, void * v)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  debook->mem = ListMenu_GetSelectedItem(debook->gui_memory);
  BookObj_CallPage(book,&DirEditor_DirAdd_SelectDir_Page);
}

int SelectMemory_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  switch(msg->msg)
  {
  case 1:
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    switch(index)
    {
    case MEM_INTERNAL:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Phone memory",ENC_UCS2,TEXTID_ANY_LEN));
      break;
    case MEM_EXTERNAL:
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Memory stick",ENC_UCS2,TEXTID_ANY_LEN));
      break;
    }
  }
  return(1);
}

int DirEditor_DirAdd_SelectMemory_PAGE_ENTER_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (!ExtMemExist())
  {
    debook->mem = MEM_INTERNAL;
    BookObj_CallPage(book,&DirEditor_DirAdd_SelectDir_Page);
    return 1;
  }
  int SelectedItem = 0;
  GUI_LIST * gui_memory = debook->gui_memory;
  if (gui_memory)
  {
    SelectedItem=ListMenu_GetSelectedItem(gui_memory);
    GUIObject_Destroy(gui_memory);
  }
  debook->gui_memory = gui_memory = CreateListMenu(book,0);
  GUIObject_SetTitleText(gui_memory,TextID_Create(L"Select memory",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(gui_memory, 2);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)gui_memory, SelectMemory_onGuiMessage);
  ListMenu_SetCursorToItem(gui_memory, SelectedItem);
  GUIObject_SoftKeys_SetAction(gui_memory,ACTION_BACK, SelectMemory_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(gui_memory,ACTION_LONG_BACK, SelectMemory_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(gui_memory,ACTION_SELECT1, SelectMemory_ACTION_SELECT1);
  GUIObject_Show(gui_memory);
  return 1;
}

int DirEditor_DirAdd_SelectMemory_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int DirEditor_DirAdd_SelectMemory_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int DirEditor_DirAdd_SelectMemory_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  if (!ExtMemExist())
  {
    BookObj_ReturnPage(book,PREVIOUS_EVENT);
  }
  return 1;
}

int DirEditor_DirAdd_SelectMemory_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_memory)
  {
    GUIObject_Destroy(debook->gui_memory);
    debook->gui_memory=0;
  }
  return 1;
}

const PAGE_MSG DirEditor_DirAdd_SelectMemory_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirAdd_SelectMemory_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirAdd_SelectMemory_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirAdd_SelectMemory_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    DirEditor_DirAdd_SelectMemory_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirAdd_SelectMemory_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirAdd_SelectMemory_Page={"DirEditor_DirAdd_SelectMemory_Page",0,DirEditor_DirAdd_SelectMemory_EventsList};
*/
