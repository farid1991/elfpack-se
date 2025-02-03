#include "..\deleaker\mem2.h"

#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\dll.h"
#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "..\\include\var_arg.h"

class CDbgPrint
{
  char* fl;
  char* fn;
public:
  CDbgPrint(char* file,char* func){debug_printf("\nfunction enter: %s/%s\n",fl=file,fn=func);}
  ~CDbgPrint(){debug_printf("\nfunction leave: %s/%s\n",fl,fn);}
};
#ifdef NDEBUG
#define FUNCTION
#else
#define FUNCTION CDbgPrint __function((char*)__FILE__,(char*)__FUNCTION__);
#endif

typedef struct
{
  int (*Dir_Registered)(wchar_t * dir);
  int (*Dir_Add)(wchar_t * dir);
  int (*Dir_Rename)(wchar_t * dir, wchar_t * new_dir);
  int (*Dir_Remove)(wchar_t * dir);
  int (*Dir_SetName)(wchar_t * dir, wchar_t * name);
  int (*Dir_GetName)(wchar_t * dir, wchar_t ** name);
  int (*Dir_SetDeleteState)(wchar_t * dir, char state);
  int (*Dir_GetDeleteState)(wchar_t * dir, char * state);
  int (*Dir_SetRenameState)(wchar_t * dir, char state);
  int (*Dir_GetRenameState)(wchar_t * dir, char * state);
  int (*Dir_SetSmallIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetSmallIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetSmallIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Dir_SetBigIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetBigIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetBigIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);

  wchar_t * (*Dir_GetShortPath)(wchar_t * dir);
  int (*Dir_GetCount)();
  LIST * (*Dir_ListCreate)();
  int (*Dir_ListDestroy)(LIST * list);
}DIRECTORY_METHODS;

typedef struct
{
  BOOK book;
  DIRECTORY_METHODS * directory_methods;
  int (*Dir_SetIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Dir_SetState)(wchar_t * dir, char state);
  int (*Dir_GetState)(wchar_t * dir, char * state);
  LIST * dir_list;
  wchar_t * dir;
  int dir_count;
  int mem;
  GUI_LIST * gui_dirlist;
  GUI_LIST * gui_memory;
  GUI_LIST * gui_dirsettings;
  GUI * gui_input;
  GUI_LIST * gui_icontype;
  GUI_ONEOFMANY * gui_actionstate;
}DEBOOK;

extern DIRECTORY_METHODS * methods;

enum
{
  DIRECTORY = 0,
  DIR_NAME,
  SMALL_ICON,
  BIG_ICON,
  DELETE_STATE,
  RENAME_STATE,
  S_COUNT
};

extern wchar_t * ItemName[S_COUNT];
extern wchar_t * StateName[3];

extern wchar_t ** extlist;
extern LIST * extdata;

int CreateEditor();
wchar_t * MakeFullName(wchar_t * path, wchar_t * name);
void elf_exit(void);
wchar_t * GetUserPath(int mem);

extern const PAGE_DESC DirEditor_DirList_Page;
extern const PAGE_DESC DirEditor_DirSettings_Page;
extern const PAGE_DESC DirEditor_DirSettings_SelectIconType_Page;
extern const PAGE_DESC DirEditor_DirSettings_SelectElf_Page;
extern const PAGE_DESC DirEditor_DirSettings_EditName_Page;
extern const PAGE_DESC DirEditor_DirAdd_SelectDir_Page;
extern const PAGE_DESC DirEditor_DirSettings_SetActionState_Page;
extern const PAGE_DESC DirEditor_DirSettings_EditName_Page;
