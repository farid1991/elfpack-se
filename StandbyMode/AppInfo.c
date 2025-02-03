#include "..\include\Lib_Clara.h"
#include "AppInfo.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"
#include "MediaInfo.h"

typedef struct
{
  BOOK * book;
  UI_APP_SESSION * session;
  int book_name;

}BOOK_ITEM;

typedef struct
{
  UI_APP_SESSION * session;
  LIST * book_list;
  int session_name;
  int book_in_session;
  int isGuiBook;
  int isJava;
  IMAGEID session_icon;
}SESSION_ITEM;

int app_show = 0;
int app_java_show = 0;
int app_book_show = 0;
int app_elf_show = 0;

LIST * session_list = 0;
int blistcnt = 0;

int javacnt = 0;
int bookcnt = 0;
int elfcnt = 0;

TEXTID javacntId = EMPTY_TEXTID;
TEXTID bookcntId = EMPTY_TEXTID;
TEXTID elfcntId = EMPTY_TEXTID;

TEXTID approwId[APP_ICN_ROW_MAX] = {EMPTY_TEXTID,};
TEXTID appcolumnId[APP_ICN_COLUMN_MAX] = {EMPTY_TEXTID,};
static int rowcnt = 0;
static int image_height = 0;

wchar_t GetBookIconItem(char * name_book);
void CreateSessoinList();
void FreeAppInfoRow();

void UpdateAppInfo(void)
{
  CreateSessoinList();
  FreeAppInfoRow();
  if(blistcnt)
  {
    SESSION_ITEM * si;
    int cnt = 0;
    int i = 0;
    int j = 0;

    si=(SESSION_ITEM *)ListElement_GetByIndex(session_list, 0);
    image_height = GetImageHeight(si->session_icon) + 1;

    if(cfg_app_fill_by_row)
    {
      while(cnt < blistcnt)
      {
        si=(SESSION_ITEM *)ListElement_GetByIndex(session_list, cnt++);
        appcolumnId[i++] = si->session_icon + 0x78000000;
        if(i >= cfg_app_column_max)
        {
          approwId[j++] = TextID_Create(appcolumnId,ENC_TEXTID,i);
          i = 0;
          ++rowcnt;
          if(j >= cfg_app_row_max) break;
        }
      }
      if(i)
      {
        approwId[j++] = TextID_Create(appcolumnId,ENC_TEXTID,i);
        ++rowcnt;
      }
    }
    else
    {
      int colcnt = 0;
      while(cnt < blistcnt)
      {
        si=(SESSION_ITEM *)ListElement_GetByIndex(session_list, cnt++);
        if(cnt <= cfg_app_row_max)
        {
          approwId[j++] = si->session_icon + 0x78000000;
        }
        else
        {
          appcolumnId[0] = approwId[j];
          appcolumnId[1] = si->session_icon + 0x78000000;
          approwId[j++] = TextID_Create(appcolumnId,ENC_TEXTID,2);
        }
        if(j >= cfg_app_row_max)
        {
          j = 0;
          ++colcnt;
        }
        if(rowcnt < cfg_app_row_max) ++rowcnt;
        else if(colcnt >= cfg_app_column_max) break;
      }
    }
  }

    {
      TEXTID buf[2];
      buf[0] = TextID_Create(cfg_app_java_msg,ENC_UCS2,TEXTID_ANY_LEN);
      buf[1] = TextID_CreateIntegerID(javacnt);
      if(javacntId != EMPTY_TEXTID) TextID_Destroy(javacntId);
      javacntId = TextID_Create(buf,ENC_TEXTID,2);
    }
    {
      TEXTID buf[2];
      buf[0] = TextID_Create(cfg_app_book_msg,ENC_UCS2,TEXTID_ANY_LEN);
      buf[1] = TextID_CreateIntegerID(bookcnt);
      if(bookcntId != EMPTY_TEXTID) TextID_Destroy(bookcntId);
      bookcntId = TextID_Create(buf,ENC_TEXTID,2);
    }
    {
      TEXTID buf[2];
      buf[0] = TextID_Create(cfg_app_elf_msg,ENC_UCS2,TEXTID_ANY_LEN);
      buf[1] = TextID_CreateIntegerID(elfcnt);
      if(elfcntId != EMPTY_TEXTID) TextID_Destroy(elfcntId);
      elfcntId = TextID_Create(buf,ENC_TEXTID,2);
    }
}

