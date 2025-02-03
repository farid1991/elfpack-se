#include "main.h"

void Dir_Destroy(CONTENT_ITEM *content_item)
{
  if(content_item->type == 0)
  {
    mfree(content_item->name);
    mfree(content_item);
  }
  else
  {
    mfree(content_item->name);
    LIST *list = content_item->list;
    mfree(content_item);
    for(int i=0;i<list->FirstFree;i++)
    {
      CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
      Dir_Destroy(item);
    }
  }
}

int CompareItems(void *_item, void *_item_from_list)
{
  CONTENT_ITEM *item = (CONTENT_ITEM *)_item;
  CONTENT_ITEM *item_from_list = (CONTENT_ITEM *)_item_from_list;
  if(!item->type)
  {
    if(item_from_list->type) return(1);
    else return(wstrcmpi(item->name,item_from_list->name));
  }
  else
  {
    if(item_from_list->type) return(wstrcmpi(item->name,item_from_list->name));
    else return(-1);
  }
}

CONTENT_ITEM *Dir_GetFolder(CONTENT_ITEM *content_item, wchar_t *folder)
{
  LIST *list = content_item->list;
  for(int i=0;i<list->FirstFree;i++)
  {
    CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
    if(item->type == 1)
      if(!wstrcmpi(item->name,folder)) return(item);
  }
  return(0);
}
CONTENT_ITEM *Dir_GetFile(CONTENT_ITEM *content_item, wchar_t *file)
{
  LIST *list = content_item->list;
  for(int i=0;i<list->FirstFree;i++)
  {
    CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
    if(item->type == 0)
      if(!wstrcmpi(item->name,file)) return(content_item);
  }
  return(0);
}

CONTENT_ITEM *Dir_AddFolder(CONTENT_ITEM *content_item, wchar_t *dir)
{
  wchar_t *wstr = wstrchr(dir, L'/');
  int len = (wstr)?wstr - dir:wstrlen(dir);
  wchar_t *name = new wchar_t[len+1];
  wstrncpy(name,dir,len);
  name[len] = 0;
  CONTENT_ITEM *created_dir = Dir_GetFolder(content_item, name);
  if(!created_dir)
  {
    created_dir = new CONTENT_ITEM;
    created_dir->type = 1;
    created_dir->name = name;
    created_dir->list = List_Create();
    created_dir->parrent = content_item;
    created_dir->real_size = 0;
    created_dir->compressed_size = 0;
    List_InsertSorted(content_item->list,created_dir,CompareItems);
  }
  else
  {
    delete name;
  }
  if(wstr)
  {
    created_dir = Dir_AddFolder(created_dir,dir+len+1);
  }
  return created_dir;
}

CONTENT_ITEM *Dir_AddFile(CONTENT_ITEM *content_item, wchar_t *file)
{
  wchar_t *wstr = wstrrchr(file, L'/');
  wchar_t *name;
  if(wstr)
  {
    int len = wstr - file;
    name = new wchar_t[len+1];
    wstrncpy(name,file,len);
    name[len] = 0;
    content_item = Dir_AddFolder(content_item,name);
    mfree(name);
    file = wstr+1;
  }
  CONTENT_ITEM *item = Dir_GetFile(content_item,file);
  if(item) return(item);
  item = new CONTENT_ITEM;
  wchar_t *file_name = new wchar_t[wstrlen(file)+1];
  item->type = 0;
  item->name = wstrcpy(file_name,file);
  item->list = 0;
  item->parrent = content_item;
  item->real_size = 0;
  item->compressed_size = 0;
  List_InsertSorted(content_item->list,item,CompareItems);
  return(item);
}

int Item_SetRealSize(CONTENT_ITEM *content_item, int real_size)
{
  if (content_item->type == 1) return 0;
  content_item->real_size = real_size;
  return 1;
}

int Item_SetCompressedSize(CONTENT_ITEM *content_item, int compressed_size)
{
  if (content_item->type == 1) return 0;
  content_item->compressed_size = compressed_size;
  return 1;
}

