#include "..\deleaker\mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "main.h"

TEXTID GetText(int id)
{
  int result;
  switch(id)
  {
  case SELECT_BOOK: result = STR("Select BOOK"); break;
  case ENTER_RAM: result = STR("Enter RAM address"); break;
  case MODE: result = STR("Mode"); break;
  case ENTER_OFFSET: result = STR("Enter offset"); break;
  case ENTER_SIZE: result = STR("Enter size"); break;
  case OX: result = STR("0x"); break;
  case ILLEGAL: result = STR("Illegal symbol!"); break;
  }
  return result;
}

int BOOK_LIST_ITEM_Filter(void* list)
{
  return (list ? 1 : 0);
}

void BOOK_LIST_ITEM_Destroy(void* data)
{
  BookList* list = (BookList*)data;
  if (list)
  {
    delete(list->book_name);
    delete list;
  }
}

void FreeBookList(MemReaderBook* mem_bk)
{
  if (mem_bk->books_list)
  {
    List_DestroyElements(mem_bk->books_list, BOOK_LIST_ITEM_Filter, BOOK_LIST_ITEM_Destroy);
    List_Destroy(mem_bk->books_list);
    mem_bk->books_list = NULL;
  }
}

void GetBookList(MemReaderBook* mem_bk)
{
  FreeBookList(mem_bk);
  
  int i, j, k, mask, count = NULL;
  mem_bk->books_list = List_Create();
  i = root_list_get_session_count();
  mask = ((int)LastExtDB())&FLASH_MASK;
  
  for (j = 0; j<i; j++)
  {
    UI_APP_SESSION* session = root_list_get_session(j);
    for (k = 0; k<session->listbook->FirstFree; k++)
    {
      BOOK* book = (BOOK*)List_Get(session->listbook, k);
      if ((((int)book->onClose)&FLASH_MASK) == mask)
      {
        if (strcmp(book->xbook->name, "CUIDisplayableBook"))
        {
          if(book->xguilist->guilist->FirstFree)
          {
            BookList* list = new BookList;
            list->book = book;
            list->book_name = new char[strlen(book->xbook->name)+1];
            strcpy(list->book_name, book->xbook->name);
            List_InsertFirst(mem_bk->books_list, list);
            count++;
          }
        }
      }
    }
  }
  mem_bk->books_count = count;
}

int IllegalSymbol(wchar_t *wstr, int len)
{
  for(;;)
  {
    wchar_t s = *wstr++;
    if(((s>='0') && (s<='9'))||((s>='a') && (s<='f'))||((s>='A') && (s<='F'))||(s=='?'))
      len--;
    else break;
  }
  return(len);
}

void wtoh(wchar_t* wstr, int len , int* r)
{
  wchar_t s;
  int res=0;
  while(len--)
  {
    s=*wstr++;
    if ((s>='0') && (s<='9')) s -= '0';
    else if((s>='A') && (s<='F')) s -= '7';
    else if((s>='a') && (s<='f')) s -= ('7' + ' ');
    res<<=4;
    res|=s;
  }
  *r = res;
}

int stoh( char *str, int len )
{
  char s;
  int res=0;
  while(len--)
  {
    s=*str++;
    if ((s>='0') && (s<='9')) s -= '0';
    else if((s>='A') && (s<='F')) s -= '7';
    else if((s>='a') && (s<='f')) s -= ('7' + ' ');
    res<<=4;
    res|=s;
  }
  return(res);
}

void SizeInput_onOk(BOOK* book, wchar_t* text, int len)
{
  if (len > 2)
  {
    if (text[0] == 0x30 && text[1] == 0x78)
    {
      text += 2;
      len -= 2;
    }
    
    if(!IllegalSymbol(text, len))
    {
      MemReaderBook* mem_bk = (MemReaderBook*)book;
      
      int size;
      wchar_t buf[128];
      
      if (mem_bk->choice_item == 0)
      {
        wtoh(text, len, &size);
        BookList* list = (BookList*)List_Get(mem_bk->books_list, mem_bk->book_item);
        
        snwprintf(buf, MAXELEMS(buf), L"%s_0x%X.bin", list->book_name, size);
        int f = _fopen(L"/card/other", buf, FSX_O_WRONLY|FSX_O_TRUNC, FSX_S_IRUSR|FSX_S_IWUSR, 0);
        fwrite(f, (char*)list->book, size);
        fclose(f);
        
        snwprintf(buf, MAXELEMS(buf), L"book: %s\nsize: 0x%X", list->book_name, size);
      }
      else
      {
        char* str_buf = new char[len+1];
        wstr2strn(str_buf, text, len);
        size = stoh(str_buf, len);
        
        char* ram_buf = new char[size];
        OSBOOLEAN i = get_mem(current_process(), mem_bk->int_offset, ram_buf, size);
        
        snwprintf(buf, MAXELEMS(buf), L"0x%X_0x%X.bin", mem_bk->int_offset, size);
        int f = _fopen(L"/card/other", buf, FSX_O_WRONLY|FSX_O_TRUNC, FSX_S_IRUSR|FSX_S_IWUSR, 0);
        fwrite(f, ram_buf, size);
        fclose(f);
        
        delete str_buf;
        delete ram_buf;
        
        snwprintf(buf, MAXELEMS(buf), L"offset: 0x%X\nsize: 0x%X", mem_bk->int_offset, size);
      }
      
      MessageBox(EMPTY_TEXTID, TextID_Create(buf, ENC_UCS2, TEXTID_ANY_LEN), NOIMAGE, 1, 3000, 0);
      FreeBook((BOOK*)mem_bk);
    }
    else MessageBox(EMPTY_TEXTID, GetText(ILLEGAL), NOIMAGE, 1, 0, 0);
  }
  else MessageBox(EMPTY_TEXTID, GetText(ENTER_SIZE), NOIMAGE, 1, 0, 0);
}

