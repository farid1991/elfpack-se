#include "main.h"

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

TEXTID *lng_textid;

int *dc_offsets;
int offs_start;
char *noffs;

int *buf;
int *buf_const;
int offset;

const PAGE_MSG DynEditorBook_EventList_Base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT     ,onExit,
  ELF_SHOW_INFO_EVENT     ,onAbout,
  NIL_EVENT_TAG           ,NULL
};

const PAGE_MSG DynEditorBook_EventList_Main[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG    ,CreateMenuGUI_MainPage,
  NIL_EVENT_TAG           ,NULL
};

const PAGE_DESC DynEditorBook_BasePage = {"DynEditor_BasePage",0,DynEditorBook_EventList_Base};
const PAGE_DESC DynEditorBook_MainPage = {"DynEditor_MainPage",0,DynEditorBook_EventList_Main};

int isRu()
{
  if(!strcmp(lang_get_name(get_system_langID()),"ru")) return(1);
  else return(0);
}

int wstr2h(wchar_t *ws, int len)
{
  wchar_t w;
  int res=0;
  while(len--)
  {
    w=*ws++;
    if ((w>=0x30) && (w<0x40)) w-=0x30;
    else if((w>=0x41) && (w<0x47)) w-=0x37;
    else if((w>=0x61) && (w<0x67)) w-=0x57;
    res<<=4;
    res|=w;
  }
  return(res);
}

int str2h(char *str, int len)
{
  char s;
  int res=0;
  while(len--)
  {
    s=*str++;
    if ((s>=0x30) && (s<0x40)) s-=0x30;
    else if((s>=0x41) && (s<0x47)) s-=0x37;
    else if((s>=0x61) && (s<0x67)) s-=0x57;
    res<<=4;
    res|=s;
  }
  return(res);
}

int CreateFolderIfNo(wchar_t *folder)
{//вернёт 0 -если папка есть, 1 -если создана
  FSTAT fst;
  if(!fstat(folder,0,&fst)) return(0);
  else mkdir(folder);
  return(1);
}

int FileBackup(wchar_t *src_path, wchar_t *src_name, wchar_t *dest_path, wchar_t *dest_name, int mode)
{//вернёт 0 -если бэкап сделан, 1 -если файл бэкапа существует (mode=0)
  //mode
  //0 - копировать, если файла нет
  //1 - копировать с заменой
  int err;
  FSTAT fst;
  if(!fstat(dest_path,dest_name,&fst))
  {
    if(mode)
    {
      FileDelete(dest_path,dest_name,&err);
      goto FCOPY;
    }
    return(1);
  }
  else
  {
  FCOPY:
    FileCopy(src_path,src_name,dest_path,dest_name,0);
    int book_id = MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[DYN_CONST_BACKUP_OK]),NOIMAGE,1,0,0);
    //BOOK *msg_book = FindBookByID(book_id);
    return(0);
  }
}

int IllegalSymbol(wchar_t *wstr, int len)
{
  wchar_t s;
  for(;;)
  {
    s=*wstr++;
    if(((s>='0') && (s<='9'))||((s>='a') && (s<='f'))||((s>='A') && (s<='F'))) len--;
    else break;
  }
  return(len);
}

int lng_textid_free(int *txt)
{
  for(int i=0;i!=NUM_TEXT;i++)
  {
    if(txt[i]!=EMPTY_TEXTID) TEXTID_DESTROY(txt[i]);
  }
  return(0);
}

void LNG_TEXTID_FREE(TEXTID *t)
{
  if(t) t = (TEXTID *)lng_textid_free(t);
}


void DynEditorBook_CloseAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  FILE_CLOSE(DynEditorBook->dyn_file);

  LIST_DESTROY(&DynEditorBook->phone_sw_h,FREE_ELEMENTS);
  LIST_DESTROY(&DynEditorBook->names_of_constants,FREE_ELEMENTS);

  LNG_TEXTID_FREE(lng_textid);
  FREE(lng_textid);

  GUI_DESTROY(DynEditorBook->menu);
  GUI_DESTROY(DynEditorBook->secGUI);

  FreeBook(book);
}

void back_e(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  GUI_DESTROY(DynEditorBook->secGUI);
}

void About(BOOK *book, GUI *)
{
  char *info = ELFNAME "\n" VERSION "\n" AUTHOR;
  TEXTID tid = TextID_Create(info,ENC_LAT1,strlen(info));
  MessageBox(EMPTY_TEXTID,tid,NOIMAGE,1,0,book);
}

void reg_lngru(BOOK *book)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  DynEditorBook->flag_lg = RUS_LNG;
  LNG_TEXTID_FREE(lng_textid);

  lng_textid[ENTER_CONSTANT] = STR("Введите константу");
  lng_textid[ENTER_OFFSET] = STR("Введите смещение");
  lng_textid[CHANGE_CONSTANT] = STR("Изменить константу");
  lng_textid[GO_TO_OFFSET] = STR("Перейти к смещению");
  lng_textid[GO_TO_END] = STR("Перейти в конец");
  lng_textid[_LANGUAGE] = STR("Язык");
  lng_textid[ABOUT_ELF] = STR("О эльфе");

  lng_textid[Q_REPLACE_CONSTANT] = STR("Заменить константу?");
  lng_textid[ADD_CHANGE_CONSTANT] = STR("Добавить/изменить константу");
  lng_textid[VIEWING_DYN] = STR("Просмотр DYN");
  lng_textid[REST_DYN] = STR("Восст. DYN");
  lng_textid[SAVE_DYN] = STR("Сохр. DYN");
  lng_textid[COLLECT_DYN] = STR("Собрать DYN");
  lng_textid[EXIT] = STR("Выход");

  lng_textid[DYN_CONST_BACKUP_OK] = STR("DYN_CONST копирование... ok!");
  lng_textid[DOES_NOT_EXIST] = STR("Не существует");
  lng_textid[ENTER_WITH_0x] = STR("Ввод с 0x");
  lng_textid[Q_CHANGE_CONSTANT] = STR("Изменить константу?");
  lng_textid[DYN_CONST_CREATED] = STR("DYN_CONST создан");
  lng_textid[NO_DYN_TAG_H] = STR("Нет DYN_tag.h");
  lng_textid[DYN_IS_RESTORED] = STR("DYN восстановлен");
  lng_textid[NO_FILES] = STR("Нет файлов");
  lng_textid[NO_FOLDER_DYN_EDITOR] = STR("Нет папки DynEditor");
  lng_textid[INACCESSIBLE] = STR("Недоступно");
  lng_textid[ILLEGAL_CHARACTERS] = STR("Введены недопустимые символы");
  lng_textid[NAMES] = STR("Имена");
  lng_textid[OFFSETS] = STR("Смещения");
  lng_textid[NEW_CONSTANT] = STR("Новая константа");
  lng_textid[DELETE_CONSTANT] = STR("Удалить");
  lng_textid[INFO] = STR("Инфо о DYN_CONST");
}

