#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "bookparam.h"
#include "shortcuts.h"
#include "main.h"
#include "folders.h"
#include "book_names.h"
#include "phone.h"
#include "util.h"

int PhoneType = UNKNOWN;
int set_2020  = 1;

wchar_t tmppath[256];
wchar_t tmpname[256];
wchar_t heap[50];
wchar_t elfy[50];

wchar_t* id_digits[ICON_DIGITS_COUNT] =
{
  IDN_DIGIT_0_ICON,
  IDN_DIGIT_1_ICON,
  IDN_DIGIT_2_ICON,
  IDN_DIGIT_3_ICON,
  IDN_DIGIT_4_ICON,
  IDN_DIGIT_5_ICON,
  IDN_DIGIT_6_ICON,
  IDN_DIGIT_7_ICON,
  IDN_DIGIT_8_ICON,
  IDN_DIGIT_9_ICON,
};

const wchar_t* img_digits[ICON_DIGITS_COUNT] =
{
  digit0,
  digit1,
  digit2,
  digit3,
  digit4,
  digit5,
  digit6,
  digit7,
  digit8,
  digit9,
};

wchar_t* id_tab_names[ICONS_TAB_COUNT]=
{
  IDN_BOOKS_DEACT_ICON, IDN_BOOKS_ACT_ICON,
  IDN_ELFS_DEACT_ICON, IDN_ELFS_ACT_ICON,
  IDN_SHORTCUTS_DEACT_ICON, IDN_SHORTCUTS_ACT_ICON,
  IDN_FOLDER_DEACT_ICON, IDN_FOLDER_ACT_ICON,
  IDN_CALL_LOGGER_DEACT_ICON, IDN_CALL_LOGGER_ACT_ICON
};

int const Folder_ID[10]=
{
    16,18,20,22,8,0,2,4,6,24
};

int const *GET_Folder_ID[] =
{
  &FirstFolder,
  &SecondFolder,
  &ThirdFolder,
  &FourthFolder,
  &FifthFolder ,
  &SixthFolder ,
  &SeventhFolder,
  &EightFolder,
  &NinthFolder,
  &TenthFolder
};

const wchar_t* img_tab_names[ICONS_TAB_COUNT] =
{
  tab1_inact, tab1_act,
  tab2_inact, tab2_act,
  tab3_inact, tab3_act,
  tab4_inact, tab4_act,
  tab5_inact, tab5_act
};

wchar_t *id_item_names[ICONS_ITEM_COUNT]=
{
  IDN_BOOK_ICON,
  IDN_ELF_ICON
};

const wchar_t * img_item_names[ICONS_ITEM_COUNT]=
{
  books_icon,
  elfs_icon
};

wchar_t * Call_Logger[12]=
{
 L"CL_INCOMING_CALLS_TXT",  L"in.txt",     L"CALLS_INCOMING_ICN",
 L"CL_OUTGOING_CALLS_TXT",  L"out.txt",    L"CALLS_OUTGOING_ICN",
 L"CL_MISSED_CALLS_TXT",    L"missed.txt", L"CALLS_MISSED_ICN",
 L"OLP_TITLE_TXT",          L"gprs.txt",   L"DB_DIRECT_LINK_SMALL_ICN"
};

int const *tabs_pos[] =
{
  &books_tab,
  &elfs_tab,
  &shortcuts_tab,
  &folders_tab,
  &loggers_tab
};

wchar_t Folders[5][16]=
{
  L"Прочее",L"Звуки",L"Картинки",L"Видео",L"Темы"
};

wchar_t * Fo_Nam_Icon(int pos)
{
    if(pos == 0 ) return L"ZBin";
    else if(pos == 1) return L"DB_LIST_FOLDER_ICN";
    else if(pos == 2) return L"Daemons";
    else if(pos == 3) return L"DB_LIST_FOLDER_ICN";
    else if(pos == 4) return L"Config";
    else if(pos == 5) return L"DB_LIST_FOLDER_ICN";
    else if(pos == 6) return L"Ini";
    else if(pos == 7) return L"DB_LIST_FOLDER_ICN";
    else if(pos == 8) return Folders[0];
    else if(pos == 9) return L"DB_OTHER_DIRECTORY_ICN";
    else if(pos == 10) return Folders[1];
    else if(pos == 11) return L"DB_SOUNDS_DIRECTORY_ICN";
    else if(pos == 12) return Folders[2];
    else if(pos == 13) return L"DB_IMAGES_DIRECTORY_ICN";
    else if(pos == 14) return Folders[3];
    else if(pos == 15) return L"DB_VIDEOS_DIRECTORY_ICN";
    else if(pos == 16) return Folders[4];
    else if(pos == 17) return L"DB_THEMES_DIRECTORY_ICN";
    else if(pos == 18) return L"Dll";
    else return L"DB_LIST_FOLDER_ICN";
}

void win12512unicode( wchar_t* ws, char* s, int len )
{
  int c;

  while( ( c = *s++ ) && len-- > 0 )
  {
    if ( c == 0xA8 ) c = 0x401;
    if ( c == 0xAA ) c = 0x404;
    if ( c == 0xAF ) c = 0x407;
    if ( c == 0xB8 ) c = 0x451;
    if ( c == 0xBA ) c = 0x454;
    if ( c == 0xBF ) c = 0x457;
    if ( c >= 0xC0 && c < 0x100 )
    c += 0x350;
    *ws++ = c;
  }
  *ws = 0;
}

char* unicode2win1251( char* s, wchar_t* ws, int len )
{
  char* d = s;
  int c;

  while( ( c = *ws++ ) && len-- > 0 )
  {
    if ( c == 0x401 ) c = 0xA8;
    if ( c == 0x404 ) c = 0xAA;
    if ( c == 0x407 ) c = 0xAF;
    if ( c == 0x451 ) c = 0xB8;
    if ( c == 0x454 ) c = 0xBA;
    if ( c == 0x457 ) c = 0xBF;
    if ( c >= 0x410 && c < 0x450 )
    c -= 0x350;
    *s++ = c;
  }
  *s = 0;

  return d;
}

int wcharh2int(wchar_t n)
{
  int r=0;
  if(n>='0' && n<='9') r=n-0x30;
  if(n=='a' || n=='A') r=10;
  if(n=='b' || n=='B') r=11;
  if(n=='c' || n=='C') r=12;
  if(n=='d' || n=='D') r=13;
  if(n=='e' || n=='E') r=14;
  if(n=='f' || n=='F') r=15;
  return r;
}

void GetIconByName(wchar_t* name,int &id)
{
  if(name[0]=='0' && name[1]=='x')
  {
    id=0;
    int N=0x1000;
    for(int i=0;i<4;i++)
    {
      id+=wcharh2int(name[2+i])*N;
      N=N/0x10;
    }
  }
  else iconidname2id(name,-1,&id);
}

int Folders_Filter(const wchar_t * ext_table, const wchar_t * path , const wchar_t * name)
{
  if(PhoneType == K750_R1CA021 || PhoneType == W800_R1BD001 || PhoneType == W700_R1CA021)
  {
  char h[9];
  unicode2win1251(h,(wchar_t*)name,9);
  if(strstr(h,"ini") ||  strstr(h,"ZBin") ||  strstr(h,"Daemons") || strstr(h,"Config") || strstr(h,"DLL")) return (0);
  return(1);
 } else {
  char h[9];
  unicode2win1251(h,(wchar_t*)name,9);
  if( strstr(h,"Daemons") || strstr(h,"Config") || strstr(h,"DLL")) return (0);
  return(1);
 }
}

//============= pages start ======================

