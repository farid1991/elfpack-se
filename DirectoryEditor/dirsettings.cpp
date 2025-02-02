//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
#include "main.h"

wchar_t * ItemName[S_COUNT] =
{
  L"Directory",
  L"Name",
  L"Small Icon",
  L"Big Icon",
  L"Action delete",
  L"Action rename",
};
wchar_t * StateName[3] =
{
  L"Hidden",
  L"Disabled",
  L"Enabled"
};

typedef int (*GETICON)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
typedef int (*GETELF)(wchar_t * ext, wchar_t ** elf);
typedef int (*GETSTATE)(wchar_t * ext, char * state);

void MenuItem_SetErrorStr(GUI_MESSAGE * msg, wchar_t * dir)
{
  if (methods->Dir_Registered(dir))
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"Error reading data",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Error reading data",ENC_UCS2,TEXTID_ANY_LEN));
  }
  else
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"Directory already unregisterd",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Directory already unregisterd",ENC_UCS2,TEXTID_ANY_LEN));
  }
}

int SetMenuItemText_Icon(GUI_MESSAGE * msg, wchar_t * ext, GETICON GetIcon, wchar_t * icon_wstr)
{FUNCTION
  FSTAT _fstat;
  wchar_t wstr[100], iconid, *path, *name;
  int state = GetIcon(ext, &iconid, &path, &name);
  switch (state)
  {
  case 0: //ошибка чтения
    MenuItem_SetErrorStr(msg, ext);
    break;
  case 1: //в проше
    if (iconid != NOIMAGE)
    {
      snwprintf(wstr,99,L"0x%X",iconid);
      GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
      snwprintf(wstr,99,L"%ls:\nIconID: 0x%X\n%lc", icon_wstr, iconid, iconid);
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
    }
    else
    {
      snwprintf(wstr,99,L"0x%X (No icon)",iconid);
      GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
      snwprintf(wstr,99,L"%ls:\nIconID: 0x%X\nNo icon", icon_wstr, iconid);
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
    }
    break;
  case 2: //файл
    //если иконка есть
    if (iconid != NOIMAGE)
    {
      GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(name,ENC_UCS2,TEXTID_ANY_LEN));
      snwprintf(wstr,99,L"%ls:\nFile: %ls\n%lc", icon_wstr, name, iconid);
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
    }
    //если иконки нет
    else
    {
      snwprintf(wstr,99,L"%ls !Error", name);
      GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
      snwprintf(wstr,
                99,
                L"%ls:\nFile: %ls\nError: %ls\nNo icon (0xFFFF)",
                icon_wstr,
                name,
                (fstat(path, name, &_fstat)<0)?
                  L"File not found"://если не найден файл
                  L"Could not get IconID");//файл есть, но все равно не грузит
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
    }
    break;
  }
  return 1;
}

int SetMenuItemText_Elf(GUI_MESSAGE * msg, wchar_t * ext, GETELF GetElf)
{FUNCTION
  wchar_t * elf;
  if (!GetElf(ext, &elf))
  {
    MenuItem_SetErrorStr(msg, ext);
    return 0;
  }
  if (elf)
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(wstrrchr(elf,'/')+1,ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(elf,ENC_UCS2,TEXTID_ANY_LEN));
  }
  else
  {
    GUIonMessage_SetMenuItemSecondLineText(msg, TextID_Create(L"Elf is not selected",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg, TextID_Create(L"Elf is not selected",ENC_UCS2,TEXTID_ANY_LEN));
  }
  return 1;
}

int SetMenuItemText_Path(GUI_MESSAGE * msg, wchar_t * ext, GETELF GetPath)
{FUNCTION
  wchar_t * path;
  if (!GetPath(ext, &path))
  {
    MenuItem_SetErrorStr(msg, ext);
    return 0;
  }
  if (path)
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(wstrrchr(path,'/')+1,ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(path,ENC_UCS2,TEXTID_ANY_LEN));
  }
  else
  {
    GUIonMessage_SetMenuItemSecondLineText(msg, TextID_Create(L"Disabled",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg, TextID_Create(L"External path not used",ENC_UCS2,TEXTID_ANY_LEN));
  }
  return 1;
}

int SetMenuItemText_Name(GUI_MESSAGE * msg, wchar_t * dir, GETELF GetName)
{FUNCTION
  wchar_t * name;
  if (!GetName(dir, &name))
  {
    MenuItem_SetErrorStr(msg, dir);
    return 0;
  }
  if (name)
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(name,ENC_UCS2,TEXTID_ANY_LEN));
    wchar_t * wstr = new wchar_t[200];
    snwprintf(wstr, 199, L"New directory name: %ls", name);
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
    delete wstr;
  }
  else
  {
    GUIonMessage_SetMenuItemSecondLineText(msg, TextID_Create(L"Disabled",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Directory name is not changed",ENC_UCS2,TEXTID_ANY_LEN));
  }
  return 1;
}

int SetMenuItemText_ActionState(GUI_MESSAGE * msg, wchar_t * dir, GETSTATE GetState)
{FUNCTION
  char state;
  if (!GetState(dir, &state))
  {
    MenuItem_SetErrorStr(msg, dir);
    return 0;
  }
  GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(StateName[state],ENC_UCS2,TEXTID_ANY_LEN));
  GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(StateName[state],ENC_UCS2,TEXTID_ANY_LEN));
  return 1;
}

