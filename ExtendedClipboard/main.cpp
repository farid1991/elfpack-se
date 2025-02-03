#include "main.h"

void elf_exit(void)
{
   kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

class IUnknown
{
public:
   virtual void* pguid();
   virtual void* QueryInterface();
   virtual void* AddRef();
   virtual void* Release();
};

typedef unsigned short FUint16;
typedef FUint16 TMsgBase;
typedef FUint16 TMsgId;
typedef FUint16 TSize;
typedef long TSigned;
typedef unsigned long TUnsigned;
typedef TSigned RVoid;
typedef TSigned RBool;
typedef void TVoid;

class IShell: public IUnknown
{
public:
   virtual RVoid AllocateMemory(TUnsigned Size, void **ppBuffer); 
   virtual RVoid CreateMsg(TMsgBase Base, TMsgId Id, TSize Size, void **ppMsg); 
   virtual RVoid CompareUUID(UUID *pIID1, UUID *pIID2); 
   virtual RVoid FreeMsg(void **ppMsg);  
   virtual RVoid CreateInstance(UUID *pCid, UUID *pIid, void **ppInterfacePointer);
   virtual RVoid FreeMemory(void **ppBuffer);
   virtual RVoid MarkInterfacesInMsg(void **ppMsg, void **ppInterfacePointer);
   virtual RVoid ReallocateMemory(TUnsigned Size, void **ppBuffer);
   virtual RVoid ReleaseInterfacesInMsg(void **ppMsg);
   virtual RBool IsKindOfInterface(void *pInterface, UUID *pIId);
   virtual RVoid InvalidateCallbackInterface(void *pICB);
   virtual RVoid GetCallerExecutionShell(void **ppIExecutionShell);
};

class IUIText: public IUnknown
{
public:
   virtual int unk_0(); //0x10
   virtual int get_text(wchar_t *ws, int max_len, int *len);
   virtual int unk_1();
   virtual int get_len(int *len);
};

class IUITextFactory: public IUnknown
{
public:
   virtual int unk_0();
   virtual int CreateFromAscii();
   virtual int CreateFromRaw(wchar_t *ws, int len, IUIText **ppIUITextPointer);
   virtual int CreateFromTextId();
   virtual int unk_1();
   virtual int unk_2();
   virtual int unk_3();
   virtual int ConcatenateIUITexts();
};

class IUIClipboard: public IUnknown
{
public:
   virtual int GetBuffer(void **buffer);
   virtual int SetIUIText(IUIText *pIUIText);
   virtual int GetIUIText(IUIText **ppIUITextPointer);
   virtual int Clear();
   virtual int isEmpty();
};

class IUIClipboardManager: public IUnknown
{
public:
   virtual int GetBuffer(void **buffer);
   virtual int GetIUIClipboard(IUIClipboard **ppIUIClipboardPointer);
};

UUID CID_CUIClipboardManager = 
{
   0x4E, 0xFE, 0x6C, 0x65, 0x6D, 0xE8, 0x4B, 0xC2, 0xB5, 0xC, 0x12, 0x3B, 0x65, 0xD1, 0x5F, 0x72
};

UUID IID_IUIClipboardManager = 
{
   0x9B, 0x5D, 0xF1, 0xA9, 0xE3, 0x6A, 0x44, 0x7E, 0x9A, 0xB2, 0x47, 0x21, 0x4E, 0xA, 0x35, 0x3
};

UUID CID_CUITextFactory = 
{
   0xEA, 0xED, 0x4, 0x64, 0x0, 0x73, 0x41, 0x81, 0xBD, 0xE3, 0xFB, 0x31, 0x9F, 0xDB, 0xDF, 0xBC
};

UUID IID_IUITextFactory = 
{
   0xFB, 0xE8, 0xC1, 0xCC, 0x97, 0x35, 0x49, 0xD9, 0x95, 0x8E, 0x3E, 0x9D, 0x99, 0x82, 0x27, 0xB3
};

/*
UUID CID_CUIManager = 
{
   0x1B, 0xFA, 0xCD, 0x74, 0x2D, 0xF, 0x47, 0x75, 0xB6, 0xEC, 0x24, 0x56, 0xC1, 0x4F, 0xAF, 0x48
};

UUID IID_IUIManager = 
{
   0x2D, 0x5F, 0xED, 0xED, 0xE5, 0xAA, 0x43, 0xCC, 0x96, 8, 0x7D, 0xF2, 0xE8, 0x62, 0x36, 0x44
};
*/

IUIClipboard *pIUIClipboard = NULL;

IUIClipboard *GetClipboardClass(void)
{
   IUIClipboard *pIUIClipboard = NULL;
   IShell *pIShell = NULL;
   OSE_GetShell(PPINTERFACE(&pIShell));
   if(pIShell)
   {
      IUIClipboardManager *pIUIClipboardManager = NULL;
      pIShell->CreateInstance(&CID_CUIClipboardManager,
                              &IID_IUIClipboardManager,
                              PPINTERFACE(&pIUIClipboardManager));
      
      if(pIUIClipboardManager)
      {
         pIUIClipboardManager->GetIUIClipboard(&pIUIClipboard);
         pIUIClipboardManager->Release();
      }
      pIShell->Release();
   }
   return(pIUIClipboard);
}

int Clipboard_ExistsText(void)
{
   int result = true;
   //IUIClipboard *pIUIClipboard = GetClipboardClass();
   if(pIUIClipboard)
   {
      pIUIClipboard->AddRef();
      result = pIUIClipboard->isEmpty();
      //pIUIClipboard->Release();
   }
   return(!result);
}

int Clipboard_SetText(wchar_t *ws, int len)
{
   int result = RESULT_FAIL;
   if(!ws || !len) return(result);
   
   IUITextFactory *pIUITextFactory = NULL;
   CoCreateInstance(&CID_CUITextFactory,
                    &IID_IUITextFactory,
                    PPINTERFACE(&pIUITextFactory));
   
   if(pIUITextFactory)
   {
      IUIText *pIUIText = NULL;
      pIUITextFactory->CreateFromRaw(ws, len, &pIUIText);
      if(pIUIText)
      {
         //IUIClipboard *pIUIClipboard = GetClipboardClass();
         if(pIUIClipboard)
         {
            pIUIClipboard->AddRef();
            pIUIClipboard->SetIUIText(pIUIText);
            //if(isA2()) pIUIClipboard->Release();
            result = RESULT_OK;
         }
         pIUIText->Release();
      }
      pIUITextFactory->Release();
   }
   return(result);
}

wchar_t *Clipboard_GetText(void)
{
   wchar_t *ret_ws = NULL;
   IUIText *pIUIText = NULL;
   //IUIClipboard *pIUIClipboard = GetClipboardClass();
   if(!pIUIClipboard) return(ret_ws);
   pIUIClipboard->AddRef();
   pIUIClipboard->GetIUIText(&pIUIText);
   //pIUIClipboard->Release();
   
   if(pIUIText)
   {
      int len = 0;
      if(pIUIText->get_len(&len) >= 0)
      {
         int new_len = 0;
         ret_ws = new wchar_t[len+1];
         pIUIText->get_text(ret_ws, len+1, &new_len);
         if(!new_len)
         {
            mfree(ret_ws);
            ret_ws = NULL;
         }
      }
      pIUIText->Release();
   }
   return(ret_ws);
}

wchar_t *MakeFullName(wchar_t *path, wchar_t *name)
{
   int len = wstrlen(path)+wstrlen(name)+1;
   wchar_t *fullname = new wchar_t[len+1];
   wstrcpy(fullname, path);
   wstrcat(fullname, L"/");
   wstrcat(fullname, name);
   return(fullname);
}

void RestoreExtClipboard(ExtClipboardBook *book)
{
   LIST *texts = book->clipboard_texts;
   
   W_FSTAT fst;
   if(w_fstat(PATH, &fst) < 0)
   {
      mkdir(PATH);
      return;
   }
   
   wchar_t *sett = wstrrchr(FILE_NAME_SET, '/')+1;
   
   void *dir = w_diropen(PATH);
   while(true)
   {
      wchar_t *dir_item = w_dirread(dir);
      if(!dir_item) break;
      if(wstrcmp(sett, dir_item))
      {
         wchar_t *full_name = MakeFullName(PATH, dir_item);
         w_fstat(full_name, &fst);
         int file = w_fopen(full_name, WA_Read, 0x1FF, 0);
         if(file != FILE_HANDLE_INVALID)
         {
            char *buffer = new char[fst.st_size];
            w_fread(file, buffer, fst.st_size);
            w_fclose(file);
            List_InsertLast(texts, buffer);
         }
         mfree(full_name);
      }
   }
   w_dirclose(dir);
}

int CopyTextToFile(void *msg, BOOK *book)
{
   wchar_t *copied_text = (wchar_t *)msg;
   
   LIST *texts = ((ExtClipboardBook *)book)->clipboard_texts;
   
   if(copied_text)
   {
      bool up = false;
      
      if(texts->FirstFree)
      {
         for(int i = 0; i < texts->FirstFree; i++)
         {
            wchar_t *item = (wchar_t *)List_Get(texts, i);
            if(!wstrcmp(item, copied_text))
            {
               List_RemoveAt(texts, i);
               List_InsertFirst(texts, item);
               up = true;
               break;
            }
         }
      }
      
      if(!up)
      {
         int len = wstrlen(copied_text);
         wchar_t *item = new wchar_t[len+1];
         wstrcpy(item, copied_text);
         item[len] = 0;
         List_InsertFirst(texts, item);
      }
      
      if(texts->FirstFree > 10)
      {
         mfree(List_RemoveAt(texts, 10));
      }
   }
   
   wchar_t *buffer = new wchar_t[34+1];
   
   for(int i = 0; i < texts->FirstFree; i++)
   {
      snwprintf(buffer, 0xFF, FILE_NAME, i);
      
      int file = w_fopen(buffer, WA_Create | WA_Truncate | WA_Write, 0x1FF, 0);
      if(file != FILE_HANDLE_INVALID)
      {
         wchar_t *item = (wchar_t *)List_Get(texts, i);
         w_fwrite(file, item, sizeof(wchar_t)*(wstrlen(item)+1));
         w_fclose(file);
      }
   }
   mfree(buffer);
   
   return(1);
}

bool DispObj_isStringInput(DISP_OBJ *disp)
{
   if(!strcmp(DispObject_GetName(disp), "StringInput")) return(true);
   return(false);
}

GUI *StringInput_GetFakeGui(DISP_OBJ *si_disp)
{
   int offset = 0xFF;
   switch(GetChipID()&CHIPID_MASK)
   {
   case CHIPID_DB2010:
   case CHIPID_DB2020:
      offset = 0x0;
      break;
   case CHIPID_DB3150:
   case CHIPID_DB3200:
   case CHIPID_DB3210:
      offset = 0x34;
      break;
   }
   if(offset == 0xFF) return(NULL);
   int *gui = new int[0x40/sizeof(int)];
   gui[offset/sizeof(int)] = (int)si_disp;
   return((GUI *)gui);
}

void ExtClipboardBook_StringInput_ExecuteAction(u16 action)
{
   ExtClipboardBook *book = Find_ExtClipboardBook();
   if(book)
   {
      DISP_OBJ *disp = Display_GetFocusedDispObject(0);
      if(DispObj_isStringInput(disp))
      {
         u16 len = 0;
         wchar_t *ws = NULL;
         GUI *fake_gui = StringInput_GetFakeGui(disp);
         StringInput_GetStringAndLen(fake_gui, &ws, &len);
         mfree(fake_gui); 
         if(action == ACTION_CLIPBOARD_COPYALL || action == ACTION_CLIPBOARD_MARKANDCOPY)
         {
            if(!len) return;
         }
         else if(action == ACTION_CLIPBOARD_PASTE)
         {
            if(!Clipboard_ExistsText()) return;
         }
         DispObject_SoftKeys_ExecuteAction(disp, action);
      }
   }
}

int CopyToClipboard(void *msg, BOOK *book)
{
   if(((ExtClipboardBook *)book)->hotkeys_state)
      ExtClipboardBook_StringInput_ExecuteAction(ACTION_CLIPBOARD_MARKANDCOPY);
   return(1);
}

int CopyAllToClipboard(void *msg, BOOK *book)
{
   if(((ExtClipboardBook *)book)->hotkeys_state)
      ExtClipboardBook_StringInput_ExecuteAction(ACTION_CLIPBOARD_COPYALL);
   return(1);
}

int PasteFromClipboard(void *msg, BOOK *book)
{
   if(((ExtClipboardBook *)book)->hotkeys_state)
      ExtClipboardBook_StringInput_ExecuteAction(ACTION_CLIPBOARD_PASTE);
   return(1);
}

int Goto_CreateViewer(void *msg, BOOK *bk)
{
   ExtClipboardBook *book = Find_ExtClipboardBook();
   if(book)
   {
      DISP_OBJ *disp = Display_GetFocusedDispObject(0);
      if(DispObj_isStringInput(disp))
      {
         book->called_from_string_input = true;
         CreateViewer(msg, bk);
      }
   }
   return(1);
}

const PAGE_MSG book_base_evlist[] @ "DYN_PAGE"  = 
{
   ELF_TERMINATE_EVENT,                  onExit,
   ELF_SHOW_INFO_EVENT,                  onAbout,
   CLIPBOARD_TEXTCOPIED_EVENT,           CopyTextToFile,
   VOLUMEUPKEY_SHORT_PRESS_EVENT_TAG,    CopyAllToClipboard,
   VOLUMEDOWNKEY_SHORT_PRESS_EVENT_TAG,  PasteFromClipboard,
   VOLUMEUPKEY_LONG_PRESS_EVENT_TAG,     CopyToClipboard,
   VOLUMEDOWNKEY_LONG_PRESS_EVENT_TAG,   Goto_CreateViewer,
   NIL_EVENT_TAG,                        NULL
};

const PAGE_MSG viewtexts_main_evlist[] @ "DYN_PAGE"  = 
{
   PAGE_ENTER_EVENT_TAG,                 CreateViewer,
   NIL_EVENT_TAG,                        NULL
};

const PAGE_DESC Book_BaseDesc = {"ExtClipboardBook_BasePage", 0, book_base_evlist};
const PAGE_DESC ViewTexts_MainDesc = {"ViewTexts_MainDesc", 0, viewtexts_main_evlist};

int onExit(void *msg, BOOK *book)
{
   FreeBook(book);
   return(1);
}

int onAbout(void *msg, BOOK *book)
{
   MessageBox(EMPTY_TEXTID, STR("Extended Clipboard\nVersion 2.43f\n\n(c) Joker XT"), NOIMAGE, 1, 0, 0);
   return(1);
}

void Book_onClose(BOOK *bk)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   GUI_DESTROY(book->list_menu);
   int count = book->clipboard_texts->FirstFree;
   for(int i = (count-1); i >= 0; i--)
   {
      mfree(List_RemoveAt(book->clipboard_texts, i));
   }
   List_Destroy(book->clipboard_texts);
   pIUIClipboard->Release();
   SUBPROC(elf_exit);
}