int Item_GetCompressedSize(CONTENT_ITEM *content_item)
{
  if(content_item->type == 0) return(content_item->compressed_size);
  else
  {
    LIST *list = content_item->list;
    int compressed_size = 0;
    for (int i=0;i<list->FirstFree;i++)
    {
      CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
      compressed_size += Item_GetCompressedSize(item);
    }
    return(compressed_size);
  }
}

int Item_GetRealSize(CONTENT_ITEM *content_item)
{
  if(content_item->type == 0) return(content_item->real_size);
  else
  {
    LIST *list = content_item->list;
    int real_size = 0;
    for (int i=0;i<list->FirstFree;i++)
    {
      CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
      real_size += Item_GetRealSize(item);
    }
    return(real_size);
  }
}

int Item_GetFoldersCount(CONTENT_ITEM *content_item)
{
  if(content_item->type == 0) return 0;
  else
  {
    LIST *list = content_item->list;
    int folders_count = 0;
    for (int i=0;i<list->FirstFree;i++)
    {
      CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
      if (item->type == 1)
      {
        folders_count += Item_GetFoldersCount(item) + 1;
      }
    }
    return(folders_count);
  }
}

int Item_GetFilesCount(CONTENT_ITEM *content_item)
{
  if(content_item->type == 0) return 0;
  else
  {
    LIST *list = content_item->list;
    int files_count = 0;
    for (int i=0;i<list->FirstFree;i++)
    {
      CONTENT_ITEM *item = (CONTENT_ITEM *)List_Get(list,i);
      if (item->type == 1)
      {
        files_count += Item_GetFilesCount(item);
      }
      else
      {
        files_count++;
      }
    }
    return(files_count);
  }
}

int Terminate(void *msg, BOOK *book)
{FUNCTION
  FreeBook(book);
  return(1);
}

