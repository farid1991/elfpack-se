#include "../include/Lib_Clara.h"
#include "../include/Dir.h"
#include "cfg_items.h"

#include "conf_loader.h"
#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE
#include "header.h"

#define ELF_BCFG_CONFIG_EVENT 994
#define ShowMsg(__s__,__t__) MessageBox(EMPTY_TEXTID,STR(__s__),NOIMAGE,1,__t__,0)

#define ELFNAME "Traffic Counter"
#define LELFNAME L"Traffic Counter"
#define LELFVERSION L"\nv1.0\n\n© 2009 Tartes\n\n(e)Ploik & BigHercules"

#define TIMER_TIMEOUT 500

static char myappname[]=ELFNAME;

enum align_t
{
  LEFT = 0,
  RIGHT,
  CENTER
};

int isTrafficCounterBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,myappname)) return true;
  return false;
}

GUI *GetStandbyBookGui(void)
{
  return SBY_GetStatusIndication(Find_StandbyBook());
}

DISP_OBJ_ONREDRAW_METHOD Status_oldReDraw = 0;
DISP_OBJ ** GUI_status = 0;
DISP_DESC * Status_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Display_oldReDraw = 0;
DISP_OBJ * GUI_display = 0;
DISP_DESC * Display_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Soft_oldReDraw = 0;
DISP_OBJ * GUI_soft = 0;
DISP_DESC * Soft_desc = 0;

int DisplayWidth  = 0;
int DisplayHeight = 0;
int SoftBarY   = 0;
int StatusBarY = 0;

bool IsInStandby = false;
u16 timer_disable = 0;

int myModifyUIHook(int event1 , int event2 , int (*PROC)(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16),int mode)
{
  if((event1) && (event1 != 0xFFFFFFFF)) return ModifyUIPageHook(event1, PROC, 0, mode);
  return ModifyUIPageHook(event2, PROC, 0, mode);
}

int StandbyModeActivatedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  IsInStandby = true;
  return(0);
}

int StandbyModeDeactivatedHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  IsInStandby = false;
  return(0);
}

//const SIGSELECT sge[]={0};

const SIGSELECT sge_kr[]={1, EVTProcess_Kill_Response_Sig};

BOOK * TrafficCounterBook;
PROCESS child = 0;

int sessiontime = 0;
int sendbytes = 0;
int recvbytes = 0;

typedef struct
{
  BOOK * book;
}MSG;

void DrawHighlightWSTR(int Font,wchar_t *NAME,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  int SIDtime;
  SetFont(Font);
  SIDtime=TextID_Create(NAME,ENC_UCS2,TEXTID_ANY_LEN);
  DrawString(SIDtime,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,60,0,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,60,0,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,60,0,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,60,0,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos,YPos,MaxXPos,MaxYPos,60,0,NormalColor,NormalColor);
  TextID_Destroy(SIDtime);
}

void CheckSize(int size)
{
  if((size >= max_size* (1024 << (10*size_unit))) && (set_size == 1))
    Request_ICA_ShutdownAllConnections(SYNC);
}

wchar_t *FormatSize(wchar_t *buf, const wchar_t *desc, int size, int maxlen)
{
  if( size >= 1024*1024 )
    snwprintf( buf, maxlen, L"%ls%.1fMb", desc, (float)( size/(1024.f*1024.f) ) );
  else
    snwprintf( buf, maxlen, L"%ls%dKb", desc, size/1024 );
  return buf;
}