const PAGE_MSG bk_main_msglst[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG, CreateBookList,
  BOOK_DESTROYED_EVENT_TAG, onRootListChanged,
  USER_INACTIVITY_EVENT_TAG, onUserInactivity,
  ACCEPT_EVENT_TAG, RecreateBookList,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC BookManager_Main_Page = { "BookManager_Main_Page", 0, bk_main_msglst };


const PAGE_MSG bk_idle_msglst[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG, IdlePage_EnterAction,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC BookManager_Idle_Page = { "BookManager_Idle_Page", 0, bk_idle_msglst };


const PAGE_MSG bk_base_msglst[] @ "DYN_PAGE" =
{
  ELF_RECONFIG_EVENT, ReconfigElf,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC BookManager_Base_Page = { "BookManager_Base_Page", 0, bk_base_msglst };

//============= pages end ======================


void elf_exit( void )
{
  trace_done();
  kill_data( &ELF_BEGIN, ( void(*)(void*) )mfree_adr() );
}

// ======================== всякая служебная ерунда start ===========================================

int GetActiveTab( MyBOOK* mbk )
{
  return TabMenuBar_GetFocusedTab( mbk->gui );
}


int isBookManager( BOOK* book )
{
  return 0 == strcmp( book->xbook->name, "BookManager" );
}

// проверка, обрабатывает ли БЕЙЗ_ПЕЙДЖ книги событие
int CheckEv( BOOK* bk, int ev )
{
  if ( bk->base_page )
  {
    const PAGE_MSG* pm = bk->base_page->msg_lst;

    if ( pm )
    {
      while ( pm->msg && pm->proc )
      {
        if ( pm->msg == ev )
        {
          return 1;
        }
        pm++;
      }
    }
  }
  return 0;
}

//Получить пользовательское имя книги
int GetParam( char* name )
{
  BOOK_PARAM data;
  int n=GetPARAM(name, &data);
  int str_id=0;

  if(n!=-1) if(wstrlen(data.name)>0) str_id=TextID_Create(data.name,ENC_UCS2,TEXTID_ANY_LEN);
  if(!str_id) str_id=TextID_Create(name,ENC_LAT1,strlen(name));

  return str_id;
}

// получить имя жавы
int GetJavaName( BOOK* bk )
{
  wchar_t ws[100];

  TextID_GetWString( BookObj_GetSession( bk )->name, ws, MAXELEMS(ws) );

  if ( !wstrncmp( ws, L"Foreign app", 11 ) )
  {
    return JavaSession_GetName();
  }

  if ( !wstrcmp( ws, L"Java" ) )
  {
    return JavaSession_GetName();
  }

  return EMPTY_TEXTID;
}

BOOK_LIST_ITEM * GetBookListItem( BOOK* bk, int list )
{
  MyBOOK* mbk = (MyBOOK*) bk;
  BOOK_LIST_ITEM* elem;

  switch ( list )
  {
  case BOOKLIST:
    if ( mbk->blistcnt )
    {
      if (MINIMIZE_TO_SESSION)
      {
        SESSION_LIST_ITEM * ses_elem = (SESSION_LIST_ITEM*)List_Get( mbk->sessions_list, mbk->blistpos );
        elem = (BOOK_LIST_ITEM*) List_Get( ses_elem->books_list, ses_elem->pos_subitem );
      }
      else
        elem = (BOOK_LIST_ITEM*) List_Get( mbk->books_list, mbk->blistpos );
    }
    else
      return 0;
    break;

  case ELFLIST:
    if ( mbk->elistcnt )
      elem = (BOOK_LIST_ITEM*) List_Get( mbk->elfs_list, mbk->elistpos );
    else
      return 0;
    break;

  case SHORTCUTS:
    if ( mbk->slistcnt )
      elem = (BOOK_LIST_ITEM*) List_Get( mbk->shortcuts_list, mbk->slistpos );
    else
      return 0;
    break;

  case FOLDERS:
    if ( mbk->flistcnt )
      elem = (BOOK_LIST_ITEM*) List_Get( mbk->folders_list, mbk->flistpos );
    else
      return 0;
    break;

  case LOGGER:
    if ( mbk->llistcnt )
      elem = (BOOK_LIST_ITEM*) List_Get( mbk->Logger_list, mbk->llistpos );
    else
      return 0;
    break;
  }
  return elem;
}

// взять выбранную книгу из листа книг/эльфов
BOOK* GetBook( BOOK* bk, int list )
{
  BOOK_LIST_ITEM * elem = GetBookListItem(bk,list);
  if (!elem) return 0;
  else return elem->book;
}

int BOOK_LIST_ITEM_Filter(void * elem)
{
  if (elem) return(1);
  return(0);
}

void BOOK_LIST_ITEM_Destroy(void * data)
{
  BOOK_LIST_ITEM * elem = (BOOK_LIST_ITEM*)data;

  if (elem)
  {
    delete(elem->book_name);
    delete(elem);
  }
}

void SHORTCUT_LIST_ITEM_Destroy(void * data)
{
  BOOK_LIST_ITEM * elem = (BOOK_LIST_ITEM*)data;

  if (elem)
  {
    TextID_Destroy(elem->book_nameId);
    delete(elem);
  }
}

int SESSION_LIST_ITEM_Filter(void * elem)
{
  if (elem) return(1);
  return(0);
}

void SESSION_LIST_ITEM_Destroy(void * data)
{
  SESSION_LIST_ITEM * elem = (SESSION_LIST_ITEM*)data;

  if (elem)
  {
    List_DestroyElements(elem->books_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
    List_Destroy(elem->books_list);
    delete(elem);
  }
}

void DestroyLists( MyBOOK* mbk )
{
  if ( mbk->sessions_list )
  {
    List_DestroyElements(mbk->sessions_list,SESSION_LIST_ITEM_Filter,SESSION_LIST_ITEM_Destroy);
    List_Destroy(mbk->sessions_list);
    mbk->sessions_list=0;
  }

  if ( mbk->books_list )
  {
    List_DestroyElements(mbk->books_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
    List_Destroy(mbk->books_list);
    mbk->books_list=0;
  }

  if ( mbk->elfs_list )
  {
    List_DestroyElements(mbk->elfs_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
    List_Destroy(mbk->elfs_list);
    mbk->elfs_list=0;
  }

  if ( mbk->shortcuts_list )
  {
    List_DestroyElements(mbk->shortcuts_list,BOOK_LIST_ITEM_Filter,SHORTCUT_LIST_ITEM_Destroy);
    List_Destroy(mbk->shortcuts_list);
    mbk->shortcuts_list=0;
  }

  if ( mbk->folders_list )
  {
    List_DestroyElements(mbk->folders_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
    List_Destroy(mbk->folders_list);
    mbk->folders_list=0;
  }

    if ( mbk->Logger_list )
  {
    List_DestroyElements(mbk->Logger_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
    List_Destroy(mbk->Logger_list);
    mbk->Logger_list=0;
  }
}

int GetBooksFromSession(MyBOOK * mbk,UI_APP_SESSION * session,LIST * books_list,LIST * elfs_list)
{
  int k;
  int res=0;
  int mask = ( (int)LastExtDB() )&FLASH_MASK;

  for ( k = 0; k < session->listbook->FirstFree; k++ )
  {
    BOOK * book = (BOOK*) List_Get( session->listbook, k );

    if ( !isBookManager( book ) )
    {
      char s[MAX_BOOK_NAME_LEN+1];
      BOOK_LIST_ITEM * elem = new BOOK_LIST_ITEM;
      elem->book=book;
      elem->isGuiBook = book->xguilist->guilist->FirstFree;
      elem->isJava=FALSE;
      elem->isJava_2010=FALSE;

      if ( strcmp( book->xbook->name, JAVA_BOOK_NAME ) )
      {
        char * bn =new char[strlen(book->xbook->name)+1];
        strcpy(bn,book->xbook->name);
        elem->book_name = bn;
      }
      else
      {
        TextID_GetString( BookObj_GetSession(book)->name, s, MAXELEMS(s) );
        char * bn =new char[strlen(s)+1];
        strcpy(bn,s);
        elem->book_name = bn;
        elem->isJava=TRUE;
      }

      if ( mbk->hidden_buf_size && strstr(mbk->hidden_buf,elem->book_name) ) elem->isHidden=TRUE;
      else elem->isHidden=FALSE;

      if(elem->isJava)
       {
         int JID;
         iconidname2id(L"DB_LIST_JAVA_ICN",TEXTID_ANY_LEN,&JID);
         elem->book_icon = JID;
         void * JavaDesc;
         char sp1;
         JavaDialog_Open(0,&sp1,&JavaDesc);

         if (!JavaAppDesc_GetFirstApp(JavaDesc))
          {
            wchar_t *book_name = new wchar_t[strlen(elem->book_name)+1];
            str2wstr(book_name,elem->book_name);
            bool yap = true;
            while (yap)
            {
              wchar_t* name;
              JavaAppDesc_GetJavaAppInfo(JavaDesc, 0, &name);

              if(!wstrncmp(book_name, name, 8))
               {
                wchar_t* fullpath;
                JavaAppDesc_GetJavaAppInfo(JavaDesc,5,&fullpath);
                JavaApp_LogoImageID_Get(fullpath,&elem->book_icon);
                delete(fullpath);
                yap = false;
               }
              delete(name);

            if (JavaAppDesc_GetNextApp(JavaDesc)) yap = false;
             }
             delete(book_name);
            }
          JavaDialog_Close(sp1);
        }
      TEXTID tmp = GetJavaName( book );

      if ( tmp != EMPTY_TEXTID )
      {
        delete(elem->book_name);
        TextID_GetString( tmp, s, MAXELEMS(s) );
        char * java_name = new char[strlen(s)+1];
        strcpy(java_name,s);
        elem->book_name = java_name;
        elem->isJava_2010=TRUE;
        TextID_Destroy( tmp );
      }

      else if ( ( ((int)book->onClose)&FLASH_MASK ) == mask )
      {
        if (mbk->books_show_hid==FALSE && elem->isHidden==TRUE)
          BOOK_LIST_ITEM_Destroy(elem);
        else if (mbk->books_show_nogui==FALSE && !elem->isGuiBook)
          BOOK_LIST_ITEM_Destroy(elem);
        else
          List_InsertFirst( books_list, elem );
      }
      else
      {
        if (mbk->elfs_show_hid==FALSE && elem->isHidden==TRUE)
          BOOK_LIST_ITEM_Destroy(elem);
        else if (mbk->elfs_show_nogui==FALSE && !elem->isGuiBook)
          BOOK_LIST_ITEM_Destroy(elem);
        else
        {
          List_InsertFirst( elfs_list, elem );
          res=1;
        }
      }
    }
  }
  return res;
}

// создаём список книг
void CreateBookLst( MyBOOK* mbk )
{
  int i, j, isElf;
  UI_APP_SESSION* session;

  char s[MAX_BOOK_NAME_LEN+1];
  int ShCount = getShortcutsNum(shortcuts_tab);

  if (MINIMIZE_TO_SESSION)
    mbk->sessions_list = List_Create();
  else
    mbk->books_list = List_Create();

  mbk->elfs_list = List_Create();
  mbk->shortcuts_list = List_Create();
  mbk->folders_list = List_Create();
  mbk->Logger_list = List_Create();

  i = root_list_get_session_count();

  for ( j = 0; j < i; j++ )
  {
    session = root_list_get_session( j );

    if (MINIMIZE_TO_SESSION)
    {
      SESSION_LIST_ITEM * elem = new SESSION_LIST_ITEM;
      elem->session=session;
      elem->books_list=List_Create();
      elem->pos_subitem=0;

      isElf = GetBooksFromSession(mbk,session,elem->books_list,mbk->elfs_list);

      if ( !elem->books_list->FirstFree || isElf )
        SESSION_LIST_ITEM_Destroy(elem);
      else
        List_InsertFirst( mbk->sessions_list, elem );
    }
    else
      GetBooksFromSession(mbk,session,mbk->books_list,mbk->elfs_list);
  }

  if(ShCount > 0)
  {
    int str_id;
    int icon_id;

    for(int i = 0; i < ShCount; i++)
    {
      BOOK_LIST_ITEM * elem = new BOOK_LIST_ITEM;

      void * w_buf;
      wstr2strn(s, shortcuts_tab_actions[i], MAXELEMS(s));

      if (mbk->isA2) w_buf = SHORTCUT_DESC_A2_Init(s);
      else w_buf = SHORTCUT_DESC_Init(s);

      str_id = Shortcut_Get_MenuItemName( w_buf );
      icon_id = Shortcut_Get_MenuItemIconID( w_buf );

      if ( icon_id == NOIMAGE ) iconidname2id( L"RN_VERT_MY_SHORTCUTS_ICN", TEXTID_ANY_LEN, &icon_id );
      delete( w_buf );

      elem->book_nameId = str_id;
      elem->book_icon = icon_id;
      List_InsertLast(mbk->shortcuts_list, elem);
    }
  }
  if (MINIMIZE_TO_SESSION)
    mbk->blistcnt = mbk->sessions_list->FirstFree;
  else
    mbk->blistcnt = mbk->books_list->FirstFree;

  mbk->elistcnt = mbk->elfs_list->FirstFree;
  mbk->slistcnt = mbk->shortcuts_list->FirstFree;
  mbk->flistcnt = mbk->folders_list->FirstFree;
  mbk->llistcnt = mbk->Logger_list->FirstFree;
}

void get_iconsID( MyBOOK* mbk )
{
  int i;
  IMAGEID imgID;
  int _imgID;

  for ( i = 0; i < ICONS_TAB_COUNT; i++ )
  {
    iconidname2id(id_tab_names[i],TEXTID_ANY_LEN,&_imgID);
    mbk->tabs_image[i].ImageID = _imgID;
  }
  for ( i = 0; i < ICONS_ITEM_COUNT; i++ )
  {
    iconidname2id(id_item_names[i],TEXTID_ANY_LEN,&_imgID);
    mbk->item_image[i].ImageID = _imgID;
  }
  for ( i = 0;i < ICON_DIGITS_COUNT;i++ )
  {
    iconidname2id(id_digits[i],TEXTID_ANY_LEN,&_imgID);
    mbk->digs_image[i].ImageID = _imgID;
  }
  for ( i = 0; i < ICONS_TAB_COUNT; i++ )
  {
    if (wstrrchr(img_tab_names[i],L'/'))
    {
      extractdir(tmppath, tmpname, img_tab_names[i]);
      if (ImageID_Get( tmppath, tmpname, &imgID ) >= 0 )
      {
        mbk->tabs_image[i].ImageID=imgID;
      }
    }
  }
  for ( i = 0; i < ICONS_ITEM_COUNT; i++ )
  {
    if (wstrrchr(img_item_names[i],L'/'))
    {
      extractdir(tmppath, tmpname, img_item_names[i]);
      if (ImageID_Get( tmppath, tmpname, &imgID ) >= 0 )
      {
        mbk->item_image[i].ImageID=imgID;
      }
    }
  }
  for ( i = 0; i < ICON_DIGITS_COUNT; i++ )
  {
    if (wstrrchr(img_digits[i],L'/'))
    {
      extractdir(tmppath, tmpname, img_digits[i]);
      if (ImageID_Get( tmppath, tmpname, &imgID ) >= 0 )
      {
        mbk->digs_image[i].ImageID=imgID;
      }
    }
  }
}

wchar_t* get_path()
{
  wchar_t* path = new wchar_t[wstrlen( GetDir( DIR_INI ) ) + sizeof("/bookman")];
  wstrcpy( path, GetDir( DIR_INI ) );
  wstrcat( path, L"/bookman" );
  return path;
}

int get_file( wchar_t* name, void** buf_set )
{
  int size = 0;
  int file;
  void* buf = 0;
  FSTAT _fstat;

  wchar_t* path = get_path();

  if ( fstat( path, name, &_fstat ) == 0 )
  {
    if ( ( file = _fopen( path, name, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
    {
      buf = malloc( _fstat.fsize+1 );
      fread( file, buf, _fstat.fsize );
      fclose( file );
      size = _fstat.fsize;
    }
  }

  *buf_set = buf;
  delete( path );
  return size;
}

void LoadHidden(MyBOOK * mbk)
{
  if ( mbk->hidden_buf )
  {
    delete (mbk->hidden_buf);
    mbk->hidden_buf=0;
  }
  void* sp;
  mbk->hidden_buf_size = get_file( INI_HIDDEN, &sp );
  mbk->hidden_buf = (char*)sp;
}

// ======================== всякая служебная ерунда end ===========================================

// при убийстве какой либо книги
int onRootListChanged( void* r0, BOOK* bk )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  if ( !mbk->mode_list && !mbk->but_list && !mbk->GUIEditor)
  {
    // создали меню
    mbk->ActiveTAB=GetActiveTab(mbk);
    CreateBookManagerGUI( mbk->ActiveTAB, mbk );
  }
  return 0;
};

// если юзер заснул...
int onUserInactivity( void* r0, BOOK* bk )
{
  MyBOOK* mbk=(MyBOOK*)bk;

  if(!mbk->GUIEditor)
  {
    if ( UserInactivityEventEnabled == 0 )
    {
      CloseMyBook( bk, 0 );
    }
    if (UserInactivityEventEnabled == 1 && !isKeylocked())
    {
      CloseMyBook( bk, 0 );
      UI_Event(LOCKKEY_EVENT);
    }
  }
  return 0;
};

TEXTID onCallback_Books_GetItemName(BOOK_LIST_ITEM * elem)
{
  TEXTID name = GetParam(elem->book_name);

  if (elem->isHidden)
  {
    TEXTID tmp[2];
    tmp[0]=TextID_Create("[H] ",ENC_LAT1,TEXTID_ANY_LEN);
    tmp[1]=name;
    name=TextID_Create(tmp,ENC_TEXTID,2);
  }
  return name;
}

int GetIcon(char*name,wchar_t not_icon)
{
  BOOK_PARAM data;
  int n=GetPARAM(name, &data);
  int icn_id=0;
  if(n!=-1) if(wstrlen(data.i_name)>0) GetIconByName(data.i_name,icn_id);
  if(!icn_id) icn_id=not_icon;
  return icn_id;
}

wchar_t * Get_Call_Logger_path(MyBOOK* mbk)
{
    mbk->llistpos =ListMenu_GetSelectedItem(mbk->llist);

    if (!mbk->Changed)
    {
      REQUEST_DATEANDTIME_GET(SYNC,&mbk->dt);
    }
    wchar_t * freg =new wchar_t[50];
    snwprintf(freg,25,L"/%02d-%02d-%02d",mbk->dt.date.year,mbk->dt.date.mon,mbk->dt.date.day);
    wchar_t * path;
    path = new wchar_t[wstrlen(GetDir(DIR_OTHER|MEM_EXTERNAL))+wstrlen(L"/Call Logger")+wstrlen(freg)+1];
    wstrcpy(path,GetDir(DIR_OTHER|MEM_EXTERNAL));
    wstrcat(path,L"/Call Logger");
    wstrcat(path,freg);
    delete(freg);
    return path;
}

// устанавливаем тексты в пунктах меню
int onCallback_Books( GUI_MESSAGE* msg )
{
  MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );
  int item;
  int parent_item;
  SESSION_LIST_ITEM * ses_elem;
  BOOK_LIST_ITEM * elem;

  switch( GUIonMessage_GetMsg( msg ) )
  {
  case LISTMSG_GetItem:
    if ( mbk->blistcnt )
    {
      item = GUIonMessage_GetCreatedItemIndex( msg );

      if (MINIMIZE_TO_SESSION)
      {
        ses_elem = (SESSION_LIST_ITEM*) List_Get( mbk->sessions_list, item );

        GUIonMessage_SetNumberOfSubItems(msg,0,ses_elem->books_list->FirstFree);
        GUIonMessage_SetMenuItemText( msg, TextID_Copy(ses_elem->session->name) );
      }
      else
      {
        elem = (BOOK_LIST_ITEM*) List_Get( mbk->books_list, item );
        GUIonMessage_SetMenuItemText( msg, onCallback_Books_GetItemName(elem) );

        if(elem->isJava) GUIonMessage_SetMenuItemIcon( msg, 0, elem->book_icon);
        else GUIonMessage_SetMenuItemIcon( msg, 0, GetIcon(elem->book_name,mbk->item_image[0].ImageID));

        if ( !elem->isGuiBook )
        {
          GUIonMessage_SetItemDisabled( msg, TRUE );
          GUIonMessage_SetMenuItemUnavailableText( msg, STR( "Can't set focus to book without GUI..." ) );
        }
      }
    }
    break;

  case LISTMSG_HighlightChanged:
    mbk->blistpos = GUIonMessage_GetSelectedItem( msg );
    RefreshBookSoftkeys( mbk );
    break;

  case LISTMSG_GetSubItem:
    parent_item = GUIonMessage_SubItem_GetCreatedParentIndex(msg);
    item = GUIonMessage_SubItem_GetCreatedIndex(msg);
    ses_elem = (SESSION_LIST_ITEM*) List_Get( mbk->sessions_list, parent_item );
    elem = (BOOK_LIST_ITEM*)List_Get(ses_elem->books_list,item);

    GUIonMessage_SubItem_SetText(msg,onCallback_Books_GetItemName(elem));

    if ( !elem->isGuiBook )
    {
      GUIonMessage_SubItem_SetDisabled( msg, TRUE );
    }
    break;

  case LISTMSG_SubItemHighlightChanged:
    ses_elem = (SESSION_LIST_ITEM*) List_Get( mbk->sessions_list, mbk->blistpos );
    ses_elem->pos_subitem = GUIonMessage_SubItem_GetSelectedIndex(msg);

    elem=(BOOK_LIST_ITEM*)List_Get(ses_elem->books_list,ses_elem->pos_subitem);
    if ( !elem->isGuiBook ) GUIonMessage_SetMenuItemUnavailableText( msg, STR( "Can't set focus to book without GUI..." ) );

    RefreshBookSoftkeys( mbk );
    break;
  }

  return 1;
}

int onCallback_Elfs( GUI_MESSAGE* msg )
{
  MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );
  int item;
  BOOK_LIST_ITEM* elem;

  switch( GUIonMessage_GetMsg( msg ) )
  {
  case LISTMSG_GetItem:
    if ( mbk->elistcnt )
    {
      item = GUIonMessage_GetCreatedItemIndex( msg );
      elem = (BOOK_LIST_ITEM*)List_Get( mbk->elfs_list, item );
      TEXTID name = GetParam(elem->book_name);

      if (elem->isHidden)
      {
        TEXTID tmp[2];
        tmp[0]=TextID_Create("[H] ",ENC_LAT1,TEXTID_ANY_LEN);
        tmp[1]=name;
        name=TextID_Create(tmp,ENC_TEXTID,2);
      }
        GUIonMessage_SetMenuItemText( msg, name );
        GUIonMessage_SetMenuItemIcon( msg, 0, GetIcon(elem->book_name,mbk->item_image[1].ImageID));
      }
    break;
  case LISTMSG_HighlightChanged:
    mbk->elistpos = GUIonMessage_GetSelectedItem( msg );
    RefreshElfSoftkeys( mbk, mbk->elistpos );
    break;
  }
  return 1;
}

int onCallback_Shortcuts(GUI_MESSAGE * msg)
{
  MyBOOK* myBook = (MyBOOK*) GUIonMessage_GetBook( msg );
  int item;
  BOOK_LIST_ITEM* elem;

  switch( GUIonMessage_GetMsg( msg ) )
  {
  case LISTMSG_GetItem:
      item = GUIonMessage_GetCreatedItemIndex( msg );
      elem = (BOOK_LIST_ITEM*)List_Get( myBook->shortcuts_list, item );
      GUIonMessage_SetMenuItemText( msg, TextID_Copy(elem->book_nameId) );
      GUIonMessage_SetMenuItemIcon( msg, 0, elem->book_icon);
    break;
  }
  return 1;
}

int onCallback_Folders(GUI_MESSAGE * msg)
{
  int item;
  int icon_id;

  switch( GUIonMessage_GetMsg(msg) )
  {
  case LISTMSG_GetItem:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      GUIonMessage_SetMenuItemText(msg,TextID_Create(Fo_Nam_Icon(2*(*GET_Folder_ID[item])),ENC_UCS2,TEXTID_ANY_LEN));
      iconidname2id(Fo_Nam_Icon(2*(*GET_Folder_ID[item])+1),TEXTID_ANY_LEN,&icon_id);
      GUIonMessage_SetMenuItemIcon(msg,0,(wchar_t)icon_id);
      break;
  }
  return 1;
}

int onCallback_Loggers(GUI_MESSAGE * msg)
{
  MyBOOK* myBook = (MyBOOK*) GUIonMessage_GetBook( msg );
  int item;
  int name_id;
  int icon_id;
  int str_id;

  switch( GUIonMessage_GetMsg(msg) )
  {
  case LISTMSG_GetItem:
    item=GUIonMessage_GetCreatedItemIndex(msg);

      textidname2id(Call_Logger[item*3],-1,&name_id);
      GUIonMessage_SetMenuItemText(msg,name_id);
      iconidname2id(Call_Logger[item*3+2],TEXTID_ANY_LEN,&icon_id);
      GUIonMessage_SetMenuItemIcon(msg,0,(wchar_t)icon_id);

    wchar_t * path = Get_Call_Logger_path(myBook);

    FSTAT _fstat;
    if (fstat(path,Call_Logger[item*3+1],&_fstat)!=0)
    {
      GUIonMessage_SetItemDisabled(msg,1);
      textidname2id(L"DB_NO_FILES_TXT",TEXTID_ANY_LEN,&str_id);
      GUIonMessage_SetMenuItemUnavailableText(msg,str_id);
    }
    delete (path);
    break;
  }
  return 1;
}

int Elf_OpenFile(const wchar_t* elf, const wchar_t* filepath, const wchar_t* filename)
{
  FSTAT _fstat;
  if(wstrlen(elf) == 0) return 1;
  extractdir(tmppath, tmpname, elf);
  if(!fstat(tmppath, tmpname,&_fstat))
  { }
  else if(!fstat(GetDir(DIR_ELFS|MEM_INTERNAL), tmpname, &_fstat))
  {
    wstrcpy(tmppath,GetDir(DIR_ELFS|MEM_INTERNAL));
  }
  else if(!fstat(GetDir(DIR_ELFS|MEM_EXTERNAL), tmpname, &_fstat))
  {
    wstrcpy(tmppath,GetDir(DIR_ELFS|MEM_EXTERNAL));
  }
  else
  {
    return 2;
  }
  if(!filepath || !filename) return 3;
  wstrcat(tmppath,L"/");
  wstrcat(tmppath, tmpname);
  elfload(tmppath, (void*)filepath, (void*)filename,0);
  return 0;
}

void BcfgConfig(BOOK * book, GUI* lt)
{
  BOOK * bk = GetBook( book, ELFLIST );

  if (bk)
  {
    if (CheckEv(bk,ELF_BCFG_CONFIG_EVENT))
    {
      int err = Elf_OpenFile(cfg_bcfgedit,0,0);
      if(err==3)
      {
        MSG_BCFG * msg = new MSG_BCFG;
        msg->book = book;
        msg->bcfg_p = tmppath;
        msg->bcfg_n = tmpname;
        UI_Event_toBookIDwData(ELF_BCFG_CONFIG_EVENT,BookObj_GetBookID(bk),msg,(void(*)(void*))mfree_adr());
      }
      else if(err==2)
        MessageBox(EMPTY_TEXTID,STR("BcfgEdit.elf не найден!"),NOIMAGE,1,3000,book);
      else if(err==1)
        MessageBox(EMPTY_TEXTID,STR("В настройках BookManager'а укажите путь до BcfgEdit.elf!"),NOIMAGE,1,10000,book);
    } else {
        MessageBox(EMPTY_TEXTID,STR("BcfgConfigEvent не поддерживается эльфом..."),NOIMAGE,1,3000,book);
    }
  }
}

void Configuration(BOOK * book, GUI* lt)
{
  if(Elf_OpenFile(cfg_bcfgedit, successed_config_path, successed_config_name))
  {
      MessageBox(EMPTY_TEXTID,STR("BcfgEdit.elf не найден"),NOIMAGE,1,3000,book);
  }
}

void executeAction(wchar_t *appName)
{
  if(appName)
  {
    if(wstrlen(appName))
    {
      Shortcut_Run(appName);
    }
  }
}

void onEnterPressedGUI( BOOK* book, GUI* lt )
{
  BOOK* bk = GetBook( book, ELFLIST );

  if ( bk )
  {
    BookObj_SetFocus( bk, 0 );
    CloseMyBook( book, 0 );
  }
}

void onEnterPressed_Books( BOOK* book, GUI* lt )
{
  BookObj_SetFocus( GetBook( book, BOOKLIST ), 0 );
  CloseMyBook( book, 0 );
}

void onEnterPressed_Elfs( BOOK* book, GUI* lt )
{
  BOOK* bk = GetBook( book, ELFLIST );

  if ( bk )
  {
    BcfgConfig( book, 0 );
  }
}

void onEnterPressed_Shortcuts(BOOK * book, GUI* lt)
{
  MyBOOK* mbk = (MyBOOK*) book;
  mbk->slistpos=ListMenu_GetSelectedItem(mbk->slist);
  wchar_t Action_name[MAX_SHORTCUTS_ACTION_LEN];
  wstrcpy(Action_name,shortcuts_tab_actions[mbk->slistpos]);
  executeAction (Action_name);
}

void onEnterPressed_Folders(BOOK * book, GUI* lt)
{
  MyBOOK* mbk = (MyBOOK*) book;
  mbk->flistpos=ListMenu_GetSelectedItem(mbk->flist);

  if(mbk->folder[0]) delete(mbk->folder[0]);
  if(mbk->folder[1]) delete(mbk->folder[1]);
  mbk->fol = 2;
  mbk->fmod = 0x12;
  int Fol_ID = Folder_ID[*GET_Folder_ID[mbk->flistpos]];
  if (Fol_ID == 22 || Fol_ID == 24)
  {
    mbk->folder[0]=GetDir(Fol_ID| MEM_EXTERNAL);
    mbk->folder[1]= 0;
    mbk->fol = 1;
  } else {
    mbk->folder[0]=GetDir(Fol_ID | MEM_INTERNAL);
    mbk->folder[1]=GetDir(Fol_ID | MEM_EXTERNAL);
  }
  if ( Fol_ID ==2 ||Fol_ID ==4 ) mbk->fmod = 0x13 ;
  mbk->folname=TextID_Create(Fo_Nam_Icon(2*(*GET_Folder_ID[mbk->flistpos])),ENC_UCS2,TEXTID_ANY_LEN);
  mbk->ActiveTAB=GetActiveTab(mbk);
  FolderCreate(book);
}

void onEnterPressed_Loggers(BOOK * book, GUI* lt)
{
  MyBOOK* mbk = (MyBOOK*) book;
  FSTAT _fstat;

  wchar_t * path = Get_Call_Logger_path(mbk);
  wchar_t * name = Call_Logger[mbk->llistpos*3+1];

  if (fstat(path, name, &_fstat)==0)
  {
    if(cfg_open_log_by_elf == 0)
    {
      char * TXTOpen = CreateURI(path, name, GetURIScheme(file));
      Browser_OpenURI(0,TXTOpen,2);
      delete(TXTOpen);
    } else {
      if(Elf_OpenFile(cfg_elf_path, path, name))
      {
          MessageBox(EMPTY_TEXTID,STR("Эльф не найден"),NOIMAGE,1,3000,book);
      }
    }
  }
  delete(path);
}

void DelRegistrer(BOOK * bk, GUI* lt)
{
  MyBOOK* mbk = (MyBOOK*) bk;
  wchar_t * path = Get_Call_Logger_path(mbk);
  FileDelete (path ,Call_Logger[mbk->llistpos*3+1] , 0);
  delete (path);

  if (mbk->YesNoQuestion)GUIObject_Destroy(mbk->YesNoQuestion);
  mbk->YesNoQuestion = 0;

  GUIObject_Destroy(mbk->llist);
  mbk->llist=0;
  GUIObject_Destroy(mbk->gui);
  mbk->gui=0;
  CreateBookManagerGUI( GetActiveTab(mbk), mbk );
}

void onDelPressed_Books( BOOK* book )
{
  BOOK_LIST_ITEM * elem = GetBookListItem( book, BOOKLIST );
  if (elem)
  {
    BOOK* bk = elem->book;

    if (elem->isJava_2010==TRUE) JavaSession_Manager( 0x0E );
    else if (elem->isJava==TRUE) UI_Event_toBookID( TERMINATE_SESSION_EVENT, BookObj_GetBookID( bk ) );
    else
    {
      BOOK * sby_book=Find_StandbyBook();
      if (!DestroyMethod && bk!=sby_book) FreeBook(bk);
      else
      {
        if ( CheckEv( bk, TERMINATE_SESSION_EVENT ) )
          UI_Event_toBookID( TERMINATE_SESSION_EVENT, BookObj_GetBookID( (BOOK*)List_Get( bk->xbook->app_session->listbook, bk->xbook->app_session->listbook->FirstFree - 1 ) ) );
        else
          UI_Event_toBookID( RETURN_TO_STANDBY_EVENT, BookObj_GetBookID( (BOOK*)List_Get( bk->xbook->app_session->listbook, bk->xbook->app_session->listbook->FirstFree - 1 ) ) );
      }
    }
  }
}

void onDelPressed_Elfs( BOOK* book )
{
  BOOK* bk = GetBook( book, ELFLIST );

  if ( bk && bk !=  Find_StandbyBook())
  {
    if ( CheckEv( bk, RETURN_TO_STANDBY_EVENT ) )
    {
      UI_Event_toBookID( RETURN_TO_STANDBY_EVENT, BookObj_GetBookID( bk ) );
    } else {
      if ( CheckEv( bk, ELF_TERMINATE_EVENT ) )
      {
        UI_Event_toBookID( ELF_TERMINATE_EVENT, BookObj_GetBookID( bk ));
      } else {
          MessageBox(EMPTY_TEXTID,STR("Выгрузка эльфа не поддерживается..."),NOIMAGE,1,3000,book);
      }
    }
  }
}

void onDelPressed_Loggers( BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;
  wchar_t * path = Get_Call_Logger_path(mbk);
  FSTAT _fstat;

  if (fstat(path,Call_Logger[mbk->llistpos*3+1],&_fstat)==0)
  {
    TEXTID DeleteQ, str_id;
    textidname2id(L"DELETEQ_TXT",-1,&DeleteQ);
    textidname2id(Call_Logger[mbk->llistpos*3],-1,&str_id);
    int icon_id;
    iconidname2id(L"POPUP_WARNING_ICN",TEXTID_ANY_LEN,&icon_id);
    mbk->YesNoQuestion=CreateYesNoQuestionVA(0,
                                      VAR_BOOK(mbk),
                                      VAR_YESNO_PRE_QUESTION(str_id),
                                      VAR_YESNO_QUESTION(DeleteQ),
                                      0);
    YesNoQuestion_SetIcon(mbk->YesNoQuestion,icon_id);
    GUIObject_SoftKeys_SetAction(mbk->YesNoQuestion, ACTION_YES, DelRegistrer);
    GUIObject_SoftKeys_SetAction(mbk->YesNoQuestion, ACTION_NO, ReturnMyBook);
    GUIObject_SoftKeys_SetAction(mbk->YesNoQuestion, ACTION_BACK, ReturnMyBook);
  }
  delete path;
}

void onDelPressed()
{
  MyBOOK* mbk = (MyBOOK*) FindBook( isBookManager );

  if (GetActiveTab(mbk) == books_tab) onDelPressed_Books( mbk );
  if (GetActiveTab(mbk) == elfs_tab) onDelPressed_Elfs( mbk ) ;
  if (GetActiveTab(mbk) == shortcuts_tab) DelShortcut( mbk, 0 );
  if (GetActiveTab(mbk) == loggers_tab) onDelPressed_Loggers( mbk );
}

// при нажатии "Автора!" на эльфе
void Author( BOOK* book, GUI* lt )
{
  BOOK* bk = GetBook( book, ELFLIST );
  if ( bk )
  {
    if ( CheckEv( bk, ELF_SHOW_INFO_EVENT ) )
    {
      MSG* msg = new MSG;
      msg->book = book;
      UI_Event_toBookIDwData( ELF_SHOW_INFO_EVENT, BookObj_GetBookID( bk ), msg, ( void(*)( void*) )mfree_adr() );
    } else {
        MessageBox(EMPTY_TEXTID,STR("Автор неизвестен"),NOIMAGE,1,3000,book);
    }
  }
}

void Copyright( BOOK* book, GUI* lt )
{
  MyBOOK* mbk = (MyBOOK*) book;
  mbk->blistpos = ListMenu_GetSelectedItem( mbk->blist );
  MessageBox(EMPTY_TEXTID,COPYRIGHT_STRING,NOIMAGE,1,30000,book);
}

void TerminateManager( BOOK* Book, GUI* )
{
  FreeBook( Book );
  MODIFYKEYHOOK( NewKey, KEY_HOOK_REMOVE, NULL );
  SUBPROC( elf_exit );
}

void HideBook( BOOK* bk, GUI* gui )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  int f;
  wchar_t* path = get_path();

  mbk->ActiveTAB=GetActiveTab(mbk);
  BOOK_LIST_ITEM* elem = GetBookListItem(mbk,mbk->ActiveTAB);

  if ( ( f = _fopen( path, INI_HIDDEN, FSX_O_CREAT|FSX_O_APPEND, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
  {
    char temp_buf[MAX_BOOK_NAME_LEN+10];
    strcpy( temp_buf, elem->book_name );
    strcat( temp_buf, "\r\n" );
    fwrite( f, temp_buf, strlen( temp_buf ) );
    fclose( f );
  }

  delete path;

  LoadHidden(mbk);

  CreateBookManagerGUI( mbk->ActiveTAB, mbk );
}

void ShowBook( BOOK* bk, GUI* gui )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  int f;
  char* pos;
  char* end_pos;
  wchar_t* path = get_path();

  mbk->ActiveTAB=GetActiveTab(mbk);

  BOOK_LIST_ITEM* elem = GetBookListItem(mbk,mbk->ActiveTAB);

  if ( mbk->hidden_buf )
  {
    pos = strstr( mbk->hidden_buf, elem->book_name );
    end_pos = pos + strlen(elem->book_name) + sizeof("\r\n") - sizeof("");
    f = _fopen( path, INI_HIDDEN, FSX_O_RDWR|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 );
    fwrite( f, mbk->hidden_buf, pos - mbk->hidden_buf );
    fwrite( f, end_pos, mbk->hidden_buf_size - (end_pos - mbk->hidden_buf) );
    fclose( f );
  }

  elem->isHidden=FALSE;

  delete path;

  LoadHidden(mbk);

  CreateBookManagerGUI( mbk->ActiveTAB, mbk );
}

void HiddenMode( BOOK* bk, GUI* gui )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  mbk->ActiveTAB=GetActiveTab(mbk);

  if (mbk->ActiveTAB==books_tab)
  {
    if (mbk->books_show_hid==FALSE) mbk->books_show_hid=TRUE;
    else mbk->books_show_hid=FALSE;
  } else {
    if (mbk->elfs_show_hid==FALSE) mbk->elfs_show_hid=TRUE;
    else mbk->elfs_show_hid=FALSE;
  }

  CreateBookManagerGUI( mbk->ActiveTAB, mbk );
}

void NoGUIMode( BOOK* bk, GUI* gui )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  mbk->ActiveTAB=GetActiveTab(mbk);

  if (mbk->ActiveTAB==books_tab)
  {
    if (mbk->books_show_nogui==FALSE) mbk->books_show_nogui=TRUE;
    else mbk->books_show_nogui=FALSE;
  } else {
    if (mbk->elfs_show_nogui==FALSE) mbk->elfs_show_nogui=TRUE;
    else mbk->elfs_show_nogui=FALSE;
  }

  CreateBookManagerGUI( mbk->ActiveTAB, mbk );
}

int StartElf( wchar_t* path, wchar_t* name )
{
  int res = -1;
  wchar_t* elfname = new wchar_t[wstrlen( path ) + wstrlen( name )+1];
  wstrcpy( elfname, path );
  wstrcat( elfname, name );

  {
    wchar_t* path;
    wchar_t* name;
    int len;
    name = wstrrchr( elfname, '/' );
    if ( name )
    {
      path = new wchar_t[ ( len = name-elfname ) + 1 ];
      memcpy( path, elfname, len * sizeof(wchar_t) );
      path[len] = 0;
      name++;
      if ( !fstat( path, name, 0 ) )
      {
        res = elfload( elfname, 0, 0, 0 );
      }
      delete( path );
    }
  }
  delete( elfname );
  return res;
}

void StartShortcut_Internal(const wchar_t *sh)
{
  MyBOOK* mbk = (MyBOOK*) FindBook( isBookManager );

  if (wstrwstr(sh,L"java:"))
  {
    int id_len=wstrlen(wstrwstr(sh,L"ID=")+3);
    wchar_t buf[20];
    wstrcpy(buf,wstrwstr(sh,L"ID=")+3);

    int ID;
    char sp_flag;
    wtoi(buf,id_len,&ID);

    REQUEST_UI_OAF_START_APPLICATION(ASYNC,ID+0x10000,&sp_flag);
    CloseMyBook(mbk,0);
    } else {
     Shortcut_Run((wchar_t *)sh);
    }
}

void StartShortcut_External(const wchar_t *sh)
{
  if(StartElf(GetDir(DIR_ELFS | MEM_INTERNAL),(wchar_t *)sh)< 0)
    if (StartElf(GetDir(DIR_ELFS | MEM_EXTERNAL),(wchar_t *)sh)< 0)
      {
        int ms[3];
        ms[0]=STR("ZBin");
        ms[1]=TextID_Create(sh,ENC_UCS2,wstrlen(sh));
        ms[2]=STR("\n\nне найден..");
        MessageBox( EMPTY_TEXTID, TextID_Create(ms,ENC_TEXTID,3), NOIMAGE, 1, 3000, FindBook(isBookManager));
    }
}

void myOnKey( DISP_OBJ* p, int keyID, int i2, int i3, int press_mode )
{
  MyBOOK* mbk = (MyBOOK*) FindBook( isBookManager );

  mbk->oldOnKey( p, keyID, i2, i3, press_mode );

  int item = keyID - KEY_DIGITAL_0;

  if ( keyID == KEY_DEL && press_mode == KBD_SHORT_RELEASE )
    onDelPressed();

  if ( keyID == KeyMinimize && press_mode == KeyMinimizePressMode )
  {
    BOOK* stby_bk = Find_StandbyBook();
    if ( stby_bk ) BookObj_SetFocus( stby_bk, 0 );
    CloseMyBook( mbk, 0 );
  }

  else if ( keyID == KeyCloseAll && press_mode == KeyCloseAllPressMode )
  {
    UI_Event( RETURN_TO_STANDBY_EVENT );
    CloseMyBook( mbk, 0 );
  }

  else if ( press_mode == KBD_SHORT_RELEASE || press_mode == KBD_LONG_PRESS )
  {
    if((item >= 0) && (item <= 9))
    {
     if (GetActiveTab(mbk)==books_tab)
     {
      const wchar_t *sh = books_tab_actions[press_mode == KBD_SHORT_RELEASE ? 0:1][item];

      if(wstrlen(sh)) StartShortcut_Internal(sh);
      } else
      if (GetActiveTab(mbk)==elfs_tab)
      {
      const wchar_t *sh = elfs_tab_actions[press_mode == KBD_SHORT_RELEASE ? 0:1][item];

      if(wstrlen(sh)) StartShortcut_External(sh);
      } else
      if (GetActiveTab(mbk)==shortcuts_tab)
      {
        wchar_t Action_name[MAX_SHORTCUTS_ACTION_LEN];
        wstrcpy(Action_name,shortcuts_tab_actions[item-1]);
        executeAction (Action_name);
      }
     if (StandbyOnTop) CloseMyBook(mbk,0);
    }
  }
}

void SetBrightness(BOOK * book, GUI* lt)
{
  MyBOOK* mbk = (MyBOOK*) book;
  int bright;

  if (mbk->Br_set == 0)
  {
    bright = bright_off;
    mbk->Br_set = 1;
  } else {
    bright = bright_on;
    mbk->Br_set = 0;
  }

  Display_SetBrightness(0,bright);

  CloseMyBook(mbk,0);
}

void onTabSwitch( BOOK* bk, int active_tab )
{
  MyBOOK* mbk = (MyBOOK*) bk;
  mbk->ActiveTAB = active_tab;
}

void onReturnStandby( BOOK* bk, GUI*)
{
  CloseMyBook(bk,0);
  UI_Event(RETURN_TO_STANDBY_EVENT);
}

void AcceptDIinput(BOOK* bk, GUI* data)
{
  MyBOOK* mbk = (MyBOOK*) bk;
  unsigned int i = DateInput_GetDateInt(data);
  mbk->dt.date.year=DI_GET_YEAR(i);
  mbk->dt.date.mon=TI_GET_MONTH(i);
  mbk->dt.date.day=TI_GET_DAY(i);
  mbk->Changed = TRUE;

  if( mbk->DateInput ) GUIObject_Destroy(mbk->DateInput);
  mbk->DateInput = 0;
  GUIObject_Destroy(mbk->llist);
  mbk->llist=0;
  GUIObject_Destroy(mbk->gui);
  mbk->gui=0;
  CreateBookManagerGUI( GetActiveTab(mbk), mbk );
}

void CloseDIinput(BOOK* bk, GUI*)
{
  MyBOOK* mbk = (MyBOOK*) bk;
  if(mbk->DateInput) GUIObject_Destroy(mbk->DateInput);
  mbk->DateInput = 0;
}

void ChangeDate(BOOK * book, GUI* lt)
{
  MyBOOK* mbk = (MyBOOK*) book;

 if (!mbk->Changed)
 {
   REQUEST_DATEANDTIME_GET(SYNC,&mbk->dt);
 }
 int Header;
 textidname2id(L"CALE_GO_TO_DATE_TXT",-1,&Header);

 int icon_id;
 int date_format=4;
 char date_f;
 if (!REQUEST_DATEFORMAT_GET(SYNC,&date_f)) date_format=date_f;
 mbk->DateInput = CreateDateInputVA(0,
                             VAR_HEADER_TEXT(Header),
                             VAR_DATEINP_DATEFORMAT(date_format),
                             VAR_DATEINP_DATE(&mbk->dt),
                             VAR_BOOK(book),
                             0);
  GUIObject_SoftKeys_SetAction(mbk->DateInput, ACTION_ACCEPT, AcceptDIinput);
  GUIObject_SoftKeys_SetText(mbk->DateInput, ACTION_ACCEPT, STR("OK"));
  GUIObject_SoftKeys_SetVisible(mbk->DateInput, ACTION_ACCEPT,1);
  GUIObject_SoftKeys_SetAction(mbk->DateInput, ACTION_BACK, CloseDIinput);
  if (iconidname2id(L"FR_INPUT_GUI_SET_DATE_ICN", -1, &icon_id))
    GUIInput_SetIcon(mbk->DateInput, icon_id);
}

void RefreshBookSoftkeys( MyBOOK* mbk )
{
  if ( mbk->blistcnt )
  {
    BOOK_LIST_ITEM * elem = GetBookListItem(mbk,BOOKLIST);

    if ( elem->isGuiBook )
      GUIObject_SoftKeys_SetVisible( mbk->blist, ACTION_SELECT1, TRUE );
    else
      GUIObject_SoftKeys_SetVisible( mbk->blist, ACTION_SELECT1, FALSE );

    GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, 1 - elem->isHidden );
    GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, elem->isHidden );
    GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, TRUE );
  }
  else
  {
    ListMenu_SetNoItemText( mbk->blist, STR( "Нет книг" ) );
    GUIObject_SoftKeys_SetVisible( mbk->blist, ACTION_SELECT1, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, FALSE );

    mbk->blistpos = 0;
  }

  GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWHIDDEN_SOFTKEY, 1 - mbk->books_show_hid );
  GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDEHIDDEN_SOFTKEY, mbk->books_show_hid );

  GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWNOGUI_SOFTKEY, 1 - mbk->books_show_nogui );
  GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDENOGUI_SOFTKEY, mbk->books_show_nogui );
}

