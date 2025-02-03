#include "..\deleaker\mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

#define BOOK_NAME "FreeMemoryTab"
#define ABOUT_TEXT "FreeMemoryTab v. 1.0\n\n(c) E1kolyan"

int IsFreeMemoryTabBook(BOOK* book)
{
  return 0 == strcmp(book->xbook->name, BOOK_NAME);
}

GUI* BookObjGetGui(BOOK* book)
{
  return (GUI*)List_Get(book->xguilist->guilist, 0);
}

int PAGE_ENTER_EVENT_PROC(void* msg, BOOK* book, PAGE_DESC* page_desc, LPARAM ClientData, u16 event)
{
  wchar_t buf[64];
  long long free_tpa, free_card;
  
  VOLUMESIZE_A2 tpa, card;
  GetVolumeSize(L"/tpa", (VOLUMESIZE*)&tpa);
  GetVolumeSize(L"/card", (VOLUMESIZE*)&card);
  
  free_tpa = tpa.FreeSpace / 1024 / 1024;
  free_card = card.FreeSpace / 1024 / 1024;
  
  if(book && !strcmp(page_desc->name, "DataBrowser_Main_Page"))
  {
    int tab = TabMenuBar_GetFocusedTab(BookObjGetGui(book));
    {
      if (tab == 0) snwprintf(buf, MAXELEMS(buf), L"—воб: %lldMB/%lldMB", free_tpa, free_card);
      else if (tab == 1) snwprintf(buf, MAXELEMS(buf), L"—воб: %lldMB", free_card);
      else if (tab == 2) snwprintf(buf, MAXELEMS(buf), L"—воб: %lldMB", free_tpa);
    }
    TabMenuBar_SetTabTitle(BookObjGetGui(book), tab, TextID_Create(buf, ENC_UCS2, TEXTID_ANY_LEN));
  }
  
  return 0;
}

typedef struct
{
  BOOK* book;
}MSG;

int TerminateElf(void* data, BOOK* book)
{
  FreeBook(book);
  return 1;
}

int ShowAuthorInfo(void* data, BOOK* book)
{
  MSG* msg = (MSG*)data;
  MessageBox(EMPTY_TEXTID, STR(ABOUT_TEXT), NOIMAGE, 1, 5000, msg->book);
  return 1;
}

const PAGE_MSG FreeMemoryTab_Base_evlist[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, TerminateElf,
  ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
  NIL_EVENT_TAG, NULL
};

PAGE_DESC FreeMemoryTab_Base_Page = {"FreeMemoryTab_Base_Page", NULL, FreeMemoryTab_Base_evlist};

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void FreeMemoryTabBook_onClose(BOOK* book)
{
  ModifyUIPageHook(PAGE_ENTER_EVENT, PAGE_ENTER_EVENT_PROC, NULL, PAGE_HOOK_REMOVE);
  SUBPROC(elf_exit);
}

BOOK* CreateFreeMemoryTabBook()
{
  BOOK* book = new BOOK;
  CreateBook(book, FreeMemoryTabBook_onClose, &FreeMemoryTab_Base_Page, BOOK_NAME, -1, 0);
  return book;
}

int main(void)
{
  BOOK* book = FindBook(IsFreeMemoryTabBook);
  
  if (book)
  {
    UI_Event_toBookID(ELF_TERMINATE_EVENT, BookObj_GetBookID(book));
    MessageBox(EMPTY_TEXTID, STR("Ёльф выгружен!"), NOIMAGE, 1, 5000, 0);
  }
  else
  {
    trace_init();
    CreateFreeMemoryTabBook();
    ModifyUIPageHook(PAGE_ENTER_EVENT, PAGE_ENTER_EVENT_PROC, NULL, PAGE_HOOK_ADD_AFTER);
  }
  
  return 0;
}
