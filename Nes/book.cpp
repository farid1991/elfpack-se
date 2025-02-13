#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "book.h"
#include "gui.h"

#include "nes.h"

#include "conf_loader.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

MyBOOK *MyBook;

int TerminateElf(void * ,BOOK* book)
{
  needstop = 1;
  return 1;
}

typedef struct
{
  BOOK * book;
}MSG;

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(0x6fFFFFFF, STR("� 2009 Tartes") ,0, 1 ,5000,msg->book);
  return 1;
}

int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA * reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    result=1;
  }
  return result;
}

typedef struct
{
  BOOK* book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

int onBcfgConfig(void *mess, BOOK *book)
{
  MSG_BCFG *msg = (MSG_BCFG *)mess;
  wchar_t *tmp = new wchar_t[256];
  wstrcpy(tmp,msg->bcfg_p);
  wstrcat(tmp,L"/");
  wstrcat(tmp,msg->bcfg_n);
  elfload(tmp,(void *)successed_config_path,(void *)successed_config_name,0);
  return(1);
}

const PAGE_MSG Base_PE[] @ "DYN_PAGE" = {
  ELF_TERMINATE_EVENT  , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  ELF_RECONFIG_EVENT   , ReconfigElf,
  ELF_BCFG_CONFIG_EVENT, onBcfgConfig,
  NIL_EVENT_TAG        , NULL
};


int MP_Enter(void *mess ,BOOK* book)
{
  if( !GUI_Create() )
  {
    FreeBook(MyBook);
    return 0;
  }
  return 1;
}


int MP_Exit(void *mess ,BOOK* book)
{
  GUI_Destroy();
  return 1;
}


const PAGE_MSG Main_PE[] @ "DYN_PAGE" = {
  PAGE_ENTER_EVENT_TAG, MP_Enter,
  PAGE_EXIT_EVENT_TAG , MP_Exit,
  NIL_EVENT_TAG       , NULL
};

PAGE_DESC base_page = {"GUI_BasePage",0 , Base_PE};
PAGE_DESC main_page = {"GUI_MainPage",0 , Main_PE};


void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


void onCloseMyBook(BOOK * book)
{
  if (book)
  {
    SUBPROC(elf_exit);
  }
}

BOOK * Book_Create()
{
  MyBook = new MyBOOK;

  CreateBook( MyBook, onCloseMyBook, &base_page, "GUI_Test", -1, 0 );

  return MyBook;
}

void Book_GoMainPage()
{
  BookObj_GotoPage( MyBook, &main_page );
}
