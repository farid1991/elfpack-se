#include "main.h"

wchar_t *SPLASH = L"/";

u16 timer;
LIST *names;
int count_of_pict;

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

unsigned int randseed;

unsigned int randint( int max )
{
  randseed = randseed * 0x08088405;
  randseed++;
  return(((long long)randseed * max ) >> 32);
}

unsigned int rand()
{
  return(randint(0x100000));
}

void Randomize()
{
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC, &dt);
  randseed = (dt.time.min | (dt.time.sec<<5));
  randseed = rand() + dt.time.sec;
}

wchar_t *MakeFullName(wchar_t const *path, wchar_t *name)
{
  int len = wstrlen(path)+wstrlen(name)+1;
  wchar_t *fullname = new wchar_t[len+1];
  memset(fullname, 0, (len+1)*sizeof(wchar_t));
  wstrcpy(fullname, path);
  wstrcat(fullname, SPLASH);
  wstrcat(fullname, name);
  return(fullname);
}

static int TerminateElf(void *msg, BOOK *book)
{
  FreeBook(book);
  return(1);
}

static int ShowAuthorInfo(void *msg ,BOOK *book)
{
  MessageBox(EMPTY_TEXTID,
             STR("Wallpaper Changer ;)\nVer 1.4f\n\n(c) Joker XT"),
             NOIMAGE, 1, 0, NULL);
  return(1);
}

void PicturesList_ItemsDestroy(void)
{
   for(int i = names->FirstFree; i > 0; i--)
   {
      mfree(List_RemoveAt(names, i-1));
   }
}

void PicturesList_Destroy(void)
{
   PicturesList_ItemsDestroy();
   List_Destroy(names);
}

void PicturesList_Renew(void)
{
   PicturesList_Destroy();
   names = List_Create();
}

void PicturesList_ItemsInit(wchar_t const *path)
{
   FSTAT fst;
   void *dir = w_diropen(path);
   wchar_t *dir_item;
   while(dir_item = w_dirread(dir))
   {
      fstat(path, dir_item, &fst);
      if(!(fst.st_mode & FSX_S_IFDIR)) //if file
      {
         wchar_t *fname = MakeFullName(path + wstrlen(PATH), dir_item);
         List_InsertLast(names, fname);
      }
      else
      {
         if(IS_IN_SUBFOLDERS)
         {
            wchar_t *fname = MakeFullName(path, dir_item);
            PicturesList_ItemsInit(fname);
            mfree(fname);
         }
      }
   }
   w_dirclose(dir);
   
   count_of_pict = (names->FirstFree)-1;
}

int GetTime(void)
{
  int time = REFRESH_TIME;
  if(TIME_UNITS == 0)      time*=1000;
  else if(TIME_UNITS == 1) time*=1000*60;
  else if(TIME_UNITS == 2) time*=1000*60*60;
  return(time);
}

int Standby_SetImage(wchar_t *fname)
{
   wchar_t *name, *path;
   int result = RESULT_FAIL;
   wchar_t *spl = wstrrchr(fname, '/');
   *spl = 0;
   name = spl+1;
   
   if(spl == fname)
      path = (wchar_t *)PATH;
   else
      path = MakeFullName(PATH, fname+1);
   
   FSTAT fstat_img;
   if(!fstat(path, name, &fstat_img))
   {
      int standby_type = 6;
      ThemeImage_Delete(standby_type);
      ThemeImage_SetImage(standby_type, 0, 0, path, name, 0);
      DispObject_InvalidateRect(DispObject_SoftKeys_Get(), NULL);
      DispObject_InvalidateRect(*(StatusRow_p()), NULL);
      DispObject_InvalidateRect(GUIObject_GetDispObject(SBY_GetStatusIndication(Find_StandbyBook())), NULL);       
      result = RESULT_OK;
   }
   *spl = '/';
   if(path != PATH) mfree(path);
   return(result);
}

