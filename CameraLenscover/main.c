#include "..\deleaker\mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"
#include "main.h"
#include "util.h"
#include "IRegistryManager.h"

void LenscoverMenu_OnClose(BOOK* book, GUI* gui)
{
  CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)book;
  
  if (cl_bk->gui)
  {
    GUIObject_Destroy(cl_bk->gui);
    cl_bk->gui = NULL;
  }
  
  BookObj_Hide(cl_bk, 0);
  SetLampLevel(cl_bk->led = 0);
  
  if (cl_bk->led_timer)
  {
    Timer_Kill(&cl_bk->led_timer);
    cl_bk->led_timer = NULL;
  }
}

void onTimer(u16 timerID, LPARAM param)
{
  CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)param;
  SetLampLevel(cl_bk->led = 0);
  Timer_Kill(&cl_bk->led_timer);
  if (cl_bk->gui) ListMenu_SetItemSecondLineText(cl_bk->gui, 2, STR("Включить фонарь"));
}

void RunCamera(CameraLenscoverBook* cl_bk, int start)
{
  IRegistryManager* pIRegistryManager = NULL;
  IRegistry* pIRegistry = NULL;
  IRegistryKey* pIRegistryKey = NULL;
  
  CoCreateInstance(&CID_CRegistryManager, &IID_IRegistryManager, PPINTERFACE(&pIRegistryManager));
  if (pIRegistryManager)
  {
    pIRegistryManager->CreateRegistry(&pIRegistry);
    if (pIRegistry)
    {
      pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
      pIRegistry->OpenKey("/gdfs", &pIRegistryKey);
      if (pIRegistryKey)
      {
        CAMERA_SETTINGS cs[24];
        pIRegistryKey->GetValue("GD_MMI_Camera_Settings", Reg_RegValueType_Binary, 24, cs, NULL);
        
        cs->start = start;
        
        pIRegistryKey->SetValue("GD_MMI_Camera_Settings", Reg_RegValueType_Binary, 24, cs);
      }
      pIRegistry->EndTransaction(TRUE);
    }
    if (pIRegistryKey) pIRegistryKey->Release();
    if (pIRegistry) pIRegistry->Release();
  }
  
  Shortcut_Run(L"Camera");
  LenscoverMenu_OnClose(cl_bk, cl_bk->gui);
}

void RunLed(CameraLenscoverBook* cl_bk)
{
  SetLampLevel(cl_bk->led ? cl_bk->led = 0 : cl_bk->led = 1);
  
  if (AutoLEDOff)
  {
    if (cl_bk->led_timer) Timer_Kill(&cl_bk->led_timer);
    cl_bk->led_timer = Timer_Set(AutoLEDOffTime * 1000, onTimer, cl_bk);
  }
}

void LenscoverMenu_OnSelect(BOOK* book, GUI* gui)
{
  CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)book;
  
  int item = ListMenu_GetSelectedItem(cl_bk->gui);
  if (item == 0) RunCamera(cl_bk, START_PHOTO);
  else if (item == 1) RunCamera(cl_bk, START_VIDEO);
  else if (item == 2)
  {
    RunLed(cl_bk);
    ListMenu_SetItemSecondLineText(gui, 2, cl_bk->led ? STR("Выкл. фонарь") : STR("Вкл. фонарь"));
  }
}

void LenscoverMenu_OnTurn(BOOK* book, GUI* gui)
{
  CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)book;
  
  if (cl_bk->gui)
  {
    GUIObject_Destroy(cl_bk->gui);
    cl_bk->gui = NULL;
  }
  
  BookObj_Hide(cl_bk, 0);
}

int LenscoverMenu_OnMessage(GUI_MESSAGE* msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int item = GUIonMessage_GetCreatedItemIndex(msg);
    if (item == 0)
    {
      GUIonMessage_SetMenuItemText(msg, STR("Фотокамера"));
      GUIonMessage_SetMenuItemSecondLineText(msg, STR("Запустить"));
      int IDIconCamera;
      iconidname2id(L"RN_SHORTCUT_LIST_CAMERA_CS_ICN", -1, &IDIconCamera);
      GUIonMessage_SetMenuItemIcon(msg, 0, IDIconCamera);
    }
    else if (item == 1)
    {
      GUIonMessage_SetMenuItemText(msg, STR("Видеокамера"));
      GUIonMessage_SetMenuItemSecondLineText(msg, STR("Запустить"));
      int IDIconVideo;
      iconidname2id(L"RN_SHORTCUT_LIST_CAMERA_ICN", -1, &IDIconVideo);
      GUIonMessage_SetMenuItemIcon(msg, 0, IDIconVideo);
    }
    else if (item == 2)
    {
      CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)GUIonMessage_GetBook(msg);
      GUIonMessage_SetMenuItemText(msg, STR("Фонарь"));
      GUIonMessage_SetMenuItemSecondLineText(msg, cl_bk->led ? STR("Выкл. фонарь") : STR("Вкл. фонарь"));
      
      int IDIconLED;
      iconidname2id(L"FLASH_LIGHT_MENU_ICN", -1, &IDIconLED);
      GUIonMessage_SetMenuItemIcon(msg, 0, IDIconLED);
    }
  }
  return 1;
}

