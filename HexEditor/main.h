#ifndef _MAIN_H_
#define _MAIN_H_

#include "..\\deleaker\mem2.h"

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"

#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"



#include "HexEditor.h"
#include "Graphics.h"
#include "LNGparser.h"
#include "page_desc.h"

#define BOOKNAME    "HexEditor"
//#define ELFNAME     "HexEditor"
#define VERSION     "ver e.3"
#define AUTHOR      "(c) Joker XT"
#define ENTER       "\n"
#define PORTED      "ported with motorola"

#define DATE        __DATE__

#define RESULT_OK              0
#define RESULT_FAIL           -1
#define FILE_HANDLE_INVALID   -1
#define FILE_INVALID          -1

#define FILENAME_MAX_LEN      255

#define DLL_DATA FILE_DIALOG

#define HE_ACTION_EDIT ACTION_YES
#define HE_ACTION_MENU ACTION_NO

typedef int FILE_HANDLE;

#define GUI_DESTROY(a) if(a){a=GUIObject_Destroy(a);};
#define FREE(b) if(b){mfree(b); b=0;};

#define NUM_OF_GUI 6

static const char GUINAME_0[]="HE_GUI0";
static const char GUINAME_1[]="HE_GUI1";
static const char GUINAME_2[]="HE_GUI2";
static const char GUINAME_3[]="HE_GUI3";
static const char GUINAME_4[]="HE_GUI4";
static const char GUINAME_5[]="HE_GUI5";

enum INPUT_TYPES
{
	IT_REAL = 0,
	IT_STRING = 1,
	IT_INTEGER = 2,
	IT_PHONE_NUMBER = 3,
	IT_DIGITAL_PASS = 4,
	IT_DIGITAL_IP = 5,
	IT_URL = 6,
        IT_ABC_AND_DIGIT = 8,
	IT_UNSIGNED_DIGIT = 9,
	IT_EXTRA_DIGIT = 10,
	IT_ABC_OR_DIGIT = 11,
	IT_EXTRA_DIGIT_2 = 12,
	IT_ABC_OR_DIGIT_2 = 13,
};

enum align_t
{
  ALIGN_LEFT = 0,
  ALIGN_RIGHT,
  ALIGN_CENTER
};

enum file_t
{
  T_OPEN_FILE = 0,
  T_CLOSE_FILE,
  T_CLOSE_AND_OPEN_FILE
};

typedef struct
{
  int c_offset;
  char correction;
  char real;
} HE_CORRECTION;

typedef struct
{
  DISP_OBJ disp_obj;
} DISP_OBJ_HE;

typedef struct
{
  wchar_t *attachment;
} MSG_HEX_EDITOR;

typedef struct
{
  BOOK *book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
} MSG_BCFG;

int _main(wchar_t *elf, wchar_t *path, wchar_t *name);

void CloseFile(void);

void Redraw_Update(int r0, int r1, int r2, int r3, int r4, int r5);

void HexEditorMenu_SelectItem(int item);

void HexEditor_ActionEdit(BOOK *book, GUI *gui);

int HexEditorMenu_GetNumItemsViseble(void);

void CreateFileDialog(wchar_t *dir);
void CreateFolderList(void);
void InputRamAddress(void);

BOOK *Find_HexEditorBook(void);
GUI *BookObj_GetGUIObject(BOOK *book, int num);

void HexEditorMenu_Item_SetEnable(int item, int enable);
void HexEditorMenu_Item_SetVisible(int item, int enable);

int HexEditorMenu_Item_GetEnable(int item);
int HexEditorMenu_Item_GetVisible(int item);

int SaveOffset(int off_set, char value);
int RelativeOffset(int off_set);
int LoadBuffer(int off_set);
int GoTo(int off_set);

wchar_t *MakeFullName(wchar_t *path, wchar_t *name);
TEXTID Lang_GetTextID(LNG_RES_ID_T num);

int HexEditor_CloseViewRAM(void);
int HexEditor_CloseFile(void);
int HexEditor_SaveFile(void);
int HexEditor_FileOpen(void);
int HexEditor_OpenRAM(void);
int HexEditor_GoToOffset(void);
int HexEditor_GoToPage(void);
int HexEditor_Edit(void);
int HexEditor_Find(void);

int CreateHexEditorGUI(void *mess, BOOK *book);
void CloseHexEditorBook(BOOK *book, GUI *gui);

#undef _MAIN_H_
#endif
