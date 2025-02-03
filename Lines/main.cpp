#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "cfg_items.h"
#include "main.h"
#include "gui.h"
#include "menu.h"
#include "game.h"
#include "random.h"
#include "resource.h"
#include "log.h"
#include "util.h"

#include "conf_loader.h"
#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

#define ELF_BCFG_CONFIG_EVENT 994

typedef struct
{
  BOOK* book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

typedef struct
{
  BOOK * bookmanbook;
}BOOKMANMSG;

MyBOOK * LinesBook;
static char myappname[]=ELFNAME;

extern wchar_t snd_path[256];

static void MainInit(void)
{
  InitConfig();
  getdir(snd_path,cfg_snd_path);
}

static void YSYes(BOOK * book, void *)
{
  MyBOOK *bk=(MyBOOK *)book;
  bk->YesNoFunc(book,1);
  FREE_GUI(bk->yes_no);
}

static void YSNo (BOOK * book, void *)
{
  MyBOOK *bk=(MyBOOK *)book;
  bk->YesNoFunc(book,0);
  FREE_GUI(bk->yes_no);
}

void MsgBoxYesNo(BOOK* book, wchar_t *qv, void(*f)(BOOK*,int))
{
  TEXTID q=TextID_Create(qv,ENC_UCS2,TEXTID_ANY_LEN);
  MyBOOK * bk = (MyBOOK *)book;
  bk->YesNoFunc=f;
  bk->yes_no=CreateYesNoQuestionVA(0,
                                   VAR_BOOK(bk),
                                   VAR_YESNO_QUESTION(q),
                                   VAR_YESNO_YES_ACTION(YSYes),
                                   VAR_YESNO_NO_ACTION(YSNo),
                                   VAR_PREV_ACTION_PROC(YSNo),
                                   0);
}

int TerminateElf(void * ,BOOK* book)
{
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  BOOKMANMSG * msg = (BOOKMANMSG*)mess;
  doAbout(msg->bookmanbook);
  return(1);
}

int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    MainInit();
    result=1;
  }
  return(result);
}

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

const PAGE_MSG Lines_BasePageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT,   TerminateElf,
  ELF_SHOW_INFO_EVENT,   ShowAuthorInfo,
  ELF_RECONFIG_EVENT,    ReconfigElf,
  ELF_BCFG_CONFIG_EVENT, onBcfgConfig,
  NIL_EVENT_TAG,         NULL
};

const PAGE_DESC base_page ={"Lines_BasePage",0,Lines_BasePageEvents};

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int isLinesBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,myappname)) return 1;
  return 0;
}

void GetImages()
{
  char *buf = new char[IDB_LOGO_len];
  if(buf)
  {
    memcpy(buf,IDB_LOGO,IDB_LOGO_len);
    if(ImageID_GetIndirect(buf, IDB_LOGO_len, 0, L"png", &LinesBook->logo) < 0)
    {
      LinesBook->logo = 0xFFFF;
    }
  }
  else LinesBook->logo = 0xFFFF;

  char *buf2 = new char[IDB_CUR_len];
  if(buf2)
  {
    memcpy(buf2,IDB_CUR,IDB_CUR_len);
    if(ImageID_GetIndirect(buf2, IDB_CUR_len, 0, L"png", &LinesBook->cursor) < 0)
    {
      LinesBook->cursor = 0xFFFF;
    }
  }
  else LinesBook->cursor = 0xFFFF;
}

void FreeImages()
{
  ImageID_Free(LinesBook->logo);
  ImageID_Free(LinesBook->cursor);
}

void onCloseLinesBook(BOOK * book)
{
  MyBOOK *mbk = (MyBOOK *)book;
//  FreeGame();
  FREE_GUI(mbk->game_gui);
  FREE_GUI(mbk->text_input);
  FREE_GUI(mbk->menu_gui);
  FREE_GUI(mbk->yes_no);
  FreeImages();
  FreeResources();
  WriteGame();
  WriteInfo();
  SUBPROC(elf_exit);
}

int main(wchar_t *elfname)
{
  trace_init();

  BOOK *bk = 0;
  if(bk = FindBook(isLinesBook))
  {
    BookObj_SetFocus(bk,0);
    SUBPROC(elf_exit);
  }
  else
  {
    LinesBook = new MyBOOK;
    memset(LinesBook,0,sizeof(MyBOOK));
    if(!CreateBook((BOOK *)LinesBook,onCloseLinesBook,&base_page,myappname,-1,0))
    {
      delete LinesBook;
      SUBPROC(elf_exit);
      return 0;
    }

    wchar_t *name = wstrrchr(elfname,0x2F);;
    if (name)
    {
      int len = name - elfname;
      memcpy(LinesBook->path,elfname,len*sizeof(wchar_t));
      LinesBook->path[len]=0;
    }
    StartLog(LinesBook->path);
    WriteLog("Start");
    randomize();
    MainInit();
    InitScreen();
    GetImages();
    LoadResources();
    GetGame();
    GetInfo();

    // ???
    BookObj_GotoPage((BOOK *)LinesBook,&menu_page);
  }
  return 0;
}

/*
Revision history:
  3.0
      + Добавлены звуки
      + Исправлена ошибка из-за которой если в момент, когда пятый шарик
        становится в линию, вернутся в меню, а потом зайти обратно, то линия
        не убирается
  2.2
      + Добавлен украинский язык
  2.1
      + Исправлена небольшая ошибка
      + Теперь эльф только на русском
  2.0
      + Возможность выбрать свои ресурсы (будут применены после перезапуска)
      + при активной игре, из меню по кнопке "назад" возврат в игру, а не выход
      + оптимизация
  1.0
      + Начальная версия
*/