void RefreshElfSoftkeys( MyBOOK* mbk, int item )
{
 if ( mbk->elistcnt )
 {
   BOOK_LIST_ITEM* elem = (BOOK_LIST_ITEM*) List_Get( mbk->elfs_list, item );

   u16 key;
   if(ConfOrGUI) key = TAB_ELFS_GUI_SOFTKEY;
   else key = ACTION_SELECT1;

   if ( !elem->isGuiBook )

   GUIObject_SoftKeys_SetEnable( mbk->elist, key, FALSE );
   else GUIObject_SoftKeys_SetEnable( mbk->elist, key, TRUE );

   GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, 1 - elem->isHidden );
   GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, elem->isHidden );
   GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, TRUE );
   GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, TRUE );

   if(ConfOrGUI) key = ACTION_SELECT1;
   else key = TAB_ELFS_GUI_SOFTKEY;

   BOOK * bk = GetBook( mbk, ELFLIST );

   if (CheckEv(bk,ELF_BCFG_CONFIG_EVENT)) GUIObject_SoftKeys_SetEnable( mbk->elist, key, TRUE );
   else GUIObject_SoftKeys_SetEnable( mbk->elist, key, FALSE );
  }
   else
  {
    ListMenu_SetNoItemText( mbk->elist, STR( "Нет эльфов" ) );
    GUIObject_SoftKeys_SetVisible( mbk->elist, ACTION_SELECT1, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, FALSE );
    GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_GUI_SOFTKEY, FALSE );

    mbk->elistpos = 0;
 }
  GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWHIDDEN_SOFTKEY, 1 - mbk->elfs_show_hid );
  GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDEHIDDEN_SOFTKEY, mbk->elfs_show_hid );

  GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWNOGUI_SOFTKEY, 1 - mbk->elfs_show_nogui );
  GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDENOGUI_SOFTKEY, mbk->elfs_show_nogui );
}