int DirSettings_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)GUIonMessage_GetBook(msg);
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(ItemName[index],ENC_UCS2,TEXTID_ANY_LEN));
    switch(index)
    {
    case DIRECTORY:
      GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(debook->dir, ENC_UCS2, TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(debook->dir, ENC_UCS2, TEXTID_ANY_LEN));
      break;
    case DIR_NAME:
      SetMenuItemText_Name(msg, debook->dir, methods->Dir_GetName);
      break;
    case SMALL_ICON:
      SetMenuItemText_Icon(msg, debook->dir, methods->Dir_GetSmallIcon, ItemName[SMALL_ICON]);
      break;
    case BIG_ICON:
      SetMenuItemText_Icon(msg, debook->dir, methods->Dir_GetBigIcon, ItemName[BIG_ICON]);
      break;
    case DELETE_STATE:
      SetMenuItemText_ActionState(msg, debook->dir, methods->Dir_GetDeleteState);
      break;
    case RENAME_STATE:
      SetMenuItemText_ActionState(msg, debook->dir, methods->Dir_GetRenameState);
      break;
    }
  }
  return(1);
}

void DirSettings_ACTION_SELECT1(BOOK * book, GUI *)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int index = ListMenu_GetSelectedItem(debook->gui_dirsettings);
  switch(index)
  {
  case DIRECTORY:
    BookObj_CallPage(book,&DirEditor_DirAdd_SelectDir_Page);
    break;
  case DIR_NAME:
    BookObj_CallPage(book,&DirEditor_DirSettings_EditName_Page);
    break;
  case SMALL_ICON:
    debook->Dir_SetIconId = methods->Dir_SetSmallIconId;
    debook->Dir_SetIconFile = methods->Dir_SetSmallIconFile;
    debook->Dir_GetIcon = methods->Dir_GetSmallIcon;
    BookObj_CallPage(book,&DirEditor_DirSettings_SelectIconType_Page);
    break;
  case BIG_ICON:
    debook->Dir_SetIconId = methods->Dir_SetBigIconId;
    debook->Dir_SetIconFile = methods->Dir_SetBigIconFile;
    debook->Dir_GetIcon = methods->Dir_GetBigIcon;
    BookObj_CallPage(book,&DirEditor_DirSettings_SelectIconType_Page);
    break;
  case DELETE_STATE:
    debook->Dir_GetState = methods->Dir_GetDeleteState;
    debook->Dir_SetState = methods->Dir_SetDeleteState;
    BookObj_CallPage(book,&DirEditor_DirSettings_SetActionState_Page);
    break;
  case RENAME_STATE:
    debook->Dir_GetState = methods->Dir_GetRenameState;
    debook->Dir_SetState = methods->Dir_SetRenameState;
    BookObj_CallPage(book,&DirEditor_DirSettings_SetActionState_Page);
    break;
  }
}


void DirSettings_ACTION_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}

void DirSettings_ACTION_LONG_BACK(BOOK * book, GUI *)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void DirSettings_ACTION_DELETE(BOOK * book, GUI *)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int index = ListMenu_GetSelectedItem(debook->gui_dirsettings);
  switch(index)
  {
  case DIR_NAME:
    methods->Dir_SetName(debook->dir, 0);
    UI_Event(ACTION_DELETE);
    break;
  case SMALL_ICON:
    methods->Dir_SetSmallIconId(debook->dir, NOIMAGE);
    UI_Event(ACTION_DELETE);
    break;
  case BIG_ICON:
    methods->Dir_SetBigIconId(debook->dir, NOIMAGE);
    UI_Event(ACTION_DELETE);
    break;
  }
}

int DirEditor_DirSettings_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  int SelectedItem = 0;
  GUI_LIST * gui_dirsettings = debook->gui_dirsettings;
  if (gui_dirsettings)
  {
    SelectedItem=ListMenu_GetSelectedItem(gui_dirsettings);
    GUIObject_Destroy(gui_dirsettings);
  }
  debook->gui_dirsettings = gui_dirsettings = CreateListMenu(book,0);
  GUIObject_SetTitleText(gui_dirsettings,TextID_Create(wstrrchr(debook->dir, L'/')+1,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemStyle(gui_dirsettings, 3);

  ListMenu_SetItemCount(gui_dirsettings, S_COUNT);
  ListMenu_SetOnMessage(gui_dirsettings, DirSettings_onGuiMessage);
  ListMenu_SetCursorToItem(gui_dirsettings, SelectedItem);
  GUIObject_SoftKeys_SetAction(gui_dirsettings,ACTION_BACK, DirSettings_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(gui_dirsettings,ACTION_LONG_BACK, DirSettings_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(gui_dirsettings,ACTION_SELECT1, DirSettings_ACTION_SELECT1);
  GUIObject_SoftKeys_SetAction(gui_dirsettings,ACTION_DELETE, DirSettings_ACTION_DELETE);
  GUIObject_SoftKeys_SetVisible(gui_dirsettings,ACTION_DELETE,0);
  GUIObject_Show(gui_dirsettings);
  return 1;
}

int DirEditor_DirSettings_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int DirEditor_DirSettings_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
 return 1;
}

int DirEditor_DirSettings_PAGE_EXIT_EVENT(void * r0, BOOK * book)
{FUNCTION
  DEBOOK * debook=(DEBOOK*)book;
  if (debook->gui_dirsettings)
  {
    GUIObject_Destroy(debook->gui_dirsettings);
    debook->gui_dirsettings=0;
  }
  return 1;
}


const PAGE_MSG DirEditor_DirSettings_EvetsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  DirEditor_DirSettings_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      DirEditor_DirSettings_PAGE_ENTER_EVENT,
  ACTION_DELETE_TAG,     DirEditor_DirSettings_PAGE_ENTER_EVENT,
  CANCEL_EVENT_TAG,      DirEditor_DirSettings_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    DirEditor_DirSettings_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   DirEditor_DirSettings_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC DirEditor_DirSettings_Page={"DirEditor_DirSettings_Page",0,DirEditor_DirSettings_EvetsList};