ExtClipboardBook *_CreateBook(char *name)
{
   ExtClipboardBook *book = new ExtClipboardBook;
   memset(book, 0, sizeof(ExtClipboardBook));
   if(!CreateBook((BOOK *)book, Book_onClose, &Book_BaseDesc, name, -1, 0))
   {
      mfree(book);
      return(NULL);
   }
   return(book);
}

int isExtClipboardBook(BOOK *book)
{
   if(!strcmp(book->xbook->name, BOOKNAME)) return(1);
   return(0);
}

ExtClipboardBook *Find_ExtClipboardBook(void)
{
   return((ExtClipboardBook *)FindBook(isExtClipboardBook));
}

int ListMenu_onMessage(GUI_MESSAGE *msg)
{
   ExtClipboardBook *book = (ExtClipboardBook *)GUIonMessage_GetBook(msg);
   int index = GUIonMessage_GetCreatedItemIndex(msg);
   switch(GUIonMessage_GetMsg(msg))
   {
   case LISTMSG_GetItem:
      wchar_t *item = (wchar_t *)List_Get(book->clipboard_texts, index);
      GUIonMessage_SetMenuItemText(msg, TextID_Create(item, ENC_UCS2, TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemSecondLineText(msg, TextID_CreateIntegerID(wstrlen(item)));
      break;
   }
   return(1);
}

void _ACTION_BACK(BOOK *bk, GUI *gui)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   book->selected_item = CLIPBOARD_CURRENT_TEXT;
   GUI_DESTROY(book->list_menu);
   BookObj_Hide(bk, 0);
}

void Edit_ACTION_BACK(BOOK *bk, GUI *gui)
{
   //ExtClipboardBook *book = (ExtClipboardBook *)bk;
   //GUI_DESTROY(book->list_menu);
   BookObj_GotoPage(bk, &ViewTexts_MainDesc);
}

void Edit_ACTION_OK(BOOK *bk, wchar_t *string, int len)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   
   wchar_t *cur_item = (wchar_t *)List_Get(book->clipboard_texts, book->selected_item);
   
   if(wstrcmpi(cur_item, string))
   {
      wchar_t *item = new wchar_t[len+1];
      wstrcpy(item, string);
      item[len] = 0;
      
      mfree(List_RemoveAt(book->clipboard_texts, book->selected_item));
      List_Insert(book->clipboard_texts, book->selected_item, item);
      CopyTextToFile(NULL, bk);
      if(book->selected_item == CLIPBOARD_CURRENT_TEXT)
         Clipboard_SetText(item, wstrlen(item));
   }
   
   //GUI_DESTROY(book->list_menu);
   BookObj_GotoPage(bk, &ViewTexts_MainDesc);
}