void CreateBMenu(MyBOOK* mbk,int pos)
{
  int str_id;
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[0].ImageID, 0 );
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[1].ImageID, 1 );

  if(mbk->blist) ListMenu_DestroyItems(mbk->blist);
  mbk->blist = CreateListMenu( mbk, 0 );
  ListMenu_SetOnMessage( mbk->blist, onCallback_Books );
  ListMenu_SetItemCount( mbk->blist, mbk->blistcnt );

  if ( mbk->blistpos > mbk->blistcnt ) ListMenu_SetCursorToItem( mbk->blist, mbk->blistcnt );
  else ListMenu_SetCursorToItem( mbk->blist, mbk->blistpos );

  if (MINIMIZE_TO_SESSION) ListMenu_SetItemStyle( mbk->blist, 3 );

  GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_BACK, CloseMyBook );

  GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_SELECT1, onEnterPressed_Books );
  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHORTCUTS_SOFTKEY, Shortcuts );
  textidname2id( TAB_SHORTCUTS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHORTCUTS_SOFTKEY, str_id );

  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, BookNames );
  textidname2id( TAB_RENAME_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, str_id );

  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, HideBook );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, STR( "Спрятать книгу" ) );
  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, ShowBook );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, STR( "Показать книгу" ) );

  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHOWHIDDEN_SOFTKEY, HiddenMode );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHOWHIDDEN_SOFTKEY, STR( "Показать скрытие" ) );
  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_HIDEHIDDEN_SOFTKEY, HiddenMode );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_HIDEHIDDEN_SOFTKEY, STR( "Спрятать скрытые" ) );

  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHOWNOGUI_SOFTKEY, NoGUIMode );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHOWNOGUI_SOFTKEY, STR( "Показать без GUI" ) );
  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_HIDENOGUI_SOFTKEY, NoGUIMode );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_HIDENOGUI_SOFTKEY, STR( "Спрятать без GUI" ) );

  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, 0 );
  textidname2id( TAB_SETTINGS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, str_id );

  GUIObject_SoftKeys_SetAction(mbk->blist,TAB_BOOKS_CONFIG_SOFTKEY,Configuration);
  textidname2id( TAB_BOOKS_SETTINGS_NAME_SOFTKEY ,TEXTID_ANY_LEN,&str_id);
  GUIObject_SoftKeys_SetText(mbk->blist,TAB_BOOKS_CONFIG_SOFTKEY,str_id);

  GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_COPYRIGHT_SOFTKEY, Copyright );
  GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_COPYRIGHT_SOFTKEY, STR( "О программе" ) );

  if (mbk->isA2)
  {
    GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_LONG_BACK, onReturnStandby );
    GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_EXIT_SOFTKEY, PreTerminateManager );
    GUIObject_SoftKeys_SetText(mbk->blist, TAB_BOOKS_EXIT_SOFTKEY, STR("Выход"));
  } else  {
    GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_LONG_BACK, PreTerminateManager );
  }

  GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_SHOWBOOK_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_HIDEBOOK_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_SHOWHIDDEN_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_HIDEHIDDEN_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_SHOWNOGUI_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_HIDENOGUI_SOFTKEY );

  mbk->oldOnKey = DispObject_GetOnKey( GUIObject_GetDispObject( mbk->blist ) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject( mbk->blist ) ), myOnKey );

  RefreshBookSoftkeys(mbk);
  TabMenuBar_SetTabGui( mbk->gui, pos, mbk->blist );

  if(cfg_heap_format == 0) snwprintf(heap,196,(wchar_t*)L"Heap: %dKB",GetFreeBytesOnHeap()/1024);
  else snwprintf(heap,196,(wchar_t*)L"Heap: %4.2f%MB",(float)GetFreeBytesOnHeap()/1024.0/1024.0);

  TabMenuBar_SetTabTitle( mbk->gui, pos, TextID_Create(heap,ENC_UCS2,TEXTID_ANY_LEN));
}