void CreateLenscoverMenu(CameraLenscoverBook* cl_bk)
{
  cl_bk->gui = CreateListMenu(cl_bk, 0);
  GUIObject_SetTitleText(cl_bk->gui, STR("Выбрать действие"));
  ListMenu_SetItemCount(cl_bk->gui, 3);
  ListMenu_SetOnMessage(cl_bk->gui, LenscoverMenu_OnMessage);
  ListMenu_SetCursorToItem(cl_bk->gui, Focus);
  ListMenu_SetHotkeyMode(cl_bk->gui, LKHM_SHORTCUT); 
  
  int is;
  if (ItemStyle == 0) is = 1;
  else if (ItemStyle == 1) is = 3;
  else if (ItemStyle == 2) is = 0;
  ListMenu_SetItemStyle(cl_bk->gui, is);
  
  GUIObject_SetStyle(cl_bk->gui, UI_OverlayStyle_Default);
  
  if (Animation) DispObject_SetAnimation(GUIObject_GetDispObject(cl_bk->gui), 0x60021000);
  
  GUIObject_SoftKeys_SetAction(cl_bk->gui, ACTION_BACK, LenscoverMenu_OnClose);
  GUIObject_SoftKeys_SetAction(cl_bk->gui, ACTION_LONG_BACK, LenscoverMenu_OnClose);
  GUIObject_SoftKeys_SetAction(cl_bk->gui, ACTION_SELECT1, LenscoverMenu_OnSelect);
  GUIObject_SoftKeys_SetAction(cl_bk->gui, ACTION_YES, LenscoverMenu_OnTurn);
  GUIObject_SoftKeys_SetText(cl_bk->gui, ACTION_YES, STR("Сверн."));
  
  BookObj_Show(cl_bk, 0);
  GUIObject_Show(cl_bk->gui);
}

int IsCameraLenscoverBook(BOOK* book)
{
  return 0 == strcmp(book->xbook->name, BOOK_NAME);
}

int NewKey(int key, int unk, int mode, LPARAM, DISP_OBJ*)
{
  if (!IsCameraBook(Display_GetTopBook(0)))
  {
    if (mode == KeyStartPressMode)
    {
      CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)FindBook(IsCameraLenscoverBook);
      
      if (key == KeyStart)
      {
        if (Start == 0) CreateLenscoverMenu(cl_bk);
        else if (Start == 1) RunCamera(cl_bk, START_PHOTO);
        else if (Start == 2) RunCamera(cl_bk, START_VIDEO);
        else if (Start == 3) RunLed(cl_bk);
        
        if ((!GetSilent() || (Silent)) && (Sound))
        {
          char vol = Volume + 18;
          extractdir(cl_bk->FilePath, cl_bk->FileName, SoundOpen);
          PlayFileV(cl_bk->FilePath, cl_bk->FileName, vol);
        }
        return -1;
      }
      
      if (key == KEY_LENS_COVER_CLOSE)
      {
        LenscoverMenu_OnClose(cl_bk, 0);
        
        if ((!GetSilent() || (Silent)) && (Sound))
        {
          char vol = Volume + 18;
          extractdir(cl_bk->FilePath, cl_bk->FileName, SoundClose);
          PlayFileV(cl_bk->FilePath, cl_bk->FileName, vol);
        }
        return -1;
      }
    }
  }
  return 0;
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

int onUserInactivity(void* data, BOOK* book)
{
  CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)book;
  if (UserInactivity & !cl_bk->led) LenscoverMenu_OnClose(cl_bk, 0);
  return 0;
}

const PAGE_MSG CameraLenscover_Base_evlist[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT, TerminateElf,
  ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
  ELF_RECONFIG_EVENT, ReconfigElf,
  ELF_BCFG_CONFIG_EVENT, onBcfgConfig,
  USER_INACTIVITY_EVENT_TAG, onUserInactivity,
  NIL_EVENT_TAG, NULL
};

PAGE_DESC CameraLenscover_Base_Page = {"CameraLenscover_Base_Page", NULL, CameraLenscover_Base_evlist};

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void CameraLenscoverBook_onClose(BOOK* book)
{
  CameraLenscoverBook* cl_bk = (CameraLenscoverBook*)book;
  
  if (cl_bk->gui)
  {
    GUIObject_Destroy(cl_bk->gui);
    cl_bk->gui = NULL;
  }
  
  if (cl_bk->led_timer) Timer_Kill(&cl_bk->led_timer);
  SetLampLevel(0);
  
  ModifyKeyHook(NewKey, KEY_HOOK_REMOVE, NULL);
  SUBPROC(elf_exit);
}

CameraLenscoverBook* CreateCameraLenscoverBook()
{
  CameraLenscoverBook* cl_bk = new CameraLenscoverBook;
  memset(cl_bk, NULL, sizeof(CameraLenscoverBook));
  CreateBook(cl_bk, CameraLenscoverBook_onClose, &CameraLenscover_Base_Page, BOOK_NAME, -1, 0);
  
  cl_bk->gui = NULL;
  cl_bk->led = NULL;
  cl_bk->led_timer = NULL;
  
  InitConfig();
  
  return cl_bk;
}

int main (void)
{
  BOOK* book = FindBook(IsCameraLenscoverBook);
  
  if (book)
  {
    UI_Event_toBookID(ELF_TERMINATE_EVENT, BookObj_GetBookID(book));
    MessageBox(EMPTY_TEXTID, STR("Эльф выгружен!"), NOIMAGE, 1, 5000, 0);
  }
  else
  {
    trace_init(L"CameraLenscover.txt");
    
    ModifyKeyHook(NewKey, KEY_HOOK_ADD, CreateCameraLenscoverBook());
    
    return 0;
  }
}