int ShowInfo(void *msg, BOOK *book)
{FUNCTION
  MessageBox(EMPTY_TEXTID,TextID_Create(ABOUT,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return(1);
}

const PAGE_MSG GRar_Base_EventsList[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, Terminate,
  ELF_SHOW_INFO_EVENT, ShowInfo,
  NIL_EVENT_TAG,       NULL
};

const PAGE_DESC GRar_Base_Page={"GRar_Base_Page",0,GRar_Base_EventsList};

void onGRarBookClose(BOOK * book)
{FUNCTION
  GRBOOK *grbook=(GRBOOK *)book;
  FREE_GUI(grbook->gui_content);
  Dir_Destroy(grbook->root_item);
  mfree(grbook->arch_path);
  mfree(grbook->arch_name);
  if(grbook->file >= 0) fclose(grbook->file);

  for(int i=0;i<grbook->ext_list->FirstFree;i++)
  {
    EXT_AND_IC *eai = (EXT_AND_IC *)List_Get(grbook->ext_list,i);
    mfree(eai->ext);
    mfree(eai);
  }
  List_Destroy(grbook->ext_list);

  if(grbook->folder_extract_in) mfree(grbook->folder_extract_in);

  BOOK *db = FindBook(get_IsDataBrowserBook());
  if(db) UI_Event_toBookID(DB_UPDATE_EVENT,BookObj_GetBookID(db));

  SUBPROC(elf_exit);
}

int isGRBook(BOOK *book)
{
  if(!strcmp(book->xbook->name,"GRar")) return(1);
  return(0);
}

void create_ext_list(LIST *ext_list, wchar_t *name)
{
  EXT_AND_IC *eai = 0;
  ////debug_printf("\next = %ls\n",ext);
  if(!name || !ext_list) return;

  wchar_t *ext = getFileExtention(name);
  wchar_t *lwr_ext = new wchar_t[wstrlen(ext)+1];
  wstrcpy(lwr_ext,ext);
  wstrnlwr(lwr_ext,10);

  for(int i=0;i<(ext_list->FirstFree);i++)
  {
    eai = (EXT_AND_IC *)List_Get(ext_list,i);
    if(!wstrcmp(lwr_ext,eai->ext))
    {
      mfree(lwr_ext);
      return;
    }
  }

  eai = new EXT_AND_IC;
  eai->ext = lwr_ext;

  EP_DATA *epd = (EP_DATA *)get_envp(get_bid(current_process()), "elfpackdata");
  eai->IconID = NOIMAGE;
  LIST *dbe = epd->DBExtList;

  for(int i=0;;i++)
  {
    DB_EXT *db = (DB_EXT *)List_Get(dbe,i);
    if(db == LastExtDB()) break;
    wchar_t *ext_db = *(db->ext_list);

    if(!wstrcmp(lwr_ext,ext_db))
    {
      //debug_printf("\nwstrcmp(%ls,%ls)\n",lwr_ext,ext_db);

      SUB_EXECUTE *se = new SUB_EXECUTE;
      FILEITEM *fi = new FILEITEM;

      fi->__fname = name;
      //debug_printf("\nname = %ls\n",name);
      db->GetMethods(se);
      se->file_item = fi;
      FILESUBROUTINE *fs = se->filesub;

      for(;fs->ON_CMD;fs++)
      {
        if(fs->cmd == DB_CMD_SETSMALLICON)
        {
          fs->ON_CMD_ICON(se,&eai->IconID);
          ////debug_printf("\neai->IconID: 0x%X\n",eai->IconID);
          break;
        }
      }

      mfree(fi);
      mfree(se);
      break;
    }
  }

  if(eai->IconID == NOIMAGE)
  {
    FILESUBROUTINE *fs, *s_fs = new FILESUBROUTINE;
    GetOtherExtMethods(&fs);
    for(;fs->ON_CMD;fs++)
    {
      if(fs->cmd == DB_CMD_SETSMALLICON)
      {
        fs->ON_CMD_ICON(0,&eai->IconID);
        break;
      }
    }
    mfree(s_fs);
  }

  List_InsertLast(ext_list,eai);
}

int InitRar(GRBOOK * grbook)
{
  LIST *list = List_Create();
  grbook->ext_list = List_Create();
  CONTENT_ITEM * root_item = new CONTENT_ITEM;
  grbook->item = grbook->root_item = root_item;
  root_item->type = 1;
  root_item->list = list;
  root_item->name = new wchar_t[wstrlen(grbook->arch_name)+1];
  wstrcpy(root_item->name,grbook->arch_name);
  root_item->parrent = 0;
  root_item->real_size = 0;
  root_item->compressed_size = 0;
  int file = grbook->file = _fopen(grbook->arch_path,grbook->arch_name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  if(file >= 0)
  {
    if(!RAR_ReadMainHeader(file)) return 0;
    int file_pos = 0x14;
    for(;;)
    {
      RAR_INFO_HEADER * header = RAR_ReadInfoHeader(file, file_pos);
      if (!header) break;
      CONTENT_ITEM * item;
      if(header->attributes & 0x10)
      {
        item = Dir_AddFolder(root_item, header->name);
      }
      else
      {
        item = Dir_AddFile(root_item, header->name);
        create_ext_list(grbook->ext_list,item->name);
        Item_SetRealSize(item, header->real_size);
        Item_SetCompressedSize(item, header->compressed_size);
      }
      item->pos = file_pos;
      file_pos += header->header_size + header->compressed_size;
      RAR_DestroyInfoHeader(header);
    }
    //fclose(in); //закроем при выходе
  }
  return(1);
}

int main(wchar_t *elf, wchar_t *path, wchar_t *name)
{FUNCTION
  //path = GetDir(DIR_OTHER|MEM_EXTERNAL);
  //name = L"123.rar";
  if(path && name)
  {
    GRBOOK * grbook = new GRBOOK;
    memset(grbook,0,sizeof(GRBOOK));
    if(!CreateBook(grbook,onGRarBookClose,&GRar_Base_Page,"GRar",-1,0))
    {
      mfree(grbook);
      SUBPROC(elf_exit);
      return(0);
    }
    grbook->arch_path = new wchar_t[wstrlen(path)+1];
    wstrcpy(grbook->arch_path,path);
    grbook->arch_name = new wchar_t[wstrlen(name)+1];
    wstrcpy(grbook->arch_name,name);

    InitRar(grbook);
    BookObj_GotoPage((BOOK *)grbook,&GRar_ContentList_Page);
  }
  return(0);
}

