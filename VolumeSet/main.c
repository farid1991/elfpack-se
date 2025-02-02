#include "..\deleaker\mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"
#include "main.h"
#include "IAudioControlManager.h"

void SetVolume()
{
  IAudioControlManager* pIAudioControlManager = NULL;
  IAudioControl* pIAudioControl = NULL;
  
  CoCreateInstance(&CID_CAudioControlManager, &IID_IAudioControlManager, PPINTERFACE(&pIAudioControlManager));
  if (pIAudioControlManager)
  {
    pIAudioControlManager->AudioControl(&pIAudioControl);
    if (pIAudioControl)
    {
      char vol;
      
      if (Mode == 0)
      {
        FSTAT _fstat;
        if (fstat(GetDir(DIR_ELFS_CONFIG), L"volume.bin", &_fstat) == 0)
        {
          int file = _fopen(GetDir(DIR_ELFS_CONFIG), L"volume.bin", FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0);
          fread(file, &vol, 1);
          fclose(file);
        }
      }
      else if (Mode == 1) vol = Volume + 18;
      
      pIAudioControl->Volume_Set(AUDIOCONTROL_VOLUMETYPE_MEDIAVOLUME, vol);
      pIAudioControl->Release();
    }
    pIAudioControlManager->Release();
  }
}

int FindBookExCB_Book(BOOK* book , int* param)
{
  return book == (BOOK*)param;
}

int PAGE_ENTER_EVENT_PROC(void* msg, BOOK* book, PAGE_DESC* page_desc, LPARAM ClientData, u16 event)
{
  bool exists = NULL != FindBookEx(FindBookExCB_Book, (int*)book);
  
  if (exists)
  {
    if (!strcmp(book->xbook->name, "VolumeControlBook"))
    {
      char vol;
      GetAudioSettings(3, &vol);
      
      int file = _fopen(GetDir(DIR_ELFS_CONFIG), L"volume.bin", FSX_O_WRONLY|FSX_O_TRUNC, FSX_S_IRUSR|FSX_S_IWUSR, 0);
      fwrite(file, &vol, 1);
      fclose(file);
    }
  }
  return 0;
}

int ReconfigElf(void* data, BOOK* book)
{
  RECONFIG_EVENT_DATA* reconf = (RECONFIG_EVENT_DATA*)data;
  if (!wstrcmpi(reconf->path, successed_config_path) && !wstrcmpi(reconf->name, successed_config_name))
  {
    InitConfig();
    return 1;
  }
  return 0;
}

int onBcfgConfig(void* data, BOOK* book)
{
  MSG_BCFG* msg = (MSG_BCFG*)data;
  wchar_t tmp[255];
  wstrcpy(tmp, msg->bcfg_p);
  wstrcat(tmp, L"/");
  wstrcat(tmp, msg->bcfg_n);
  elfload(tmp, (void*)successed_config_path, (void*)successed_config_name, 0);
  return 1;
}

int TerminateElf(void* data, BOOK* book)
{
  FreeBook(book);
  return 1;
}

int ShowAuthorInfo(void* data, BOOK* book)
{
  MSG* msg = (MSG*)data;
  MessageBox(EMPTY_TEXTID, STR(ABOUT_TEXT), NOIMAGE, 1, 5000, msg->book);
  return 1;
}

const PAGE_MSG VolumeSet_Base_evlist[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, TerminateElf,
  ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
  ELF_RECONFIG_EVENT, ReconfigElf,
  ELF_BCFG_CONFIG_EVENT, onBcfgConfig,
  NIL_EVENT_TAG, NULL
};

PAGE_DESC VolumeSet_Base_Page = {"VolumeSet_Base_Page", 0, VolumeSet_Base_evlist};

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void VolumeSetBook_onClose(BOOK* book)
{
  ModifyUIPageHook(PAGE_ENTER_EVENT, PAGE_ENTER_EVENT_PROC, NULL, PAGE_HOOK_REMOVE);
  SUBPROC(elf_exit);
}

BOOK* CreateVolumeSetBook()
{
  BOOK* book = new BOOK;
  CreateBook(book, VolumeSetBook_onClose, &VolumeSet_Base_Page, BOOK_NAME, -1, 0);
  return book;
}

int IsVolumeSetBook(BOOK* book)
{
  return 0 == strcmp(book->xbook->name, BOOK_NAME);
}

int main (void)
{
  BOOK* book = FindBook(IsVolumeSetBook);
  
  if (book)
  {
    UI_Event_toBookID(ELF_TERMINATE_EVENT, BookObj_GetBookID(book));
    MessageBox(EMPTY_TEXTID, STR("Ёльф выгружен!"), NOIMAGE, 1, 5000, 0);
  }
  else
  {
    trace_init(L"VolumeSet.txt");
    
    InitConfig();
    CreateVolumeSetBook();
    SetVolume();
    ModifyUIPageHook(PAGE_ENTER_EVENT, PAGE_ENTER_EVENT_PROC, NULL, PAGE_HOOK_ADD_AFTER);
    
    return 0;
  }
}