void ShowAppInfo(int y, int location)
{
   if(app_show && rowcnt)
   {
     int oldFont = SetFont(FONT_E_14R);
     int offset = 0 - y;
     for(int i=0; i<rowcnt; i++)
     {
       if(cfg_app_align == RIGHT)
         DrawString(approwId[i], cfg_app_align, 1, cfg_app_y + offset, cfg_app_x, DisplayHeight, 0, 1, clRed, clGreen);
       else
         DrawString(approwId[i], cfg_app_align, cfg_app_x, cfg_app_y + offset, DisplayWidth, DisplayHeight, 0, 1, clRed, clGreen);
       offset += image_height;
     }
     SetFont(oldFont);
   }
   if(app_java_show)
   {
     if(cfg_app_java_align == RIGHT)
        myDrawString(cfg_app_java_font, javacntId, cfg_app_java_align, 1, cfg_app_java_y - y, cfg_app_java_x, DisplayHeight, cfg_app_java_border, cfg_app_java_color, cfg_app_java_highlight);
     else
        myDrawString(cfg_app_java_font, javacntId, cfg_app_java_align, cfg_app_java_x, cfg_app_java_y - y, DisplayWidth, DisplayHeight, cfg_app_java_border, cfg_app_java_color, cfg_app_java_highlight);
   }
   if(app_book_show)
   {
     if(cfg_app_book_align == RIGHT)
        myDrawString(cfg_app_book_font, bookcntId, cfg_app_book_align, 1, cfg_app_book_y - y, cfg_app_book_x, DisplayHeight, cfg_app_book_border, cfg_app_book_color, cfg_app_book_highlight);
     else
        myDrawString(cfg_app_book_font, bookcntId, cfg_app_book_align, cfg_app_book_x, cfg_app_book_y - y, DisplayWidth, DisplayHeight, cfg_app_book_border, cfg_app_book_color, cfg_app_book_highlight);
   }
   if(app_elf_show)
   {
     if(cfg_app_elf_align == RIGHT)
        myDrawString(cfg_app_elf_font, elfcntId, cfg_app_elf_align, 1, cfg_app_elf_y - y, cfg_app_elf_x, DisplayHeight, cfg_app_elf_border, cfg_app_elf_color, cfg_app_elf_highlight);
     else
        myDrawString(cfg_app_elf_font, elfcntId, cfg_app_elf_align, cfg_app_elf_x, cfg_app_elf_y - y, DisplayWidth, DisplayHeight, cfg_app_elf_border, cfg_app_elf_color, cfg_app_elf_highlight);
   }
}

void UpdateAppInfoParams(void)
{
/*
  if(cfg_app_show)
  {
  }
*/
  UpdateAppInfo();
}

void UpdateAppShowParams(void)
{
    app_show = cfg_app_show && (!(PlayerIsStarted && cfg_app_not_show_if_player));
    app_java_show = cfg_app_java_show && (!(PlayerIsStarted && cfg_app_java_not_show_if_player));
    app_book_show = cfg_app_book_show && (!(PlayerIsStarted && cfg_app_book_not_show_if_player));
    app_elf_show = cfg_app_elf_show && (!(PlayerIsStarted && cfg_app_elf_not_show_if_player));
}

// получить имя жавы
int GetJavaName(BOOK * bk)
{
  wchar_t ws[100];
  TextID_GetWString(Book_GetSession(bk)->name,ws,100);
  if (!wstrncmp(ws,L"Foreign app",11))
  {
    return(JavaSession_GetName());
  }
  if (!wstrcmp(ws,L"Java"))
  {
    return(JavaSession_GetName());
  }
  return(0);
}

// при убийстве какой либо книги
int onRootListChanged(void * r0, BOOK * bk)
{
  UpdateAppInfo();
  return(0);
};