void DrawParams(int y)
{
  if(ConnectionManager_Connection_GetState() || activ_session == 1)
  {
  wchar_t buf[50];

  if( totalshow )
     if(align_total == RIGHT)
     DrawHighlightWSTR( totalfont, FormatSize( buf, totaldesc, recvbytes + sendbytes, MAXELEMS(buf) ),
                      align_total, 1, totalypos-y, totalxpos, DisplayHeight, totalbordercolor, totalfontcolor );
  else
     DrawHighlightWSTR( totalfont, FormatSize( buf, totaldesc, recvbytes + sendbytes, MAXELEMS(buf) ),
                      align_total, totalxpos, totalypos-y, DisplayWidth, DisplayHeight, totalbordercolor, totalfontcolor );

  if( inshow )
     if(align_in == RIGHT)
     DrawHighlightWSTR( infont, FormatSize( buf, indesc, recvbytes, MAXELEMS(buf) ),
                      align_in, 1, inypos-y, inxpos,DisplayHeight,  inbordercolor, infontcolor );
  else
     DrawHighlightWSTR( infont, FormatSize( buf, indesc, recvbytes, MAXELEMS(buf) ),
                      align_in, inxpos, inypos-y, DisplayWidth,DisplayHeight,  inbordercolor, infontcolor );
  if( outshow )
     if(align_out == RIGHT)
     DrawHighlightWSTR( outfont, FormatSize( buf, outdesc, sendbytes, MAXELEMS(buf) ),
                      align_out, 1, outypos-y, outxpos, DisplayHeight, outbordercolor, outfontcolor );
  else
     DrawHighlightWSTR( outfont, FormatSize( buf, outdesc, sendbytes, MAXELEMS(buf) ),
                      align_out, outxpos, outypos-y, DisplayWidth, DisplayHeight, outbordercolor, outfontcolor );

  if( timeshow )
   {
     if( sessiontime >= 3600 )
      snwprintf( buf, MAXELEMS(buf), L"%ls%02d:%02d:%02d", timedesc, sessiontime/3600, (sessiontime%3600)/60, (sessiontime%3600)%60 );
     else
      snwprintf( buf, MAXELEMS(buf), L"%ls%02d:%02d", timedesc, sessiontime/60, sessiontime%60 );

     if(align_time == RIGHT)
       DrawHighlightWSTR( timefont, buf, align_time, 1, timeypos-y, timexpos, DisplayHeight, timebordercolor, timefontcolor );
     else
       DrawHighlightWSTR( timefont, buf, align_time, timexpos, timeypos-y, DisplayWidth, DisplayHeight, timebordercolor, timefontcolor );
    }
  }
}

void onTimer_disable (u16 timerID, LPARAM lparam)
{
  IsInStandby = false;
}

void Display_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{
  IsInStandby = true;
  Timer_ReSet(&timer_disable, TIMER_TIMEOUT, onTimer_disable,0);
  if(IsInStandby) DrawParams(StatusBarY);
  Display_oldReDraw(DO,a,b,c);
}

void Status_ReDraw(DISP_OBJ *d, int a, int b, int c)
{
  Status_oldReDraw(d, a, b, c);
  if(IsInStandby || (!standby_only)) DrawParams(0);
}

void Soft_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{
  if(IsInStandby) DrawParams(SoftBarY);
  Soft_oldReDraw(DO,a,b,c);
}

void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE * sbm = new REDRAW_RELEASE_MESSAGE;

  sbm->SI_OldOnRedraw = Status_oldReDraw;
  sbm->SI_NewOnRedraw = Status_ReDraw;

  sbm->SB_OldOnRedraw = Display_oldReDraw;
  sbm->SB_NewOnRedraw = Display_ReDraw;

  sbm->SK_OldOnRedraw = Soft_oldReDraw;
  sbm->SK_NewOnRedraw = Soft_ReDraw;

  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, sbm, (void (*)(void*))mfree_adr());
}

