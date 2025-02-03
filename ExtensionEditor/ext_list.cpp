//#include "mem2.h"
//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
#include "main.h"

int ExtList_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    EEBOOK * eebook=(EEBOOK*)GUIonMessage_GetBook(msg);
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    wchar_t * name = eebook->ext_list[index];
    if (methods->Ext_IsFile(name))
    {
      wchar_t * wstr = new wchar_t[wstrlen(name)+3];
      wstrcpy(wstr, L"  ");
      wstrcat(wstr, name);
      GUIonMessage_SetMenuItemText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
      delete wstr;
    }
    else
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(name,ENC_UCS2,TEXTID_ANY_LEN));
    }
  }
  return(1);
}

void ExtList_ACTION_BACK(BOOK * book, GUI *)
{FUNCTION
  FreeBook(book);
}

void ExtList_ACTION_LONG_BACK(BOOK * book, GUI *)
{FUNCTION
  FreeBook(book);
}

void ExtList_ACTION_DELETE(BOOK * book, GUI *)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->ext_count)
  {
    int index = ListMenu_GetSelectedItem(eebook->extlist);
    methods->Ext_Remove(eebook->ext_list[index]);
    UI_Event(ACTION_DELETE);
  }
}

void ExtList_ACTION_EDIT(BOOK * book, GUI *)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int index = ListMenu_GetSelectedItem(eebook->extlist);
  wchar_t * ext = eebook->ext_list[index];
  eebook->ext = new wchar_t[wstrlen(ext)+1];
  wstrcpy(eebook->ext, ext);
  //
  //eebook->extf = (EXTF*)List_Get(eebook->extdata, index);
  BookObj_CallPage(book,&ExtEditor_ExtSettings_Page);
}

void ExtList_ACTION_ADD(BOOK * book, GUI *)
{FUNCTION
  BookObj_CallPage(book, &ExtEditor_ExtAdd_SelectType_Page);
}

int ExtEditor_ExtList_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int SelectedItem = 0;
  GUI_LIST * extlist = eebook->extlist;
  if (extlist)
  {
    SelectedItem=ListMenu_GetSelectedItem(extlist);
    GUIObject_Destroy(extlist);
  }
  int ext_count = eebook->ext_count;
  if (ext_count)
  {
    wchar_t ** ext_list = eebook->ext_list;
    for (int i=0; i<ext_count; i++)
    {
      delete ext_list[i];
    }
    delete ext_list;
  }
  eebook->ext_count = ext_count = methods->GetFullList(&eebook->ext_list);
  wchar_t * ext;
  if (ext = eebook->ext)
  {
    wchar_t ** ext_list = eebook->ext_list;
    for (int i=0; i<ext_count; i++)
    {
      if (!wstrcmp(ext_list[i], ext))
      {
        SelectedItem = i;
        break;
      }
    }
    delete ext;
    eebook->ext = 0;
  }
  eebook->extlist = extlist = CreateListMenu(book,0);
  GUIObject_SetTitleText(extlist,STR("Extensions"));

  ListMenu_SetItemCount(extlist, eebook->ext_count);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)extlist, ExtList_onGuiMessage);
  ListMenu_SetCursorToItem(extlist, SelectedItem);
  GUIObject_SoftKeys_SetAction(extlist,ACTION_BACK, ExtList_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(extlist,ACTION_LONG_BACK, ExtList_ACTION_LONG_BACK);
  if (eebook->ext_count)
  {
    GUIObject_SoftKeys_SetAction(extlist,ACTION_EDIT,ExtList_ACTION_EDIT);
  }
  GUIObject_SoftKeys_SetAction(extlist,ACTION_ADD,ExtList_ACTION_ADD);
  GUIObject_SoftKeys_SetAction(extlist,ACTION_DELETE,ExtList_ACTION_DELETE);
  GUIObject_SoftKeys_SetVisible(extlist,ACTION_DELETE,0);
  GUIObject_SoftKeys_SuppressDefaultAction(extlist,ACTION_SELECT1);

  GUIObject_Show(extlist);
  return 1;
}

int ExtEditor_ExtList_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  FreeBook(book);
  return 1;
}

int ExtEditor_ExtList_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
  return 1;
}

int ExtEditor_ExtList_PAGE_EXIT_EVENT(void * r0, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->extlist)
  {
    GUIObject_Destroy(eebook->extlist);
    eebook->extlist=0;
  }
  return 1;
}

const PAGE_MSG ExtEditor_ExtList_EvetsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtList_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtList_PAGE_ENTER_EVENT,
  ACTION_DELETE_TAG,     ExtEditor_ExtList_PAGE_ENTER_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtList_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    ExtEditor_ExtList_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtList_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtList_Page={"ExtEditor_ExtList_Page",0,ExtEditor_ExtList_EvetsList};
