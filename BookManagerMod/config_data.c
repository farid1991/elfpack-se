#include "..\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr1_0={CFG_LEVEL,SET_BUTTON_MSG,1,0};
  CFGVAR CFG_HDR cfghdr1_0_1={CFG_KEYCODE,START_BUTTON_MSG,0,0};
  CFGVAR int KeyActiv=0x24;
  CFGVAR int KeyPressMode=3;

  CFGVAR CFG_HDR cfghdr1_0_2={CFG_KEYCODE,MIN_BUTTON_MSG,0,0};
  CFGVAR int KeyMinimize=0x22;
  CFGVAR int KeyMinimizePressMode=3;

  CFGVAR CFG_HDR cfghdr1_0_3={CFG_KEYCODE,CLOSE_BUTTON_MSG,0,0};
  CFGVAR int KeyCloseAll=0x23;
  CFGVAR int KeyCloseAllPressMode=3;

CFGVAR CFG_HDR cfghdr1_2={CFG_LEVEL,CHANGE_BUTTON_MSG,1,0};

  CFGVAR CFG_HDR cfghdr1_0_4={CFG_KEYCODE,CHANGE_RIGHT_BUTTON_MSG,0,0};
  CFGVAR int KeyChangeTabRight=0x16;
  CFGVAR int KeyChangeTabPressModeRight=0;

  CFGVAR CFG_HDR cfghdr1_0_5={CFG_KEYCODE,CHANGE_LEFT_BUTTON_MSG,0,0};
  CFGVAR int KeyChangeTabLeft=0x17;
  CFGVAR int KeyChangeTabPressModeLeft=0;

