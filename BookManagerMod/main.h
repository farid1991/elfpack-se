#include "bookparam.h"
#ifndef _MAIN_H_
#define _MAIN_H_

#define COPYRIGHT_STRING STR( "\nBookManager v 4.5.3\n\nbuild 1.08.11\n\n(c) 2007-2008\nHussein\n\nRespect\nIronMaster, KreN\n\nMods: Ploik\nBigHercules\nMoneyMasteR\nJoker XT" )

#define DI_GET_YEAR(x) ((unsigned int)x&0xFFFF)
#define TI_GET_MONTH(x) (((unsigned int)x>>16)&0xFF)
#define TI_GET_DAY(x) ((unsigned int)x>>24)

#define BOOKLIST  0
#define ELFLIST   1
#define SHORTCUTS 2
#define FOLDERS   3
#define LOGGER    4

#define ICONS_TAB_COUNT   10
#define ICONS_ITEM_COUNT  2
#define ICON_DIGITS_COUNT 10

#define IDN_BOOKS_DEACT_ICON _T("RN_TAB_SESSION_MANAGER_DESELECTED_ICN")
#define IDN_BOOKS_ACT_ICON _T("RN_TAB_SESSION_MANAGER_SELECTED_ICN")
#define IDN_ELFS_DEACT_ICON _T("RN_TAB_BOOKMARKS_DESELECTED_ICN")
#define IDN_ELFS_ACT_ICON _T("RN_TAB_BOOKMARKS_SELECTED_ICN")
#define IDN_SHORTCUTS_DEACT_ICON _T("RN_TAB_MY_SHORTCUTS_DESELECTED_ICN")
#define IDN_SHORTCUTS_ACT_ICON _T("RN_TAB_MY_SHORTCUTS_SELECTED_ICN")
#define IDN_FOLDER_DEACT_ICON _T("DB_THUMB_FOLDER_ICN")
#define IDN_FOLDER_ACT_ICON _T("DB_FOLDER_MM_ICN")
#define IDN_CALL_LOGGER_DEACT_ICON _T("MY5_CIRCLE_ACTION_CALL_ICN")
#define IDN_CALL_LOGGER_ACT_ICON _T("MY5_CIRCLE_ACTION_FOCUS_CALL_ICN")

#define IDN_BOOK_ICON _T("ORGANIZER_MENU_APPLICATIONS_ICN")
#define IDN_ELF_ICON _T("CALE_LUNAR_12ANIMALS_1ST_MOUSE_ICN")

#define ELF_BCFG_CONFIG_EVENT 994

#define JAVA_BOOK_NAME "CUIDisplayableBook"

#define FLASH_MASK 0xF8000000

#define MAX_BOOK_NAME_LEN 50

#define INI_HIDDEN L"hidden.ini"

typedef enum
{
  TAB_BOOKS_SHORTCUTS_SOFTKEY = 0,
  TAB_BOOKS_RENAME_SOFTKEY,
  TAB_BOOKS_CONFIG_SOFTKEY,
  TAB_BOOKS_COPYRIGHT_SOFTKEY,
  TAB_BOOKS_SETTINGS_SOFTKEY,
  TAB_BOOKS_HIDEBOOK_SOFTKEY,
  TAB_BOOKS_SHOWBOOK_SOFTKEY,
  TAB_BOOKS_SHOWHIDDEN_SOFTKEY,
  TAB_BOOKS_HIDEHIDDEN_SOFTKEY,
  TAB_BOOKS_SHOWNOGUI_SOFTKEY,
  TAB_BOOKS_HIDENOGUI_SOFTKEY,
  TAB_BOOKS_EXIT_SOFTKEY
}TAB_BOOKS_SOFTKEYS;

typedef enum
{
  TAB_ELFS_SHORTCUTS_SOFTKEY = 0,
  TAB_ELFS_RENAME_SOFTKEY,
  TAB_ELFS_GUI_SOFTKEY,
  TAB_ELFS_AUTHOR_SOFTKEY,
  TAB_ELFS_SETTINGS_SOFTKEY,
  TAB_ELFS_HIDEBOOK_SOFTKEY,
  TAB_ELFS_SHOWBOOK_SOFTKEY,
  TAB_ELFS_SHOWHIDDEN_SOFTKEY,
  TAB_ELFS_HIDEHIDDEN_SOFTKEY,
  TAB_ELFS_SHOWNOGUI_SOFTKEY,
  TAB_ELFS_HIDENOGUI_SOFTKEY,
  TAB_ELFS_EXIT_SOFTKEY
}TAB_ELFS_SOFTKEYS;