void SizeInput_onBack(BOOK* book, wchar_t* text, int len)
{
  MemReaderBook* mem_bk = (MemReaderBook*)book;
  
  if (mem_bk->choice_item == 0)
    SetBook(mem_bk);
  else
    Offset_Select(mem_bk);
  
  GUI_DESTROY(mem_bk->gui_size);
}

void OffsetInput_onOk(BOOK* book, wchar_t* text, int len)
{
  if (len > 2)
  {
    if (text[0] == 0x30 && text[1] == 0x78)
    {
      text += 2;
      len -= 2;
    }
    
    if(!IllegalSymbol(text, len))
    {
      MemReaderBook* mem_bk = (MemReaderBook*)book;
      char* str_buf = new char[len+1];
      wstr2strn(str_buf, text, len);
      mem_bk->int_offset = stoh(str_buf, len);
      Book_Select((BOOK*)mem_bk, 0);
      GUI_DESTROY(mem_bk->gui_offset);
      delete str_buf;
    }
    else MessageBox(EMPTY_TEXTID, GetText(ILLEGAL), NOIMAGE, 1, 0, 0);
  }
  else MessageBox(EMPTY_TEXTID, GetText(ENTER_OFFSET), NOIMAGE, 1, 0, 0);
}

void OffsetInput_onBack(BOOK* book, wchar_t* text, int len)
{
  MemReaderBook* mem_bk = (MemReaderBook*)book;
  CreateChoise(0, (BOOK*)mem_bk);
  GUI_DESTROY(mem_bk->gui_offset);
}

void Book_Select(BOOK* book, GUI* gui)
{
  MemReaderBook* mem_bk = (MemReaderBook*)book;
  
  if (mem_bk->choice_item == 0)
    mem_bk->book_item = ListMenu_GetSelectedItem(mem_bk->gui_book);
  
  mem_bk->gui_size = CreateStringInputVA(0,
                                         VAR_BOOK((BOOK*)mem_bk),
                                         VAR_HEADER_TEXT(GetText(ENTER_SIZE)),
                                         VAR_STRINP_TEXT(GetText(OX)),
                                         VAR_STRINP_MODE(IT_STRING),
                                         VAR_STRINP_MAX_INT_VAL(10),
                                         VAR_OK_PROC(SizeInput_onOk),
                                         VAR_PREV_ACTION_PROC(SizeInput_onBack),
                                         0);
  GUIObject_SetStyle(mem_bk->gui_size, UI_OverlayStyle_InternalFrameSmall);
}

void Offset_Select(MemReaderBook* mem_bk)
{
  GUI_DESTROY(mem_bk->gui_offset);
  mem_bk->gui_offset = CreateStringInputVA(0,
                                         VAR_BOOK((BOOK*)mem_bk),
                                         VAR_HEADER_TEXT(GetText(ENTER_OFFSET)),
                                         VAR_STRINP_TEXT(GetText(OX)),
                                         VAR_STRINP_MODE(IT_STRING),
                                         VAR_STRINP_MAX_INT_VAL(10),
                                         VAR_OK_PROC(OffsetInput_onOk),
                                         VAR_PREV_ACTION_PROC(OffsetInput_onBack),
                                         0);
  GUIObject_SetStyle(mem_bk->gui_offset, UI_OverlayStyle_InternalFrameSmall);
}

void Book_Back(BOOK* book, GUI* gui)
{
   MemReaderBook* mem_bk = (MemReaderBook*)book;
   GUI_DESTROY(mem_bk->gui_book);
}

