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
  DEBOOK * debook=(DEBOOK*)book;
  if (!len) string = 0;
  methods->Dir_SetName(debook->dir, string);
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}


int DirEditor_DirSettings_EditName_PAGE_ENTER_EVENT(void *, BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  wchar_t * name;
  methods->Dir_GetName(debook->dir, &name);
  int strid = (name)?TextID_Create(name, ENC_UCS2, TEXTID_ANY_LEN):0;
  debook->gui_input = CreateStringInputVA(0,
                                           VAR_HEADER_TEXT(STR("Dir name")),
                                           VAR_STRINP_MAX_LEN(99),
                                           VAR_STRINP_MODE(1),
                                           VAR_BOOK(debook),
                                           VAR_STRINP_ENABLE_EMPTY_STR(1),
                                           VAR_STRINP_TEXT(strid),
                                           VAR_PREV_ACTION_PROC(EditName_ACTION_BACK),
                                           VAR_LONG_BACK_PROC(EditName_ACTION_LONG_BACK),
                                           VAR_OK_PROC(EditName_ACTION_OK),
                                           0);
  return 1;
}

int DirEditor_DirSettings_EditName_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_input)
  {
    GUIObject_Destroy(debook->gui_input);
    debook->gui_input=0;
  }
  return 1;
}

const PAGE_MSG DirEditor_DirSettings_EditName_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirSettings_EditName_PAGE_ENTER_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirSettings_EditName_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirSettings_EditName_Page={"DirEditor_DirSettings_EditName_Page",0,DirEditor_DirSettings_EditName_EventsList};