void onTimer(u16 timerID, LPARAM lparam)
{
   bool start_timer = true;
   FSTAT fstat_img;
   if(IS_ENABLED)
   {
      if(!fstat(PATH, NULL, &fstat_img))
      {
         BOOK *book = Display_GetTopBook(DISPLAY);
         BOOK *StandbyBook = Find_StandbyBook();
         
         if((book == StandbyBook) || !IS_STANDBY_FOCUS)
         {
            if(!names->FirstFree) PicturesList_ItemsInit(PATH);
            while(true)
            {
               if(!names->FirstFree) return;
               else if(names->FirstFree == 1)
               {
                  start_timer = false;
                  count_of_pict = 0;
               }
               else
               {
                  start_timer = true;
                  if(IS_RANDOM) count_of_pict = randint(names->FirstFree-1);
                  else
                     if(count_of_pict < 0) count_of_pict = names->FirstFree-1;
               }
               
               wchar_t *fname = (wchar_t *)List_Get(names, count_of_pict--);
               if(Standby_SetImage(fname) == RESULT_OK) break;
            }
         }
      }
      
      if(start_timer) Timer_ReSet(&timer, GetTime(), onTimer, NULL);
   }
}

static int ReconfigElf(void *mess, BOOK *book)
{
  FSTAT fst;
  RECONFIG_EVENT_DATA *reconf = (RECONFIG_EVENT_DATA *)mess;
  int result = 0;
  if(!wstrcmpi(reconf->path, (wchar_t *)successed_config_path) && !wstrcmpi(reconf->name, (wchar_t *)successed_config_name))
  {
    Randomize();
    InitConfig();
    if(!fstat(PATH, NULL, &fst))
    {
      if(timer) Timer_Kill(&timer);
      PicturesList_Renew();
      PicturesList_ItemsInit(PATH);
      Timer_ReSet(&timer, 100, onTimer, NULL);
    }
    else MessageBox(EMPTY_TEXTID,
                 STR("”кажите папку с изображени€ми правильно!"),
                 NOIMAGE, 1, 0, NULL);
    
    Timer_ReSet(&timer, GetTime(), onTimer, NULL);
    result = 1;
  }
  return(result);
}

static void onBookClose(BOOK *book)
{
  if(timer) Timer_Kill(&timer);
  PicturesList_Destroy();
  SUBPROC(elf_exit);
}

int onBcfgConfig(void *mess, BOOK *book)
{
  MSG_BCFG *msg = (MSG_BCFG *)mess;
  wchar_t bcfg[FILENAME_MAX_LEN+1];
  wstrcpy(bcfg, msg->bcfg_p);
  wstrcat(bcfg, SPLASH);
  wstrcat(bcfg, msg->bcfg_n);
  elfload(bcfg, (void *)successed_config_path, (void *)successed_config_name, NULL);
  return(1);
}

int StartTimer(void *mess, BOOK *book)
{
   if(!IS_SLEEP_ACTIVATED) Timer_ReSet(&timer, GetTime(), onTimer, NULL);
   return(1);
}

int StopTimer(void *mess, BOOK *book)
{
   if(!IS_SLEEP_ACTIVATED) Timer_Kill(&timer);
   return(1);
}

const PAGE_MSG WC_PageEvents[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT,                    TerminateElf,
  ELF_SHOW_INFO_EVENT,                    ShowAuthorInfo,
  ELF_RECONFIG_EVENT,                     ReconfigElf,
  ELF_BCFG_CONFIG_EVENT,                  onBcfgConfig,
  UI_SLEEPMODE_DEACTIVATED_EVENT_TAG,     StartTimer,
  UI_SLEEPMODE_ACTIVATED_EVENT_TAG,       StopTimer,
  NIL_EVENT_TAG,                          NULL
};

PAGE_DESC base_page ={"WallpaperChanger_Base_Page", 0, WC_PageEvents};

BOOK *NewBook(char *name)
{
  BOOK *book = new BOOK;
  memset(book, 0, sizeof(BOOK));
  if(!CreateBook(book, onBookClose, &base_page, name, -1, 0))
  {
    mfree(book);
    return(NULL);
  }
  return(book);
}

int main(void)
{
   BOOK *book = NewBook(BOOKNAME);
   if(book)
   {
      Randomize();
      InitConfig();
      names = List_Create();
      timer = Timer_Set(100, onTimer, NULL);
   }
   else SUBPROC(elf_exit);
   return(RESULT_OK);
}