void FreeSessoinList()
{
  SESSION_ITEM * si;

  if(!session_list) return;

  while (session_list->FirstFree)
  {
    si=(SESSION_ITEM *)ListElement_Remove(session_list,0);
    while (si->book_list->FirstFree)
    {
      BOOK_ITEM * bi=(BOOK_ITEM *)ListElement_Remove(si->book_list,0);
      TextID_Destroy(bi->book_name);
      delete(bi);
    }
    List_Free(si->book_list);
    TextID_Destroy(si->session_name);
    ImageID_Free(si->session_icon);
    delete(si);
  }
  List_Free(session_list);
  session_list = 0;
}

void FreeAppInfoImages(void)
{
}

void FreeAppInfoRow(void)
{
  int row = rowcnt;
  rowcnt = 0;
  if(row)
  {
     for(int i=0; i<row; i++)
     {
       TextID_Destroy(approwId[i]);
     }
  }
}

void FreeAppInfoParams(void)
{
  FreeAppInfoImages();
  FreeSessoinList();
  FreeAppInfoRow();

  if(javacntId != EMPTY_TEXTID) TextID_Destroy(javacntId);
  javacntId = EMPTY_TEXTID;

  if(bookcntId != EMPTY_TEXTID) TextID_Destroy(bookcntId);
  bookcntId = EMPTY_TEXTID;

  if(elfcntId != EMPTY_TEXTID) TextID_Destroy(elfcntId);
  elfcntId = EMPTY_TEXTID;
}

// создаём списки сессий/книг
void CreateSessoinList()
{

  int i,j,k,cnt=0,fgui,jcnt=0,bcnt=0,ecnt=0;
  int mask=((int)LastExtDB())&0xF8000000;
  BOOK * book;
  UI_APP_SESSION * session;
  SESSION_ITEM *si;
  i=root_list_get_session_count();
  FreeSessoinList();
  session_list=List_New();
  for (j=0;j<i;j++)
  {
    si=0;
    session=root_list_get_session(j);
    for (k=0;k<session->listbook->FirstFree;k++)
    {
      book=(BOOK*)ListElement_GetByIndex(session->listbook,k);
      if (!isBookmanDaemonBook(book))
      {
        if( (fgui=book->xguilist->guilist->FirstFree)||(((((int)book->onClose)&0xF8000000)!=mask)))
        {
          if ((!isVolumeControllerBook(book))&&(!isRightNowBook(book))&&(Find_StandbyBook()!= book))
          {
            {
              {
                char s[100];
                wchar_t ws[100];
                si=new SESSION_ITEM;
                si->book_list=List_New();
                int isJava = isJavaBook(book);

                if(!isJava)
                {
                  str2wstr(ws,book->xbook->name);
                }
                else
                {
                  TextID_GetWString(book->xbook->app_session->name,ws,100);
                }
                si->session_name = TextID_Create(ws,ENC_UCS2,wstrlen(ws));
                si->session=session;
                si->isGuiBook=fgui;

                if(!isJava)
                {
                  str2wstr(ws,book->xbook->name);
                  si->session_icon = GetBookIconItem(wstr2strn (s,ws,100));
                  si->isJava = 0;
                }
                else
                {
                  int JID;
                  iconidname2id(L"DB_LIST_JAVA_ICN",TEXTID_ANY_LEN,&JID);
                  si->session_icon = JID;
                  si->isJava = 1;
                  void * JavaDesc;
                  char sp1;
                  JavaDialog_Open(0,&sp1,&JavaDesc);
                  if (!JavaAppDesc_GetFirstApp(JavaDesc))
                  {
                    bool yap = true;
                    while (yap)
                    {
                       wchar_t * sp;
                       JavaAppDesc_GetJavaAppInfo(JavaDesc,0,&sp);
                       if((ws[0] == *sp++)&&(ws[1] == *sp++)&&(ws[2] == *sp++)&&(ws[3] == *sp++)&&(ws[4] == *sp))
                       {
                            JavaAppDesc_GetJavaAppInfo(JavaDesc,5,&sp);
                            JavaApp_LogoImageID_Get(sp,&si->session_icon);
                            yap = false;
                       }
                       if (JavaAppDesc_GetNextApp(JavaDesc))yap = false;
                    }
                  }
                  JavaDialog_Close(sp1);
                }
              }
              BOOK_ITEM * b = new BOOK_ITEM;
              b->book=book;
              b->session=session;
              b->book_name=EMPTY_TEXTID;

              int tmp;
              if ((tmp=GetJavaName(book)))
              {
                TextID_Destroy(si->session_name);
                si->session_name=tmp;
              }


              ListElement_Add(si->book_list,b);
            }
            if (((((int)book->onClose)&0xF8000000)==mask))
            {
              si->book_in_session=si->book_list->FirstFree;
              ListElement_AddtoTop(session_list,si);
              if(si->isJava) ++jcnt;
              else ++bcnt;
              cnt++;
            }
            else
            {
              ++ecnt;
              while (i<si->book_list->FirstFree)
              {
                BOOK_ITEM * bi=(BOOK_ITEM *)ListElement_Remove(si->book_list,0);
                TextID_Destroy(bi->book_name);
                delete(bi);
              }
              List_Free(si->book_list);
              TextID_Destroy(si->session_name);
              ImageID_Free(si->session_icon);
              delete(si);
            }
          }
        }
      }
    }
  }
  blistcnt=cnt;

  javacnt = jcnt;
  bookcnt = bcnt;
  elfcnt = ecnt;
}

