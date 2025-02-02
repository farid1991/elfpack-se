#ifndef _CONFIGDATA_H_
#define _CONFIGDATA_H_

#define MAX_SHORTCUTS_ACTION_LEN 96
#define MAX_TABS_IMAGE_PATH_LEN 255

#define SET_BUTTON_MSG "��������� ������"
#define START_BUTTON_MSG "������ ������"
#define MIN_BUTTON_MSG "��������"
#define CLOSE_BUTTON_MSG "�������"
#define CHANGE_BUTTON_MSG "����� �������"
#define CHANGE_RIGHT_BUTTON_MSG "������"
#define CHANGE_LEFT_BUTTON_MSG "�����"
#define TABS_POSITIONS_MSG "��������� �������"
#define BOOKS_MSG "�����"
#define ELFS_MSG "�����"
#define SHORTCUTS_MSG "������"
#define FOLDERS_MSG "�����"
#define LOGGERS_MSG "Call Logger"
#define BOOKS_TAB_MSG "������� �����"
#define ELFS_TAB_MSG "������� �����"
#define SHORTCUTS_TAB_MSG "������� ������"
#define LONG_PRESS_MSG "������� �������"
#define SHORT_PRESS_MSG "�������� �������"
#define FIRST_MSG "� ������"
#define SECOND_MSG "�� ������"
#define THIRD_MSG "� �������"
#define FOURTH_MSG "� ��������"
#define FIFTH_MSG "� �����"
#define TABS_IMAGES_MSG "������ �������"
#define ACTIVE_MSG "��������"
#define INACTIVE_MSG "�� ��������"
#define TOTAL_MSG "�����"
#define BOOKMAN_CONFIG_MSG "������ Bookman"
#define TAB_COUNT_MSG "���������� �������"
#define IGNORE_KEYLOCK_MSG "�����. ����. ����."
#define CLOSE_WHEN_INACTIVE_MSG "������� ��� �����."
#define CLOSE_MSG "�������"
#define CLOSE_KEYLOCK_MSG "������� � ����. ����."
#define SHOW_AFTER_SHORTCUT_MSG "����� ������"
#define CURRENT_BOOK_MSG "�������"
#define STANDBY_MSG "����� ��������"
#define ACTIVE_TAB_MSG "�������� �������"
#define LAST_ACTIVE_MSG "���������"
#define SHORTCUTS_CONFIG_MSG "��������� ������"
#define SHORTCUTS_COUNT_MSG "����������"
#define SHORTCUTS_ACTIONS_MSG "��������"
#define FIRST_ACTIONS_MSG "������"
#define SECOND_ACTIONS_MSG "������"
#define THIRD_ACTIONS_MSG "������"
#define FOURTH_ACTIONS_MSG "���������"
#define FIFTH_ACTIONS_MSG "�����"
#define SIXTH_ACTIONS_MSG "������"
#define SEVENTH_ACTIONS_MSG "�������"
#define EIGHTH_ACTIONS_MSG "�������"
#define NINTH_ACTIONS_MSG "�������"
#define FOLDERS_CONFIG_MSG "��������� �����"
#define FILTER_ON_MSG "�������� ������"
#define FOLDERS_COUNT_MSG "����������"
#define FOLDERS_NAMES_MSG "�����"
#define FOLDERS_ACTIONS_MSG "��������"
#define FIRST_FOLDER_MSG "������ �����"
#define SECOND_FOLDER_MSG "������ �����"
#define THIRD_FOLDER_MSG "������ �����"
#define FOURTH_FOLDER_MSG "��������� �����"
#define FIFTH_FOLDER_MSG "����� �����"
#define SIXTH_FOLDER_MSG "������ �����"
#define SEVENTH_FOLDER_MSG "������� �����"
#define EIGHT_FOLDER_MSG "������� �����"
#define NINTH_FOLDER_MSG "������� �����"
#define TENTH_FOLDER_MSG "������� �����"
#define BRIGHTNESS_MSG "�������"
#define BRIGHTNESS_ON_MSG "��������� ��"
#define BRIGHTNESS_OFF_MSG "��������� ��"
#define VIEW_MSG "��� � ������"
#define OPEN_FOLDER1_MSG "ZBin"
#define OPEN_FOLDER2_MSG "Daemons"
#define OPEN_FOLDER3_MSG "Config"
#define OPEN_FOLDER4_MSG "Ini"
#define OPEN_FOLDER5_MSG "������"
#define OPEN_FOLDER6_MSG "�����"
#define OPEN_FOLDER7_MSG "��������"
#define OPEN_FOLDER8_MSG "�����"
#define OPEN_FOLDER9_MSG "����"
#define OPEN_FOLDER10_MSG "Dll"
#define HEAP_MSG "Heap"
#define KB_MSG "Kb"
#define MB_MSG "Mb"
#define BCFGEDIT_PATH_MSG "���� � BcfgEdit.elf"
#define LOGGERS_CONFIG_MSG "��������� Call Logger"
#define OPEN_BY_MSG "��������� ����"
#define OPEN_BY_BROWSER_MSG "���������"
#define OPEN_BY_ELF_MSG "������"
#define ELF_PATH_MSG "���� � �����"
#define GUI_FOCUS_MSG "������ ������ ������ GUI"
#define GUI_SETTINGS_MSG "��������� GUI"
#define BOOK_SETTINGS_MSG "������� �����"
#define ELFS_SETTINGS_MSG "������� �����"
#define SHOW_HIDDEN_MSG "���������� �������"
#define SHOW_NOGUI_MSG "���������� ��� GUI"
#define SHOW_BOOKS_AS_MSG "���������� ����� ���..."
#define CLOSE_BOOKS_AS_MSG "����� �������� ����"
#define SHOW_AS_BOOKS_MSG "��� �����"
#define SHOW_AS_SESSIONS_MSG "��� ������"