void CreateEMenu(MyBOOK* mbk,int pos)
{
  int str_id;
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[2].ImageID, 0 );
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[3].ImageID, 1 );

  if(mbk->elist) ListMenu_DestroyItems(mbk->elist);
  mbk->elist = CreateListMenu( mbk, 0 );
  ListMenu_SetCursorToItem( mbk->elist, 0 );
  ListMenu_SetOnMessage( mbk->elist, onCallback_Elfs );
  ListMenu_SetItemCount( mbk->elist, mbk->elistcnt );

  if ( mbk->elistpos > mbk->elistcnt ) ListMenu_SetCursorToItem( mbk->elist, mbk->elistcnt );
  else ListMenu_SetCursorToItem( mbk->elist, mbk->elistpos );

  GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_BACK, CloseMyBook );

  if(ConfOrGUI)
  {
    GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_SELECT1, onEnterPressed_Elfs );
    GUIObject_SoftKeys_SetText( mbk->elist, ACTION_SELECT1, STR( "Конфиг" ) );
    GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_GUI_SOFTKEY, onEnterPressedGUI );
    textidname2id( TAB_ELFS_GUI_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
    GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_GUI_SOFTKEY, str_id );
  }
  else
  {
    GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_SELECT1, onEnterPressedGUI );
    GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_GUI_SOFTKEY, onEnterPressed_Elfs );
    GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_GUI_SOFTKEY, STR( "Конфиг" ) );
  }

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHORTCUTS_SOFTKEY, Shortcuts );
  textidname2id( TAB_SHORTCUTS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHORTCUTS_SOFTKEY, str_id );

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, BookNames );
  textidname2id( L"DB_RENAME_TXT", TEXTID_ANY_LEN, &str_id );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, str_id );

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, HideBook );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, STR( "Спрятать Elf" ) );
  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, ShowBook );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, STR( "Показать Elf" ) );

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHOWHIDDEN_SOFTKEY, HiddenMode );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHOWHIDDEN_SOFTKEY, STR( "Показать скрытые" ) );
  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_HIDEHIDDEN_SOFTKEY, HiddenMode );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_HIDEHIDDEN_SOFTKEY, STR( "Спрятать скрытые" ) );

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHOWNOGUI_SOFTKEY, NoGUIMode );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHOWNOGUI_SOFTKEY, STR( "Показать без GUI" ) );
  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_HIDENOGUI_SOFTKEY, NoGUIMode );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_HIDENOGUI_SOFTKEY, STR( "Спрятать без GUI" ) );

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, 0 );
  textidname2id( TAB_SETTINGS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, str_id );

  GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, Author );
  GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, STR( "Об ELF" ) );

  if (mbk->isA2)
  {
    GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_LONG_BACK, onReturnStandby );
    GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_EXIT_SOFTKEY, PreTerminateManager );
    GUIObject_SoftKeys_SetText(mbk->elist, TAB_ELFS_EXIT_SOFTKEY, STR("Выход"));
  } else  {
    GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_LONG_BACK, PreTerminateManager );
  }

  GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_SHOWBOOK_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_HIDEBOOK_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_SHOWHIDDEN_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_HIDEHIDDEN_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_SHOWNOGUI_SOFTKEY );
  GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_HIDENOGUI_SOFTKEY );

  mbk->oldOnKey = DispObject_GetOnKey( GUIObject_GetDispObject( mbk->elist ) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject( mbk->elist ) ), myOnKey );

  RefreshElfSoftkeys( mbk, ListMenu_GetSelectedItem( mbk->elist ) );
  TabMenuBar_SetTabGui( mbk->gui, pos, mbk->elist );

  snwprintf(elfy,49,(wchar_t*)L"Elfs в памяти: %d", mbk->elistcnt);

  TabMenuBar_SetTabTitle( mbk->gui, pos, TextID_Create(elfy,ENC_UCS2,TEXTID_ANY_LEN));
}