void _ACTION_EDIT(BOOK *bk, GUI *gui)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   
   int i = ListMenu_GetSelectedItem(gui);
   book->selected_item = i;
   GUI_DESTROY(book->list_menu);
   wchar_t *item = (wchar_t *)List_Get(book->clipboard_texts, i);
   
   book->list_menu = CreateStringInputVA(0,
                                         VAR_BOOK(bk),
                                         VAR_HEADER_TEXT(STR("Edit")),
                                         VAR_STRINP_MODE(1),
                                         VAR_STRINP_MAX_LEN(wstrlen(item)+0xFF),
                                         VAR_STRINP_ENABLE_EMPTY_STR(0),
                                         VAR_STRINP_TEXT(TextID_Create(item, ENC_UCS2, TEXTID_ANY_LEN)),
                                         VAR_PREV_ACTION_PROC(Edit_ACTION_BACK),
                                         VAR_LONG_BACK_PROC(_ACTION_BACK),
                                         VAR_OK_PROC(Edit_ACTION_OK),
                                         0);
}

void Folder_FilesDelete(wchar_t *path)
{
   wchar_t *sett = wstrrchr(FILE_NAME_SET, '/')+1;
   void *dir = w_diropen(path);
   while(true)
   {
      wchar_t *dir_item = w_dirread(dir);
      if(!dir_item) break;
      if(wstrcmp(sett, dir_item)) FileDelete(path, dir_item, 0);
   }
   w_dirclose(dir);
}