void reg_lngen(BOOK *book)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  DynEditorBook->flag_lg = OTHER_LNG;

  LNG_TEXTID_FREE(lng_textid);

  lng_textid[ENTER_CONSTANT] = STR("Enter constant");
  lng_textid[ENTER_OFFSET] = STR("Enter offset");
  lng_textid[CHANGE_CONSTANT] = STR("Change constant");
  lng_textid[GO_TO_OFFSET] = STR("Pass to offset");
  lng_textid[GO_TO_END] = STR("Go to end");
  lng_textid[_LANGUAGE] = STR("Language");
  lng_textid[ABOUT_ELF] = STR("About elf");

  lng_textid[Q_REPLACE_CONSTANT] = STR("Replace constant?");
  lng_textid[ADD_CHANGE_CONSTANT] = STR("Add/change constant");
  lng_textid[VIEWING_DYN] = STR("Viewing DYN");
  lng_textid[REST_DYN] = STR("Rest. DYN");
  lng_textid[SAVE_DYN] = STR("Save DYN");
  lng_textid[COLLECT_DYN] = STR("Collect DYN");
  lng_textid[EXIT] = STR("Exit");

  lng_textid[DYN_CONST_BACKUP_OK] = STR("DYN_CONST backup... ok!");
  lng_textid[DOES_NOT_EXIST] = STR("Does Not exist");
  lng_textid[ENTER_WITH_0x] = STR("Enter with 0x");
  lng_textid[Q_CHANGE_CONSTANT] = STR("Change constant?");
  lng_textid[DYN_CONST_CREATED] = STR("DYN_CONST created");
  lng_textid[NO_DYN_TAG_H] = STR("No DYN_tag.h");
  lng_textid[DYN_IS_RESTORED] = STR("DYN is restored");
  lng_textid[NO_FILES] = STR("No files");
  lng_textid[NO_FOLDER_DYN_EDITOR] = STR("No folder DynEditor");
  lng_textid[INACCESSIBLE] = STR("INACCESSIBLE");
  lng_textid[ILLEGAL_CHARACTERS] = STR("Illegal characters are entered");
  lng_textid[NAMES] = STR("Names");
  lng_textid[OFFSETS] = STR("Offsets");

  lng_textid[NEW_CONSTANT] = STR("New constant");
  lng_textid[DELETE_CONSTANT] = STR("Delete");

  lng_textid[INFO] = STR("Info about DYN_CONST");
}

int onAbout(void *mess, BOOK *book)
{
  char *info = ELFNAME "\n" VERSION "\n" AUTHOR;
  TEXTID tid = TextID_Create(info,ENC_LAT1,strlen(info));
  MessageBox(EMPTY_TEXTID,tid,NOIMAGE,1,0,*(BOOK **)mess);
  return(1);
}

int onExit(void *, BOOK *book)
{
  DynEditorBook_CloseAction(book,0);
  return(1);
}

int ViewingDyn_PageExit(void *, BOOK *book)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  DynEditorBook->menu_item = 0;
  FREE(dc_offsets);
  FREE(noffs);
  FREE(buf_const);
  LIST_DESTROY(&DynEditorBook->names_of_constants,FREE_ELEMENTS);
  //GUI_DESTROY(DynEditorBook->menu);
  BookObj_GotoPage(book,&DynEditorBook_MainPage);
  return(1);
}

void ViewingDyn_BackAction(BOOK *book, GUI *)
{
  //BookObj_ReturnPage(book,PAGE_EXIT_EVENT_TAG);
  ViewingDyn_PageExit(0,book);
}

int ViewingDyn_onMessage(GUI_MESSAGE *msg)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)GUIonMessage_GetBook(msg);

  for(int i = 0;i<=0x11;i++)
  {
    TEXTID_DESTROY(dc_offsets[i]);
  }

  int items = 0x10;
  int first_const_last_list = (DynEditorBook->dyn_size/4-1)/0x10;
  first_const_last_list *= 0x10;

  if( first_const_last_list == offs_start )
    items = DynEditorBook->dyn_size/4 - offs_start;

  //dc_offsets[0x11] = 0;
  for( int i=0 ; i<items ; i++ ) //получаем смещения/имена констант для пунктов
  {
    if(DynEditorBook->offsets == 1)
    {
      sprintf(noffs,"0x%X",i+offs_start);
      dc_offsets[i] = TextID_Create(noffs,ENC_LAT1,strlen(noffs));
    }
    else
    {
      wchar_t *name_const;

      if(DynEditorBook->names_of_constants && (i+offs_start)<(DynEditorBook->names_of_constants->FirstFree))
        name_const = (wchar_t *)List_Get(DynEditorBook->names_of_constants,i+offs_start);
      else
        name_const = L"UNDEFINED";

      dc_offsets[i] = TextID_Create(name_const,ENC_UCS2,TEXTID_ANY_LEN);
    }
  }

  if( items == 0x10 ) dc_offsets[0x10] = STR("-->");

  if( (offs_start != 0) && (offs_start != first_const_last_list) )
  {
    dc_offsets[0x11] = STR("<--");
    ListMenu_SetItemCount(DynEditorBook->menu,0x10+2);
  }
  else if( offs_start == first_const_last_list )
  {
    TextID_Destroy(dc_offsets[items]);
    TextID_Destroy(dc_offsets[items+1]);

    dc_offsets[items] = TextID_Copy(lng_textid[NEW_CONSTANT]);
    dc_offsets[items+1] = STR("<--");
    ListMenu_SetItemCount(DynEditorBook->menu,items+1+1);
    if(ListMenu_GetSelectedItem(DynEditorBook->menu) == (items-1))
      GUIObject_SoftKeys_SetVisible(DynEditorBook->menu,5,1);
    else
      GUIObject_SoftKeys_SetVisible(DynEditorBook->menu,5,0);
  }
  else ListMenu_SetItemCount(DynEditorBook->menu,0x10+1);

  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int item = GUIonMessage_GetCreatedItemIndex(msg);
    GUIonMessage_SetMenuItemText(msg,TextID_Copy(dc_offsets[item]));

    if(!(item == 0x10 || item == 0x11 || (offs_start == first_const_last_list && item == items+1)))
    {
      int offs_num = offs_start + item;
      int dc_const = buf_const[offs_num];
      int len = strlen(UNDEFINED);
      char offset[10+1];


      if( (offs_start == first_const_last_list) && (item == items) )
        sprintf(offset,"0x%X",offs_num);
      else
        sprintf(offset,"0x%X",dc_const);

      TEXTID id = TextID_Create(offset,ENC_LAT1,strlen(offset));
      GUIonMessage_SetMenuItemSecondLineText(msg,id);
      //FREE(offset);
    }
    break;
  }

  for(int i = 0;i<ListMenu_GetItemCount(DynEditorBook->menu);i++)
  {
    TEXTID_DESTROY(dc_offsets[i]);
  }

  return(1);
};