extern const int KeyActiv;
extern const int KeyPressMode;
extern const int KeyMinimize;
extern const int KeyMinimizePressMode;
extern const int KeyCloseAll;
extern const int KeyCloseAllPressMode;
extern const int KeyChangeTabRight;
extern const int KeyChangeTabPressModeRight;
extern const int KeyChangeTabLeft;
extern const int KeyChangeTabPressModeLeft;
extern const int Ignore_KeyLock;
extern const int ConfOrGUI;
extern const int books_show_hid;
extern const int books_show_nogui;
extern const int MINIMIZE_TO_SESSION;
extern const int elfs_show_hid;
extern const int elfs_show_nogui;
extern const int DestroyMethod;
extern const int UserInactivityEventEnabled;
extern const int StandbyOnTop;
extern const wchar_t cfg_bcfgedit[256];
extern const int FirstTab;
extern const wchar_t tab1_act[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab1_inact[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab2_act[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab2_inact[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab3_act[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab3_inact[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab4_act[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab4_inact[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab5_act[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t tab5_inact[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t books_icon[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t elfs_icon[MAX_TABS_IMAGE_PATH_LEN+1];

extern const wchar_t digit0[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit1[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit2[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit3[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit4[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit5[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit6[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit7[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit8[MAX_TABS_IMAGE_PATH_LEN+1];
extern const wchar_t digit9[MAX_TABS_IMAGE_PATH_LEN+1];

extern const int cfg_open_log_by_elf;
extern const wchar_t cfg_elf_path[256];

extern const int books_tab;
extern const int elfs_tab;
extern const int shortcuts_tab;
extern const int folders_tab;
extern const int loggers_tab;

extern const int TabCount;
extern const int FoldersCount;

extern const int FirstFolder;
extern const int SecondFolder;
extern const int ThirdFolder;
extern const int FourthFolder;
extern const int FifthFolder;
extern const int SixthFolder;
extern const int SeventhFolder;
extern const int EightFolder;
extern const int NinthFolder;
extern const int TenthFolder;

extern const int bright_on;
extern const int bright_off;
extern const int filter_on;

extern const int cfg_heap_format;

extern const wchar_t books_short_action_0[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_1[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_2[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_3[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_4[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_5[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_6[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_7[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_8[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_short_action_9[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_0[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_1[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_2[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_3[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_4[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_5[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_6[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_7[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_8[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t books_long_action_9[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_0[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_1[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_2[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_3[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_4[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_5[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_6[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_7[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_8[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_short_action_9[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_0[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_1[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_2[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_3[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_4[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_5[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_6[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_7[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_8[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t elfs_long_action_9[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_1[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_2[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_3[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_4[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_5[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_6[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_7[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_8[MAX_SHORTCUTS_ACTION_LEN];
extern const wchar_t shortcuts_action_9[MAX_SHORTCUTS_ACTION_LEN];


//////////////////////////////////////////
extern const CFG_HDR cfghdr_lang;
extern const CFG_CBOX_ITEM cfgcbox_lang[2];
extern const CFG_HDR cfghdr_button;
extern const CFG_HDR cfghdr_order_start;
extern const CFG_HDR cfghdr_order_active;
extern const CFG_CBOX_ITEM cfgcbox_order_active[6];
extern const CFG_HDR cfghdr_order_books;
extern const CFG_CBOX_ITEM cfgcbox_order_books[5];
extern const CFG_HDR cfghdr_order_elfs;
extern const CFG_CBOX_ITEM cfgcbox_order_elfs[5];
extern const CFG_HDR cfghdr_order_shortcuts;
extern const CFG_CBOX_ITEM cfgcbox_order_shortcuts[5];
extern const CFG_HDR cfghdr_order_folders;
extern const CFG_CBOX_ITEM cfgcbox_order_folders[5];
extern const CFG_HDR cfghdr_order_loggers;
extern const CFG_CBOX_ITEM cfgcbox_order_loggers[5];
extern const CFG_HDR cfghdr_tabs_images_start;
extern const CFG_HDR cfghdr_tabs_images_1;
extern const CFG_HDR cfghdr_tabs_images_2;
extern const CFG_HDR cfghdr_tabs_images_3;
extern const CFG_HDR cfghdr_tabs_images_4;
extern const CFG_HDR cfghdr_tabs_images_5;
extern const CFG_HDR cfghdr_tabs_images_6;
extern const CFG_HDR cfghdr_tabs_images_7;
extern const CFG_HDR cfghdr_tabs_images_8;
extern const CFG_HDR cfghdr_tabs_images_9;
extern const CFG_HDR cfghdr_tabs_images_10;
extern const CFG_HDR cfghdr_tabs_images_11;
extern const CFG_HDR cfghdr_tabs_images_12;
extern const CFG_HDR cfghdr_bookman;
extern const CFG_HDR cfghdr_bookman_1;
extern const CFG_HDR cfghdr_bookman_2;
extern const CFG_HDR cfghdr_bookman_3;
extern const CFG_HDR cfghdr_bookman_4;
extern const CFG_CBOX_ITEM cfgcbox_bookman_1[2];
extern const CFG_HDR cfghdr_bookman_5;
extern const CFG_CBOX_ITEM cfgcbox_bookman_2[2];
extern const CFG_HDR cfghdr_bookman_6;
extern const CFG_CBOX_ITEM cfgcbox_bookman_3[2];
extern const CFG_HDR cfghdr_folders_config_start;
extern const CFG_HDR cfghdr_filter;
extern const CFG_HDR cfghdr_folders_cnt;
extern const CFG_HDR cfghdr_folders_actions_start;
extern const CFG_HDR cfghdr_folders_action_1;
extern const CFG_CBOX_ITEM cfgcbox_action_1[10];
extern const CFG_HDR cfghdr_folders_action_2;
extern const CFG_CBOX_ITEM cfgcbox_action_2[10];
extern const CFG_HDR cfghdr_folders_action_3;
extern const CFG_CBOX_ITEM cfgcbox_action_3[10];
extern const CFG_HDR cfghdr_folders_action_4;
extern const CFG_CBOX_ITEM cfgcbox_action_4[10];
extern const CFG_HDR cfghdr_folders_action_5;
extern const CFG_CBOX_ITEM cfgcbox_action_5[10];
extern const CFG_HDR cfghdr_folders_action_6;
extern const CFG_CBOX_ITEM cfgcbox_action_6[10];
extern const CFG_HDR cfghdr_folders_action_7;
extern const CFG_CBOX_ITEM cfgcbox_action_7[10];
extern const CFG_HDR cfghdr_folders_action_8;
extern const CFG_CBOX_ITEM cfgcbox_action_8[10];
extern const CFG_HDR cfghdr_folders_action_9;
extern const CFG_CBOX_ITEM cfgcbox_action_9[10];
extern const CFG_HDR cfghdr_folders_action_10;
extern const CFG_CBOX_ITEM cfgcbox_action_10[10];
extern const CFG_HDR cfghdr_brightnes;
extern const CFG_HDR cfghdr_bright_on;
extern const CFG_HDR cfghdr_bright_off;

extern const CFG_HDR cfghdr_hidden;
extern const CFG_HDR cfghdr_hidden_books;
extern const CFG_HDR cfghdr_hidden_books_short;
extern const CFG_HDR cfghdr_hidden_books_long;
extern const CFG_HDR cfghdr_hidden_elfs;
extern const CFG_HDR cfghdr_hidden_elfs_short;
extern const CFG_HDR cfghdr_hidden_elfs_long;
extern const CFG_HDR cfghdr_hidden_shortcuts;

extern const CFG_HDR cfghdr_bcfgedit;

extern const CFG_HDR cfghdr_loggers_config_start;
extern const CFG_HDR cfghdr_oggers_config_1;
extern const CFG_CBOX_ITEM cfgcbox_loggers_config_1[2];
extern const CFG_HDR cfghdr_loggers_config_2;


#endif
