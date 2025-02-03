//#include "mem2.h"
//#include "..\\include\Lib_Clara.h"
//#include "..\\include\Dir.h"
#include "main.h"
/*
wchar_t GetItemIcon(CONTENT_ITEM * item)
{
  FILEITEM * file = FILEITEM_Create();
  if (item->type)
  {
    return 0xe545;
    file->MIME_type = "semc/dir";
  }
  file->__fname= item->name;
  SUB_EXECUTE * sub_execute = DataBrowser_CreateSubExecute(-1, file);
  wchar_t iconid;
  DataBrowser_ExecuteSubroutine(sub_execute, DB_CMD_SETSMALLICON, (u16*)&iconid);
  //DataBrowser_ExecuteSubroutine(sub_execute, DB_CMD_SETSMALLICON, (u16*)&iconid);
  FILEITEM_Destroy(file);
  return iconid;
}
*/

enum action
{
  ACTION_OPEN = 0,
  ACTION_FILEINFO,
  ACTION_EXTRACT,
  ACTION_EXTRACT_IN,
  ACTION_EXTRACT_ALL,
  ACTION_ARCHIVE_INFO,
  ACTION_ABOUT_ELF
};

void SoftKeys_SetSrate(GRBOOK * grbook)
{
  GUI_LIST *gui_content = grbook->gui_content;
  int index = ListMenu_GetSelectedItem(gui_content);
  CONTENT_ITEM * item = (CONTENT_ITEM*)List_Get(grbook->item->list, index);
  if(grbook->item->list->FirstFree)
  {
    //GUIObject_SoftKeys_SetVisible(gui_content,ACTION_BACK,0);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_FILEINFO,1);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_EXTRACT,1);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_EXTRACT_ALL,1);
    if (item->type == 1)
    {
      GUIObject_SoftKeys_SetVisible(gui_content,ACTION_OPEN,1);
    }
    else
    {
      GUIObject_SoftKeys_SetVisible(gui_content,ACTION_OPEN,0);
    }
  }
  else
  {
    //GUIObject_SoftKeys_SetVisible(gui_content,ACTION_BACK,1);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_OPEN,1);
    GUIObject_SoftKeys_SetEnable(gui_content,ACTION_OPEN,0);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_FILEINFO,0);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_EXTRACT,0);
    GUIObject_SoftKeys_SetVisible(gui_content,ACTION_EXTRACT_ALL,1);
  }
}

wchar_t GR_GetIconID(LIST *ext_list, wchar_t *ext)
{
  if(!ext || !ext_list) return(NOIMAGE);

  wchar_t *lwr_ext = new wchar_t[wstrlen(ext)+1];
  wstrcpy(lwr_ext,ext);
  wstrnlwr(lwr_ext,10);

  for(int i=0;i<ext_list->FirstFree;i++)
  {
    EXT_AND_IC *eai = (EXT_AND_IC *)List_Get(ext_list,i);
    if(!wstrcmp(lwr_ext,eai->ext))
    {
      mfree(lwr_ext);
      return(eai->IconID);
    }
  }
  mfree(lwr_ext);
  return(NOIMAGE);
}

int ContentList_onGuiMessage(GUI_MESSAGE *msg)
{FUNCTION
  GRBOOK * grbook;
  int index;
  CONTENT_ITEM *item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case LISTMSG_HighlightChanged:
    grbook=(GRBOOK *)GUIonMessage_GetBook(msg);;
    SoftKeys_SetSrate(grbook);
    break;
  case LISTMSG_GetItem:
    grbook=(GRBOOK *)GUIonMessage_GetBook(msg);
    index = GUIonMessage_GetCreatedItemIndex(msg);
    if(grbook->item->list->FirstFree)
    {
      item = (CONTENT_ITEM *)List_Get(grbook->item->list,index);
      wchar_t *wstr = item->name;
      GUIonMessage_SetMenuItemText(msg,TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN));
      if (item->type == 1)
      {
        int id;
        wchar_t iconid = (iconidname2id(L"DB_LIST_FOLDER_ICN",-1,&id))?id:NOIMAGE;
        GUIonMessage_SetMenuItemIcon(msg,0,iconid);
      }
      else
      {
        GUIonMessage_SetMenuItemIcon(msg,0,GR_GetIconID(grbook->ext_list,getFileExtention(wstr)));
      }
    }
    else GUIonMessage_SetMenuItemText(msg,STR("Пусто"));
  }
  return(1);
}