#define TAB_SHORTCUTS_NAME_SOFTKEY L"SHC_EDIT_SHORTCUT_TXT"
#define TAB_RENAME_NAME_SOFTKEY L"DB_RENAME_TXT"
#define TAB_SETTINGS_NAME_SOFTKEY L"CALE_ADVANCED_TXT"

#define TAB_BOOKS_SETTINGS_NAME_SOFTKEY L"MENU_MSG_SETTINGS_TXT"
#define TAB_ELFS_GUI_NAME_SOFTKEY L"GUI_SELECT_SK"

typedef struct
{
  wchar_t ImageID;
}IMG;

typedef struct _MYBOOK : BOOK
{
  GUI_TABMENUBAR* gui;
  char* hidden_buf;
  int hidden_buf_size;
  DISP_OBJ_ONKEY_METHOD oldOnKey;
  LIST* sessions_list;
  LIST* books_list;
  LIST* elfs_list;
  LIST* java_list;
  LIST* shortcuts_list;
  LIST* folders_list;
  LIST* Logger_list;
  GUI_LIST* java_list_menu;
  GUI_LIST* blist;
  GUI_LIST* elist;
  GUI_LIST* slist;
  GUI_LIST* flist;
  GUI_LIST* llist;
  GUI_LIST* mode_list;
  GUI_LIST* but_list;
  GUI_LIST* GUIEditor;
  BOOK_PARAM BPEditor;
  int NEditor;
  GUI* YesNoQuestion;
  GUI* StringInput;
  GUI* DateInput;
  IMG tabs_image[ICONS_TAB_COUNT];
  IMG item_image[ICONS_ITEM_COUNT];
  IMG digs_image[ICON_DIGITS_COUNT];
  int MainMenuID;
  wchar_t blistpos;
  wchar_t elistpos;
  wchar_t slistpos;
  wchar_t flistpos;
  wchar_t llistpos;
  wchar_t blistcnt;
  wchar_t elistcnt;
  wchar_t slistcnt;
  wchar_t flistcnt;
  wchar_t llistcnt;
  wchar_t ActiveTAB;
  BOOL isA2;
  int Br_set;
  DATETIME dt;
  BOOL Changed;
  int PhoneType;
  int set_2020;
  wchar_t*folder[2];
  int fmod;
  int fol;
  int folname;
  BOOL books_show_hid;
  BOOL books_show_nogui;
  BOOL elfs_show_hid;
  BOOL elfs_show_nogui;
}MyBOOK;

typedef struct
{
  BOOK * book;
  char * book_name;
  int book_nameId;
  IMAGEID book_icon;
  int isGuiBook;
  BOOL isJava;
  BOOL isJava_2010;
  BOOL isHidden;
}BOOK_LIST_ITEM;

typedef struct
{
  UI_APP_SESSION * session;
  LIST * books_list;
  wchar_t pos_subitem;
}SESSION_LIST_ITEM;

typedef struct
{
  int BookID;
}DESTROYBOOK_DATA;

typedef struct
{
  BOOK* book;
}MSG;

typedef struct
{
  BOOK* book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

int IdlePage_EnterAction( void* r0, BOOK* bk );
int ReconfigElf( void* mess, BOOK* book );
void RefreshBookSoftkeys( MyBOOK* mbk );
void RefreshElfSoftkeys( MyBOOK* mbk, int item );
void GetIconByName(wchar_t* name,int &id);
int CreateBookList( void* r0, BOOK* bk );
int RecreateBookList( void* r0, BOOK* bk );
int isBookManager( BOOK* struc );
int onUserInactivity( void* r0, BOOK* bk );
int onRootListChanged( void* r0, BOOK* bk );
void CloseMyBook( BOOK* Book, GUI* );
void CreateBookManagerGUI( int r0, MyBOOK* mbk );
void PreTerminateManager( BOOK* Book, GUI* );
int NewKey( int key, int rep_count, int mode, MyBOOK * mbk, DISP_OBJ* );
int GetActiveTab(MyBOOK*);
void win12512unicode( wchar_t* ws, char* s, int len );
int GetPhoneType();
void ReturnMyBook( BOOK* bk, GUI*);
int Folders_Filter(const wchar_t * ext_table, const wchar_t * path , const wchar_t * name);
wchar_t* get_path();
BOOK_LIST_ITEM * GetBookListItem( BOOK* bk, int list );

#endif
