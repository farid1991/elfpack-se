#include "RegistryClasses.h"

#ifndef _REGEDITOR_H_
#define _REGEDITOR_H_

int TerminateElf(void * ,BOOK * book);
int ShowAuthorInfo(void *mess ,BOOK * book);
int RegistryEditorBook_MainPage_Enter(void * r0,BOOK * book);
int RegistryEditorBook_SelectBackup_Page_Enter(void * r0,BOOK * book);
int RegistryEditorBook_SelectBackup_Page_Accept(void *mess ,BOOK *book);
int RegistryEditorBook_SelectBackup_Page_Previous(void *mess ,BOOK *book);
void registry_list_gui_delete_no_action(BOOK * book, GUI * gui);


#define BACKUP_EXT L".gdfs"
#define BACKUP_EXT_FILTER L"*.gdfs"

#define BACKUP_VAR_EXT L".bin"
#define BACKUP_VAR_EXT_FILTER L"*.bin"

#define BACKUP_REGISTRY_EXT L".reg"


#define KEY_ICON _T("KEYLOCK_ICN")
#define VALUE_ICON _T("DB_LIST_OTHER_ICN")


#define RENAME_ACTION 0x100
#define CREATE_KEY_ACTION 0x101
#define CREATE_VALUE_ACTION 0x102
#define CREATE_UINT_ACTION 0x103
#define CREATE_STRING_ACTION 0x104
#define CREATE_WSTRING_ACTION 0x105
#define CREATE_BINARY_ACTION 0x106
#define EDIT_ACTION 0x107
#define SORTING_ACTION 0x108
#define SORTING_NOSORT_ACTION 0x109
#define SORTING_ASCENDING_ACTION 0x10A
#define SORTING_DESCENDING_ACTION 0x10B
#define EDITOR_MODE_ACTION 0x10C
#define GDFS_TOOLS_ACTION 0x10D
#define GDFS_SAVE_ACTION 0x10E
#define GDFS_RESTORE_ACTION 0x10F
#define GDFS_SAVE_VAR_ACTION 0x110
#define GDFS_RESTORE_VAR_ACTION 0x111
#define REGISTRY_EXPORT_ACTION 0x112


#define REG_UINT "uint"
#define REG_STRING "string"
#define REG_WSTRING "wstring"
#define REG_BINARY "binary"


typedef enum
{
  KEY = 1,
  VALUE = 2,
  EMPTY = 3
}ELEM_ID;


typedef enum
{
  REGISTRY_MODE = 0,
  GDFS_MODE = 1
}EDITOR_MODE;


typedef enum
{
  REAL = 0,
  STRING = 1,
  INTEGER = 2,
  PHONE_NUMBER = 3,
  DIGITAL_PASS = 4,
  DIGITAL_IP = 5,
  URL = 6,
  UNSIGNED_DIGIT = 9
}SI_INPUT_MODE;


typedef enum
{
  Nosort = 1,
  Ascending_order = 2,
  Descending_order = 3
}RegBrowser_SortOrder;


typedef struct
{
  BOOK * book;
}MSG;


typedef struct _MYBOOK : BOOK
{
  GUI_LIST * registry_list_gui;
  GUI * yesno_gui;
  GUI * stringinput_gui;
  Reg_RegValueType_t value_type;
  char value_create_edit[39];
  bool edit_flag;
  bool reboot_flag;
  bool restore_var_flag;
  bool is_A2;
  EDITOR_MODE editor_mode;
  wchar_t sel_item;
  LIST * pRegList;
  IRegistry * pIRegistry;
  LIST * Stack_List;
  FILE_DIALOG * dll;
  char CurrentKeyPath[0x122];
}RegistryEditorBook;



#endif