//GUI *BookObj_GetGUIObject(BOOK *b)
//{
//  return((GUI *)List_Get(b->xguilist->guilist,0));
//}

void ViewingDyn_SelectItemAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  int item = ListMenu_GetSelectedItem(DynEditorBook->menu);

  int first_const_last_list = (DynEditorBook->dyn_size/4-1)/0x10;
  first_const_last_list *= 0x10;

  int items = (DynEditorBook->dyn_size/4)-first_const_last_list;

  if((first_const_last_list != offs_start) && item==0x10)
  {
    offs_start+=0x10;
    DynEditorBook->menu_item = 0;
    BookObj_GotoPage(book,&ViewingDyn_MainPage);
  }
  else if(((first_const_last_list != offs_start) && item==0x11) ||
    ((first_const_last_list == offs_start) && item==(items+1)))
  {
    offs_start-=0x10;
    DynEditorBook->menu_item = 0;
    BookObj_GotoPage(book,&ViewingDyn_MainPage);
  }
  else if( (first_const_last_list == offs_start) && (item == items) )
  {
    DynEditorBook->menu_item = item;
    wchar_t ent_offset[15+1];
    snwprintf(ent_offset,0xFF,L"0x%X",offs_start+item);
    StringInput_EnterOffsetAction(book, ent_offset, wstrlen(ent_offset));
  }
  else if(((first_const_last_list != offs_start) && item>=0x0 && item<0x10) ||
     ((first_const_last_list == offs_start) && item>=0x0 && item<items))
  {

    int offs_num = offs_start+item;
    int dc_const = buf_const[offs_num];

    char header[18];
    sprintf(header,"Offset 0x%X",offs_num);
    wchar_t *cname;
    if(DynEditorBook->names_of_constants && offs_num<(DynEditorBook->names_of_constants->FirstFree))
      cname = (wchar_t *)List_Get(DynEditorBook->names_of_constants,offs_num);
    else cname = L"------";
    wchar_t *body = new wchar_t[wstrlen(cname)+23+1];
    if(dc_const==0xFFFFFFFF) snwprintf(body,wstrlen(cname)+23,L"%ls\nUNDEFINED (0x%X)",cname,dc_const);
    else snwprintf(body,wstrlen(cname)+12,L"%ls\n0x%X",cname,dc_const);
    TEXTID bodyID = TextID_Create(body,ENC_UCS2,TEXTID_ANY_LEN);
    TEXTID headerID = TextID_Create(header,ENC_LAT1,strlen(header));

    int mbox_bookid = MessageBox(headerID,bodyID,NOIMAGE,2,0,book);
    //GUI *mbox_gui = BookObj_GetGUIObject(FindBookByID(mbox_bookid));

    //GUIObject_SoftKeys_SetAction(mbox_gui,0,ViewingDyn_EditConstantAction);
    //GUIObject_SoftKeys_SetText(mbox_gui,0,TextID_Copy(lng_textid[change_constant]));

    FREE(body);
    //FREE(header);
  }
}

void ViewingDyn_GoToOffset_EnterAction(BOOK *book, wchar_t *txt, int len)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  if(*txt=='0' && *(txt+1)=='x')
  {
    if(*(txt+2))
    {
      txt+=2;
      len-=2;
      if(!IllegalSymbol(txt,len))
      {
        int offs = wstr2h(txt,len);
        if(offs>=DynEditorBook->dyn_size/4)
          MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[DOES_NOT_EXIST]),NOIMAGE,1,0,0);
        else
        {
          int sh = offs/0x10;
          offs_start = sh*0x10;
          DynEditorBook->menu_item = offs - offs_start;
        }
        BookObj_GotoPage(book,&ViewingDyn_MainPage);
      }
      else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ILLEGAL_CHARACTERS]),NOIMAGE,1,0,0);
    }
    else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_OFFSET]),NOIMAGE,1,0,0);
  }
  else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_WITH_0x]),NOIMAGE,1,0,0);
}

void ViewingDyn_GoToOffsetAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  DynEditorBook->secGUI = CreateStringInputVA(0,
                             VAR_BOOK(book),
                             VAR_HEADER_TEXT(TextID_Copy(lng_textid[ENTER_OFFSET])),
                             VAR_STRINP_TEXT(STR("0x")),
                             VAR_STRINP_MODE(11),
                             VAR_OK_PROC(ViewingDyn_GoToOffset_EnterAction),
                             0);
  GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_BACK, back_e);
}

void ViewingDyn_GoToEndOffsetAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  int sh = ((DynEditorBook->dyn_size/4-1)/0x10);
  offs_start = sh*0x10;

  DynEditorBook->menu_item = 0;
  BookObj_GotoPage(book,&ViewingDyn_MainPage);
}

void VD_QuestionReplace_YesPressAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  int item = ListMenu_GetSelectedItem(DynEditorBook->menu);
  lseek(DynEditorBook->dyn_file,(item+offs_start)*4,0);
  buf_const[item+offs_start] = DynEditorBook->constant;
  fwrite(DynEditorBook->dyn_file,&DynEditorBook->constant,4);
  lseek(DynEditorBook->dyn_file,0,0);
  GUI_DESTROY(DynEditorBook->secGUI);
  DynEditorBook->menu_item = item;
  BookObj_GotoPage(book,&ViewingDyn_MainPage);
}

void VD_QuestionReplace_NoPressAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  GUI_DESTROY(DynEditorBook->secGUI);
}

