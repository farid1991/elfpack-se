#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

#define DISPLAY                 0
#define BCFG_FULLPATH_MAXLEN    255
#define ELF_BCFG_CONFIG_EVENT   994
#define ELFNAME                 "CallKeylock"
#define ABOUT                   "Блокировка клавиатуры при звонке\n\n(c) Joker XT\n2011"

bool mod_kh;

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

static int TerminateElf(void *msg, BOOK *book)
{
  FreeBook(book);
  return(1);
}

static int ShowAuthorInfo(void *msg, BOOK *book)
{
  MessageBox(EMPTY_TEXTID, STR(ABOUT), NOIMAGE, 1, 0, NULL);
  return(1);
}

static void onMyBookClose(BOOK *book)
{
  SUBPROC(elf_exit);
}

GUI *BookObj_GetGUI(BOOK *book, int num)
{
  return((GUI *)List_Get(book->xguilist->guilist, num));
}

void Keylock_Activate(BOOK *book, GUI *gui)
{
  UI_Event(UI_KEYLOCK_ACTIVATE_EVENT);
}

void Keylock_Deactivate(BOOK *book, GUI *gui)
{
  UI_Event(SLIDER_OPEN_EVENT);
}

int keyhook(int key, int repeat_count, int mode, LPARAM lparam, DISP_OBJ* disp)
{
  if(isKeylocked())
  {
    if((key == KEY_ESC && lock_key_call_end) || (key == KEY_TASK && lock_key_task))
      return(BLOCK_EVENT_IN_THIS_SESSION);
  }
  return(0);
}

enum
{
        CALLMANAGER_IDLE            =0,
        CALLMANAGER_CALL_CONNECTED  =1,
        CALLMANAGER_CALL_SETUP      =2,
        CALLMANAGER_CALL_ALERT      =4,
        CALLMANAGER_CALL_WAITING    =5,
        CALLMANAGER_CALL_TERMINATED =6
};

static int OnCallManager(void *msg, BOOK *book)
{
  int eventdata;
  switch(GetChipID()&CHIPID_MASK)
  {
  case CHIPID_DB2000:
  case CHIPID_DB2010:
  case CHIPID_DB2020:
      eventdata = (((CALLMANAGER_EVENT_DATA *)msg)->CallState);
      break;
  
  case CHIPID_DB3150:
  case CHIPID_DB3200:
  case CHIPID_DB3210:
  case CHIPID_DB3350:
      eventdata = (((CALLMANAGER_EVENT_DATA_A2 *)msg)->CallState);
      break;
  }
  
  switch(eventdata)
  {
  case CALLMANAGER_CALL_CONNECTED:
      book = Display_GetTopBook(DISPLAY);
      BookObj_SoftKeys_SetAction(book, 0, Keylock_Activate);
      BookObj_SoftKeys_SetText(book, 0, STR("Заблок. кл."));
      if(auto_keylock) Keylock_Activate(NULL, NULL);
      ModifyKeyHook(keyhook, KEY_HOOK_ADD, NULL);
      mod_kh = true;
      break;
      
  case CALLMANAGER_CALL_TERMINATED:
      if(mod_kh)
      {
          if(isKeylocked()) Keylock_Deactivate(NULL, NULL);
          ModifyKeyHook(keyhook, KEY_HOOK_REMOVE, NULL);
          mod_kh = false;
      }
      break;
  }
  return(1);
}

static int ReconfigElf(void *msg ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf = (RECONFIG_EVENT_DATA *)msg;
  int result=0;
  if(!wstrcmpi(reconf->path, successed_config_path) && !wstrcmpi(reconf->name, successed_config_name))
  {
    InitConfig();
    result=1;
  }
  return(result);
}

typedef struct
{
  BOOK *book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

static int BcfgConfig(void *msg, BOOK *book)
{
  MSG_BCFG *data = (MSG_BCFG *)msg;
  wchar_t bcfg[BCFG_FULLPATH_MAXLEN+1];
  memset(bcfg, 0, (BCFG_FULLPATH_MAXLEN+1)*sizeof(wchar_t));
  wstrcpy(bcfg, data->bcfg_p);
  wstrcat(bcfg, L"/");
  wstrcat(bcfg, data->bcfg_n);
  elfload(bcfg, (void *)successed_config_path, (void *)successed_config_name, NULL);
  return(1);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,       TerminateElf,
  ELF_SHOW_INFO_EVENT,       ShowAuthorInfo,
  ELF_RECONFIG_EVENT,        ReconfigElf,
  ELF_BCFG_CONFIG_EVENT,     BcfgConfig,
  ON_CALLMANAGER_EVENT_TAG,  OnCallManager,
  NIL_EVENT_TAG,             NULL
};

const PAGE_DESC bk_base = {"CallKeylock_Base_Page", 0, bk_msglst_base};

int isCallKeylockBook(BOOK *book)
{
  if(!strcmp(book->xbook->name, ELFNAME )) return(true);
  return(false);
}

int main(void)
{
  if(FindBook(isCallKeylockBook))    
  {
    MessageBox(EMPTY_TEXTID, STR("CallKeylock already running"), 0, 1, 2000, 0);
    SUBPROC(elf_exit);
  }
  else
  {
    mod_kh = false;
    BOOK *book = new BOOK;
    memset(book, 0, sizeof(BOOK));
    if(!CreateBook(book, onMyBookClose, &bk_base, ELFNAME, -1, 0))
    {
      mfree(book);
      SUBPROC(elf_exit);
    }
    else InitConfig();
  }
  return(0);
}
