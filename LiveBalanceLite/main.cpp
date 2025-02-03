#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "main.h"

#define ELF_BCFG_CONFIG_EVENT 994

#define ELFNAME "LiveBalanceLite"
#define LELFNAME L"LiveBalanceLite\nver 1.21"
#define LELFVERSION L"\n(c)Kren\n(e)Ploik &\nBigHercules &\nJoker XT"

#define TIMER_TIMEOUT 500

#pragma segment="ELFBEGIN"
void elf_exit(void)
{
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}

enum align_t
{
  LEFT = 0,
  RIGHT,
  CENTER
};

enum type
{
  outgoing_call = 0,
  incoming_call,
  msg_sms,
  msg_mms,
  internet,
  internet_est,
  error,
  no
};

static char myappname[]=ELFNAME;

wchar_t cashfname[64];
wchar_t cashpath[64];

int isLiveBalanceBook(BOOK *book)
{
  if(!strcmp(book->xbook->name,myappname)) return true;
  return false;
}

int isUSSDBook(BOOK *book)
{
  if(!strcmp(book->xbook->name,"USSDBook")) return true;
  return false;
}

static const char *const patterns[2]=
{
  CashPatPlus,
  CashPatMinus
};

int set_font;

int incom=0;
int state=0;

int MaxCASH;
int CurrentCASH;

volatile int ussdreq_sended=0;

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

int myModifyUIHook(int event1 , int event2 , int (*PROC)(void *msg, BOOK *book, PAGE_DESC * page_desc,LPARAM ClientData, u16),int mode)
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

int char16to8(int c)
{
  if (c==0x401) c=0xA8;
  if (c==0x404) c=0xAA;
  if (c==0x407) c=0xAF;
  if (c==0x451) c=0xB8;
  if (c==0x454) c=0xBA;
  if (c==0x457) c=0xBF;
  if ((c>=0x410)&&(c<0x450)) c-=0x350;
  return (c);
}

void imsi2str(const char *imsi, char *str)
{
  unsigned int c, c1;
  int j, m;
  m=0;
  j=0;
  while(j<IMSI_DATA_BYTE_LEN)
  {
    if (m&1) {c1=c>>4; j++;}
    else c1=(c=imsi[j])&0x0F;
    *str++=c1+'0';
    m++;
  }
  *str=0;
}

int request_type;

const wchar_t *check[] =
{
  L"Исходящий звонок",
  L"Входящий звонок",
  L"Отправка SMS",
  L"Отправка MMS",
  L"Интернет (сессия закрыта)",
  L"Интернет (сессия открыта)",
  L"Ошибка",
  L""
};

int CashPrintf(wchar_t *ws, int cur)
{
  int ret = snwprintf(ws,10,
                   L"%s%u.%02u",
                   ((cur<0)?"-":""),
                   ((cur<0)?(0-cur):cur)/100,
                   ((cur<0)?(0-cur):cur)%100);
  return(ret);
}

int get_money_string(wchar_t *ws, int len, int cur, int t)
{
  int ret = 0;
  if(t == error)
  {
    ret = snwprintf(ws,len,L"(%ls)\r\n",check[t]);
  }
  else
  {
    ret = CashPrintf(ws,cur);
    ret += snwprintf(ws+ret,
                     len,
                     L"%ls (%ls)\r\n",
                     src,
                     check[t]);
  }
  return(ret);
}

static void WriteLog(wchar_t *text, int len)
{
  FSTAT fst;
  if(!fstat(log_path,0,&fst))
  {
    DATETIME dt;
    REQUEST_DATEANDTIME_GET(SYNC,&dt);

    wchar_t log_name[30];
    snwprintf(log_name,30,L"LOG_LBL_%02d.%02d.%04d.txt",dt.date.day,dt.date.mon,dt.date.year);

    int f;
    if((f=_fopen(log_path,
                 log_name,
                 FSX_O_CREAT|FSX_O_APPEND,
                 FSX_S_IREAD|FSX_S_IWRITE,
                 0)) != -1)
    {
      wchar_t s[40];
      int tmp = snwprintf(s, 40, L"[%02d:%02d]: ", dt.time.hour, dt.time.min);
      fwrite(f, s, tmp*2);
      fwrite(f, text, len*2);
      fclose(f);
    }
  }
}

void ___log(void)
{
  if(wstrlen(log_path))
  {
    wchar_t buf[255+1];
    int len = get_money_string(buf,255,CurrentCASH,request_type);
    WriteLog(buf,len);
  }
}