void ViewingDyn_EditConstant_ReplaceConstantAction(BOOK *book, wchar_t *txt, int len)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  if(*txt=='0' && *(txt+1)=='x')
  {
    if(*(txt+2))
    {
      txt+=2;
      len-=2;
      if(!IllegalSymbol(txt,len))
      {
        int _const = DynEditorBook->constant = wstr2h(txt,len);
        int item = ListMenu_GetSelectedItem(DynEditorBook->menu);

        GUI_DESTROY(DynEditorBook->secGUI);

        if(buf_const[item+offs_start] != _const)
        {
          DynEditorBook->secGUI = CreateYesNoQuestionVA(0,
                                    VAR_BOOK(book),
                                    VAR_YESNO_PRE_QUESTION(TextID_Copy(lng_textid[Q_CHANGE_CONSTANT])),
                                    0);
          GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_YES, VD_QuestionReplace_YesPressAction);
          GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_NO, VD_QuestionReplace_NoPressAction);
          GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_BACK, VD_QuestionReplace_NoPressAction);
        }
      }
      else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ILLEGAL_CHARACTERS]),NOIMAGE,1,0,0);
    }
    else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_CONSTANT]),NOIMAGE,1,0,0);
  }
  else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_WITH_0x]),NOIMAGE,1,0,0);
}

void ViewingDyn_EditConstantAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  int end_item = (DynEditorBook->dyn_size/4)-offs_start + 1;
  int item = ListMenu_GetSelectedItem(DynEditorBook->menu);
  if(item==0x10 || item==0x11 || item==end_item)
    MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[INACCESSIBLE]),NOIMAGE,1,0,0);
  else
  {
    char dc_c[13];
    int dc_const = buf_const[item+offs_start];
    TEXTID text;
    if(dc_const==0xFFFFFFFF) text = STR(UNDEFINED);//STR("undefined");
    else
    {
      sprintf(dc_c,"0x%X",dc_const);
      text = TextID_Create(dc_c,ENC_LAT1,strlen(dc_c));
    }
    DynEditorBook->secGUI = CreateStringInputVA(0,
                               VAR_BOOK(book),
                               VAR_HEADER_TEXT(TextID_Copy(lng_textid[0])),
                               VAR_STRINP_MODE(11),
                               VAR_OK_PROC(ViewingDyn_EditConstant_ReplaceConstantAction),
                               VAR_STRINP_TEXT(text),
                               VAR_STRINP_MAX_LEN(10),
                               0);
    GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI,ACTION_BACK, back_e);
    //TEXTID_DESTROY(text);
    //FREE(dc_c);
  }
}

void ViewingDyn_SetNamesAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  DynEditorBook->offsets = 0;
  DynEditorBook->menu_item = ListMenu_GetSelectedItem(DynEditorBook->menu);
  BookObj_GotoPage(book,&ViewingDyn_MainPage);
}

void ViewingDyn_SetOffsetsAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  DynEditorBook->offsets = 1;
  DynEditorBook->menu_item = ListMenu_GetSelectedItem(DynEditorBook->menu);
  BookObj_GotoPage(book,&ViewingDyn_MainPage);
}

void ViewingDyn_DeleteAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  int item = ListMenu_GetSelectedItem(DynEditorBook->menu);

  DynEditorBook->menu_item = item;
  DynEditorBook->constant = 0xFFFFFFFF;
  VD_QuestionReplace_YesPressAction(book, gui);
}

void ViewingDyn_DeleteEndConstantAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  //FREE(buf_const);

  DynEditorBook->dyn_size -= 4;

  int *__buf_const = new int[DynEditorBook->dyn_size/4];

  memcpy(__buf_const, buf_const, DynEditorBook->dyn_size);
  FREE(buf_const);
  buf_const = __buf_const;

  FILE_CLOSE(DynEditorBook->dyn_file);

  int err;
  FileDelete(CONFIG_INTERNAL,DYN_CONST,&err);
  FILE_CREATE(DynEditorBook->dyn_file,CONFIG_INTERNAL,DYN_CONST);

  fwrite(DynEditorBook->dyn_file,buf_const,DynEditorBook->dyn_size);
  lseek(DynEditorBook->dyn_file,0,SEEK_SET);

  BookObj_GotoPage(book,&ViewingDyn_MainPage);
}

int ViewingDyn_PageEnter(void *, BOOK *book)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  GUI_DESTROY(DynEditorBook->secGUI);
  GUI_DESTROY(DynEditorBook->menu);

  FREE(dc_offsets);
  dc_offsets = new int[0x12];
  for(int i = 0;i<=0x11;i++)
  {
    dc_offsets[i] = EMPTY_TEXTID;
  }

  GUI *menu = DynEditorBook->menu = CreateListMenu(book,0);
  GUIObject_SetTitleText(menu,TextID_Copy(lng_textid[VIEWING_DYN]));
  //ListMenu_SetItemCount(menu,0x11);
  ListMenu_SetItemStyle( menu, 3 );
  ListMenu_SetCursorToItem(menu,DynEditorBook->menu_item);
  ListMenu_SetOnMessage(menu,ViewingDyn_onMessage);
  GUIObject_SoftKeys_SetAction(menu,ACTION_BACK, ViewingDyn_BackAction);
  GUIObject_SoftKeys_SetAction(menu,ACTION_SELECT1,ViewingDyn_SelectItemAction);

  GUIObject_SoftKeys_SetAction(menu,ACTION_DELETE, ViewingDyn_DeleteAction);
  GUIObject_SoftKeys_SetVisible(menu, ACTION_DELETE, 0);


  if(DynEditorBook->offsets)
  {
    GUIObject_SoftKeys_SetText(menu,4,TextID_Copy(lng_textid[NAMES]));
    GUIObject_SoftKeys_SetAction(menu,4,ViewingDyn_SetNamesAction);
    if(!DynEditorBook->names_of_constants || !DynEditorBook->names_of_constants->FirstFree)
    {
      GUIObject_SoftKeys_SetEnable(menu,4,0);
      GUIObject_SoftKeys_AddErrorStr(menu,4,TextID_Copy(lng_textid[NO_DYN_TAG_H]));
    }
    else GUIObject_SoftKeys_SetEnable(menu,4,1);
  }
  else
  {
    GUIObject_SoftKeys_SetText(menu,4,TextID_Copy(lng_textid[OFFSETS]));
    GUIObject_SoftKeys_SetAction(menu,4,ViewingDyn_SetOffsetsAction);
  }

  GUIObject_SoftKeys_SetAction(menu,1,ViewingDyn_GoToOffsetAction);
  GUIObject_SoftKeys_SetAction(menu,2,ViewingDyn_EditConstantAction);
  GUIObject_SoftKeys_SetAction(menu,3,ViewingDyn_GoToEndOffsetAction);

  GUIObject_SoftKeys_SetText(menu,1,TextID_Copy(lng_textid[GO_TO_OFFSET]));
  GUIObject_SoftKeys_SetText(menu,2,TextID_Copy(lng_textid[CHANGE_CONSTANT]));
  GUIObject_SoftKeys_SetText(menu,3,TextID_Copy(lng_textid[GO_TO_END]));

  GUIObject_SoftKeys_SetAction(menu,5,ViewingDyn_DeleteEndConstantAction);
  GUIObject_SoftKeys_SetText(menu,5,TextID_Copy(lng_textid[DELETE_CONSTANT]));
  GUIObject_SoftKeys_SetVisible(menu,5,0);

  GUIObject_Show(menu);
  return(1);
}

