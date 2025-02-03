#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"

#define FREE_GUI(a) if(a) a=GUIObject_Destroy(a)

#define ABOUT L"GRar\nVer 1.1\n\n(c) Joker XT\n&\nSlawwan"

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

struct LIST_ELEM;

typedef struct
{
  wchar_t *ext;
  wchar_t IconID;
}EXT_AND_IC;

typedef struct
{
  int header_type;
  int flags;
  int header_size;
  int compressed_size;
  int real_size;
  int version;
  int method;
  int name_size;
  int attributes;
  wchar_t * name;
}RAR_INFO_HEADER;

typedef struct CONTENT_ITEM
{
  int type;
  LIST *list;
  struct CONTENT_ITEM *parrent;
  wchar_t *name;
  int compressed_size;
  int real_size;
  int pos;
}_CONTENT_ITEM;

struct GRBOOK : public BOOK
{
  CONTENT_ITEM *root_item;
  CONTENT_ITEM *item;
  GUI_LIST *gui_content;
  GUI *gui_extract_in;
  int focused_item;
  wchar_t *arch_path;
  wchar_t *arch_name;
  int file;
  LIST *ext_list;
  wchar_t *folder_extract_in;
};

extern const PAGE_DESC GRar_ContentList_Page;
void bi8t2unicode(wchar_t *ws, char *s, int len);
void elf_exit(void);
//void UnpackAll(BOOK *book, void *);
int ExtractOne(int file, int file_pos, wchar_t * new_name);
//int ExtractAll(wchar_t * path, wchar_t * name, wchar_t * dir);
int RAR_ReadMainHeader(int file);
int RAR_DestroyInfoHeader(RAR_INFO_HEADER * header);
RAR_INFO_HEADER * RAR_ReadInfoHeader(int file, int file_pos);

int Item_GetCompressedSize(CONTENT_ITEM *content_item);
int Item_GetRealSize(CONTENT_ITEM *content_item);
int Item_GetFoldersCount(CONTENT_ITEM *content_item);
int Item_GetFilesCount(CONTENT_ITEM *content_item);
void Item_Extract(CONTENT_ITEM *content_item, wchar_t * path, int a_file);

extern const PAGE_DESC GRar_ExtractIn_SelectFolder_Page;