void CreateSMenu(MyBOOK* mbk,int pos)
{
  int str_id;
  wchar_t brightness[40];
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[4].ImageID, 0);
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[5].ImageID, 1);

  if(mbk->slist) ListMenu_DestroyItems(mbk->slist);
  mbk->slist = CreateListMenu( mbk, 0 );

  char bright = 0;
  bright = Display_GetBrightness(0);
  ListMenu_SetCursorToItem(mbk->slist,0);
  ListMenu_SetOnMessage( mbk->slist, onCallback_Shortcuts );
  int ShCnt = getShortcutsNum(shortcuts_tab);
  ListMenu_SetItemCount(mbk->slist,ShCnt);

  if (mbk->slistpos > ShCnt) ListMenu_SetCursorToItem(mbk->slist,ShCnt);
  else ListMenu_SetCursorToItem(mbk->slist, mbk->slistpos);

  GUIObject_SoftKeys_SetAction(mbk->slist,ACTION_BACK, CloseMyBook);

  if (!mbk->shortcuts_list->FirstFree)
  {
    ListMenu_SetNoItemText(mbk->slist,TextID_Create(L"Нет ссылок",ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(mbk->slist,0,onEnterPressed_Shortcuts);
    GUIObject_SoftKeys_SetAction(mbk->slist,ACTION_SELECT1,AddShortcut);
    textidname2id(L"HOMESCREEN_RN_ADD_SHORTCUT_TXT",TEXTID_ANY_LEN,&str_id);
    GUIObject_SoftKeys_SetText(mbk->slist,ACTION_SELECT1,str_id);
  }
  else
  {
    GUIObject_SoftKeys_SetAction(mbk->slist,ACTION_SELECT1,onEnterPressed_Shortcuts);
    GUIObject_SoftKeys_SetAction(mbk->slist,0,AddShortcut);
    textidname2id(L"HOMESCREEN_RN_ADD_SHORTCUT_TXT",TEXTID_ANY_LEN,&str_id);
    GUIObject_SoftKeys_SetText(mbk->slist,0,str_id);
  }

  GUIObject_SoftKeys_SetAction(mbk->slist,1,Configuration);
  textidname2id(L"MENU_MSG_SETTINGS_TXT",TEXTID_ANY_LEN,&str_id);
  GUIObject_SoftKeys_SetText(mbk->slist,1,str_id);
  GUIObject_SoftKeys_SetAction(mbk->slist,2,SetBrightness);
  snwprintf(brightness,40,(wchar_t*)L"Яркость: %d",bright);
  GUIObject_SoftKeys_SetText(mbk->slist,2,TextID_Create(brightness,ENC_UCS2,TEXTID_ANY_LEN));

  if (mbk->isA2)
  {
    GUIObject_SoftKeys_SetAction( mbk->slist, ACTION_LONG_BACK, onReturnStandby );
    GUIObject_SoftKeys_SetAction( mbk->slist, 3, PreTerminateManager );
    GUIObject_SoftKeys_SetText(mbk->slist, 3, STR("Выход"));
  } else  {
    GUIObject_SoftKeys_SetAction( mbk->slist, ACTION_LONG_BACK, PreTerminateManager );
  }

  mbk->oldOnKey = DispObject_GetOnKey(GUIObject_GetDispObject(mbk->slist));
  DISP_DESC_SetOnKey( DispObject_GetDESC (GUIObject_GetDispObject(mbk->slist)), (DISP_OBJ_ONKEY_METHOD)myOnKey );

  TabMenuBar_SetTabGui( mbk->gui, pos, mbk->slist);

  textidname2id(L"HOMESCREEN_RN_MY_SHORTCUTS_TITLE_TXT",TEXTID_ANY_LEN,&str_id);
  TabMenuBar_SetTabTitle( mbk->gui, pos, str_id );
}

void CreateFMenu(MyBOOK* mbk,int pos)
{
  int str_id;
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[6].ImageID, 0);
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[7].ImageID, 1);

  if(mbk->flist) ListMenu_DestroyItems(mbk->flist);
  mbk->flist = CreateListMenu( mbk, 0 );

  ListMenu_SetCursorToItem(mbk->flist, 0);
  ListMenu_SetOnMessage(mbk->flist, onCallback_Folders);
  ListMenu_SetItemCount(mbk->flist, FoldersCount);

  if (mbk->flistpos > FoldersCount) ListMenu_SetCursorToItem(mbk->flist, FoldersCount);
  else ListMenu_SetCursorToItem(mbk->flist, mbk->flistpos);

  GUIObject_SoftKeys_SetAction(mbk->flist,ACTION_BACK, CloseMyBook);

  if (FoldersCount >= 1) GUIObject_SoftKeys_SetAction(mbk->flist,ACTION_SELECT1,onEnterPressed_Folders);

  GUIObject_SoftKeys_SetAction(mbk->flist,0,Configuration);
  textidname2id(L"MENU_SETTINGS_TXT",TEXTID_ANY_LEN,&str_id);
  GUIObject_SoftKeys_SetText(mbk->flist,0,str_id);

  if (mbk->isA2)
  {
    GUIObject_SoftKeys_SetAction( mbk->flist, ACTION_LONG_BACK, onReturnStandby );
    GUIObject_SoftKeys_SetAction( mbk->flist, 1, PreTerminateManager );
    GUIObject_SoftKeys_SetText(mbk->flist, 1, STR("Выход"));
  } else  {
    GUIObject_SoftKeys_SetAction( mbk->flist, ACTION_LONG_BACK, PreTerminateManager );
  }

  if (!mbk->folders_list->FirstFree) ListMenu_SetNoItemText(mbk->flist,STR("Нет папок"));

  mbk->oldOnKey = DispObject_GetOnKey(GUIObject_GetDispObject(mbk->flist));
  DISP_DESC_SetOnKey( DispObject_GetDESC (GUIObject_GetDispObject(mbk->flist)), myOnKey );

  TabMenuBar_SetTabGui( mbk->gui, pos, mbk->flist);

  textidname2id(L"DB_DATABROWSER_TXT",TEXTID_ANY_LEN,&str_id);
  TabMenuBar_SetTabTitle( mbk->gui, pos, str_id );
}