CFGVAR CFG_HDR cfghdr1_3={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr1_1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_order_start={CFG_LEVEL,TABS_POSITIONS_MSG,1,0};

  CFGVAR CFG_HDR cfghdr_order_active={CFG_CBOX,ACTIVE_TAB_MSG,0,6};
  CFGVAR int FirstTab=0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_order_active[6]={LAST_ACTIVE_MSG,BOOKS_MSG,ELFS_MSG,SHORTCUTS_MSG,FOLDERS_MSG,LOGGERS_MSG};

  CFGVAR CFG_HDR cfghdr_order_books={CFG_CBOX,BOOKS_MSG,0,5};
  CFGVAR int books_tab=0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_order_books[5]={FIRST_MSG,SECOND_MSG,THIRD_MSG,FOURTH_MSG,FIFTH_MSG};

  CFGVAR CFG_HDR cfghdr_order_elfs={CFG_CBOX,ELFS_MSG,0,5};
  CFGVAR int elfs_tab=1;
  CFGVAR CFG_CBOX_ITEM cfgcbox_order_elfs[5]={FIRST_MSG,SECOND_MSG,THIRD_MSG,FOURTH_MSG,FIFTH_MSG};

  CFGVAR CFG_HDR cfghdr_order_shortcuts={CFG_CBOX,SHORTCUTS_MSG,0,5};
  CFGVAR int shortcuts_tab=2;
  CFGVAR CFG_CBOX_ITEM cfgcbox_order_shortcuts[5]={FIRST_MSG,SECOND_MSG,THIRD_MSG,FOURTH_MSG,FIFTH_MSG};

  CFGVAR CFG_HDR cfghdr_order_folders={CFG_CBOX,FOLDERS_MSG,0,5};
  CFGVAR int folders_tab=3;
  CFGVAR CFG_CBOX_ITEM cfgcbox_order_folders[5]={FIRST_MSG,SECOND_MSG,THIRD_MSG,FOURTH_MSG,FIFTH_MSG};

  CFGVAR CFG_HDR cfghdr_order_loggers={CFG_CBOX,LOGGERS_MSG,0,5};
  CFGVAR int loggers_tab=4;
  CFGVAR CFG_CBOX_ITEM cfgcbox_order_loggers[5]={FIRST_MSG,SECOND_MSG,THIRD_MSG,FOURTH_MSG,FIFTH_MSG};

CFGVAR CFG_HDR cfghdr_order_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_images_start={CFG_LEVEL,"Настройка иконок",1,0};

CFGVAR CFG_HDR cfghdr_tabs_images_start={CFG_LEVEL,TABS_IMAGES_MSG,1,0};

  CFGVAR CFG_HDR cfghdr_tabs_images_1={CFG_UTF16_STRING,ACTIVE_MSG " (" BOOKS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab1_act[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Book_on.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_2={CFG_UTF16_STRING,INACTIVE_MSG " (" BOOKS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab1_inact[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Book_off.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_3={CFG_UTF16_STRING,ACTIVE_MSG " (" ELFS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab2_act[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Elfs_on.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_4={CFG_UTF16_STRING,INACTIVE_MSG " (" ELFS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab2_inact[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Elfs_off.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_5={CFG_UTF16_STRING,ACTIVE_MSG " (" SHORTCUTS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab3_act[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Short_on.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_6={CFG_UTF16_STRING,INACTIVE_MSG " (" SHORTCUTS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab3_inact[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Short_off.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_7={CFG_UTF16_STRING,ACTIVE_MSG " (" FOLDERS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab4_act[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Fold_on.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_8={CFG_UTF16_STRING,INACTIVE_MSG " (" FOLDERS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab4_inact[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Fold_off.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_9={CFG_UTF16_STRING,ACTIVE_MSG " (" LOGGERS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab5_act[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Logg_on.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_10={CFG_UTF16_STRING,INACTIVE_MSG " (" LOGGERS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t tab5_inact[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Logg_off.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_11={CFG_UTF16_STRING,TOTAL_MSG " (" BOOKS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t books_icon[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Book.png";

  CFGVAR CFG_HDR cfghdr_tabs_images_12={CFG_UTF16_STRING,TOTAL_MSG " (" ELFS_MSG ")",0,MAX_TABS_IMAGE_PATH_LEN};
  CFGVAR wchar_t elfs_icon[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/Elfs.png";

CFGVAR CFG_HDR cfghdr_tabs_images_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr2_0_2={CFG_LEVEL,"Иконки цифр",1,0};
    CFGVAR CFG_HDR cfghdr2_0_2_0={CFG_UTF16_STRING,"Цифра 0",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit0[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/0.png";

    CFGVAR CFG_HDR cfghdr2_0_2_1={CFG_UTF16_STRING,"Цифра 1",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit1[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/1.png";

    CFGVAR CFG_HDR cfghdr2_0_2_2={CFG_UTF16_STRING,"Цифра 2",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit2[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/2.png";

    CFGVAR CFG_HDR cfghdr2_0_2_3={CFG_UTF16_STRING,"Цифра 3",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit3[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/3.png";

    CFGVAR CFG_HDR cfghdr2_0_2_4={CFG_UTF16_STRING,"Цифра 4",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit4[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/4.png";

    CFGVAR CFG_HDR cfghdr2_0_2_5={CFG_UTF16_STRING,"Цифра 5",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit5[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/5.png";

    CFGVAR CFG_HDR cfghdr2_0_2_6={CFG_UTF16_STRING,"Цифра 6",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit6[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/6.png";

    CFGVAR CFG_HDR cfghdr2_0_2_7={CFG_UTF16_STRING,"Цифра 7",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit7[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/7.png";

    CFGVAR CFG_HDR cfghdr2_0_2_8={CFG_UTF16_STRING,"Цифра 8",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit8[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/8.png";

    CFGVAR CFG_HDR cfghdr2_0_2_9={CFG_UTF16_STRING,"Цифра 9",0,MAX_TABS_IMAGE_PATH_LEN};
    CFGVAR wchar_t digit9[MAX_TABS_IMAGE_PATH_LEN+1]=L"%INI_INT%/bookman/9.png";
  CFGVAR CFG_HDR cfghdr2_0_3={CFG_LEVEL,"",0,0};

  CFGVAR CFG_HDR cfghdr_images_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_bookman={CFG_LEVEL,BOOKMAN_CONFIG_MSG,1,0};

  CFGVAR CFG_HDR cfghdr_bookman_1={CFG_INT,TAB_COUNT_MSG,0,5};
  CFGVAR int TabCount=5;

  CFGVAR CFG_HDR cfghdr_heap={CFG_CBOX,HEAP_MSG,0,2};
  CFGVAR int cfg_heap_format=0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_heap[2]={KB_MSG,MB_MSG};

  CFGVAR CFG_HDR cfghdr_bookman_2={CFG_CHECKBOX,IGNORE_KEYLOCK_MSG,0,0};
  CFGVAR int Ignore_KeyLock=0;

  CFGVAR CFG_HDR cfghdr_bookman_4={CFG_CBOX,CLOSE_WHEN_INACTIVE_MSG,0,2};
  CFGVAR int UserInactivityEventEnabled=1;
  CFGVAR CFG_CBOX_ITEM cfgcbox_bookman_1[2]={CLOSE_MSG,CLOSE_KEYLOCK_MSG};

  CFGVAR CFG_HDR cfghdr_bookman_6={CFG_CBOX,SHOW_AFTER_SHORTCUT_MSG,0,2};
  CFGVAR int StandbyOnTop=0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_bookman_3[2]={CURRENT_BOOK_MSG,STANDBY_MSG};

  CFGVAR CFG_HDR cfghdr_bookman_n6={CFG_CHECKBOX,GUI_FOCUS_MSG,0,0};
  CFGVAR int ConfOrGUI=0;

  CFGVAR CFG_HDR cfghdr3_0={CFG_LEVEL,GUI_SETTINGS_MSG,1,0};

  CFGVAR CFG_HDR cfghdr3_0_0={CFG_LEVEL,BOOK_SETTINGS_MSG,1,0};
  CFGVAR CFG_HDR cfghdr3_0_0_0={CFG_CHECKBOX,SHOW_HIDDEN_MSG,0,0};
  CFGVAR int books_show_hid=0;

  CFGVAR CFG_HDR cfghdr3_0_0_1={CFG_CHECKBOX,SHOW_NOGUI_MSG,0,0};
  CFGVAR int books_show_nogui=0;

  CFGVAR CFG_HDR cfghdr3_0_0_2={CFG_CBOX,SHOW_BOOKS_AS_MSG,0,2};
  CFGVAR int MINIMIZE_TO_SESSION=0;
  CFGVAR CFG_CBOX_ITEM cfgcbox3_0_0_2_0[2]={SHOW_AS_BOOKS_MSG,SHOW_AS_SESSIONS_MSG};
  CFGVAR CFG_HDR cfghdr3_0_1={CFG_LEVEL,"",0,0};

  CFGVAR CFG_HDR cfghdr3_0_2={CFG_LEVEL,ELFS_SETTINGS_MSG,1,0};
  CFGVAR CFG_HDR cfghdr3_0_2_0={CFG_CHECKBOX,SHOW_HIDDEN_MSG,0,0};
  CFGVAR int elfs_show_hid=0;

  CFGVAR CFG_HDR cfghdr3_0_2_1={CFG_CHECKBOX,SHOW_NOGUI_MSG,0,0};
  CFGVAR int elfs_show_nogui=0;
  CFGVAR CFG_HDR cfghdr3_0_4={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr3_1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr9_0={CFG_CBOX,CLOSE_BOOKS_AS_MSG,0,2};

  CFGVAR int DestroyMethod=1;
  CFGVAR CFG_CBOX_ITEM cfgcbox9_0_0[2]={SHOW_AS_BOOKS_MSG,SHOW_AS_SESSIONS_MSG};

  CFGVAR CFG_HDR cfghdr_bcfgedit={CFG_UTF16_STRING,BCFGEDIT_PATH_MSG,0,255};
  CFGVAR wchar_t cfg_bcfgedit[256]=L"%ELFS_INT%/BcfgEdit.elf";

CFGVAR CFG_HDR cfghdr3={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_folders_config_start={CFG_LEVEL,FOLDERS_CONFIG_MSG,1,0};

  CFGVAR CFG_HDR cfghdr_filter={CFG_CHECKBOX,FILTER_ON_MSG,0,0};
  CFGVAR int filter_on=0;

  CFGVAR CFG_HDR cfghdr_folders_cnt={CFG_INT,FOLDERS_COUNT_MSG,0,10};
  CFGVAR int FoldersCount=10;

  CFGVAR CFG_HDR cfghdr_folders_actions_start={CFG_LEVEL,FOLDERS_ACTIONS_MSG,3,0};

  CFGVAR CFG_HDR cfghdr_folders_action_1={CFG_CBOX,FIRST_FOLDER_MSG,0,10};
  CFGVAR int FirstFolder=0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_1[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_2={CFG_CBOX,SECOND_FOLDER_MSG,0,10};
  CFGVAR int SecondFolder=1;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_2[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_3={CFG_CBOX,THIRD_FOLDER_MSG,0,10};
  CFGVAR int ThirdFolder=2;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_3[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_4={CFG_CBOX,FOURTH_FOLDER_MSG,0,10};
  CFGVAR int FourthFolder=9;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_4[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_5={CFG_CBOX,FIFTH_FOLDER_MSG,0,10};
  CFGVAR int FifthFolder=3;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_5[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_6={CFG_CBOX,SIXTH_FOLDER_MSG,0,10};
  CFGVAR int SixthFolder=4;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_6[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_7={CFG_CBOX,SEVENTH_FOLDER_MSG,0,10};
  CFGVAR int SeventhFolder=5;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_7[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_8={CFG_CBOX,EIGHT_FOLDER_MSG,0,10};
  CFGVAR int EightFolder=6;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_8[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_9={CFG_CBOX,NINTH_FOLDER_MSG,0,10};
  CFGVAR int NinthFolder=7;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_9[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_action_10={CFG_CBOX,TENTH_FOLDER_MSG,0,10};
  CFGVAR int TenthFolder=8;
  CFGVAR CFG_CBOX_ITEM cfgcbox_action_10[10]={OPEN_FOLDER1_MSG,OPEN_FOLDER2_MSG,OPEN_FOLDER3_MSG,OPEN_FOLDER4_MSG,OPEN_FOLDER5_MSG,OPEN_FOLDER6_MSG,OPEN_FOLDER7_MSG,OPEN_FOLDER8_MSG,OPEN_FOLDER9_MSG,OPEN_FOLDER10_MSG};

  CFGVAR CFG_HDR cfghdr_folders_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_folders_config_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_loggers_config_start={CFG_LEVEL,LOGGERS_CONFIG_MSG,1,0};

  CFGVAR CFG_HDR cfghdr_oggers_config_1={CFG_CBOX,OPEN_BY_MSG,0,2};
  CFGVAR int cfg_open_log_by_elf = 0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_loggers_config_1[2]={OPEN_BY_BROWSER_MSG,OPEN_BY_ELF_MSG};

  CFGVAR CFG_HDR cfghdr_loggers_config_2={CFG_UTF16_STRING,ELF_PATH_MSG,0,255};
  CFGVAR wchar_t cfg_elf_path[256]=L"";

CFGVAR CFG_HDR cfghdr_loggers_config_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_brightnes={CFG_LEVEL,BRIGHTNESS_MSG,1,2};

  CFGVAR CFG_HDR cfghdr_bright_on={CFG_INT,BRIGHTNESS_ON_MSG,0,100};
  CFGVAR int bright_on=100;
  CFGVAR CFG_HDR cfghdr_bright_off={CFG_INT,BRIGHTNESS_OFF_MSG,0,50};
  CFGVAR int bright_off=20;

CFGVAR CFG_HDR cfghdr_brightnes_end={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_hidden={CFG_LEVEL,SHORTCUTS_MSG,1,0};
  CFGVAR CFG_HDR cfghdr_hidden_books={CFG_LEVEL,BOOKS_TAB_MSG,1,0};
    CFGVAR CFG_HDR cfghdr_hidden_books_short={CFG_LEVEL,SHORT_PRESS_MSG,1,0};
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_0={CFG_UTF16_STRING,"0",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_0[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_1={CFG_UTF16_STRING,"1",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_1[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_2={CFG_UTF16_STRING,"2",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_2[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_3={CFG_UTF16_STRING,"3",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_3[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_4={CFG_UTF16_STRING,"4",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_4[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_5={CFG_UTF16_STRING,"5",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_5[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_6={CFG_UTF16_STRING,"6",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_6[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_7={CFG_UTF16_STRING,"7",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_7[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_8={CFG_UTF16_STRING,"8",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_8[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_short_action_9={CFG_UTF16_STRING,"9",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_short_action_9[MAX_SHORTCUTS_ACTION_LEN]=L"";
    CFGVAR CFG_HDR cfghdr_hidden_books_short_end={CFG_LEVEL,"",0,0};

    CFGVAR CFG_HDR cfghdr_hidden_books_long={CFG_LEVEL,LONG_PRESS_MSG,1,0};
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_0={CFG_UTF16_STRING,"0",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_0[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_1={CFG_UTF16_STRING,"1",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_1[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_2={CFG_UTF16_STRING,"2",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_2[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_3={CFG_UTF16_STRING,"3",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_3[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_4={CFG_UTF16_STRING,"4",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_4[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_5={CFG_UTF16_STRING,"5",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_5[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_6={CFG_UTF16_STRING,"6",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_6[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_7={CFG_UTF16_STRING,"7",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_7[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_8={CFG_UTF16_STRING,"8",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_8[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_books_long_action_9={CFG_UTF16_STRING,"9",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t books_long_action_9[MAX_SHORTCUTS_ACTION_LEN]=L"";
    CFGVAR CFG_HDR cfghdr_hidden_books_long_end={CFG_LEVEL,"",0,0};
  CFGVAR CFG_HDR cfghdr_hidden_books_end={CFG_LEVEL,"",0,0};

  CFGVAR CFG_HDR cfghdr_hidden_elfs={CFG_LEVEL,ELFS_TAB_MSG,1,0};
    CFGVAR CFG_HDR cfghdr_hidden_elfs_short={CFG_LEVEL,SHORT_PRESS_MSG,1,0};
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_0={CFG_UTF16_STRING,"0",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_0[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_1={CFG_UTF16_STRING,"1",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_1[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_2={CFG_UTF16_STRING,"2",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_2[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_3={CFG_UTF16_STRING,"3",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_3[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_4={CFG_UTF16_STRING,"4",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_4[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_5={CFG_UTF16_STRING,"5",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_5[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_6={CFG_UTF16_STRING,"6",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_6[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_7={CFG_UTF16_STRING,"7",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_7[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_8={CFG_UTF16_STRING,"8",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_8[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_short_action_9={CFG_UTF16_STRING,"9",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_short_action_9[MAX_SHORTCUTS_ACTION_LEN]=L"";
    CFGVAR CFG_HDR cfghdr_hidden_elfs_short_end={CFG_LEVEL,"",0,0};

    CFGVAR CFG_HDR cfghdr_hidden_elfs_long={CFG_LEVEL,LONG_PRESS_MSG,1,0};
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_0={CFG_UTF16_STRING,"0",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_0[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_1={CFG_UTF16_STRING,"1",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_1[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_2={CFG_UTF16_STRING,"2",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_2[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_3={CFG_UTF16_STRING,"3",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_3[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_4={CFG_UTF16_STRING,"4",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_4[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_5={CFG_UTF16_STRING,"5",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_5[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_6={CFG_UTF16_STRING,"6",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_6[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_7={CFG_UTF16_STRING,"7",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_7[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_8={CFG_UTF16_STRING,"8",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_8[MAX_SHORTCUTS_ACTION_LEN]=L"";
      CFGVAR CFG_HDR cfghdr_hidden_elfs_long_action_9={CFG_UTF16_STRING,"9",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t elfs_long_action_9[MAX_SHORTCUTS_ACTION_LEN]=L"";
    CFGVAR CFG_HDR cfghdr_hidden_elfs_long_end={CFG_LEVEL,"",0,0};
  CFGVAR CFG_HDR cfghdr_hidden_elfs_end={CFG_LEVEL,"",0,0};

  CFGVAR CFG_HDR cfghdr_hidden_shortcuts={CFG_LEVEL,SHORTCUTS_TAB_MSG,1,0};
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_1={CFG_UTF16_STRING,"1",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_1[MAX_SHORTCUTS_ACTION_LEN]=L"Msg_WriteNew";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_2={CFG_UTF16_STRING,"2",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_2[MAX_SHORTCUTS_ACTION_LEN]=L"BlueTooth_Settings";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_3={CFG_UTF16_STRING,"3",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_3[MAX_SHORTCUTS_ACTION_LEN]=L"MenuItem_SetTemporaryAlarm";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_4={CFG_UTF16_STRING,"4",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_4[MAX_SHORTCUTS_ACTION_LEN]=L"Menu_CalendarMain";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_5={CFG_UTF16_STRING,"5",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_5[MAX_SHORTCUTS_ACTION_LEN]=L"Application_Calculator_Id";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_6={CFG_UTF16_STRING,"6",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_6[MAX_SHORTCUTS_ACTION_LEN]=L"Calls";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_7={CFG_UTF16_STRING,"7",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_7[MAX_SHORTCUTS_ACTION_LEN]=L"Phonebook";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_8={CFG_UTF16_STRING,"8",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_8[MAX_SHORTCUTS_ACTION_LEN]=L"Settings";
      CFGVAR CFG_HDR cfghdr_hidden_shortcuts_action_9={CFG_UTF16_STRING,"9",0,MAX_SHORTCUTS_ACTION_LEN - 1};
      CFGVAR wchar_t shortcuts_action_9[MAX_SHORTCUTS_ACTION_LEN]=L"Camera";
  CFGVAR CFG_HDR cfghdr_hidden_shortcuts_end={CFG_LEVEL,"",0,0};
CFGVAR CFG_HDR cfghdr_hidden_end={CFG_LEVEL,"",0,0};