void ContentList_ACTION_BACK(BOOK *book, GUI *)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  if(!grbook->item->parrent) FreeBook(book);
  else
  {
    int index = List_IndexOf(grbook->item->parrent->list,grbook->item);
    grbook->focused_item = (index!=LIST_ERROR)?index:0;
    grbook->item = grbook->item->parrent;
    UI_Event(ACTION_DELETE);
  }
}

void ContentList_ACTION_OPEN(BOOK *book, GUI *)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  if(grbook->item->list->FirstFree)
  {
    int index = ListMenu_GetSelectedItem(grbook->gui_content);
    CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(grbook->item->list,index);
    if(item->type == 1)
      if(item->list)
        //if(elem->list->FirstFree)
        {
          grbook->item = item;
          grbook->focused_item = 0;
          UI_Event(ACTION_DELETE);
        }
  }
}

void ContentList_ACTION_FILEINFO(BOOK *book, GUI *)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  if(grbook->item->list->FirstFree)
  {
    int index = ListMenu_GetSelectedItem(grbook->gui_content);
    CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(grbook->item->list,index);
    wchar_t *wstr = new wchar_t[wstrlen(item->name)+200];
    if (item->type)
    snwprintf(wstr,0xffff,L"Папка:\n%ls\nРазмер в архиве:\n%u byte\nРеальный размер:\n%u byte\nПапок: %u\nФайлов: %u",
              item->name,
              Item_GetCompressedSize(item),
              Item_GetRealSize(item),
              Item_GetFoldersCount(item),
              Item_GetFilesCount(item));
    else
    snwprintf(wstr,0xffff,L"Файл:\n%ls\nРазмер в архиве:\n%u byte\nРеальный размер:\n%u byte",
              item->name,
              Item_GetCompressedSize(item),
              Item_GetRealSize(item));
    MessageBox(STR("Инфо"),TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,2,0,book);
    mfree(wstr);
  }
}

void ContentList_ACTION_LONG_BACK(BOOK *book, GUI *)
{FUNCTION
  FreeBook(book);
}

wchar_t * _wstrcat(wchar_t * path, wchar_t * name)
{FUNCTION
  int len = wstrlen(path)+wstrlen(name)+1;
  wchar_t * fullname = new wchar_t[len+1];
  wstrcpy(fullname, path);
  wstrcat(fullname, L"/");
  wstrcat(fullname, name);
  return fullname;
}
int r=0;
void Item_Extract(CONTENT_ITEM *content_item, wchar_t * path, int a_file)
{
  wchar_t * new_name = _wstrcat(path, content_item->name);
  //MessageBox(EMPTY_TEXTID,TextID_Create(new_name,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  if(content_item->type == 0)
  {
    int a = ExtractOne(a_file, content_item->pos, new_name);
  }
  else
  {
    mkdir(new_name);
    LIST *list = content_item->list;
    for (int i=0;i<list->FirstFree;i++)
    {
      CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
      Item_Extract(item, new_name, a_file);
    }
  }
  delete new_name;
}

void ContentList_ACTION_EXTRACT(BOOK *book, GUI *)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  int index = ListMenu_GetSelectedItem(grbook->gui_content);
  CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(grbook->item->list,index);
  Item_Extract(item, grbook->arch_path, grbook->file);
}

wchar_t * cutext(wchar_t * name)
{
  wchar_t * point = wstrrchr(name, L'.');
  int len = (point)?point-name:wstrlen(name);
  wchar_t * res = new wchar_t[len+1];
  wstrncpy(res, name, len);
  res[len] = 0;
  return res;
}

void ContentList_ACTION_EXTRACT_IN(BOOK *book, GUI *)
{FUNCTION
  BookObj_CallPage(book,&GRar_ExtractIn_SelectFolder_Page);
}

void ContentList_ACTION_EXTRACT_ALL(BOOK *book, GUI *)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  CONTENT_ITEM *content_item = new CONTENT_ITEM;
  memset(content_item, 0x0, sizeof(CONTENT_ITEM));
  content_item->name = cutext(grbook->root_item->name);
  content_item->type = grbook->root_item->type;
  content_item->list = grbook->root_item->list;
  Item_Extract(content_item, grbook->arch_path, grbook->file);
  delete content_item->name;
  delete content_item;
}