int SB_ELF_Killed(void *mess, BOOK *book)
{
  REDRAW_RELEASE_MESSAGE * sbm =(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE * ms = 0;

  if (sbm->SI_NewOnRedraw == Status_oldReDraw)
  {
    ms = new REDRAW_RELEASE_MESSAGE;
    memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));

    if (sbm->SI_OldOnRedraw!=EMPTY_REDRAW_METHOD) Status_oldReDraw = sbm->SI_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(*GUI_status), Status_ReDraw);

    ms->SI_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SI_NewOnRedraw = Status_ReDraw;
  }
  if (sbm->SB_NewOnRedraw == Display_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }
    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) Display_oldReDraw = sbm->SB_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_display), Display_ReDraw);

    ms->SB_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SB_NewOnRedraw = Display_ReDraw;
  }
  if (sbm->SK_NewOnRedraw == Soft_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }
    if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) Soft_oldReDraw = sbm->SK_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_soft), Soft_ReDraw);

    ms->SK_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SK_NewOnRedraw = Soft_ReDraw;
  }
  if(ms)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return(BLOCK_EVENT_GLOBALLY);
  }
  return(0);
}

int TerminateElf(void * ,BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(0x6fFFFFFF,TextID_Create( LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),0, 1 ,5000,msg->book);
  return(1);
}

int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    result=1;
  }
  return(result);
}

int onBcfgConfig(void* mess,BOOK* b)
{
  FSTAT _fstat;
  wchar_t path[256];

  if(fstat(GetDir(DIR_ELFS|MEM_INTERNAL),L"BcfgEdit.elf",&_fstat)==0)
    wstrcpy(path,GetDir(DIR_ELFS|MEM_INTERNAL));
  else if(fstat(GetDir(DIR_ELFS|MEM_EXTERNAL),L"BcfgEdit.elf",&_fstat)==0)
    wstrcpy(path,GetDir(DIR_ELFS|MEM_EXTERNAL));
  else
  {
    ShowMsg("BcfgEdit.elf not found!",2000);
    return (1);
  }
  wstrcat(path,L"/BcfgEdit.elf");
  elfload(path,(void*)successed_config_path,(void*)successed_config_name,0);
  return (1);
}

const PAGE_MSG TrafficCounter_PageEvents[]@ "DYN_PAGE" = {
  ELF_TERMINATE_EVENT  , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  ELF_RECONFIG_EVENT   , ReconfigElf,
  ELF_BCFG_CONFIG_EVENT, onBcfgConfig,
  NIL_EVENT_TAG, NULL
};

PAGE_DESC base_page ={"TrafficCounter_BasePage",0,TrafficCounter_PageEvents};

void killchild()
{
  union SIGNAL *sig;
  sig = alloc( sizeof(union SIGNAL), EVTProcess_Kill_Sig );
  send( &sig, child );

  sig = receive(sge_kr);

  free_buf(&sig);
}

void elf_exit(void)
{
  if(child)
    killchild();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onCloseTrafficCounterBook(BOOK * book)
{
   myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,0);
   myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,0);

   if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
   if(Status_desc) DISP_DESC_SetOnRedraw(Status_desc, Status_oldReDraw);
   if(Soft_desc) DISP_DESC_SetOnRedraw(Soft_desc, Soft_oldReDraw);

   if(timer_disable) Timer_Kill(&timer_disable);

   SUBPROC(elf_exit);
}

void Redraw_MMP()
{
  DispObject_InvalidateRect( *StatusRow_p(), 0 );
}

