//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
//#include "..\\include\dll.h"
//#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
//#include "..\\include\var_arg.h"
#include "main.h"

void SetActionState_ACTION_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
}

void SetActionState_ACTION_LONG_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void SetActionState_ACTION_SELECT1(BOOK * book, GUI * v)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  char state = OneOfMany_GetSelected(debook->gui_actionstate);
  debook->Dir_SetState(debook->dir, state);
  BookObj_ReturnPage(book, ACTION_ACCEPT);
}

int DirEditor_DirSettings_SetActionState_PAGE_ENTER_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  GUI_ONEOFMANY * gui_actionstate = CreateOneOfMany(book);
  debook->gui_actionstate = gui_actionstate;
  int index=ListMenu_GetSelectedItem(debook->gui_dirsettings);
  GUIObject_SetTitleText(gui_actionstate,TextID_Create(ItemName[index],ENC_UCS2,TEXTID_ANY_LEN));
  OneOfMany_SetItemCount(gui_actionstate, 3);
  char state;
  debook->Dir_GetState(debook->dir, &state);
  OneOfMany_SetChecked(gui_actionstate, state);
  int * strid = new int[3];
  for (int i=0; i<3; i++)
  {
    strid[i] = TextID_Create(StateName[i],ENC_UCS2,TEXTID_ANY_LEN);
  }
  OneOfMany_SetTexts(gui_actionstate, strid, 3);
  GUIObject_SoftKeys_SetAction(gui_actionstate,ACTION_BACK, SetActionState_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(gui_actionstate,ACTION_LONG_BACK, SetActionState_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(gui_actionstate,ACTION_SELECT1, SetActionState_ACTION_SELECT1);
  GUIObject_Show(gui_actionstate);
  return 1;
}

int DirEditor_DirSettings_SetActionState_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int DirEditor_DirSettings_SetActionState_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int DirEditor_DirSettings_SetActionState_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  return 1;
}

int DirEditor_DirSettings_SetActionState_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_actionstate)
  {
    GUIObject_Destroy(debook->gui_actionstate);
    debook->gui_actionstate=0;
  }
  return 1;
}

const PAGE_MSG DirEditor_DirSettings_SetActionState_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirSettings_SetActionState_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirSettings_SetActionState_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirSettings_SetActionState_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    DirEditor_DirSettings_SetActionState_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirSettings_SetActionState_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirSettings_SetActionState_Page={"DirEditor_DirSettings_SetActionState_Page",0,DirEditor_DirSettings_SetActionState_EventsList};