void CreateCMenu(MyBOOK* mbk,int pos)
{
  int str_id;
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[8].ImageID, 0);
  TabMenuBar_SetTabIcon( mbk->gui, pos, mbk->tabs_image[9].ImageID, 1);

  if(mbk->llist) ListMenu_DestroyItems(mbk->llist);
  mbk->llist = CreateListMenu( mbk, 0 );
  ListMenu_SetItemCount(mbk->llist, 4);
  ListMenu_SetOnMessage(mbk->llist, onCallback_Loggers);

  if (mbk->llistpos > 4) ListMenu_SetCursorToItem(mbk->llist, 4);
  else ListMenu_SetCursorToItem(mbk->llist, mbk->llistpos);

  GUIObject_SoftKeys_SetAction(mbk->llist, ACTION_BACK, CloseMyBook);

  GUIObject_SoftKeys_SetAction(mbk->llist, ACTION_SELECT1, onEnterPressed_Loggers);
  GUIObject_SoftKeys_SetAction(mbk->llist, 1, ChangeDate);
  textidname2id(L"DB_BYDATE_TXT", -1, &str_id);
  GUIObject_SoftKeys_SetText(mbk->llist, 1, str_id);

  if (mbk->isA2)
  {
    GUIObject_SoftKeys_SetAction( mbk->llist, ACTION_LONG_BACK, onReturnStandby );
    GUIObject_SoftKeys_SetAction( mbk->llist, 2, PreTerminateManager );
    GUIObject_SoftKeys_SetText(mbk->llist, 2, STR("Выход"));
  } else  {
    GUIObject_SoftKeys_SetAction( mbk->llist, ACTION_LONG_BACK, PreTerminateManager );
  }

  mbk->oldOnKey = DispObject_GetOnKey(GUIObject_GetDispObject(mbk->llist));
  DISP_DESC_SetOnKey( DispObject_GetDESC (GUIObject_GetDispObject(mbk->llist)), myOnKey );

  TabMenuBar_SetTabGui( mbk->gui, pos, mbk->llist);

  TabMenuBar_SetTabTitle( mbk->gui, pos, STR("Call Logger"));
}

void DestroyIniBuffers(MyBOOK* mbk)
{
  if ( mbk->hidden_buf )
  {
    delete mbk->hidden_buf;
    mbk->hidden_buf=0;
  }
}

void DestroyAllGUIs(MyBOOK* mbk)
{
  if ( mbk->java_list_menu )
  {
    GUIObject_Destroy( mbk->java_list_menu );
    mbk->java_list_menu=0;
    List_DestroyElements( mbk->java_list, elem_filter, elem_free );
    List_Destroy( mbk->java_list );
    mbk->java_list = 0;
  }

  if ( mbk->mode_list )
  {
    GUIObject_Destroy( mbk->mode_list );
    mbk->mode_list=0;
  }

  if ( mbk->but_list )
  {
    GUIObject_Destroy( mbk->but_list );
    mbk->but_list=0;
  }

  if ( mbk->YesNoQuestion )
  {
    GUIObject_Destroy( mbk->YesNoQuestion );
    mbk->YesNoQuestion=0;
  }

  if ( mbk->blist )
  {
    GUIObject_Destroy( mbk->blist );
    mbk->blist=0;
    mbk->blistpos=0;
  }

  if ( mbk->elist )
  {
    GUIObject_Destroy( mbk->elist );
    mbk->elist=0;
    mbk->elistpos=0;
  }

  if ( mbk->slist )
  {
    GUIObject_Destroy( mbk->slist );
    mbk->slist=0;
    mbk->slistpos=0;
  }

  if ( mbk->flist )
  {
    GUIObject_Destroy( mbk->flist );
    mbk->flist=0;
    mbk->flistpos=0;
  }

  if ( mbk->llist )
  {
    GUIObject_Destroy( mbk->llist );
    mbk->llist=0;
    mbk->llistpos=0;
  }

  if ( mbk->gui )
  {
    GUIObject_Destroy( mbk->gui );
    mbk->gui=0;
  }

  if ( mbk->MainMenuID != -1 )
  {
    BOOK* MainMenu = FindBookByID( mbk->MainMenuID );

    if ( MainMenu )
      FreeBook( MainMenu );

    mbk->MainMenuID = -1;
  }
  mbk->oldOnKey=0;
}

void DestroyIcons(MyBOOK* mbk)
{
  int i;

  for ( i = 0;i < ICONS_TAB_COUNT;i++ )
  {
    ImageID_Free( mbk->tabs_image[i].ImageID );
  }
  for ( i = 0;i < ICONS_ITEM_COUNT;i++ )
  {
    ImageID_Free( mbk->item_image[i].ImageID );
  }
  for ( i = 0;i < ICON_DIGITS_COUNT;i++ )
  {
    ImageID_Free( mbk->digs_image[i].ImageID );
  }
}

// при закрытии книги
void onMyBookClose( BOOK* book )
{
  MyBOOK* mbk = (MyBOOK*) book;

  DestroyIniBuffers(mbk);
  DestroyAllGUIs(mbk);
  DestroyLists( mbk );
  DestroyIcons(mbk);
}

void ReturnMyBook( BOOK* bk, GUI* )
{
  MyBOOK* mbk = (MyBOOK*) bk;
  GUIObject_Destroy( mbk->YesNoQuestion );
  mbk->YesNoQuestion = 0;
  mbk->ActiveTAB=GetActiveTab(mbk);
  CreateBookManagerGUI( mbk->ActiveTAB, mbk );
}

