//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
#include "main.h"

wchar_t * MakeFullName(wchar_t * path, wchar_t * name)
{FUNCTION
  int len = wstrlen(path)+wstrlen(name)+1;
  wchar_t * fullname = new wchar_t[len+1];
  wstrcpy(fullname, path);
  wstrcat(fullname, L"/");
  wstrcat(fullname, name);
  return fullname;
}

EEBOOK * ExtEditor_Book;

int Terminate(void * r0, BOOK * book)
{FUNCTION
  FreeBook(book);
  return 1;
}

int ShowInfo(void * r0, BOOK * book)
{FUNCTION
  MessageBox(EMPTY_TEXTID,STR("DBExtReg\n(c)Slawwan\nпечаль..."),NOIMAGE,1,0,0);
  return 1;
}


const PAGE_MSG ExtEditor_Base_EventsList[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, Terminate,
  ELF_SHOW_INFO_EVENT, ShowInfo,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_Base_Page={"ExtEditor_Base_Page",0,ExtEditor_Base_EventsList};

void onExtEditorBookClose(BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->input) GUIObject_Destroy(eebook->input);
  if (eebook->extlist) GUIObject_Destroy(eebook->extlist);
  if (eebook->extension) GUIObject_Destroy(eebook->extension);
  SUBPROC(elf_exit);
}

int isEEBook(BOOK * book)
{
  if (!strcmp(book->xbook->name, "ExtEditor")) return 1;
  return 0;
}

int CreateEditor()
{FUNCTION
  BOOK * book = FindBook(isEEBook);
  if (book)
  {
    BookObj_SetFocus(book, 0);
    return 0;
  }
  EEBOOK * ExtEditor_Book = new EEBOOK;
  memset(ExtEditor_Book,0,sizeof(EEBOOK));
  CreateBook((BOOK*)ExtEditor_Book,onExtEditorBookClose,&ExtEditor_Base_Page,"ExtEditor",-1,0);
  ExtEditor_Book->extension_methods = methods;
  ExtEditor_Book->ext_list = 0;
  ExtEditor_Book->extlist = 0;
  ExtEditor_Book->extension = 0;
  ExtEditor_Book->input = 0;
  BookObj_GotoPage((BOOK*)ExtEditor_Book,&ExtEditor_ExtList_Page);
  return 0;
}