void _ACTION_DELETE(BOOK *bk, GUI *gui)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   
   int i = ListMenu_GetSelectedItem(gui);
   
   if(i == CLIPBOARD_CURRENT_TEXT)
   {
      MessageBox(EMPTY_TEXTID, STR("Невозможно удалить!\nТекст находится в буфере."), NOIMAGE, 1, 0, 0);
      return;
   }
   
   LIST *texts = book->clipboard_texts;
   Folder_FilesDelete(PATH);
   
   mfree(List_RemoveAt(texts, i));
   CopyTextToFile(NULL, bk);
   
   book->selected_item = CLIPBOARD_CURRENT_TEXT;
   BookObj_GotoPage(bk, &ViewTexts_MainDesc);
}

void _ACTION_COPY(BOOK *bk, GUI *gui)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   
   int i = ListMenu_GetSelectedItem(gui);
   LIST *texts = book->clipboard_texts;
   
   wchar_t *item = (wchar_t *)List_Get(texts, i);
   Clipboard_SetText(item, wstrlen(item));
   
   book->selected_item = CLIPBOARD_CURRENT_TEXT;
   GUI_DESTROY(book->list_menu);
   BookObj_Hide(bk, 0);
}

void RemoveAll(ExtClipboardBook *book)
{
   Folder_FilesDelete(PATH);
   
   int count = book->clipboard_texts->FirstFree;
   for(int i = (count-1); i >= 1; i--)
   {
      mfree(List_RemoveAt(book->clipboard_texts, i));
   }
   
   CopyTextToFile(NULL, (BOOK *)book);
   book->selected_item = CLIPBOARD_CURRENT_TEXT;
   BookObj_GotoPage((BOOK *)book, &ViewTexts_MainDesc);
}