void DynEditorBook_OnClose(BOOK *book)
{
  SUBPROC(elf_exit);
}

int Menu_onMessage(GUI_MESSAGE *msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int item = GUIonMessage_GetCreatedItemIndex(msg);
    GUIonMessage_SetMenuItemText(msg,TextID_Copy(lng_textid[item+ADD_CHANGE_CONSTANT]));
    break;
  }
  return(1);
}

void QuestionReplace_YesPressAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  GUI_DESTROY(DynEditorBook->secGUI);
  fwrite(DynEditorBook->dyn_file,&(DynEditorBook->constant),4);
  lseek(DynEditorBook->dyn_file,0,0);
}

void QuestionReplace_NoPressAction(BOOK *book, GUI *)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  lseek(DynEditorBook->dyn_file,0,0);
  GUI_DESTROY(DynEditorBook->secGUI);
}

void StringInput_AddConstAction(BOOK *book, wchar_t *txt, int len)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  if(*txt=='0' && *(txt+1)=='x')
  {
    if(*(txt+2))
    {
      txt+=2;
      len-=2;
      if(!IllegalSymbol(txt,len))
      {
        FSTAT stat;
        int _const = DynEditorBook->constant = wstr2h(txt,len);
        GUI_DESTROY(DynEditorBook->secGUI);
        if(!buf)
        {
          if(buf_const[offset] != 0xFFFFFFFF)
          {
            char *gues = new char[34];
            sprintf(gues,"new: 0x%X\nold: 0x%X",_const,buf_const[offset]);
            TEXTID text_gues = TextID_Create(gues,ENC_LAT1,strlen(gues));

            DynEditorBook->secGUI = CreateYesNoQuestionVA(0,
                                                        VAR_BOOK(book),
                                                        VAR_YESNO_QUESTION(text_gues),
                                                        VAR_YESNO_PRE_QUESTION(TextID_Copy(lng_textid[Q_REPLACE_CONSTANT])),
                                                        0);
            GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_YES, QuestionReplace_YesPressAction);
            GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_NO, QuestionReplace_NoPressAction);
            GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_BACK, QuestionReplace_NoPressAction);
            FREE(gues);
          }
          else
          {
            fwrite(DynEditorBook->dyn_file,&_const,4);
            lseek(DynEditorBook->dyn_file,0,0);
          }
        }
        else
        {
          buf[offset-DynEditorBook->dyn_size/4] = _const;
          fwrite(DynEditorBook->dyn_file,buf,offset*4-DynEditorBook->dyn_size+4);
          FREE(buf);
          FILE_CLOSE( DynEditorBook->dyn_file );
          FILE_OPEN_RW( DynEditorBook->dyn_file, CONFIG_INTERNAL, DYN_CONST );

          fstat(CONFIG_INTERNAL,DYN_CONST,&stat);
          DynEditorBook->dyn_size = stat.fsize;
        }
        FREE(buf_const);
      }
      else
        MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ILLEGAL_CHARACTERS]),NOIMAGE,1,0,0);
    }
    else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_CONSTANT]),NOIMAGE,1,0,0);
  }
  else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_WITH_0x]),NOIMAGE,1,0,0);

  if(book->current_page == &ViewingDyn_MainPage)
  {
    if(DynEditorBook->menu_item == 0x10)
    {
      DynEditorBook->menu_item = 0;
      offs_start+=0x10;
    }

    FILE_CLOSE( DynEditorBook->dyn_file );
    FILE_OPEN_RW( DynEditorBook->dyn_file, CONFIG_INTERNAL, DYN_CONST );

    //DynEditorBook->offsets = 1;

    FREE(noffs);
    FREE(buf_const);

    //dc_offsets = 0;
    //offs_start = 0;
    noffs = new char[11];
    buf_const = new int[DynEditorBook->dyn_size/4];
    fread(DynEditorBook->dyn_file,buf_const,DynEditorBook->dyn_size);
    lseek(DynEditorBook->dyn_file,0,SEEK_SET);

    DynEditor_GetNamesOfConstants(DynEditorBook);
    BookObj_GotoPage(book,&ViewingDyn_MainPage);
  }
}

void StringInput_EnterOffsetAction(BOOK *book, wchar_t *txt, int len)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  if(*txt=='0' && *(txt+1)=='x')
  {
    if(*(txt+2))
    {
      txt+=2;
      len-=2;
      if(!IllegalSymbol(txt,len))
      {
        offset = wstr2h(txt,len);
        FILE_CLOSE( DynEditorBook->dyn_file );
        FILE_OPEN_RW( DynEditorBook->dyn_file, CONFIG_INTERNAL, DYN_CONST );

        FREE(buf_const);

        buf_const = new int[DynEditorBook->dyn_size/4];
        fread(DynEditorBook->dyn_file,buf_const,DynEditorBook->dyn_size);
        if(offset*4 >= DynEditorBook->dyn_size)
        {
          lseek(DynEditorBook->dyn_file,DynEditorBook->dyn_size,0);
          FREE(buf);
          buf = new int[offset-DynEditorBook->dyn_size/4+1];
          memset(buf,0xFF,offset*4-DynEditorBook->dyn_size+4);
        }
        else
        {
          buf = 0;
          lseek(DynEditorBook->dyn_file,offset*4,0);
        }

        char offs[18];
        sprintf(offs,"Offset 0x%X",offset);
        TEXTID text_offs = TextID_Create(offs,ENC_LAT1,strlen(offs));
        GUI_DESTROY(DynEditorBook->secGUI);
        DynEditorBook->secGUI = CreateStringInputVA(0,
                                      VAR_BOOK(book),
                                      VAR_HEADER_TEXT(text_offs),
                                      VAR_STRINP_MODE(11),
                                      VAR_OK_PROC(StringInput_AddConstAction),
                                      VAR_STRINP_TEXT(STR("0x")),
                                      VAR_STRINP_MAX_LEN(10),
                                      0);
        GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI, ACTION_BACK, back_e);
        //FREE(offs);
      }
      else
        MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ILLEGAL_CHARACTERS]),NOIMAGE,1,0,0);
    }
    else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_OFFSET]),NOIMAGE,1,0,0);
  }
  else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[ENTER_WITH_0x]),NOIMAGE,1,0,0);
}

