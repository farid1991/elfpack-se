//#include "mem2.h"
//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
#include "main.h"

int DirList_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    DEBOOK * debook=(DEBOOK*)GUIonMessage_GetBook(msg);
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    wchar_t * name = (wchar_t*)List_Get(debook->dir_list, index);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(name, ENC_UCS2, TEXTID_ANY_LEN));
  }
  return(1);
}

void DirList_ACTION_BACK(BOOK * book, GUI *)
{FUNCTION
  FreeBook(book);
}

void DirList_ACTION_LONG_BACK(BOOK * book, GUI *)
{FUNCTION
  FreeBook(book);
}

void DirList_ACTION_DELETE(BOOK * book, GUI *)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->dir_count)
  {
    int index = ListMenu_GetSelectedItem(debook->gui_dirlist);
    wchar_t * name = (wchar_t*)List_Get(debook->dir_list, index);
    methods->Dir_Remove(name);
    UI_Event(ACTION_DELETE);
  }
}

void DirList_ACTION_EDIT(BOOK * book, GUI *)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int index = ListMenu_GetSelectedItem(debook->gui_dirlist);
  wchar_t * dir = (wchar_t*)List_Get(debook->dir_list, index);
  debook->dir = new wchar_t[wstrlen(dir)+1];
  wstrcpy(debook->dir, dir);
  BookObj_CallPage(book,&DirEditor_DirSettings_Page);
}

void DirList_ACTION_ADD(BOOK * book, GUI *)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  debook->dir = 0;
  BookObj_CallPage(book, &DirEditor_DirAdd_SelectDir_Page);
}

int DirEditor_DirList_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int SelectedItem = 0;
  GUI_LIST * gui_dirlist = debook->gui_dirlist;
  if (gui_dirlist)
  {
    SelectedItem=ListMenu_GetSelectedItem(gui_dirlist);
    GUIObject_Destroy(gui_dirlist);
  }
  methods->Dir_ListDestroy(debook->dir_list);
  int dir_count = debook->dir_count = methods->Dir_GetCount();
  debook->dir_list = methods->Dir_ListCreate();
  wchar_t * dir = debook->dir;

  if (dir)
  {
    LIST * dir_list = debook->dir_list;
    for (int i=0; i<dir_count; i++)
    {
      if (!wstrcmp((wchar_t*)List_Get(dir_list, i), dir))
      {
        SelectedItem = i;
        break;
      }
    }
    delete dir;
    debook->dir = 0;
  }

  debook->gui_dirlist = gui_dirlist = CreateListMenu(book,0);
  GUIObject_SetTitleText(gui_dirlist,STR("Directory"));

  ListMenu_SetItemCount(gui_dirlist, debook->dir_count);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)gui_dirlist, DirList_onGuiMessage);
  ListMenu_SetCursorToItem(gui_dirlist, SelectedItem);
  GUIObject_SoftKeys_SetAction(gui_dirlist,ACTION_BACK, DirList_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(gui_dirlist,ACTION_LONG_BACK, DirList_ACTION_LONG_BACK);
  if (debook->dir_count)
  {
    GUIObject_SoftKeys_SetAction(gui_dirlist,ACTION_EDIT,DirList_ACTION_EDIT);
  }
  GUIObject_SoftKeys_SetAction(gui_dirlist,ACTION_ADD,DirList_ACTION_ADD);
  GUIObject_SoftKeys_SetAction(gui_dirlist,ACTION_DELETE,DirList_ACTION_DELETE);
  GUIObject_SoftKeys_SetVisible(gui_dirlist,ACTION_DELETE,0);
  GUIObject_SoftKeys_SuppressDefaultAction(gui_dirlist,ACTION_SELECT1);

  GUIObject_Show(gui_dirlist);
  return 1;
}

int DirEditor_DirList_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  FreeBook(book);
  return 1;
}

int DirEditor_DirList_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  return 1;
}

int DirEditor_DirList_PAGE_EXIT_EVENT(void * r0, BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_dirlist)
  {
    GUIObject_Destroy(debook->gui_dirlist);
    debook->gui_dirlist=0;
  }
  return 1;
}

const PAGE_MSG DirEditor_DirList_EvetsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirList_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirList_PAGE_ENTER_EVENT,
  ACTION_DELETE_TAG,     DirEditor_DirList_PAGE_ENTER_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirList_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    DirEditor_DirList_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirList_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirList_Page={"DirEditor_DirList_Page",0,DirEditor_DirList_EvetsList};