void ClearInputField(ExtClipboardBook *book)
{
   book->selected_item = CLIPBOARD_CURRENT_TEXT;
   GUI_DESTROY(book->list_menu);
   BookObj_Hide((BOOK *)book, 0);
   
   DISP_OBJ *disp = Display_GetFocusedDispObject(0);
   if(DispObj_isStringInput(disp))
   {
      StringInput_DispObject_SetText(disp, EMPTY_TEXTID);
   }
}

void ViewHelp(ExtClipboardBook *book)
{
   MessageBox(EMPTY_TEXTID, 
              STR("Горячие клавиши:\n(долгое нажатие)\n* - очистить все тексты, кроме текущего;\n# - очистить поле ввода текста;\n1 - Вкл./откл. горячие главиши в режиме редактирования;\n0 - посмотреть помощь."), 
              NOIMAGE, 0, 0, 0);
}

void Hotkeys_ChangeState(ExtClipboardBook *book)
{
   int file = w_fopen(FILE_NAME_SET, WA_Create | WA_Truncate | WA_Write, 0x1FF, 0);
   if(file != FILE_HANDLE_INVALID)
   {
      int state = !book->hotkeys_state;
      w_fwrite(file, &state, sizeof(int));
      w_fclose(file);
      book->hotkeys_state = state;
      TEXTID textid_state = EMPTY_TEXTID;
      if(state) textid_state = STR("Hotkeys ON");
      else textid_state = STR("Hotkeys OFF");
      MessageBox(EMPTY_TEXTID, textid_state, NOIMAGE, 0, 1000, 0);
   }
}

