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
  int unk;
  wchar_t *** fname;
}_INDATA;

/*
enum
{
  ICON_INTERNAL = 0,
  ICON_EXTERNAL
};*/
#define ICON_INTERNAL 0
#define ICON_EXTERNAL 1

typedef struct
{
  EP_DATA * (*getepd)(void);
  int (*ON_CMD_RUN)(struct SUB_EXECUTE*);
  int (*ON_CMD_SETSMALLICON)(struct SUB_EXECUTE*, wchar_t * iconid);
  int (*ON_CMD_SETTHUMBNAILICON)(struct SUB_EXECUTE*, wchar_t * iconid);
  int (*ON_CMD_SETSAVEDICON)(struct SUB_EXECUTE*, wchar_t * iconid);
}DBEXTDATA;

typedef struct
{
  int (*Ext_Exist)(wchar_t * ext);
  int (*ExtMemExist)();
  int (*GetExtList)(wchar_t *** ext_list);
  int (*GetFullList)(wchar_t *** ext_list);
  int (*Ext_IsFile)(wchar_t * ext);
  int (*Ext_Add)(wchar_t * ext);
  int (*Ext_Rename)(wchar_t * ext, wchar_t * ext_new);
  int (*Ext_Remove)(wchar_t * ext);
  int (*Ext_SetElfOpen)(wchar_t * ext, wchar_t * elf);
  int (*Ext_GetElfOpen)(wchar_t * ext, wchar_t ** elf);
  int (*Ext_SetElfEdit)(wchar_t * ext, wchar_t * elf);
  int (*Ext_GetElfEdit)(wchar_t * ext, wchar_t ** elf);
  int (*Ext_SetSmallIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetSmallIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetSmallIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetBigIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetBigIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetBigIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetSavedIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetSavedIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetSavedIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetPathInt)(wchar_t * ext, wchar_t * path);
  int (*Ext_GetPathInt)(wchar_t * ext, wchar_t ** path);
  int (*Ext_SetPathExt)(wchar_t * ext, wchar_t * path);
  int (*Ext_GetPathExt)(wchar_t * ext, wchar_t ** path);
}EXTENSION_METHODS;

typedef struct
{
  BOOK book;
  EXTENSION_METHODS * extension_methods;
  int (*Ext_SetElf)(wchar_t * ext, wchar_t * elf);
  int (*Ext_SetIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetPath)(wchar_t * ext, wchar_t * path);
  int (*Ext_GetPath)(wchar_t * ext, wchar_t ** path);
  int path;
  wchar_t ** ext_list;
  wchar_t * ext;
  int ext_count;
  GUI_LIST * extlist;
  GUI_LIST * extension;
  GUI * input;
  GUI_LIST * icontype;
}EEBOOK;

extern EXTENSION_METHODS * methods;

enum
{
  EXTENSION = 0,
  SMALL_ICON,
  BIG_ICON,
  SAVED_ICON,
  OPEN_ELF,
  EDIT_ELF,
  PATH_INT,
  PATH_EXT,
  S_COUNT
};

extern wchar_t * ItemName[S_COUNT];

extern wchar_t ** extlist;
extern LIST * extdata;

int CreateEditor();
wchar_t * MakeFullName(wchar_t * path, wchar_t * name);
void elf_exit(void);
wchar_t * GetUserPath(int mem);

extern const PAGE_DESC ExtEditor_ExtList_Page;
extern const PAGE_DESC ExtEditor_ExtSettings_Page;
extern const PAGE_DESC ExtEditor_ExtSettings_SelectIconType_Page;
extern const PAGE_DESC ExtEditor_ExtSettings_SelectElf_Page;
extern const PAGE_DESC ExtEditor_ExtSettings_EditName_Page;
extern const PAGE_DESC ExtEditor_ExtAdd_SelectType_Page;
extern const PAGE_DESC ExtEditor_SelectFile_Page;
extern const PAGE_DESC ExtEditor_ExtAdd_Page;
extern const PAGE_DESC ExtEditor_ExtSettings_SelectFolder_Page;