//--------------------------------------------ok

void CollectDyn_ExitAction(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  for(int i = 0;i<DynEditorBook->phone_sw_h->FirstFree;i++)
  {
    TEXTID_DESTROY(dc_offsets[i]);
  }

  FREE(dc_offsets);
  LIST_DESTROY(&DynEditorBook->phone_sw_h,FREE_ELEMENTS);
  GUI_DESTROY(DynEditorBook->menu);
  BookObj_GotoPage(book,&DynEditorBook_MainPage);
}

int CollectDyn_onMessage(GUI_MESSAGE *msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int item = GUIonMessage_GetCreatedItemIndex(msg);
    GUIonMessage_SetMenuItemText(msg,TextID_Copy(dc_offsets[item]));
    break;
  }
  return(1);
}

void CollectDyn_SelectItemAction(BOOK *book, GUI *collect_dyn_menu)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  LIST *list_offsets_const;
  LIST *list_importances_const;

  int len = wstrlen(CONFIG_INTERNAL)+wstrlen(L"/DynEditor")+30;
  wchar_t *de_conf = new wchar_t[len+1];
  memset(de_conf,0,len);
  wstrcpy(de_conf,CONFIG_INTERNAL);
  wstrcat(de_conf,L"/DynEditor");

  GUI *menu = DynEditorBook->menu;

  int item = ListMenu_GetSelectedItem(menu);

  wchar_t fname[29];
  TextID_GetWString(dc_offsets[item],fname,28);
  wstrcat(fname,L".h");

  FSTAT stat, stat2;
  if(!fstat(de_conf,fname,&stat))
  {
    if(!fstat(de_conf,L"DYN_tag.h",&stat2))
    {
      int phone_h_file;
      FILE_OPEN_RW( phone_h_file, de_conf, fname );
      char *phone_h = new char[stat.fsize+1];
      fread(phone_h_file,phone_h,stat.fsize);
      phone_h[stat.fsize] = 0;
      FILE_CLOSE( phone_h_file );

      int dyn_tag_h_file;
      FILE_OPEN_RW( dyn_tag_h_file, de_conf, L"DYN_tag.h" );
      char *tagbuf = new char[stat2.fsize+1];
      fread(dyn_tag_h_file,tagbuf,stat2.fsize);
      tagbuf[stat2.fsize] = 0;
      FILE_CLOSE( dyn_tag_h_file );

      DynEditorBook->names_of_constants = List_Create();
      list_importances_const = List_Create();
      list_offsets_const = List_Create();

      char *phone_h_cpy = phone_h;

      char *str = strstr(tagbuf,"#define ");
      for(;;)
      {
        char *str2 = strstr(str+1,"#define ");
        if(!str2) break;
        char *name_const = str2+8;
        char *str_tag = strstr(str2,"_TAG ");
        *str_tag=0;
        int *importance_const;

  L_BACK:
        char *str3 = strstr(phone_h,name_const);
        if(str3)
        {
          if(*(str3-1) != ' ')
          {
            phone_h = str3+1;
            goto L_BACK;
          }

          char *str_importance_const = str3+strlen(name_const)+1;
          char *st = strstr(str_importance_const,"\r\n");

          char rest = *st;
          *st = 0;

          char *stx = strstr(str_importance_const," //");

          if(stx)
          {
            *st = rest;
            rest = *stx;
            *stx = 0;
          }

          importance_const = new int[1]; // (int *)malloc(4);
          if(*(str_importance_const+1)=='x')
            importance_const[0] = str2h(str_importance_const+2,strlen(str_importance_const+2));
          else if(*(str_importance_const)=='U' || *(str_importance_const)=='u')
            importance_const[0] = 0xFFFFFFFF;
          else
            importance_const[0] = str2h(str_importance_const,strlen(str_importance_const));

          if(!stx) *st = rest;
          else *stx = rest;
        }
        else
        {
          importance_const = new int[1]; // (int *)malloc(4);
          importance_const[0] = 0xFFFFFFFF;
        }

        List_InsertLast(list_importances_const,importance_const);

        int *offset_const = new int[1]; // (int *)malloc(4);
        offset_const[0] = str2h(str_tag+7,8);
        List_InsertLast(list_offsets_const,offset_const);

        wchar_t *wstr_name_const = new wchar_t[strlen(name_const)+1];
        str2wstr(wstr_name_const,name_const);
        List_InsertLast(DynEditorBook->names_of_constants,wstr_name_const);
        str = str_tag;
        phone_h = phone_h_cpy;
      }

      int *buf_dyn = new int[DynEditorBook->names_of_constants->FirstFree];
      for(int i=0;i<(DynEditorBook->names_of_constants->FirstFree);i++)
      {
        int *k = (int *)List_Get(list_offsets_const,i);
        int *t = (int *)List_Get(list_importances_const,i);
        buf_dyn[*k] = *t;
     }
      wstrcat(de_conf,L"/");
      *(wstrrchr(fname,'.'))=0;
      wstrcat(de_conf,fname);

      int f;
      FILE_CREATE( f, de_conf, DYN_CONST );
      fwrite(f,buf_dyn,(DynEditorBook->names_of_constants->FirstFree)*4);
      FILE_CLOSE(f);

      LIST_DESTROY(&DynEditorBook->names_of_constants,FREE_ELEMENTS);
      LIST_DESTROY(&list_importances_const,FREE_ELEMENTS);
      LIST_DESTROY(&list_offsets_const,FREE_ELEMENTS);
      FREE(tagbuf);
      FREE(phone_h);
      FREE(buf_dyn);

      MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[DYN_CONST_CREATED]),NOIMAGE,1,0,0);
    }
    else MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[NO_DYN_TAG_H]),NOIMAGE,1,0,0);
  }
  FREE(de_conf);
  //FREE(fname);
}

