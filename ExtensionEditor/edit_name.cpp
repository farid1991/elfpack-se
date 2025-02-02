//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\var_arg.h"
#include "main.h"

void EditName_ACTION_BACK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void EditName_ACTION_LONG_BACK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void EditName_ACTION_OK(BOOK * book, wchar_t *string, int len)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  wchar_t * ext_old = eebook->ext;
  eebook->ext = new wchar_t[wstrlen(string)+1];
  wstrcpy(eebook->ext, string);
  wstrnlwr(eebook->ext, TEXTID_ANY_LEN);
  Ext_Rename(ext_old, eebook->ext);
  delete ext_old;
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}


int ExtEditor_ExtSettings_EditName_PAGE_ENTER_EVENT(void *, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  eebook->input = CreateStringInputVA(0,
                                           VAR_HEADER_TEXT(STR("Extension")),
                                           VAR_STRINP_MAX_LEN(12),
                                           VAR_STRINP_MODE(1),
                                           VAR_BOOK(eebook),
                                           VAR_STRINP_ENABLE_EMPTY_STR(0),
                                           VAR_STRINP_TEXT(TextID_Create(eebook->ext,ENC_UCS2,TEXTID_ANY_LEN)),
                                           VAR_PREV_ACTION_PROC(EditName_ACTION_BACK),
                                           VAR_LONG_BACK_PROC(EditName_ACTION_LONG_BACK),
                                           VAR_OK_PROC(EditName_ACTION_OK),
                                           0);
  return 1;
}

int ExtEditor_ExtSettings_EditName_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->input)
  {
    GUIObject_Destroy(eebook->input);
    eebook->input=0;
  }
  return 1;
}

const PAGE_MSG ExtEditor_ExtSettings_EditName_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_EditName_PAGE_ENTER_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_EditName_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_EditName_Page={"ExtEditor_ExtSettings_EditName_Page",0,ExtEditor_ExtSettings_EditName_EventsList};