int Book_OnMessage(GUI_MESSAGE* msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
    case LISTMSG_GetItem:
      int item = GUIonMessage_GetCreatedItemIndex(msg);
      
      MemReaderBook* mem_bk = (MemReaderBook*)GUIonMessage_GetBook(msg);
      BookList* list = (BookList*)List_Get(mem_bk->books_list, item);
      wchar_t name[MAX_BOOK_NAME_LEN+1];
      str2wstr(name, list->book_name);
      
      GUIonMessage_SetMenuItemText(msg, TextID_Create(name, ENC_UCS2, TEXTID_ANY_LEN));
      break;
  }
  return 1;
}

void SetBook(MemReaderBook* mem_bk)
{
  GUI_DESTROY(mem_bk->gui_book);
  mem_bk->gui_book = CreateListMenu((BOOK*)mem_bk, 0);
  GUIObject_SetTitleText(mem_bk->gui_book, GetText(SELECT_BOOK));
  ListMenu_SetItemCount(mem_bk->gui_book, mem_bk->books_count);
  ListMenu_SetOnMessage(mem_bk->gui_book, Book_OnMessage);
  ListMenu_SetCursorToItem(mem_bk->gui_book, 0);
  ListMenu_SetItemStyle(mem_bk->gui_book, 0);
  GUIObject_SoftKeys_SetAction(mem_bk->gui_book, ACTION_SELECT1, Book_Select);
  GUIObject_SoftKeys_SetAction(mem_bk->gui_book, ACTION_BACK, Book_Back);
  GUIObject_Show(mem_bk->gui_book);
}

void Choise_Select(BOOK* book, GUI* gui)
{
  MemReaderBook* mem_bk = (MemReaderBook*)book;
  mem_bk->choice_item = ListMenu_GetSelectedItem(mem_bk->gui_sel);
  
  if (mem_bk->choice_item == 0)
  {
    GetBookList(mem_bk);
    SetBook(mem_bk);
  }
  if (mem_bk->choice_item == 1)
    Offset_Select(mem_bk);
}

void Choise_Back(BOOK* book, GUI* gui)
{
   MemReaderBook* mem_bk = (MemReaderBook*)book;
   GUI_DESTROY(mem_bk->gui_sel);
   FreeBook((BOOK*)mem_bk);
}

int Choise_OnMessage(GUI_MESSAGE* msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
    case LISTMSG_GetItem:
      int item = GUIonMessage_GetCreatedItemIndex(msg);
      GUIonMessage_SetMenuItemText(msg, GetText(item));
      break;
  }
  return 1;
}

int CreateChoise(void* data, BOOK* book)
{
  MemReaderBook* mem_bk = (MemReaderBook*)book;
  GUI_DESTROY(mem_bk->gui_sel);
  mem_bk->gui_sel = CreateListMenu((BOOK*)mem_bk, 0);
  GUIObject_SetTitleText(mem_bk->gui_sel, GetText(MODE));
  ListMenu_SetItemCount(mem_bk->gui_sel, 2);
  ListMenu_SetOnMessage(mem_bk->gui_sel, Choise_OnMessage);
  ListMenu_SetCursorToItem(mem_bk->gui_sel, 0);
  ListMenu_SetItemStyle(mem_bk->gui_sel, 0);
  GUIObject_SoftKeys_SetAction(mem_bk->gui_sel, ACTION_SELECT1, Choise_Select);
  GUIObject_SoftKeys_SetAction(mem_bk->gui_sel, ACTION_BACK, Choise_Back);
  GUIObject_Show(mem_bk->gui_sel);
  return 1;
}

int TerminateElf(void*, BOOK* book)
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

const PAGE_MSG base_evlist[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, TerminateElf,
  ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
  NIL_EVENT_TAG, NULL
};

const PAGE_MSG main_evlist[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG, CreateChoise,
  NIL_EVENT_TAG, NULL
};

PAGE_DESC MemReader_Base_Page = {"MemReader_Base_Page", NULL, base_evlist};
PAGE_DESC MemReader_Main_Page = {"MemReader_Main_Page", NULL, main_evlist};

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void MemReaderBook_onClose(BOOK* book)
{
  MemReaderBook* mem_bk = (MemReaderBook*)book;
  FreeBookList(mem_bk);
  
  GUI_DESTROY(mem_bk->gui_sel);
  GUI_DESTROY(mem_bk->gui_book);
  GUI_DESTROY(mem_bk->gui_size);
  GUI_DESTROY(mem_bk->gui_offset);
  
  SUBPROC(elf_exit);
}

int main (void)
{
  trace_init();
  MemReaderBook* mem_bk = new MemReaderBook;
  memset((BOOK*)mem_bk, NULL, sizeof(MemReaderBook));
  if(!CreateBook((BOOK*)mem_bk, MemReaderBook_onClose, &MemReader_Base_Page, BOOK_NAME, -1, 0))
  {
    delete mem_bk;
    SUBPROC(elf_exit);
    return 0;
  }
  BookObj_GotoPage((BOOK*)mem_bk, &MemReader_Main_Page);
  return 0;
}