int DynEditor_GetNamesOfConstants(TDynEditorBook *DynEditorBook)
{
  int error = 0;
  int len = wstrlen(CONFIG_INTERNAL)+wstrlen(L"/DynEditor")+18;
  wchar_t *de_conf = new wchar_t[len+1];
  memset(de_conf,0,len);
  wstrcpy(de_conf,CONFIG_INTERNAL);
  wstrcat(de_conf,L"/DynEditor");

  LIST_DESTROY(&DynEditorBook->names_of_constants, FREE_ELEMENTS);

  FSTAT stat;
  if(!fstat(de_conf,L"DYN_tag.h",&stat))
  {
    int dyn_tag_h_file;
    FILE_OPEN_RW( dyn_tag_h_file, de_conf, L"DYN_tag.h" );
    char *tagbuf = new char[stat.fsize+1];
    fread(dyn_tag_h_file,tagbuf,stat.fsize);
    tagbuf[stat.fsize] = 0;
    FILE_CLOSE(dyn_tag_h_file);

    DynEditorBook->names_of_constants = List_Create();
    char *str = strstr(tagbuf,"#define ");
    for(;;)
    {
      char *str2 = strstr(str+1,"#define ");
      if(!str2) break;
      char *name_const = str2+8;
      char *str_tag = strstr(str2,"_TAG ");
      *str_tag = 0;

      wchar_t *wstr_name_const = new wchar_t[strlen(name_const)+1];
      str2wstr(wstr_name_const,name_const);
      List_InsertLast(DynEditorBook->names_of_constants,wstr_name_const);
      str = str_tag;
    }
    FREE(tagbuf);

    if(!DynEditorBook->names_of_constants->FirstFree)
    {
      LIST_DESTROY(&DynEditorBook->names_of_constants, NOT_FREE_ELEMENTS);
      error = -2; //DYN_tag.h пустой
    }
  }
  else error = -1; //нет DYN_tag.h
  FREE(de_conf);
  return(error);
}

void Menu_SelectItemAction(BOOK *book, GUI *menu)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  int item = ListMenu_GetSelectedItem(menu); //CONFIG_INTERNAL

  FSTAT stat;
  fstat(CONFIG_INTERNAL,DYN_CONST,&stat);
  DynEditorBook->dyn_size = stat.fsize;

  if(item==0) //изменить/добавить константу
  {
    DynEditorBook->secGUI = CreateStringInputVA(0,
                                    VAR_BOOK(book),
                                    VAR_HEADER_TEXT(TextID_Copy(lng_textid[ENTER_OFFSET])),
                                    VAR_STRINP_MODE(11),
                                    VAR_OK_PROC(StringInput_EnterOffsetAction),
                                    VAR_STRINP_TEXT(STR("0x")),
                                    VAR_STRINP_MAX_LEN(10),
                                    0);
    GUIObject_SoftKeys_SetAction(DynEditorBook->secGUI,ACTION_BACK,back_e);
  }
  if(item==1) //просмотр дина
  {
    //BookObj_ReturnPage
    FILE_CLOSE( DynEditorBook->dyn_file );
    FILE_OPEN_RW( DynEditorBook->dyn_file, CONFIG_INTERNAL, DYN_CONST );

    DynEditorBook->offsets = 1;

    FREE(noffs);
    FREE(buf_const);

    dc_offsets = 0;
    offs_start = 0;
    noffs = new char[11];
    buf_const = new int[DynEditorBook->dyn_size/4];
    fread(DynEditorBook->dyn_file,buf_const,DynEditorBook->dyn_size);
    lseek(DynEditorBook->dyn_file,0,SEEK_SET);


    DynEditor_GetNamesOfConstants(DynEditorBook);
    BookObj_GotoPage(book,&ViewingDyn_MainPage);
  }
  if(item==2) //восстановить дин
  {
    int err;
    FILE_CLOSE(DynEditorBook->dyn_file);

    if(!fstat(CONFIG_INTERNAL,DYN_CONST,&stat)) FileDelete(CONFIG_INTERNAL,DYN_CONST,&err);
    FileCopy(backup_path,backup_name,CONFIG_INTERNAL,DYN_CONST,0);

    //FILE_OPEN_RW( DynEditorBook->dyn_file, CONFIG_INTERNAL, DYN_CONST );
    fstat(CONFIG_INTERNAL,DYN_CONST,&stat);
    DynEditorBook->dyn_size = stat.fsize;
    MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[DYN_IS_RESTORED]),NOIMAGE,1,0,book);
  }
  if(item==3) //сохранить дин
  {
    CreateFolderIfNo(backup_path);
    FileBackup(CONFIG_INTERNAL,DYN_CONST,backup_path,backup_name,REPLACMENT);
  }
  if(item==4) //собрать дин
  {
    wchar_t *de_conf = new wchar_t[wstrlen(CONFIG_INTERNAL)+wstrlen(L"/DynEditor")+1];
    wstrcpy(de_conf,CONFIG_INTERNAL);
    wstrcat(de_conf,L"/DynEditor");

    if(!fstat(de_conf,0,&stat))
    {
      DynEditorBook->phone_sw_h = List_Create();
      FILELISTITEM *mem = new FILELISTITEM; //(FILELISTITEM *)malloc(0x300);
      DIR_HANDLE *handle = AllocDirHandle(de_conf);
      FILELISTITEM *fli;
      while(fli=(FILELISTITEM *)GetFname(handle,mem))
      {
        wchar_t *ext = wstrrchr(fli->fname,'.');
        if(!wstrcmp(ext,L".h"))
        {
          if(wstrcmp(fli->fname,L"DYN_tag.h")) //не DYN_tag.h ли это
          { //не DYN_tag.h
            wchar_t *name = new wchar_t[wstrlen(fli->fname)+1];
            wstrcpy(name,fli->fname);
            List_InsertLast(DynEditorBook->phone_sw_h,name);
          }
        }
      }
      DestroyDirHandle(handle);
      FREE(mem);
      FREE(de_conf);

      if(DynEditorBook->phone_sw_h->FirstFree)
      {
        GUI_DESTROY(DynEditorBook->menu);
        GUI *menu = DynEditorBook->menu = CreateListMenu(book,0);
        GUIObject_SetTitleText(menu,TextID_Copy(lng_textid[COLLECT_DYN]));
        ListMenu_SetItemCount(menu,DynEditorBook->phone_sw_h->FirstFree);
        ListMenu_SetCursorToItem(menu,0);
        ListMenu_SetOnMessage(menu,CollectDyn_onMessage);
        GUIObject_SoftKeys_SetAction(menu,ACTION_BACK, CollectDyn_ExitAction);
        GUIObject_SoftKeys_SetAction(menu,ACTION_SELECT1,CollectDyn_SelectItemAction);

        FREE(dc_offsets);
        dc_offsets = new int[DynEditorBook->phone_sw_h->FirstFree]; //item_name

        for(int i=0;i<(DynEditorBook->phone_sw_h->FirstFree);i++)
        {
          wchar_t *fname = (wchar_t *)List_Get(DynEditorBook->phone_sw_h,i);
          wchar_t *cutname = new wchar_t[wstrlen(fname)+1];
          wstrcpy(cutname,fname);
          *(wstrrchr(cutname,'.'))=0;
          dc_offsets[i] = TextID_Create(cutname,ENC_UCS2,TEXTID_ANY_LEN);
          FREE(cutname);
        }
        GUIObject_Show(menu);
      }
      else
      {
        LIST_DESTROY(&DynEditorBook->phone_sw_h,NOT_FREE_ELEMENTS);
        MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[NO_FILES]),NOIMAGE,1,0,book);
      }
    }
    else
    {
      FREE(de_conf);
      MessageBox(EMPTY_TEXTID,TextID_Copy(lng_textid[NO_FOLDER_DYN_EDITOR]),NOIMAGE,1,0,book);
    }
  }
  if(item==5)
  {
    wchar_t text[0xFF+1];
    snwprintf(text,
              0xFF,
              L"Name: %ls\nFolder: %ls\nSize: %d byte\nConstants: 0x%X\n\nBackup dir: %ls",
              DYN_CONST,
              CONFIG_INTERNAL,
              DynEditorBook->dyn_size,
              DynEditorBook->dyn_size/4,
              backup_path);
    TEXTID info = TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN);
    MessageBox(TextID_Copy(lng_textid[INFO]),info,NOIMAGE,2,0,book);
  }
}

