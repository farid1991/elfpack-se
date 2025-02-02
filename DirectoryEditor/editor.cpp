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

DEBOOK * DirEditor_Book;

int Terminate(void * r0, BOOK * book)
{FUNCTION
  FreeBook(book);
  return 1;
}

int ShowInfo(void * r0, BOOK * book)
{FUNCTION
  MessageBox(EMPTY_TEXTID,STR("diredit\n(c)Slawwan\nïè÷àààààààëü..."),NOIMAGE,1,0,0);
  return 1;
}


const PAGE_MSG DirEditor_Base_EventsList[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, Terminate,
  ELF_SHOW_INFO_EVENT, ShowInfo,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_Base_Page={"DirEditor_Base_Page",0,DirEditor_Base_EventsList};

void onDirEditorBookClose(BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_input) GUIObject_Destroy(debook->gui_input);
  if (debook->gui_dirlist) GUIObject_Destroy(debook->gui_dirlist);
  if (debook->gui_dirsettings) GUIObject_Destroy(debook->gui_dirsettings);
  if (debook->gui_actionstate) GUIObject_Destroy(debook->gui_actionstate);
  SUBPROC(elf_exit);
}

int isDEBook(BOOK * book)
{
  if (!strcmp(book->xbook->name, "DirEditor")) return 1;
  return 0;
}

int CreateEditor()
{FUNCTION
  BOOK * book = FindBook(isDEBook);
  if (book)
  {
    BookObj_SetFocus(book, 0);
    return 0;
  }
  DEBOOK * DirEditor_Book = new DEBOOK;
  memset(DirEditor_Book,0,sizeof(DEBOOK));
  CreateBook((BOOK*)DirEditor_Book,onDirEditorBookClose,&DirEditor_Base_Page,"DirEditor",-1,0);
  DirEditor_Book->directory_methods = methods;
 /* DirEditor_Book->dir_list = 0;
  DirEditor_Book->gui_dirlist = 0;
  DirEditor_Book->gui_dirsettigs = 0;
  DirEditor_Book->gui_input = 0;*/
  BookObj_GotoPage((BOOK*)DirEditor_Book,&DirEditor_DirList_Page);
  return 0;
}