void * SHORTCUT_DESC_Init(wchar_t * param)
{
  SHORTCUT_DESC * w_buf = new SHORTCUT_DESC;
  memset( w_buf, 0, sizeof(w_buf) );
  w_buf->shortcut_state = SC_State_Activated;
  wstrncpy( w_buf->name, param, sizeof(w_buf->name)/sizeof(wchar_t) );
  if (!wstrcmp(w_buf->name,L"MainMenu")) w_buf->shortcut_state = SC_State_MainMenu;
  return(w_buf);
}

void * SHORTCUT_DESC_A2_Init(wchar_t * param)
{
  SHORTCUT_DESC_A2 * w_buf = new SHORTCUT_DESC_A2;
  memset( w_buf, 0, sizeof(w_buf) );
//  w_buf->shortcut_state = SC_State_Activated;
  wstrncpy( w_buf->name, param, sizeof(w_buf->name)/sizeof(wchar_t) );
  if (!wstrcmp(w_buf->name,L"MainMenu")) w_buf->item_type = SC_Type_MainMenu;
  else w_buf->item_type = SC_Type_Standard;
  return(w_buf);
}

wchar_t myShortcut_Get_MenuItemIconID(wchar_t *MenuItemName)
{
    wchar_t MenuItemIconID = NOIMAGE;
    void * w_buf = 0;

    switch(ChipID)
    {
    case CHIPID_DB2000:
    case CHIPID_DB2010:
    case CHIPID_DB2020:
        w_buf = SHORTCUT_DESC_Init(MenuItemName);
        break;
    case CHIPID_DB3150:
        w_buf = SHORTCUT_DESC_A2_Init(MenuItemName);
        break;
    }
    MenuItemIconID = Shortcut_Get_MenuItemIconID(w_buf);
    if(w_buf) delete(w_buf);
    return MenuItemIconID;
}