void Hotkeys_SetState(ExtClipboardBook *book)
{
   int state = 1;
   int file = w_fopen(FILE_NAME_SET, WA_Read, 0x1FF, 0);
   if(file != FILE_HANDLE_INVALID)
   {
      if(w_fread(file, &state, sizeof(int)) == sizeof(int))
      {
         w_fclose(file);
      }
   }
   book->hotkeys_state = state;
}

void onKey(DISP_OBJ *disp, int scan_code, int unk, int repeat_num, int key_mode)
{
   GUI *gui = DispObject_GetGUI(disp);
   ExtClipboardBook *book = (ExtClipboardBook *)GUIObject_GetBook(gui);
   
   book->old_on_key(disp, scan_code, unk, repeat_num, key_mode);
   
   if(key_mode == KBD_LONG_PRESS)
   {
      if(scan_code == KEY_STAR) RemoveAll(book);
      if((scan_code == KEY_DIEZ) && (book->called_from_string_input == true)) ClearInputField(book);
      if(scan_code == KEY_DIGITAL_0) ViewHelp(book);
      if(scan_code == (KEY_DIGITAL_0+1)) Hotkeys_ChangeState(book);
   }
   
   if(book->called_from_string_input == true)
   {
      if(key_mode == KBD_SHORT_PRESS && scan_code == KEY_RIGHT)
      {
         int i = ListMenu_GetSelectedItem(gui);
         LIST *texts = book->clipboard_texts;
         
         wchar_t *item = (wchar_t *)List_Get(texts, i);
         Clipboard_SetText(item, wstrlen(item));
         book->selected_item = CLIPBOARD_CURRENT_TEXT;
         DISP_DESC_SetOnKey(DispObject_GetDESC(disp), book->old_on_key);
         BookObj_Hide((BOOK *)book, 0);
         PasteFromClipboard(NULL, NULL);
         GUI_DESTROY(book->list_menu);
      }
   }
}

