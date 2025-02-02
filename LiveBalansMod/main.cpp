#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\classes.h"
#include "..\\include\funcs.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "main.h"

#define ELF_BCFG_CONFIG_EVENT 994
#define SBY_REDRAW_RELEASE_EVENT 995


#define ELFNAME "LiveBalanceMod"
#define LELFNAME L"LiveBalanceMod\nver 1.0"
#define LELFVERSION L"\n(c)Kren\n(e)Ploik &\nBigHercules &\nJoker XT"

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

int incom=0;
int state=0;
// 0 - в идле
// 1 - звоним
// 2 - дозвонилиссь
// 3 - завершили

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

int DisplayWidth  = 0; //ширина дисплея в пикселях
int DisplayHeight = 0; //высота дисплея в пикселях
int SoftBarY   = 0;
int StatusBarY = 0;

bool IsInStandby = false;

int myModifyUIHook(int event1 , int event2 , int (*PROC)(UI_MESSAGE*),int mode)
{FUNCTION
  if((event1) && (event1 != 0xFFFFFFFF)) return ModifyUIHook(event1, PROC, mode);
  return ModifyUIHook(event2, PROC, mode);
}

int StandbyModeActivatedHook(UI_MESSAGE *)
{FUNCTION
  IsInStandby = true;
  return(0);
}

int StandbyModeDeactivatedHook(UI_MESSAGE *)
{FUNCTION
  IsInStandby = false;
  return(0);
}

IUnknown * pFont=0;

int SetFont3210(int font_size)
{
  IFontManager * pFontManager=0;
  IFontFactory * pFontFactory=0;
  FONT_DATA pFontData;
  memset(&pFontData,0,sizeof(FONT_DATA));

  if (pFont)
  {
    pFont->Release();
    pFont=0;
  }

  CoCreateInstance(CFontManagerCreator_guid,IFontManager_guid,&pFontManager);
  pFontManager->CreateFontFactory(&pFontFactory);

  pFontFactory->GetDefaultFontSettings(large_size,&pFontData);
  pFontData.size=(float)(font_size/5*5);
  pFontData.style=((font_size%5)&bold)|((font_size%5)&italic);
  pFontFactory->CreateFont(&pFontData,&pFont);

  if (pFontManager) pFontManager->Release();
  if (pFontFactory) pFontFactory->Release();

  return(1);
}

void DrawString3210(STRID strid,int align,int x1,int y1,int x2,int y2,int unk,int unk1,int pen_color,int brush_color)
{
  WINDOW_RECT rect;

  int display_width = x2-x1;

  ITextRenderingManager * pTextRenderingManager=0;
  ITextRenderingFactory * pTextRenderingFactory=0;
  IRichTextLayout * pRichTextLayout=0;
  IRichText * pTextObject=0;
  IUnknown * pGC =0;

  CoCreateInstance(CTextRenderingManagerCreator_guid,ITextRenderingManager_guid,&pTextRenderingManager);
  pTextRenderingManager->CreateTextRenderingFactory(&pTextRenderingFactory);

  pTextRenderingFactory->CreateRichText(&pTextObject);
  pTextRenderingFactory->CreateRichTextLayout(pTextObject,0,0,&pRichTextLayout);

  if (!pFont) SetFont3210(FONT_BAR);
  TextObject_SetText(pTextObject,strid);
  TextObject_SetFont(pTextObject,pFont,0x8000000A,0x7FFFFFF5);
  pTextObject->SetForegroundColor(pen_color,0x8000000A,0x7FFFFFF5);

  pTextObject->SetTextAlign(align,0x8000000A,0x7FFFFFF5);

  pRichTextLayout->SetDisplayWidth(display_width);

  rect.x1=x1;
  rect.y1=y1;
  rect.width=x2-x1;
  rect.height=y2-y1;

  DisplayGC_AddRef(get_DisplayGC(),&pGC);

  pRichTextLayout->DrawString(pGC,x1,y1,&rect);

  if (pTextRenderingManager) pTextRenderingManager->Release();
  if (pTextRenderingFactory) pTextRenderingFactory->Release();
  if (pRichTextLayout) pRichTextLayout->Release();
  if (pTextObject) pTextObject->Release();
  if (pGC) pGC->Release();
}

