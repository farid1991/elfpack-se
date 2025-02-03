#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\dll.h"
#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "..\\include\var_arg.h"
#include "main.h"

FILE_DIALOG *FILE_DIALOG_FOLDER;

void GRar_ExtractIn_OpebDialog(BOOK *book, int index)
{
  FILE_DIALOG_Create(FILE_DIALOG_FOLDER,
                     book,
                     OFD_OPEN_FOLDER_DIALOG|OFD_NEW_FOLDERS_ENABLE|OFD_EMPTY_FOLDER_ENABLE,
                     L"Выбор папки",
                     L"*.",
                     GetDir(index), //DIR_USER|MEM_INTERNAL, DIR_USER|MEM_EXTERNAL
                     -1);
}

void GRar_ExtractIn_Exit(BOOK *book, GUI *gui)
{
  GRBOOK *grbook = (GRBOOK *)book;
  FREE_GUI(grbook->gui_extract_in);
}

void GRar_ExtractIn_GUIonSelect(BOOK *book, GUI *gui)
{
  switch( ListMenu_GetSelectedItem(gui) )
  {
  case 0:
    GRar_ExtractIn_OpebDialog(book,DIR_USER|MEM_INTERNAL);
    break;
  case 1:
    GRar_ExtractIn_OpebDialog(book,DIR_USER|MEM_EXTERNAL);
    break;
  }
}

int GRar_ExtractIn_GUIonMessage(GUI_MESSAGE *msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
  case LISTMSG_GetItem:
    wchar_t *idname;
    wchar_t *iconname;
    switch(GUIonMessage_GetCreatedItemIndex(msg))
    {
    case 0:
      idname = L"DB_LOCATION_PHONEMEMORY_TXT";
      iconname = L"DB_2ROW_PHONE_ICN";
      break;
    case 1:
      idname = L"DB_LOCATION_MEMORYSTICK_TXT";
      iconname = L"DB_2ROW_MS_ICN";
      break;
    }
    int sid;
    int icon;
    textidname2id(idname,TEXTID_ANY_LEN,&sid);
    iconidname2id(iconname,TEXTID_ANY_LEN,&icon);
    GUIonMessage_SetMenuItemText(msg,sid);
    GUIonMessage_SetMenuItemIcon(msg,0,icon);
    break;
  }
  return(1);
};

int GRar_ExtractIn_SelectFolder_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  GRBOOK *grbook = (GRBOOK *)book;
  wchar_t wstr[100];
  FILE_DIALOG_FOLDER = (FILE_DIALOG *)LoadDLL(L"FileDialogDll.dll");
  if((INVALID(FILE_DIALOG_FOLDER)))
  {
    BookObj_ReturnPage(book, PREVIOUS_EVENT);
    snwprintf(wstr,99,L"Error Load FileDialogDll.dll : %d",FILE_DIALOG_FOLDER);
    MessageBox(EMPTY_TEXTID,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
    return 1;
  }

  GUI *lst;
  if(!(lst = CreateListMenu(book,0))) return(1);

  GUIObject_SetTitleText(lst,STR("Извлечь в..."));
  ListMenu_SetItemCount(lst,2);
  ListMenu_SetCursorToItem(lst,0);
  ListMenu_SetOnMessage(lst,GRar_ExtractIn_GUIonMessage);
  GUIObject_SetStyle(lst,UI_OverlayStyle_PopupNoFrame);
  GUIObject_SoftKeys_SetAction(lst,ACTION_BACK,GRar_ExtractIn_Exit);
  GUIObject_SoftKeys_SetAction(lst,ACTION_LONG_BACK,GRar_ExtractIn_Exit);
  GUIObject_SoftKeys_SetAction(lst,ACTION_SELECT1,GRar_ExtractIn_GUIonSelect);
  GUIObject_Show(lst);
  grbook->gui_extract_in = lst;

  return 1;
}

wchar_t * MakeFullName(wchar_t * path, wchar_t * name)
{FUNCTION
  int len = wstrlen(path)+wstrlen(name)+1;
  wchar_t * fullname = new wchar_t[len+1];
  wstrcpy(fullname, path);
  wstrcat(fullname, L"/");
  wstrcat(fullname, name);
  return fullname;
}

int GRar_ExtractIn_SelectFolder_ACCEPT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  GRBOOK *grbook = (GRBOOK *)book;
  FILEITEM *fi = (FILEITEM *)mess;
  grbook->folder_extract_in = MakeFullName(FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));

  int index = ListMenu_GetSelectedItem(grbook->gui_content);
  CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(grbook->item->list,index);
  Item_Extract(item, grbook->folder_extract_in, grbook->file);

  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return 1;
}

int GRar_ExtractIn_SelectFolder_CANCEL_EVENT(void *mess ,BOOK *book)
{FUNCTION
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}

int GRar_ExtractIn_SelectFolder_PAGE_EXIT_EVENT(void *mess ,BOOK *book)
{FUNCTION
  GRBOOK *grbook = (GRBOOK *)book;
  GRar_ExtractIn_Exit(book,grbook->gui_extract_in);
  UnLoadDLL(FILE_DIALOG_FOLDER);
  return 1;
}

const PAGE_MSG GRar_ExtractIn_SelectFolder_EventsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  GRar_ExtractIn_SelectFolder_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      GRar_ExtractIn_SelectFolder_ACCEPT_EVENT,
  CANCEL_EVENT_TAG,      GRar_ExtractIn_SelectFolder_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   GRar_ExtractIn_SelectFolder_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,         NULL
};

const PAGE_DESC GRar_ExtractIn_SelectFolder_Page={"GRar_ExtractIn_SelectFolder_Page",0,GRar_ExtractIn_SelectFolder_EventsList};