static int BookHide(void *data, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  if(FindBook(isUSSDBook) == book)
  {
    if(!strcmp(page_desc->name,"USSD_Progress_Page"))
    {
      BookObj_Hide(book,0);
      ModifyUIPageHook(PAGE_ENTER_EVENT,BookHide,0,PAGE_HOOK_REMOVE);
    }
  }
  return(1);
}

int SaveCash(void)
{
  int f;
  int res=0;
  if (*cashpath && *cashfname)
  {
    if ((f=_fopen(cashpath,cashfname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))!=-1)
    {
      fwrite(f,&CurrentCASH,sizeof(CurrentCASH));
      fwrite(f,&MaxCASH,sizeof(MaxCASH));
      fclose(f);
      res=1;
    }
  }
  return (res);
}

int LoadCash(void)
{
  int s=0;
  int r=0;
  int f=_fopen(cashpath,cashfname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  if (f!=-1)
  {
    s=fread(f,&CurrentCASH,sizeof(CurrentCASH));
    s=fread(f,&MaxCASH,sizeof(MaxCASH));
    fclose(f);
    if (s==sizeof(MaxCASH)) r=1;
  }
  if (!r)
  {
    MaxCASH = CurrentCASH;
    r=SaveCash();
  }
  return (r);
}

void InitCache(void)
{
  char imsi_str[IMSI_DATA_BYTE_LEN*2+1];
  imsi2str(GetIMSI(),imsi_str);
  snwprintf(cashfname,MAXELEMS(cashfname)-1,L"CallCenter_cash_%s.tmp",imsi_str);
  snwprintf(cashpath,MAXELEMS(cashpath)-1,L"%ls/var",GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL));
  if (LoadCash()<=0)
  {
    snwprintf(cashpath,MAXELEMS(cashpath)-1,L"%ls/var",GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
    LoadCash();
  }
}

static u16 ussd_tmr=0;
static u16 hours_tmr=0;

void EndUSSDtimer(void)
{
  Timer_Kill(&ussd_tmr);
  ussdreq_sended=0;
}

static void ussd_timeout(u16 timer, LPARAM)
{
  ussdreq_sended=0;
  StartHoursTimer();
}

static void ussd_send(u16 timer, LPARAM)
{
  if (Display_GetFocusedDispObject(0)==GUIObject_GetDispObject(SBY_GetMainInput(Find_StandbyBook())) || FindBook(get_IsScreenSaverBook()))
  {
    char mv[256];
    wchar_t ws[80];
    ussdreq_sended=1;
    VCALL_Init(mv);
    str2wstr(ws,CASHREQNUM);
    VCALL_SetNumber(mv,ws,wstrlen(ws));
    MakeVoiceCall(BookObj_GetSessionID(Find_StandbyBook()),mv,4);
    Timer_ReSet(&ussd_tmr,60*1000,ussd_timeout,NULL);
    ModifyUIPageHook(PAGE_ENTER_EVENT,BookHide,0,PAGE_HOOK_ADD_AFTER);
  }
  else
  {
    SendCashReq();
  }
}

void SendCashReq(void)
{
  if (ussdreq_sended) return;
  Timer_ReSet(&ussd_tmr,3*1000,ussd_send,NULL);
}

static void HoursTimerProc(u16 timer, LPARAM)
{
  SendCashReq();
}

void StartHoursTimer(void)
{
  if(CHECK_MIN) Timer_ReSet(&hours_tmr,CHECK_MIN*60*1000, HoursTimerProc, NULL);
}

void DrawStr(int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void DrawHighlightStrV1(int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  DrawString(StrID,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void DrawHighlightStrV2(int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  DrawString(StrID,CEN,XPos+1,YPos,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos+1,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos-1,YPos,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos-1,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void DrawShadowStr(int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  DrawString(StrID,CEN,XPos+1,YPos+1,MaxXPos,MaxYPos,0,1,borderColor,borderColor);
  DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

void myDrawString(int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor, int type)
{
  switch(type){
  case 1:
    DrawHighlightStrV1(StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  case 2:
    DrawHighlightStrV2(StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  case 3:
    DrawShadowStr(StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  default:
    DrawStr(StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
  }
}

void DrawParams(int y)
{
  int str_id;
  wchar_t ws[32];

  int start_y, end_y, fill, cur, max;
  cur=CurrentCASH;
  max=MaxCASH;

  if(!set_text)
  {
    int set_font = SetFont(FONT_BAR);
    int font_size=GetImageHeight(' ')+1;
    fill=((long long)((cur<0)?0:cur))*(DisplayWidth-IDLE_X_BAR-4)/max;
    start_y=IDLE_Y_BAR;
    end_y=IDLE_Y_BAR+font_size-1;
    DrawRect(IDLE_X_BAR, start_y-y, DisplayWidth-IDLE_X_BAR-1, end_y-y+4, COLOR_BORDER_BAR, 0x00000000);
    DrawRect(IDLE_X_BAR+1, start_y-y+1, fill+2, end_y-y+3, COLOR_FILL_BAR, COLOR_FILL_BAR);
    int len = CashPrintf(ws,cur);
    snwprintf(ws+len, MAXELEMS(ws)-1, L"/%u.%02u", max/100, max%100);
    str_id=TextID_Create(ws,ENC_UCS2,MAXELEMS(ws)-1);
    DrawString(str_id, 2, IDLE_X_BAR+3, IDLE_Y_BAR-y+1, DisplayWidth-3, DisplayHeight-1 ,60 , 1, COLOR_TEXT_BAR, 0x00000000);
    TextID_Destroy(str_id);
  }
  else
  {
    int set_font = SetFont(FONT_TEXT);
    CashPrintf(ws,cur);
    wstrcat(ws,src);
    str_id=TextID_Create(ws,ENC_UCS2,MAXELEMS(ws)-1);

    if(align == RIGHT)
      myDrawString(str_id, align, 1, IDLE_Y_TEXT-y, IDLE_X_TEXT, DisplayHeight, COLOR_BORDER_TEXT, COLOR_TEXT, TIPE_BORDER);
    else
      myDrawString(str_id, align, IDLE_X_TEXT, IDLE_Y_TEXT-y, DisplayWidth, DisplayHeight, COLOR_BORDER_TEXT, COLOR_TEXT, TIPE_BORDER);
    TextID_Destroy(str_id);
  }
  SetFont(set_font);
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
  if(Display_oldReDraw) Display_oldReDraw(DO,a,b,c);
}

void Status_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{
  if(Status_oldReDraw) Status_oldReDraw(DO, a, b, c);
  if(IsInStandby || (!standby_only)) DrawParams(0);
}

void Soft_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{
  if(IsInStandby) DrawParams(SoftBarY);
  if(Soft_oldReDraw) Soft_oldReDraw(DO,a,b,c);
}

void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE *sbm = new REDRAW_RELEASE_MESSAGE;

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

static int FindCash(const char *s, BOOK *book)
{
  int n=0;
  const char *pat;
  int i;
  char *ep;
  const char *p;
  const char *ps = s;
  const char *pval;
  int c;
  int cc;
  while(n<1)
  {
    L_RESTARTPAT:
    p=s;
    s++;
    pval=NULL;
    pat=patterns[n];
    while((c=*pat++,cc=*p))
    {
      if (c=='#')
      {
	if (((cc<'0')||(cc>'9'))) goto L_RESTARTPAT;
	pval=p;
	do
	{
	  cc=*++p;
	}
	while(((cc>='0')&&(cc<='9'))||(cc=='.')||(cc==','));
      }
      else
      {
	if (!c) break;

	if (cc!=c) goto L_RESTARTPAT;
	p++;
      }
    }
    if(c)
    {
      if(n==1) break;
      n++;
      s = ps;
      goto L_RESTARTPAT;
    }
    if(!pval) break;
    s=p;
    i=strtol(pval,&ep,10)*100;
    pval=ep;
    if ((*pval=='.')||(*pval==','))
    {
      pval++;

      int j=strtol(pval, &ep, 10);
      int k=ep-pval;
      if (k==1)
       j*=10;
      else
       while (k>2) {
        j/=10; k--;
       }
      i+=j;
      pval=ep;
    }
    if(i>(CurrentCASH+(MaxCASH/100)))
    {
      if(!n) MaxCASH=i;
    }
    if(n==1) CurrentCASH=i*(-1);
    else CurrentCASH=i;
    n++;
  }
  if(pval) SaveCash();
  return(n);
}

wchar_t *(*USSD_GetText)(void *msg)=NULL;
wchar_t *(*USSD_Reguest_GetText)(void *msg)=NULL;

wchar_t *USSD_GetText_2020(void *msg) {return (msg?((wchar_t **)msg)[0x2C/4]:NULL);}
wchar_t *USSD_GetText_Other(void *msg) {return (msg?((wchar_t **)msg)[0x24/4]:NULL);}

wchar_t *USSD_Reguest_GetText_2020(void *msg) {return (msg?((wchar_t **)msg)[0x28/4]:NULL);}
wchar_t *USSD_Reguest_GetText_Other(void *msg) {return (msg?((wchar_t **)msg)[0x20/4]:NULL);}

wchar_t *USSD_GetText_dummy(void *msg)
{
  return NULL;
}

void SetGetTextFunc()
{
  switch (GetChipID()&CHIPID_MASK)
  {
  case CHIPID_DB2020:
  case CHIPID_DB3150:
  case CHIPID_DB3200:
  case CHIPID_DB3210:
  case CHIPID_DB3350:
    USSD_GetText=USSD_GetText_2020;
    USSD_Reguest_GetText=USSD_Reguest_GetText_2020;
    break;
  case CHIPID_DB2000:
  case CHIPID_DB2010:
    USSD_GetText=USSD_GetText_Other;
    USSD_Reguest_GetText=USSD_Reguest_GetText_Other;
    break;
  default:
    USSD_GetText=USSD_GetText_dummy;
    USSD_Reguest_GetText=USSD_GetText_dummy;
  }
}

int UssdProc(BOOK *book, wchar_t *ussd_text)
{
  char *s1, *s2;
  int len;
  int c;
  int i=0;

  EndUSSDtimer();
  if(ussd_text)
  {
    if((len=wstrlen(ussd_text)))
    {
      s1 = s2 = new char[len+3];
      while(c=*ussd_text++)
      {
        c=char16to8(c);
        if (c<32) c='?';
        *s2++=c;
      }
      *s2=0;
      i = FindCash(s1,book);
      mfree(s1);
      if(i)
      {
        DISP_OBJ *StBy_DispObj = GUIObject_GetDispObject((GUI *)SBY_GetStatusIndication(Find_StandbyBook()));
        DispObject_InvalidateRect(StBy_DispObj,0);
      }
      else request_type = error;
    }
    else request_type = error;
  }
  else request_type = error;

  if(log) ___log();

  StartHoursTimer();
  return(i);
}

int OnReceiveUssdRequest(void *data, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  int i = 0;
  if(ussdreq_sended)
  {
    wchar_t *text = USSD_Reguest_GetText(data);
    //MessageBox(EMPTY_TEXTID, TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 0, 0);
    i = UssdProc(book,text);
    if(book) FreeBook(book);
  }
  return(i?BLOCK_EVENT_GLOBALLY:1);
}

static int OnReceiveUssd(void *data, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
  int i = 0;
  if(ussdreq_sended)
  {
    wchar_t *text = USSD_GetText(data);
    //MessageBox(EMPTY_TEXTID, TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 0, 0);
    i = UssdProc(book,text);
    if(book) FreeBook(book);
  }
  return(i?BLOCK_EVENT_GLOBALLY:1);
}

static int TerminateElf(void *, BOOK *book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
}

static int OnTurnOff(void *, BOOK *book)
{
  FreeBook(book);
  UI_CONTROLLED_SHUTDOWN_RESPONSE(BookObj_GetBookID(book));
  return(1);
}

int ShowAuthorInfo(void *mess, BOOK *book)
{
  MessageBox(EMPTY_TEXTID,TextID_Create(LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,10000, 0);
  return(1);
}

static void onMyBookClose(BOOK *book)
{
  myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,PAGE_HOOK_REMOVE);
  myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,PAGE_HOOK_REMOVE);

  ModifyUIPageHook(USSD_RECIEVED_EVENT,OnReceiveUssd,0,PAGE_HOOK_REMOVE);
  ModifyUIPageHook(USSD_REQUEST_RECIEVED_EVENT,OnReceiveUssdRequest,0,PAGE_HOOK_REMOVE);

  if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
  if(Status_desc) DISP_DESC_SetOnRedraw(Status_desc, Status_oldReDraw);
  if(Soft_desc) DISP_DESC_SetOnRedraw(Soft_desc, Soft_oldReDraw);

  EndUSSDtimer();
  Timer_Kill(&hours_tmr);
  if(timer_disable) Timer_Kill(&timer_disable);
  SaveCash();

  SUBPROC(elf_exit);
}

static int OnCallManager(void *mess, BOOK *book)
{
  int eventdata;

  switch (GetChipID()&CHIPID_MASK)
  {
  case CHIPID_DB2000:
  case CHIPID_DB2010:
  case CHIPID_DB2020:
      eventdata= (((CALLMANAGER_EVENT_DATA*)mess)->CallState);
      break;
  case CHIPID_DB3150:
  case CHIPID_DB3200:
  case CHIPID_DB3210:
  case CHIPID_DB3350:
      eventdata=(((CALLMANAGER_EVENT_DATA_A2*)mess)->CallState);
  }
  switch(eventdata)
  {
  case 0:
    if (state==3 && on_call == 1)
    {
      request_type = outgoing_call;
      SendCashReq();
    }
    state=0;
    incom=0;
    break;
  case 2:
    state=1;
    break;
  case 1:
    if (state==1) state=2;
    break;
  case 6:
    if (state==2) state=3;
    if(incom == 1 && on_con == 1)
    {
      request_type = incoming_call;
      SendCashReq();
    }
    incom=0;
    break;
  }
  return(1);
}

static int onSmsSent(void *mess, BOOK *book)
{
  if(on_sms == 1)
  {
    request_type = msg_sms;
    SendCashReq();
  }
  return (0);
}

static int onMmsSent(void *mess, BOOK *book)
{
  if(on_mms == 1)
  {
    request_type = msg_mms;
    SendCashReq();
  }
  return (0);
}

static int GPRS_Terminated(void *mess, BOOK *book)
{
  if(on_net == 1)
  {
    request_type = internet;
    SendCashReq();
  }
  return(0);
}

static int GPRS_Created(void *mess, BOOK *book)
{
  if(on_net_est == 1)
  {
    request_type = internet_est;
    SendCashReq();
  }
  return(0);
}

static int onConnect(void *mess, BOOK *book)
{
  if(incom == 0) incom=1;
  return(0);
}

void InitVar()
{
  DisplayWidth  = Display_GetWidth(0);
  DisplayHeight = Display_GetHeight(0);
  StatusBarY    = DispObject_GetWindowHeight(*StatusRow_p());
  SoftBarY      = DisplayHeight - DispObject_GetWindowHeight(DispObject_SoftKeys_Get());
}

static int onReconfigElf(void *mess, BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if(wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    StartHoursTimer();
    result=1;
  }
  return(result);
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
  wchar_t tmp[255];
  wstrcpy(tmp,msg->bcfg_p);
  wstrcat(tmp,L"/");
  wstrcat(tmp,msg->bcfg_n);
  elfload(tmp,(void *)successed_config_path,(void *)successed_config_name,0);
  return(1);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_RECONFIG_EVENT,                                 onReconfigElf,
  ELF_TERMINATE_EVENT,                                TerminateElf,
  ELF_SHOW_INFO_EVENT,                                ShowAuthorInfo,
  UI_MESSAGING_SMS_SENT_EVENT_TAG,                    onSmsSent,
  UI_MESSAGING_MMS_SENT_EVENT_TAG,                    onMmsSent,
  UI_CONNECTIONMANAGER_SESSION_TERMINATED_EVENT_TAG,  GPRS_Terminated,
  UI_CONNECTIONMANAGER_SESSION_CREATED_EVENT_TAG,     GPRS_Created,
  ON_CALLMANAGER_EVENT_TAG,                           OnCallManager,
  ONGOINGCALL_CALL_CONNECTED_EVENT_TAG ,              onConnect,
  UI_CONTROLLED_SHUTDOWN_REQUESTED_EVENT_TAG,         OnTurnOff,
  ELF_BCFG_CONFIG_EVENT,                              onBcfgConfig,
  SBY_REDRAW_RELEASE_EVENT,                           SB_ELF_Killed,
  NIL_EVENT_TAG,                                      NULL
};

const PAGE_DESC bk_base = {"LiveBalanceMod_Base_Page",0,bk_msglst_base};

int main(void)
{
  if(FindBook(isLiveBalanceBook))
  {
    MessageBox(EMPTY_TEXTID, STR("LiveBalanceMod\nуже запущен!"), NOIMAGE, 1, 10000, 0);
    SUBPROC(elf_exit);
  }
  else
  {
    request_type = no;

    BOOK *book = new BOOK;
    memset(book,0,sizeof(BOOK));
    if(!CreateBook(book,onMyBookClose,&bk_base,ELFNAME,-1,0))
    {
      mfree(book);
      SUBPROC(elf_exit);
      return(0);
    }

    SetGetTextFunc();
    InitVar();
    InitConfig();
    InitCache();

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
      GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
      Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
      Display_desc = DispObject_GetDESC (GUI_display);
      DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
    }

    myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,PAGE_HOOK_ADD_BEFORE);
    myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,PAGE_HOOK_ADD_BEFORE);

    ModifyUIPageHook(USSD_RECIEVED_EVENT,OnReceiveUssd,0,PAGE_HOOK_ADD_BEFORE);

    ModifyUIPageHook(USSD_REQUEST_RECIEVED_EVENT,OnReceiveUssdRequest,0,PAGE_HOOK_ADD_BEFORE);


    StartHoursTimer();
  }
  return(0);
}