int char16to8(int c)
{FUNCTION
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
{FUNCTION
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

int SaveCash(void)
{FUNCTION
  int f;
  int res=0;
  if (*cashpath && *cashfname)
  {
    if ((f=_fopen(cashpath,cashfname,0x204,0x180,0))!=-1)
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
{FUNCTION
  int s=0;
  int r=0;
  int f=_fopen(cashpath,cashfname,0x001,0x180,0);
  if (f!=-1)
  {
    s=fread(f,&CurrentCASH,sizeof(CurrentCASH));     //by BoBa 4.07.07
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
{FUNCTION
  char imsi_str[IMSI_DATA_BYTE_LEN*2+1];
  imsi2str("0123456789"/*GetIMSI()*/,imsi_str);
  snwprintf(cashfname,MAXELEMS(cashfname)-1,L"CallCenter_cash_%s.tmp",imsi_str);
  snwprintf(cashpath,MAXELEMS(cashpath)-1,L"%ls/var",GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL));
  if (LoadCash()<=0)
  {
    snwprintf(cashpath,MAXELEMS(cashpath)-1,L"%ls/var",GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
    LoadCash();
  }
//  while ((CASH_SIZE<MAX_CASH_SIZE)&&(*patterns[CASH_SIZE])) CASH_SIZE++;
}

static u16 ussd_tmr=0;
static u16 hours_tmr=0;
static u16 ussd_hide_tmr=0;

void EndUSSDtimer(void)
{FUNCTION
  Timer_Kill(&ussd_tmr);
  ussdreq_sended=0;
}

static void ussd_timeout(u16 timer, void *)
{FUNCTION
  ussdreq_sended=0;
  StartHoursTimer();
}

static void ussd_hide(u16 timer, void *)
{FUNCTION
  BookObj_Hide(FindBook(isUSSDBook),0);
  if(ussdreq_sended) Timer_ReSet(&ussd_hide_tmr,150,ussd_hide,NULL);
}

static void ussd_send(u16 timer, void *)
{FUNCTION
  if (Display_GetFocusedDispObject(0)==GUIObject_GetDispObject(SBY_GetMainInput(Find_StandbyBook())))
  {
    Timer_ReSet(&ussd_hide_tmr,150,ussd_hide,NULL);
    char mv[256];
    wchar_t ws[80];
    ussdreq_sended=1;
    VCALL_Init(mv);
    str2wstr(ws,CASHREQNUM);
    VCALL_SetNumber(mv,ws,wstrlen(ws));
    MakeVoiceCall(BookObj_GetSessionID(Find_StandbyBook()),mv,4);
//    BookObj_Hide(FindBook(isUSSDBook),0);
    Timer_ReSet(&ussd_tmr,60*1000,ussd_timeout,NULL);
  }
  else
  {
    SendCashReq();
  }
}

void SendCashReq(void)
{FUNCTION
  if (ussdreq_sended) return; //Ужо летим ;)
  Timer_ReSet(&ussd_tmr,3*1000,ussd_send,NULL);
}

static void HoursTimerProc(u16 timer, void *)
{FUNCTION
  SendCashReq();
}

void StartHoursTimer(void)
{FUNCTION
  if(CHECK_MIN) Timer_ReSet(&hours_tmr,CHECK_MIN*60*1000, HoursTimerProc, NULL);
}

void DrawHighlightStrV1(DISP_OBJ *db,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{FUNCTION
  DrawString3210(StrID, CEN, XPos-1, YPos-1, MaxXPos-1, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos-1, YPos+1, MaxXPos-1, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos+1, YPos-1, MaxXPos+1, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos+1, YPos+1, MaxXPos+1, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos, YPos, MaxXPos, MaxYPos, 0, 0, 0x00000000, NormalColor);
}

void DrawHighlightStrV2(DISP_OBJ *db,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{FUNCTION
  DrawString3210(StrID, CEN, XPos+1, YPos, MaxXPos, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos, YPos+1, MaxXPos, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos-1, YPos, MaxXPos, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos, YPos-1, MaxXPos, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos, YPos, MaxXPos, MaxYPos, 0, 0, 0x00000000, NormalColor);
}

void DrawShadowStr(DISP_OBJ *db,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{FUNCTION
  DrawString3210(StrID, CEN, XPos+1, YPos+1, MaxXPos, MaxYPos, 0, 0, borderColor, 0x00000000);
  DrawString3210(StrID, CEN, XPos, YPos, MaxXPos, MaxYPos, 0, 0, 0x00000000, NormalColor);
}

void myDrawString(DISP_OBJ *db, int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor, int type)
{FUNCTION
  //Тип обводки (Overlay type)
  switch(type){
  case 0: //Полная (Full) v1
    DrawHighlightStrV1(db, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  case 1: //Полная (Full) v2
    DrawHighlightStrV2(db, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  case 2: //Тень (Shadow)
    DrawShadowStr(db, StrID, CEN,  XPos, YPos, MaxXPos, MaxYPos, borderColor, NormalColor);
    break;
  default: //Нет (No)
    DrawString3210(StrID, CEN, XPos, YPos, MaxXPos, MaxYPos, 0, 0, 0x00000000, NormalColor);
  }
}

void DrawParams(DISP_OBJ *DO, int y)
{FUNCTION
  int str_id;
  wchar_t ws[32];

  int start_y, end_y, fill, cur, max;
  cur=CurrentCASH;
  max=MaxCASH;
  int old_font=SetFont3210(FONT_BAR);
  int font_size=FONT_BAR+1;

  if(!set_text)
  {
//    int set_font = SetFont(FONT_BAR);
    fill=((long long)((cur<0)?0:cur))*(DisplayWidth-IDLE_X_BAR-4)/max;
    start_y=IDLE_Y_BAR;
    end_y=IDLE_Y_BAR+font_size-1;
    DrawRect(IDLE_X_BAR, start_y-y, DisplayWidth-IDLE_X_BAR-1, end_y-y+4, COLOR_BORDER_BAR, 0x00000000);
    DrawRect(IDLE_X_BAR+1, start_y-y+1, fill+2, end_y-y+3, COLOR_FILL_BAR, COLOR_FILL_BAR);
    snwprintf(ws, MAXELEMS(ws)-1, L"%s%u.%02u/%u.%02u", ((cur<0)?"-":""), ((cur<0)?(0-cur):cur)/100, ((cur<0)?(0-cur):cur)%100, max/100, max%100);
    str_id=Str2ID(ws,0,MAXELEMS(ws)-1);
    DrawString3210(str_id, 2, IDLE_X_BAR+2, IDLE_Y_BAR-y+1, DisplayWidth-3, DisplayHeight-1, 0, 0, COLOR_TEXT_BAR, 0x00000000);
    TextFree(str_id);
  }
  else
  {
//    int set_font = SetFont(FONT_TEXT);
    snwprintf(ws, MAXELEMS(ws)-1, L"%s%u.%02u", ((cur<0)?"-":""), ((cur<0)?(0-cur):cur)/100, ((cur<0)?(0-cur):cur)%100);
    wstrcat(ws,src);
    str_id=Str2ID(ws,0,MAXELEMS(ws)-1);

    if(align == RIGHT)
      myDrawString(DO,str_id, align, 1, IDLE_Y_TEXT-y, IDLE_X_TEXT, DisplayHeight, COLOR_BORDER_TEXT, COLOR_TEXT, TIPE_BORDER);
    else
      myDrawString(DO,str_id, align, IDLE_X_TEXT, IDLE_Y_TEXT-y, DisplayWidth, DisplayHeight, COLOR_BORDER_TEXT, COLOR_TEXT, TIPE_BORDER);

    TextFree(str_id);
  }
  SetFont3210(old_font);
}

void Display_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{FUNCTION
  if(IsInStandby) DrawParams(DO,StatusBarY);
  if(Display_oldReDraw) Display_oldReDraw(DO,a,b,c);
}

void Status_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{FUNCTION
  if(Status_oldReDraw) Status_oldReDraw(DO, a, b, c);
  if(IsInStandby || (!standby_only)) DrawParams(DO,0);
}

void Soft_ReDraw(DISP_OBJ *DO, int a, int b, int c)
{FUNCTION
  if(IsInStandby) DrawParams(DO,SoftBarY);
  if(Soft_oldReDraw) Soft_oldReDraw(DO,a,b,c);
}


void Send_REDRAW_RELEASE()
{FUNCTION
  REDRAW_RELEASE_MESSAGE *sbm = new REDRAW_RELEASE_MESSAGE;

  // Status
  sbm->SI_OldOnRedraw = Status_oldReDraw;
  sbm->SI_NewOnRedraw = Status_ReDraw;
  // Screen
  sbm->SB_OldOnRedraw = Display_oldReDraw;
  sbm->SB_NewOnRedraw = Display_ReDraw;
  // Soft
  sbm->SK_OldOnRedraw = Soft_oldReDraw;
  sbm->SK_NewOnRedraw = Soft_ReDraw;

  // посылаем всем  мессагу со старыми и своими методами Redraw
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, sbm, (void (*)(void*))mfree_adr());
}


int SB_ELF_Killed(void *mess, BOOK *book)
{FUNCTION
  // если был убит эльф рисующий на ГЭ или просто нужно перетосовать методы
  REDRAW_RELEASE_MESSAGE * sbm =(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE * ms = 0;

  //Проверим статус
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SI_NewOnRedraw == Status_oldReDraw)
  {
    ms = new REDRAW_RELEASE_MESSAGE;
    memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SI_OldOnRedraw) Status_oldReDraw = sbm->SI_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(*GUI_status), Status_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SI_OldOnRedraw = 0;
    ms->SI_NewOnRedraw = Status_ReDraw;
  }

  //Проверим главный экран
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SB_NewOnRedraw == Display_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    // если он был убит, то заменяем свой oldRedraw на его..
    //if (sbm->SB_OldOnRedraw)
      Display_oldReDraw = sbm->SB_OldOnRedraw;

    // ставим свой метод наверх
    //DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_display), Display_ReDraw);
    DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SB_OldOnRedraw = 0;
    ms->SB_NewOnRedraw = Display_ReDraw;
  }

  //Проверим софт
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SK_NewOnRedraw == Soft_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SK_OldOnRedraw) Soft_oldReDraw = sbm->SK_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_soft), Soft_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SK_OldOnRedraw = 0;
    ms->SK_NewOnRedraw = Soft_ReDraw;
  }
  if(ms)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return(1);
  }
  return(0);
}


static int FindCash(const char *s, BOOK *book)
{FUNCTION
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
    //if(n==1) break; //если нашли "+" паттерн, то "-" не нужно искать
    //if (!patterns[n]) break; //Больше паттернов нет
    L_RESTARTPAT:
    p=s; //Текущая позиция в строке
    s++;
    pval=NULL; //Число пока не нашли
    pat=patterns[n]; //Начинаем с начала паттерна
    while((c=*pat++,cc=*p)) //Пока не дошли до конца строки, в которой ищем
    {
      if (c=='#') //А не число ли сейчас требуется
      {
	//Проверяем на число
	if (((cc<'0')||(cc>'9'))) goto L_RESTARTPAT; //Первая цифра не число - рестарт поиска
	pval=p; //Возможно число будет здесь
	do
	{
	  cc=*++p; //Следующий символ
	}
	while(((cc>='0')&&(cc<='9'))||(cc=='.')||(cc==',')); //Пока это еще число
      }
      else
      {
	//Обычное сравнение
	if (!c) break; //Дошли до конца паттерна, значит все совпало, выходим
	//Сравниваем символы
	if (cc!=c) goto L_RESTARTPAT; //Нет совпадения, рестартуем
	p++;
      }
    }
    if(c)
    { //Не дошли до конца паттерна, те не нашли его
      if(n==1) break;
      n++; //берём следующий паттерн
      s = ps; //устанавливаем на начало текст от USSD
      goto L_RESTARTPAT; //ищем другой паттерн
    }
    if(!pval) break; //Не нашли число
    s=p; //Поиск будем продолжать отсюда
    i=strtol(pval,&ep,10)*100;
    pval=ep;
    if ((*pval=='.')||(*pval==','))
    {
      pval++;

      int j=strtol(pval, &ep, 10);//надо домножить число после запятой...
      int k=ep-pval;//... на 10^(2-k)
      if (k==1)
       j*=10;
      else
       while (k>2) {//делим j на 10^(k-2)
        j/=10; k--;
       }//ибо, бывают ещё балансы в у.е. с 3-4 знаками после запятой. Сам видел у МТСа. Lost, 15.01.2008
      i+=j;

      pval=ep;
    }
    if(i>(CurrentCASH+(MaxCASH/100))) //Если новый больше чем текущий +1 процент от максимального
    {
      if(!n) MaxCASH=i; //устанавливаем максимальный баланс только по "+" паттерну
    }
    if(n==1) CurrentCASH=i*(-1); //отрицательный баланс
    else CurrentCASH=i;
    n++;
  }
  //while(n<CASH_SIZE);  Может быть ни одного пока идет поиск сети
  if(pval) SaveCash();
  return(n);
}

wchar_t *(*USSD_GetText)(void *msg)=NULL;
wchar_t *USSD_GetText_2020(void *msg) {return (msg?((wchar_t **)msg)[0x2C/4]:NULL);}
wchar_t *USSD_GetText_Other(void *msg) {return (msg?((wchar_t **)msg)[0x24/4]:NULL);}

void SetGetTextFunc()
{FUNCTION
  switch (GetChipID())
  {
  case 0x9900:
  case 0xE8:
  case 0xE9:
    USSD_GetText=USSD_GetText_2020;
    break;
  default:
    USSD_GetText=USSD_GetText_Other;
  }
}

static int OnReceiveUssd(void *data, BOOK *book)
{FUNCTION
  char *s1, *s2;
  wchar_t *ws;
  int len;
  int c;
  int i=0;
  if (ussdreq_sended)
  {
    ws=USSD_GetText(data);
    EndUSSDtimer();
    if (ws)
    {
      if ((len=wstrlen(ws)))
      {
        s1=s2=new char[len+3];
        while(c=*ws++)
        {
          c=char16to8(c);
          if (c<32) c='?';
          *s2++=c;
        }
        *s2=0;
        i=FindCash(s1,book);
        delete s1;
        if (i)  // Если что то нашли
        {
          DISP_OBJ *StBy_DispObj=GUIObject_GetDispObject((GUI*)SBY_GetStatusIndication(Find_StandbyBook()));
          DispObject_InvalidateRect(StBy_DispObj,0);
        }
      }
    }
    StartHoursTimer();
  }
  return(i?666:1);
}

static int TerminateElf(void *, BOOK *book)
{FUNCTION
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
}

static int OnTurnOff(void *, BOOK *book)
{FUNCTION
  FreeBook(book);
  UI_CONTROLLED_SHUTDOWN_RESPONSE(BookObj_GetBookID(book));
  return(1);
}

int ShowAuthorInfo(void *mess, BOOK *book)
{FUNCTION
  MessageBox(EMPTY_SID,Str2ID(LELFNAME LELFVERSION,0,SID_ANY_LEN),NOIMAGE, 1 ,10000, 0);
  return(1);
}

static void onMyBookClose(BOOK *book)
{FUNCTION
  myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,0);
  myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,0);

  ModifyUIPageHook(USSD_RECIEVED_EVENT,OnReceiveUssd,BookObj_GetBookID(book),0);

  if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
  if(Status_desc) DISP_DESC_SetOnRedraw(Status_desc, Status_oldReDraw);
  if(Soft_desc) DISP_DESC_SetOnRedraw(Soft_desc, Soft_oldReDraw);

  EndUSSDtimer();
  Timer_Kill(&hours_tmr);
  if (pFont) pFont->Release();
  SaveCash();
  SUBPROC(elf_exit);
}

static int OnCallManager(void *mess, BOOK *book)
{FUNCTION
  CALLMANAGER_EVENT_DATA_A2 *calldata=(CALLMANAGER_EVENT_DATA_A2 *)mess;
  switch(calldata->CallState)
  {
  case 0:  // Завершили вызов выходим
    if (state==3 && on_call == 1)  SendCashReq();
    state=0;
    incom=0;
    break;
  case 2:  // Звоним куда то
    state=1;
    break;
  case 1:  // Дозвонились
    if (state==1) state=2;
    break;
  case 6:  // Закончили вызов
    if (state==2) state=3;
    if(incom == 1 && on_con == 1) SendCashReq();
    incom=0;
    break;
  }
  return(1);
}

static int onSmsSent(void *mess, BOOK *book)
{FUNCTION
  if(on_sms == 1) SendCashReq();
  return (0);
}

static int onMmsSent(void *mess, BOOK *book)
{FUNCTION
  if(on_mms == 1) SendCashReq();
  return (0);
}

static int GPRS_Terminated(void *mess, BOOK *book)
{FUNCTION
  if(on_net == 1) SendCashReq();
  return(0);
}

static int onConnect(void *mess, BOOK *book)
{FUNCTION
  if(incom == 0) incom=1;
  return(0);
}

void InitVar()
{FUNCTION
  DisplayWidth  = Display_GetWidth(0);
  DisplayHeight = Display_GetHeight(0);
  StatusBarY    = DispObject_GetWindowHeight(*StatusRow_p());
  SoftBarY      = DisplayHeight - DispObject_GetWindowHeight(DispObject_SoftKeys_Get());
}

static int onReconfigElf(void *mess, BOOK *book)
{FUNCTION
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

int onBcfgConfig(void *mess, BOOK *book)
{FUNCTION
  FSTAT _fstat;
  wchar_t path[256];

  if(!fstat(GetDir(DIR_ELFS|MEM_INTERNAL),L"BcfgEdit.elf",&_fstat))
    wstrcpy(path,GetDir(DIR_ELFS|MEM_INTERNAL));
  else if(!fstat(GetDir(DIR_ELFS|MEM_EXTERNAL),L"BcfgEdit.elf",&_fstat))
    wstrcpy(path,GetDir(DIR_ELFS|MEM_EXTERNAL));
  else
  {
    MessageBox(EMPTY_SID, STR("BcfgEdit.elf не найден!"), 0, 1 ,10000, 0);
    return(1);
  }
  wstrcat(path,L"/BcfgEdit.elf");
  elfload(path,(void *)successed_config_path,(void *)successed_config_name,0);
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
  ON_CALLMANAGER_EVENT_TAG,                           OnCallManager,
  ONGOINGCALL_CALL_CONNECTED_EVENT_TAG ,              onConnect,
  UI_CONTROLLED_SHUTDOWN_REQUESTED_EVENT_TAG,         OnTurnOff,
  ELF_BCFG_CONFIG_EVENT,                              onBcfgConfig,
  SBY_REDRAW_RELEASE_EVENT,                           SB_ELF_Killed,
  NIL_EVENT_TAG,                                      NULL
};

const PAGE_DESC bk_base = {"LiveBalanceMod_Base_Page",0,bk_msglst_base};

int main(void)
{FUNCTION
  if(FindBook(isLiveBalanceBook))
  {
    MessageBox(EMPTY_SID, STR("LiveBalanceMod\nуже запущен!"), NOIMAGE, 1, 10000, 0);
    SUBPROC(elf_exit);
  }
  else
  {
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
      DISP_OBJ *GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
      Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
      Display_desc = DispObject_GetDESC (GUI_display);
      DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
    }

    myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,1);
    myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,1);

    ModifyUIPageHook(USSD_RECIEVED_EVENT,OnReceiveUssd,BookObj_GetBookID(book),1);
    StartHoursTimer();
  }
  return(0);
}