int CreateViewer(void *msg, BOOK *bk)
{
   ExtClipboardBook *book = (ExtClipboardBook *)bk;
   
   GUI_DESTROY(book->list_menu);
   
   GUI *list_menu = CreateListMenu((BOOK *)book, 0);
   book->list_menu = list_menu;
   
   ListMenu_SetItemStyle(list_menu, 3);
   GUIObject_SetTitleText(list_menu, STR("ExtClipboard"));
   int count = book->clipboard_texts->FirstFree;
   ListMenu_SetItemCount(list_menu, count);
   ListMenu_SetCursorToItem(list_menu, book->selected_item);
   ListMenu_SetOnMessage(list_menu, ListMenu_onMessage);
   GUIObject_SoftKeys_SetAction(list_menu, ACTION_BACK, _ACTION_BACK);
   GUIObject_SoftKeys_SetAction(list_menu, ACTION_LONG_BACK, _ACTION_BACK);
   
   if(count)
   {
      GUIObject_SoftKeys_SetAction(list_menu, ACTION_ADD, _ACTION_COPY);
      GUIObject_SoftKeys_SetText(list_menu, ACTION_ADD, STR("Скопир."));
      GUIObject_SoftKeys_SetAction(list_menu, ACTION_DELETE, _ACTION_DELETE);
      GUIObject_SoftKeys_SetVisible(list_menu, ACTION_DELETE, false);
      GUIObject_SoftKeys_SuppressDefaultAction(list_menu, ACTION_SELECT1);
      GUIObject_SoftKeys_SetAction(list_menu, ACTION_EDIT, _ACTION_EDIT);
   }
   
   DISP_OBJ *disp = GUIObject_GetDispObject(list_menu);
   book->old_on_key = DispObject_GetOnKey(disp);
   DISP_DESC_SetOnKey(DispObject_GetDESC(disp), onKey);
   
   GUIObject_Show(list_menu);
   BookObj_Show((BOOK *)book, 0);
   return(1);
}

int main(wchar_t *elf, wchar_t *path, wchar_t *name)
{
   ExtClipboardBook *book = Find_ExtClipboardBook();
   if(!book)
   {
      book = _CreateBook(BOOKNAME);
      if(book)
      {
         pIUIClipboard = GetClipboardClass();
         Hotkeys_SetState(book);
         book->clipboard_texts = List_Create();
         RestoreExtClipboard(book);
         if(Clipboard_ExistsText())
         {
            wchar_t *ws = Clipboard_GetText();
            CopyTextToFile(ws, (BOOK *)book);
            mfree(ws);
         }
         else
         {
            if(book->clipboard_texts->FirstFree)
            {
               wchar_t *item = (wchar_t *)List_Get(book->clipboard_texts, CLIPBOARD_CURRENT_TEXT);
               Clipboard_SetText(item, wstrlen(item));
            }
         }
         return(0);
      }
   }
   else
   {
      book->called_from_string_input = false;
      BookObj_GotoPage((BOOK *)book, &ViewTexts_MainDesc);
      return(0);
   }
   SUBPROC(elf_exit);
   return(0);
}
