//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
#include "main.h"

wchar_t * ItemName[S_COUNT] =
{
  L"Extension",
  L"Small Icon",
  L"Big Icon",
  L"Saved Icon",
  L"Open elf",
  L"Edit elf",
  L"Internal path",
  L"External path"
};

typedef int (*GETICON)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
typedef int (*GETELF)(wchar_t * ext, wchar_t ** elf);

void MenuItem_SetErrorStr(GUI_MESSAGE * msg, wchar_t * ext)
{
  if (methods->Ext_Exist(ext))
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"Error reading data",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Error reading data",ENC_UCS2,TEXTID_ANY_LEN));
  }
  else
  {
    GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"Extension already is not registerd",ENC_UCS2,TEXTID_ANY_LEN));
    GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(L"Extension already is not registerd",ENC_UCS2,TEXTID_ANY_LEN));
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

int ExtSettings_onGuiMessage(GUI_MESSAGE * msg)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)GUIonMessage_GetBook(msg);
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(ItemName[index],ENC_UCS2,TEXTID_ANY_LEN));
    switch(index)
    {
    case EXTENSION:
      if (methods->Ext_IsFile(eebook->ext))
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"File",ENC_UCS2,TEXTID_ANY_LEN));
      }
      GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(eebook->ext,ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemInfoText(msg,TextID_Create(eebook->ext,ENC_UCS2,TEXTID_ANY_LEN));
      break;
    case SMALL_ICON:
      SetMenuItemText_Icon(msg, eebook->ext, methods->Ext_GetSmallIcon, ItemName[SMALL_ICON]);
      break;
    case BIG_ICON:
      SetMenuItemText_Icon(msg, eebook->ext, methods->Ext_GetBigIcon, ItemName[BIG_ICON]);
      break;
    case SAVED_ICON:
      SetMenuItemText_Icon(msg, eebook->ext, methods->Ext_GetSavedIcon, ItemName[SAVED_ICON]);
      break;
    case OPEN_ELF:
      SetMenuItemText_Elf(msg, eebook->ext, methods->Ext_GetElfOpen);
      break;
    case EDIT_ELF:
      SetMenuItemText_Elf(msg, eebook->ext, methods->Ext_GetElfEdit);
      break;
    case PATH_INT:
      SetMenuItemText_Path(msg, eebook->ext, methods->Ext_GetPathInt);
      if (methods->Ext_IsFile(eebook->ext)) GUIonMessage_SetItemDisabled (msg, 1);
      break;
    case PATH_EXT:
      SetMenuItemText_Path(msg, eebook->ext, methods->Ext_GetPathExt);
      if (methods->Ext_IsFile(eebook->ext)) GUIonMessage_SetItemDisabled (msg, 1);
      break;
    }
  }
  return(1);
}

void ExtSettings_ACTION_SELECT1(BOOK * book, GUI *)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int index = ListMenu_GetSelectedItem(eebook->extension);
  switch(index)
  {
  case EXTENSION:
    if (methods->Ext_IsFile(eebook->ext))
    {
      BookObj_CallPage(book,&ExtEditor_SelectFile_Page);
    }
    else
    {
      BookObj_CallPage(book,&ExtEditor_ExtAdd_Page);
    }
    break;
  case SMALL_ICON:
    eebook->Ext_SetIconId = methods->Ext_SetSmallIconId;
    eebook->Ext_SetIconFile = methods->Ext_SetSmallIconFile;
    eebook->Ext_GetIcon = methods->Ext_GetSmallIcon;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectIconType_Page);
    break;
  case BIG_ICON:
    eebook->Ext_SetIconId = methods->Ext_SetBigIconId;
    eebook->Ext_SetIconFile = methods->Ext_SetBigIconFile;
    eebook->Ext_GetIcon = methods->Ext_GetBigIcon;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectIconType_Page);
    break;
  case SAVED_ICON:
    eebook->Ext_SetIconId = methods->Ext_SetSavedIconId;
    eebook->Ext_SetIconFile = methods->Ext_SetSavedIconFile;
    eebook->Ext_GetIcon = methods->Ext_GetSavedIcon;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectIconType_Page);
    break;
  case OPEN_ELF:
    eebook->Ext_SetElf = methods->Ext_SetElfOpen;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectElf_Page);
    break;
  case EDIT_ELF:
    eebook->Ext_SetElf = methods->Ext_SetElfEdit;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectElf_Page);
    break;
  case PATH_INT:
    eebook->path = MEM_INTERNAL;
    eebook->Ext_SetPath = methods->Ext_SetPathInt;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectFolder_Page);
    break;
  case PATH_EXT:
    eebook->path = MEM_EXTERNAL;
    eebook->Ext_SetPath = methods->Ext_SetPathExt;
    BookObj_CallPage(book,&ExtEditor_ExtSettings_SelectFolder_Page);
    break;
  }
}