void CloseMyBook( BOOK* bk, GUI* )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  mbk->Changed = FALSE;
  mbk->ActiveTAB=GetActiveTab(mbk);
  BookObj_GotoPage( mbk, &BookManager_Idle_Page);
}

void PreTerminateManager( BOOK* bk, GUI*)
{
  MyBOOK* mbk = (MyBOOK*) bk;
  mbk->YesNoQuestion=CreateYesNoQuestionVA(0,
                                             VAR_BOOK(mbk),
                                             VAR_YESNO_PRE_QUESTION(TextID_Create(L"BookManager запущен.",ENC_UCS2,TEXTID_ANY_LEN)),
                                             VAR_YESNO_QUESTION(TextID_Create(L"Вы хотите выгрузить его?",ENC_UCS2,TEXTID_ANY_LEN)),
                                             0);
  GUIObject_SoftKeys_SetAction( mbk->YesNoQuestion, ACTION_YES, TerminateManager );
  GUIObject_SoftKeys_SetAction( mbk->YesNoQuestion, ACTION_NO, CloseMyBook );
  GUIObject_SoftKeys_SetAction( mbk->YesNoQuestion, ACTION_BACK, ReturnMyBook );
}

int RecreateBookList( void* r0, BOOK* bk )
{
  MyBOOK* mbk = (MyBOOK*) bk;
  mbk->ActiveTAB=GetActiveTab(mbk);
  CreateBookManagerGUI( mbk->ActiveTAB, mbk );
  return(0);
}

// создание меню
void CreateBookManagerGUI( int tab_pos, MyBOOK* mbk )
{
  int my_books_tab     = books_tab;
  int my_elfs_tab      = elfs_tab;
  int my_shortcuts_tab = shortcuts_tab;
  int my_folders_tab   = folders_tab;
  int my_loggers_tab   = loggers_tab;

  DestroyAllGUIs(mbk);
  CreateBookLst( mbk );

  if ( !mbk->gui )
  {
    mbk->gui = CreateTabMenuBar( mbk );

    if (TabCount == 0) TabMenuBar_SetTabCount( mbk->gui, 1 );
    else TabMenuBar_SetTabCount( mbk->gui, TabCount );

    if(my_books_tab<TabCount)     CreateBMenu(mbk,my_books_tab);
    if(my_elfs_tab<TabCount)      CreateEMenu(mbk,my_elfs_tab);
    if(my_shortcuts_tab<TabCount) CreateSMenu(mbk,my_shortcuts_tab);
    if(my_folders_tab<TabCount)   CreateFMenu(mbk,my_folders_tab);
    if(my_loggers_tab<TabCount)   CreateCMenu(mbk,my_loggers_tab);

    if(tab_pos>=TabCount) tab_pos=TabCount-1;

    TabMenuBar_SetFocusedTab( mbk->gui, tab_pos );
    TabMenuBar_SetOnTabSwitch(mbk->gui,onTabSwitch);
  }
  GUIObject_Show(mbk->gui);
}

// собственно старт
__root int CreateBookList( void* r0, BOOK* bk )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  get_iconsID( mbk );
  int tab_pos;

  if ( !FirstTab )
  {
    tab_pos = mbk->ActiveTAB;
  }
  else
    tab_pos = *tabs_pos[FirstTab-1];

  CreateBookManagerGUI( tab_pos, mbk );
  return 0;
}

int IdlePage_EnterAction( void* r0, BOOK* bk )
{
  MyBOOK* mbk = (MyBOOK*) bk;

  DestroyAllGUIs(mbk);
  DestroyLists(mbk);
  DestroyIcons(mbk);

  BookObj_WindowSetWantsFocus(mbk,0,FALSE);
  BookObj_Hide(mbk,0);
  return 0;
}

int ReconfigElf( void* mess, BOOK* book )
{
  RECONFIG_EVENT_DATA* reconf = ( RECONFIG_EVENT_DATA* )mess;

  if ( !wstrcmpi( reconf->path, successed_config_path ) && !wstrcmpi( reconf->name, successed_config_name ) )
  {
    InitConfig();
    return 1;
  }
  return 0;
}

int NewKey( int key, int rep_count, int mode, MyBOOK * mbk, DISP_OBJ* disp_obj )
{
  DISP_OBJ * disp;

  BOOK * top_book = Display_GetTopBook(0);

  if ( mode == KeyPressMode )
  {
    if ( key == KeyActiv )
    {
      if ( isKeylocked() && !Ignore_KeyLock)
        return 0;

      if ( top_book!=mbk )
      {
        BookObj_WindowSetWantsFocus(mbk,0,TRUE);
        BookObj_SetFocus(mbk,0);
        BookObj_GotoPage( mbk, &BookManager_Main_Page );
        return -1;
      }
      else
      {
        if ( !mbk->mode_list && !mbk->YesNoQuestion && !mbk->StringInput && !mbk->DateInput )
        {
          CloseMyBook( mbk, 0 );
          return 0;
        }
        return -1;
      }
    }
  }
  if ( top_book==mbk )
  {
  if (MINIMIZE_TO_SESSION)
  {
    if ( key == KEY_LEFT || key == KEY_RIGHT )
    {
      if (mbk->oldOnKey)
      {
        if (GetActiveTab(mbk)==books_tab)
        disp = GUIObject_GetDispObject(mbk->blist);
        if (GetActiveTab(mbk)==elfs_tab)
        disp = GUIObject_GetDispObject(mbk->elist);
        if (GetActiveTab(mbk)==shortcuts_tab)
        disp = GUIObject_GetDispObject(mbk->slist);
        if (GetActiveTab(mbk)==folders_tab)
        disp = GUIObject_GetDispObject(mbk->flist);
        if (GetActiveTab(mbk)==loggers_tab)
        disp = GUIObject_GetDispObject(mbk->llist);

        if (disp==disp_obj)
        {
          mbk->oldOnKey( disp, key, 0, rep_count, mode );
          return -1;
        }
      }
    }

    if ( key == KeyChangeTabRight && mode == KeyChangeTabPressModeRight )
    {
      if (GetActiveTab(mbk)==books_tab)
        disp = GUIObject_GetDispObject(mbk->blist);
      if (GetActiveTab(mbk)==elfs_tab)
        disp = GUIObject_GetDispObject(mbk->elist);
      if (GetActiveTab(mbk)==shortcuts_tab)
        disp = GUIObject_GetDispObject(mbk->slist);
      if (GetActiveTab(mbk)==folders_tab)
        disp = GUIObject_GetDispObject(mbk->flist);
      if (GetActiveTab(mbk)==loggers_tab)
        disp = GUIObject_GetDispObject(mbk->llist);

      if (disp==disp_obj) return KEY_RIGHT;
    }
    if ( key == KeyChangeTabLeft && mode == KeyChangeTabPressModeLeft )
    {
      if (GetActiveTab(mbk)==loggers_tab)
        disp = GUIObject_GetDispObject(mbk->llist);
      if (GetActiveTab(mbk)==folders_tab)
        disp = GUIObject_GetDispObject(mbk->flist);
      if (GetActiveTab(mbk)==shortcuts_tab)
        disp = GUIObject_GetDispObject(mbk->slist);
      if (GetActiveTab(mbk)==elfs_tab)
        disp = GUIObject_GetDispObject(mbk->elist);
      if (GetActiveTab(mbk)==books_tab)
        disp = GUIObject_GetDispObject(mbk->blist);

      if (disp==disp_obj) return KEY_LEFT;
      }
    }
  }
  return 0;
}

MyBOOK * CreateBookManagerBook()
{
  MyBOOK* mbk = (MyBOOK*) malloc( sizeof( MyBOOK ) );
  memset( mbk, 0, sizeof( MyBOOK ) );
  CreateBook( mbk, onMyBookClose, &BookManager_Base_Page, "BookManager", -1, 0 );
  //Init flags
  mbk->gui = 0;
  mbk->hidden_buf = 0;
  mbk->hidden_buf_size = 0;
  mbk->oldOnKey=0;
  mbk->sessions_list = 0;
  mbk->books_list = 0;
  mbk->elfs_list = 0;
  mbk->java_list = 0;
  mbk->shortcuts_list = 0;
  mbk->folders_list = 0;
  mbk->Logger_list = 0;
  mbk->java_list_menu = 0;
  mbk->blist = 0;
  mbk->elist = 0;
  mbk->slist = 0;
  mbk->flist = 0;
  mbk->llist = 0;
  mbk->mode_list = 0;
  mbk->but_list = 0;
  mbk->YesNoQuestion = 0;
  mbk->StringInput = 0;
  mbk->DateInput = 0;
  mbk->MainMenuID = -1;
  mbk->blistpos = 0;
  mbk->elistpos = 0;
  mbk->slistpos = 0;
  mbk->flistpos = 0;
  mbk->llistpos = 0;
  mbk->blistcnt = 0;
  mbk->elistcnt = 0;
  mbk->slistcnt = 0;
  mbk->flistcnt = 0;
  mbk->llistcnt = 0;
  mbk->ActiveTAB = 0;
  mbk->isA2 = FALSE;
  mbk->Br_set = 0;
  mbk->Changed = 0;
  mbk->NEditor=0;
  mbk->GUIEditor=0;
  mbk->set_2020=set_2020;
  mbk->PhoneType=PhoneType;
  mbk->folder[0]=0;
  mbk->folder[1]=0;
  mbk->books_show_hid = books_show_hid;
  mbk->books_show_nogui = books_show_nogui;
  mbk->elfs_show_hid = elfs_show_hid;
  mbk->elfs_show_nogui = elfs_show_nogui;
  //End init
  int platform=GetChipID()&CHIPID_MASK;
  if (platform==CHIPID_DB3150||platform==CHIPID_DB3200||platform==CHIPID_DB3210||platform==CHIPID_DB3350)
    mbk->isA2 = TRUE;

  LoadHidden(mbk);

  BookObj_GotoPage( mbk, &BookManager_Idle_Page);

  return mbk;
}

int main ( void )
{
  switch(GetChipID()&CHIPID_MASK)
  {
    case CHIPID_DB2000:
    case CHIPID_DB2010:
      PhoneType = GetPhoneType();
      set_2020 = 0;
      break;
    default:
      set_2020 = 1;
      break;
  }
  if ( !FindBook( isBookManager ) )
  {
    trace_init(L"bookmanmem.txt");
    InitConfig();
    MODIFYKEYHOOK( NewKey, KEY_HOOK_ADD, CreateBookManagerBook() );
  }
  else
  {
    MessageBox( EMPTY_TEXTID, STR( "BookManager уже запущен." ), NOIMAGE, 1, 5000, 0 );
    SUBPROC( elf_exit );
  }
  return 0;
}

int GetPhoneType()
{
  char tmp[8];
  for(int i=0; i<phoneNums; i++)
  {
    if(PhoneDB[i].addr)
    {
      memcpy(tmp,PhoneDB[i].addr,7);
      tmp[7] = 0;
      if(strcmp(tmp,PhoneDB[i].firmware) == 0)
      {
        return PhoneDB[i].type;
      }
    }
  }
  return UNKNOWN;
}