wchar_t GetBookIconItem(char * name_book)
{
  char * name;
  name = name_book;
  int icon_id;
  if(strstr(name,"MenuB")||strstr(name,"Menu_Gl"))
  {
    return myShortcut_Get_MenuItemIconID(L"MainMenu");
  }
  if(strstr(name,"Audio")||strstr(name,"alkman Men")||strstr(name,"MM_BrowserTople"))
  {
    return myShortcut_Get_MenuItemIconID(L"MediaPlayer");
  }
  if(strstr(name,"Video")||strstr(name,"alkman Vide"))
  {
    return myShortcut_Get_MenuItemIconID(L"MediaPlayer_Video");
  }
  if(strstr(name,"Radio"))
  {
    return myShortcut_Get_MenuItemIconID(L"FMRadio");
  }
  if(strstr(name,"MSG UI"))
  {
    return myShortcut_Get_MenuItemIconID(L"Messaging");
  }
  if(strstr(name,"SMS Edito"))
  {
    return myShortcut_Get_MenuItemIconID(L"Msg_WriteNew");
  }
  if(strstr(name,"SMS Vie")||strstr(name,"SMSVie"))
  {
    iconidname2id(L"MSG_UI_READ_TEXT_MESSAGE_ICN",TEXTID_ANY_LEN,&icon_id);
    return (wchar_t)icon_id;
  }
  if(strstr(name,"SMS UI"))
  {
    return myShortcut_Get_MenuItemIconID(L"Msg_SentItems");
  }
  if(strstr(name,"Camera"))
  {
    return myShortcut_Get_MenuItemIconID(L"Camera");
  }
  if(strstr(name,"Session"))
  {
    return myShortcut_Get_MenuItemIconID(L"Games");
  }
  if(strstr(name,"DataBrowser"))
  {
    return myShortcut_Get_MenuItemIconID(L"FileManager");
  }
  if(strstr(name,"Notes"))
  {
    return myShortcut_Get_MenuItemIconID(L"Menu_ViewNotes");
  }
  if(strstr(name,"MessageBox"))
  {
    iconidname2id(L"DB_LIST_OTHER_ICN",TEXTID_ANY_LEN,&icon_id);
    return (wchar_t)icon_id;
  }
  if(strstr(name,"ImageVie"))
  {
    iconidname2id(L"DB_LIST_PICTURE_ICN",TEXTID_ANY_LEN,&icon_id);
    return (wchar_t)icon_id;
  }
  if(strstr(name,"Browser Book"))
  {
    return myShortcut_Get_MenuItemIconID(L"OperatorWebPage");
  }
  if(strstr(name,"PB_UI_CallList") || strstr(name,"OngoingCall")||strstr(name,"Calling")||strstr(name,"MOCall"))//in Call
  {
    return myShortcut_Get_MenuItemIconID(L"Calls");
  }
  if(strstr(name,"PB_UI_")||strstr(name,"pb_ui_")||strstr(name,"PBCon")||strstr(name,"PBGr"))
  {
    return myShortcut_Get_MenuItemIconID(L"Phonebook");
  }
  if(strstr(name,"oundRecord"))
  {
    return myShortcut_Get_MenuItemIconID(L"SoundRecorder");
  }
  if(strstr(name,"MIDIComposer"))
  {
    return myShortcut_Get_MenuItemIconID(L"MidiComposer");
  }
  if(strstr(name,"VideoDJ_B"))
  {
    return myShortcut_Get_MenuItemIconID(L"VideoDJ");
  }
  if(strstr(name,"PictureEditorB"))
  {
    return myShortcut_Get_MenuItemIconID(L"PictureEditor");
  }
  if(strstr(name,"HIDBook"))
  {
    return myShortcut_Get_MenuItemIconID(L"MenuItem_Bt_HID");
  }
  if(strstr(name,"Stop"))
  {
    return myShortcut_Get_MenuItemIconID(L"Menu_StopWatch");
  }
    if(strstr(name,"Code"))
  {
    return myShortcut_Get_MenuItemIconID(L"Menu_CodeWallet");
  }
    if(strstr(name,"Timer"))
  {
    return myShortcut_Get_MenuItemIconID(L"Menu_Timer");
  }
  /*  if(strstr(name,"CaleBook")||strstr(name,"Calendar"))o
  {
    str2wstr(w_buf,"Menu_ViewAllToDos");
    return Shortcut_Get_MenuItemIconID(w_buf);
  }*/
  if(strstr(name,"Cale"))
  {
    return myShortcut_Get_MenuItemIconID(L"Menu_CalendarMain");
  }
  if(strstr(name,"Set Item")||strstr(name,"Calcul"))
  {
    return myShortcut_Get_MenuItemIconID(L"Application_Calculator_Id");
  }
  if(strstr(name,"Settings")||strstr(name,"SetBo"))
  {
    return myShortcut_Get_MenuItemIconID(L"Settings");
  }
  if(strstr(name,"Bluetooth"))
  {
    return myShortcut_Get_MenuItemIconID(L"BlueTooth_Settings");
  }
  if(strstr(name,"RSSReader"))
  {
    return myShortcut_Get_MenuItemIconID(L"RSS_Reader");
  }
  if(strstr(name,"ClockBook"))
  {
    return myShortcut_Get_MenuItemIconID(L"MenuItem_SetTemporaryAlarm");
  }

  iconidname2id(L"DB_LIST_OTHER_ICN",TEXTID_ANY_LEN,&icon_id);
  return (wchar_t)icon_id;
}