void ExtSettings_ACTION_BACK(BOOK * book, GUI * v)
{FUNCTION
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}

void ExtSettings_ACTION_LONG_BACK(BOOK * book, GUI *)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
}

void ExtSettings_ACTION_DELETE(BOOK * book, GUI *)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int index = ListMenu_GetSelectedItem(eebook->extension);
  switch(index)
  {
  case SMALL_ICON:
    methods->Ext_SetSmallIconId(eebook->ext, NOIMAGE);
    UI_Event(ACTION_DELETE);
    break;
  case BIG_ICON:
    methods->Ext_SetBigIconId(eebook->ext, NOIMAGE);
    UI_Event(ACTION_DELETE);
    break;
  case SAVED_ICON:
    methods->Ext_SetSavedIconId(eebook->ext, NOIMAGE);
    UI_Event(ACTION_DELETE);
    break;
  case OPEN_ELF:
    methods->Ext_SetElfOpen(eebook->ext, 0);
    UI_Event(ACTION_DELETE);
    break;
  case EDIT_ELF:
    methods->Ext_SetElfEdit(eebook->ext, 0);
    UI_Event(ACTION_DELETE);
    break;
  case PATH_INT:
    break;
  case PATH_EXT:
    if (methods->Ext_IsFile(eebook->ext)) break;
    methods->Ext_SetPathExt(eebook->ext, 0);
    UI_Event(ACTION_DELETE);
    break;
  }
}

int ExtEditor_ExtSettings_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  int SelectedItem = 0;
  GUI_LIST * extension = eebook->extension;
  if (extension)
  {
    SelectedItem=ListMenu_GetSelectedItem(extension);
    GUIObject_Destroy(extension);
  }
  eebook->extension = extension = CreateListMenu(book,0);
  GUIObject_SetTitleText(extension,TextID_Create(eebook->ext,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemStyle(extension, 3);

  ListMenu_SetItemCount(extension, (methods->ExtMemExist())?S_COUNT:S_COUNT-1);
  ListMenu_SetOnMessage((GUI_ONEOFMANY*)extension, ExtSettings_onGuiMessage);
  ListMenu_SetCursorToItem(extension, SelectedItem);
  GUIObject_SoftKeys_SetAction(extension,ACTION_BACK, ExtSettings_ACTION_BACK);
  GUIObject_SoftKeys_SetAction(extension,ACTION_LONG_BACK, ExtSettings_ACTION_LONG_BACK);
  GUIObject_SoftKeys_SetAction(extension,ACTION_SELECT1, ExtSettings_ACTION_SELECT1);
  GUIObject_SoftKeys_SetAction(extension,ACTION_DELETE, ExtSettings_ACTION_DELETE);
  GUIObject_SoftKeys_SetVisible(extension,ACTION_DELETE,0);
  GUIObject_Show(extension);
  return 1;
}

int ExtEditor_ExtSettings_CANCEL_EVENT(void * r0, BOOK * book)
{FUNCTION
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return 1;
}

int ExtEditor_ExtSettings_PREVIOUS_EVENT(void * r0, BOOK * book)
{FUNCTION
 return 1;
}

int ExtEditor_ExtSettings_PAGE_EXIT_EVENT(void * r0, BOOK * book)
{FUNCTION
  EEBOOK * eebook=(EEBOOK*)book;
  if (eebook->extension)
  {
    GUIObject_Destroy(eebook->extension);
    eebook->extension=0;
  }
  return 1;
}


const PAGE_MSG ExtEditor_ExtSettings_EvetsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  ExtEditor_ExtSettings_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      ExtEditor_ExtSettings_PAGE_ENTER_EVENT,
  ACTION_DELETE_TAG,     ExtEditor_ExtSettings_PAGE_ENTER_EVENT,
  CANCEL_EVENT_TAG,      ExtEditor_ExtSettings_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    ExtEditor_ExtSettings_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   ExtEditor_ExtSettings_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

const PAGE_DESC ExtEditor_ExtSettings_Page={"ExtEditor_ExtSettings_Page",0,ExtEditor_ExtSettings_EvetsList};