OS_PROCESS(EVTListener)
{
  union SIGNAL *sig;

  int s[2] = {0,0};

  SIGSELECT sge[] = {4, EVTProcess_Kill_Sig, EVENT_ID_ICA_CONNECTIONSTATUSINFORMATION_RESPONSE,
  EVENT_ID_ICA_DATACOUNTERS_ENDSESSION_RESPONSE, EVENT_ID_ICA_DATACOUNTERS_RESPONSE};


  //Request_EventChannel_Subscribe(s,0,EVENT_ID_ICA_CONNECTIONSTATUSINFORMATION);
  Request_EventChannel_Subscribe(s,0,EVENT_ID_ICA_DATACOUNTERS_ENDSESSION_REQUEST);
  Request_EventChannel_Subscribe(s,0,EVENT_ID_ICA_DATACOUNTERS_REQUEST);

  for(;;)
  {
    sig=receive(sge);

    if( sig->signo == EVTProcess_Kill_Sig )
    {
      //Request_EventChannel_Subscribe(s,1,EVENT_ID_ICA_CONNECTIONSTATUSINFORMATION);
      Request_EventChannel_Subscribe(s,1,EVENT_ID_ICA_DATACOUNTERS_ENDSESSION_REQUEST);
      Request_EventChannel_Subscribe(s,1,EVENT_ID_ICA_DATACOUNTERS_REQUEST);

      PROCESS sndr = sender(&sig);

      free_buf(&sig);

      sig = alloc( sizeof(union SIGNAL), EVTProcess_Kill_Response_Sig );
      send( &sig, sndr );

      kill_proc( current_process() );
      return;
    }
    else if( sig->signo == EVENT_ID_ICA_DATACOUNTERS_RESPONSE ||
             sig->signo ==  EVENT_ID_ICA_DATACOUNTERS_ENDSESSION_RESPONSE )
    {
      DATACOUNTERS_SIGNAL *ds = (DATACOUNTERS_SIGNAL *)sig;

      sessiontime = ds->connectiontime;
      sendbytes = ds->sendbytes;
      recvbytes = ds->receivedbytes;

      CheckSize(sendbytes + recvbytes);

      //MMIPROC(Redraw_MMP);
    }
    free_buf(&sig);
  }
}

void InitVar()
{
  DisplayWidth  = Display_GetWidth(0);
  DisplayHeight = Display_GetHeight(0);
  StatusBarY    = DispObject_GetWindowHeight(*StatusRow_p());
  SoftBarY      = DisplayHeight - DispObject_GetWindowHeight(DispObject_SoftKeys_Get());
}

BOOK * CreateTrafficCounterBook()
{
  child = create_process(OS_PRI_PROC,"EVTListener_Process",EVTListener,16384,30,0,0,0,0,0);
  start(child);

  TrafficCounterBook = new BOOK;
  CreateBook(TrafficCounterBook,onCloseTrafficCounterBook,&base_page,myappname,-1,0);

  InitConfig();
  InitVar();

  return(TrafficCounterBook);
}

int main (void)
{
  if(FindBook(isTrafficCounterBook))
  {
    InitConfig();

    if( Language == 0 )
      MessageBox(0x6fFFFFFF, TextID_Create( LELFNAME L"\nуже запущен",ENC_UCS2,TEXTID_ANY_LEN), 0, 1, 5000, 0);
    else
      MessageBox(0x6fFFFFFF, TextID_Create( LELFNAME L"\nalready runned",ENC_UCS2,TEXTID_ANY_LEN), 0, 1, 5000, 0);

    SUBPROC(elf_exit);
  }
  else
  {
    CreateTrafficCounterBook();

    if(!GUI_status)
    {
      GUI_status = StatusRow_p();
      Status_desc = DispObject_GetDESC (* GUI_status);
      Status_oldReDraw = DispObject_GetOnRedraw (* GUI_status);
      DISP_DESC_SetOnRedraw (Status_desc, (DISP_OBJ_ONREDRAW_METHOD)Status_ReDraw);
    }
    if(!GUI_soft)
    {
      GUI_soft = DispObject_SoftKeys_Get();
      Soft_desc = DispObject_GetDESC (GUI_soft);
      Soft_oldReDraw = DispObject_GetOnRedraw(GUI_soft);
      DISP_DESC_SetOnRedraw(Soft_desc, (DISP_OBJ_ONREDRAW_METHOD)Soft_ReDraw);
    }
    if(!GUI_display)
    {
      GUI_display = GUIObject_GetDispObject( GetStandbyBookGui() );
      Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
      Display_desc = DispObject_GetDESC (GUI_display);
      DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
    }

    myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,1);
    myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,1);
  }
  return(0);
}