void ContentList_ACTION_ARCHIVE_INFO(BOOK *book, GUI *)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  CONTENT_ITEM *item = grbook->root_item;
  wchar_t *wstr = new wchar_t[wstrlen(item->name)+200];
  int r_size = Item_GetRealSize(item);
  int c_size = Item_GetCompressedSize(item);
  snwprintf(wstr,0xffff,L"Имя:\n%ls\nРазмер файлов в архиве:\n%u byte\nРеальный размер файлов:\n%u byte\nСтепень сжатия: %d%%\nПапок: %u\nФайлов: %u",
            item->name,
            r_size,
            c_size,
            c_size/(r_size/100),
            Item_GetFoldersCount(item),
            Item_GetFilesCount(item));
  MessageBox(STR("Инфо об архиве"),TextID_Create(wstr,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,2,0,book);
  mfree(wstr);
}

void ContentList_ACTION_ABOUT_ELF(BOOK *book, GUI *)
{FUNCTION
  MessageBox(EMPTY_TEXTID,TextID_Create(ABOUT,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,book);
}

int GRar_ContentList_PAGE_ENTER_EVENT(void *r0, BOOK *book)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  GUI_LIST *gui_content = grbook->gui_content;
  FREE_GUI(gui_content);
  grbook->gui_content = gui_content = CreateListMenu(book,0);
  GUIObject_SetTitleText(gui_content,TextID_Create(grbook->item->name,ENC_UCS2,TEXTID_ANY_LEN));
  int cnt = grbook->item->list->FirstFree;
  ListMenu_SetItemCount(gui_content,(cnt)?cnt:1);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY *)gui_content,ContentList_onGuiMessage);
  ListMenu_SetCursorToItem(gui_content,grbook->focused_item);

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_BACK,ContentList_ACTION_BACK);
  //GUIObject_SoftKeys_SetText(gui_content,ACTION_BACK,STR("Back"));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_LONG_BACK,ContentList_ACTION_LONG_BACK);

  GUIObject_SoftKeys_SuppressDefaultAction(gui_content,ACTION_SELECT1);
  GUIObject_SoftKeys_SetAction(gui_content,ACTION_OPEN,ContentList_ACTION_OPEN);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_OPEN,STR("Открыть"));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_EXTRACT,ContentList_ACTION_EXTRACT);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_EXTRACT,STR("Извлечь"));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_FILEINFO,ContentList_ACTION_FILEINFO);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_FILEINFO,STR("Инфо"));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_EXTRACT_IN,ContentList_ACTION_EXTRACT_IN);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_EXTRACT_IN,STR("Извлечь в..."));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_EXTRACT_ALL,ContentList_ACTION_EXTRACT_ALL);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_EXTRACT_ALL,STR("Извлечь всё"));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_ARCHIVE_INFO,ContentList_ACTION_ARCHIVE_INFO);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_ARCHIVE_INFO,STR("Инфо об архиве"));

  GUIObject_SoftKeys_SetAction(gui_content,ACTION_ABOUT_ELF,ContentList_ACTION_ABOUT_ELF);
  GUIObject_SoftKeys_SetText(gui_content,ACTION_ABOUT_ELF,STR("Об эльфе"));

  GUIObject_Show(gui_content);
  SoftKeys_SetSrate(grbook);
  return(1);
}

int GRar_ContentList_CANCEL_EVENT(void *r0, BOOK *book)
{FUNCTION
  FreeBook(book);
  return(1);
}

int GRar_ContentList_PREVIOUS_EVENT(void *r0, BOOK *book)
{FUNCTION
  return(1);
}

int GRar_ContentList_PAGE_EXIT_EVENT(void *r0, BOOK *book)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  if(grbook->gui_content) FREE_GUI(grbook->gui_content);
  return(1);
}

const PAGE_MSG GRar_ContentList_EvetsList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  GRar_ContentList_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      GRar_ContentList_PAGE_ENTER_EVENT,
  ACTION_DELETE_TAG,     GRar_ContentList_PAGE_ENTER_EVENT,
  CANCEL_EVENT_TAG,      GRar_ContentList_CANCEL_EVENT,
  PREVIOUS_EVENT_TAG,    GRar_ContentList_PREVIOUS_EVENT,
  PAGE_EXIT_EVENT_TAG,   GRar_ContentList_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,         NULL
};

const PAGE_DESC GRar_ContentList_Page={"GRar_ContentList_Page",0,GRar_ContentList_EvetsList};
