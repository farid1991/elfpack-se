#ifndef _MAIN_H_
#define _MAIN_H_

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\var_arg.h"

#define BOOKNAME "DynEditorBook"
#define ELFNAME "DynEditor"
#define VERSION "Ver 2.3"
#define AUTHOR "(c) Joker XT"

#define UNDEFINED "0xFFFFFFFF"

#define CONFIG_INTERNAL GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL)
#define DYN_CONST L"DYN_CONST.bin"
#define backup_path L"/tpa/dyn_const"
#define backup_name L"_DYN_CONST.bin"

#define NUM_TEXT ENUM_LNG_TXTID_LAST

#define GUI_DESTROY(a) if(a){a=GUIObject_Destroy(a);};
//#define LIST_DESTROY(b) if(b){List_Destroy(b); b=0;}
#define FREE(c) if(c){mfree(c); c=0;};
#define FILE_CLOSE(d) if(d){fclose(d); d=0;};
#define TEXTID_DESTROY(e) if(e!=EMPTY_TEXTID){TextID_Destroy(e); e=EMPTY_TEXTID;};

#define FILE_OPEN_RW(x,r0,r1) x = _fopen(r0,r1,0x004,0x180,0);
#define FILE_CREATE(x,r0,r1) x = _fopen(r0,r1,0x002,0x180,0);

enum
{
  NOT_FREE_ELEMENTS = 0,
  FREE_ELEMENTS
}LIST_DESTROY_MODE;

void LIST_DESTROY(LIST **lst, int mode)
{
  if(*lst)
  {
    if((*lst)->FirstFree && mode)
    {
      for(int i=0;i<((*lst)->FirstFree);i++) mfree(List_Get(*lst,i));
    }
    List_Destroy(*lst);
    *lst = NULL;
  }
}

typedef struct
{
  BOOK book;
  GUI *menu; //GUI_LIST *VD_list;
  GUI *secGUI; //GUI *guiYN;
  int flag_lg;
  //view dyn
  LIST *phone_sw_h;
  LIST *names_of_constants;
  int offsets;
  int menu_item;
  //dyn
  int dyn_file;
  int dyn_size;
  //constants
  int constant;
}TDynEditorBook;

enum
{
  CREATE = 0,
  REPLACMENT
}FILE_BACKUP_MODE;

enum
{
  RUS_LNG = 0,
  OTHER_LNG
}LANG_TYPE;

enum
{
  ENTER_CONSTANT,
  ENTER_OFFSET,
  CHANGE_CONSTANT,
  GO_TO_OFFSET,
  GO_TO_END,
  _LANGUAGE,
  ABOUT_ELF,
  Q_REPLACE_CONSTANT,
  //--menu-----
  ADD_CHANGE_CONSTANT,
  VIEWING_DYN,
  REST_DYN,
  SAVE_DYN,
  COLLECT_DYN,
  INFO,
  //----------
  EXIT,
  DYN_CONST_BACKUP_OK,
  DOES_NOT_EXIST,
  ENTER_WITH_0x,
  Q_CHANGE_CONSTANT,
  DYN_CONST_CREATED,
  NO_DYN_TAG_H,
  DYN_IS_RESTORED,
  NO_FILES,
  NO_FOLDER_DYN_EDITOR,
  INACCESSIBLE,
  ILLEGAL_CHARACTERS,
  NAMES,
  OFFSETS,
  NEW_CONSTANT,
  DELETE_CONSTANT,
  ENUM_LNG_TXTID_LAST
}enum_lng_textid;

int isDynEditorBook(BOOK *book);


int CreateMenuGUI_MainPage(void *mess, BOOK *book);
int onAbout(void *mess, BOOK *book);
int onExit(void *mess, BOOK *book);

int wstr2h(wchar_t *ws, int len);
int str2h(char *str, int len);
int isRu();
int IllegalSymbol(wchar_t *wstr, int len);
int TextmFree(int *txt);
void Regrus(BOOK *book);
void Regeng(BOOK *book);

int MSG_Box_lg(wchar_t *msg_ru, wchar_t *msg_en);


int ViewingDyn_PageEnter(void *, BOOK *book);
int ViewingDyn_PageExit(void *, BOOK *book);
void ViewingDyn_EditConstantAction(BOOK *book, GUI *gui);

void StringInput_EnterOffsetAction(BOOK *book, wchar_t *txt, int len);
void StringInput_AddConstAction(BOOK *book, wchar_t *txt, int len);
int DynEditor_GetNamesOfConstants(TDynEditorBook *DynEditorBook);

void ViewingDyn_EditConstantAction(BOOK *book, GUI *gui);
void VD_QuestionReplace_YesPressAction(BOOK *book, GUI *);


const PAGE_MSG ViewingDyn_MainPage_evlist[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,     ViewingDyn_PageEnter,
  //ACTION_DELETE_TAG,        ViewingDyn_PageEnter,
  //PAGE_EXIT_EVENT_TAG,      ViewingDyn_PageExit,
  NIL_EVENT_TAG,            NULL
};

const PAGE_DESC ViewingDyn_MainPage = {"ViewingDyn_MainPage",0,ViewingDyn_MainPage_evlist};

#endif