void SetRu(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  if(DynEditorBook->flag_lg != RUS_LNG)
  {
    reg_lngru(book);
    BookObj_GotoPage(book,&DynEditorBook_MainPage);
  }
}

void SetEn(BOOK *book, GUI *gui)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;
  if(DynEditorBook->flag_lg != OTHER_LNG)
  {
    reg_lngen(book);
    BookObj_GotoPage(book,&DynEditorBook_MainPage);
  }
}

int CreateMenuGUI_MainPage(void *, BOOK *book)
{
  TDynEditorBook *DynEditorBook = (TDynEditorBook *)book;

  GUI_DESTROY(DynEditorBook->menu);

  GUI *list_menu = DynEditorBook->menu = CreateListMenu(book,0);
  GUIObject_SetTitleText(list_menu,TextID_Create(ELFNAME,ENC_LAT1,strlen(ELFNAME)));
  ListMenu_SetItemCount(list_menu,6);//6);
  ListMenu_SetCursorToItem(list_menu,0);
  ListMenu_SetOnMessage(list_menu,Menu_onMessage);
  GUIObject_SoftKeys_SetAction(list_menu,ACTION_BACK,DynEditorBook_CloseAction);
  GUIObject_SoftKeys_SetText(list_menu,ACTION_BACK,TextID_Copy(lng_textid[EXIT]));
  GUIObject_SoftKeys_SetAction(list_menu,ACTION_LONG_BACK,DynEditorBook_CloseAction);
  GUIObject_SoftKeys_SetAction(list_menu,ACTION_SELECT1,Menu_SelectItemAction);
  GUIObject_SoftKeys_SetAction(list_menu,1,0);
  GUIObject_SoftKeys_SetItemAsSubItem(list_menu,1,3);
  GUIObject_SoftKeys_SetItemAsSubItem(list_menu,1,4);
  GUIObject_SoftKeys_SetText(list_menu,3,STR("Русский"));
  GUIObject_SoftKeys_SetAction(list_menu,3,SetRu);
  GUIObject_SoftKeys_SetText(list_menu,4,STR("English"));
  GUIObject_SoftKeys_SetAction(list_menu,4,SetEn);
  GUIObject_SoftKeys_SetAction(list_menu,2,About);
  GUIObject_SoftKeys_SetText(list_menu,1,TextID_Copy(lng_textid[_LANGUAGE]));
  GUIObject_SoftKeys_SetText(list_menu,2,TextID_Copy(lng_textid[ABOUT_ELF]));
  GUIObject_Show(list_menu);
  return(1);
}

int isDynEditorBook(BOOK *book)
{
  if(!strcmp(book->xbook->name,BOOKNAME)) return(1);
  return(0);
}

void init_lng_reg(TDynEditorBook *DynEditorBook)
{
  lng_textid = new int[NUM_TEXT];
  for(int i=0;i!=NUM_TEXT;i++) lng_textid[i] = EMPTY_TEXTID;
  if(isRu())
    reg_lngru((BOOK *)DynEditorBook);
  else
    reg_lngen((BOOK *)DynEditorBook);
}

void CreateDynEditorBook()
{
  TDynEditorBook *DynEditorBook = new TDynEditorBook;
  memset(DynEditorBook,0,sizeof(TDynEditorBook));
  if(!
     CreateBook((BOOK *)DynEditorBook,
                 DynEditorBook_OnClose,
                 &DynEditorBook_BasePage,
                 BOOKNAME,
                 NO_BOOK_ID,
                 0)
       )
    mfree(DynEditorBook);
  else
  {
    FSTAT stat;
    fstat(CONFIG_INTERNAL,DYN_CONST,&stat);
    DynEditorBook->dyn_size = stat.fsize;

    init_lng_reg(DynEditorBook);

    CreateFolderIfNo(backup_path);
    FileBackup(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),DYN_CONST,backup_path,backup_name,0);

    BookObj_GotoPage((BOOK *)DynEditorBook,&DynEditorBook_MainPage);
  }
}

int MSG_Box_lg(wchar_t *msg_ru, wchar_t *msg_en)
{
  wchar_t *msg;
  if(isRu()) msg = msg_ru;
  else msg = msg_en;
  TEXTID message = TextID_Create(msg,ENC_UCS2,TEXTID_ANY_LEN);
  int MessageBoxBookID = MessageBox(EMPTY_TEXTID,message,NOIMAGE,1,NULL,NULL);
  return(MessageBoxBookID);
}

int main()
{
  trace_init();

  if(!FindBook(isDynEditorBook))
  {
    FSTAT stat;
    if(!fstat(CONFIG_INTERNAL,DYN_CONST,&stat))
    {
      CreateDynEditorBook();
      return(0);
    }
    else if(!fstat(backup_path,backup_name,&stat))
    {
      FileCopy(backup_path,backup_name,CONFIG_INTERNAL,DYN_CONST,0);
      MSG_Box_lg(L"DYN_CONST восстановлен! \n Перезапустите эльф",0);
    }
    else
      MSG_Box_lg(L"DYN_CONST не найден",L"DYN_CONST not found");
  }
  else
    MSG_Box_lg(L"DynEditor уже запущен!",L"DynEditor already runed!");

  SUBPROC(elf_exit);
  return(0);
}
