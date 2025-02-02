#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

#pragma segment="ELFBEGIN"
void elf_exit(void){
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}

#define ELF_BCFG_CONFIG_EVENT 994

#define COUNT_IMAGES 11

enum R1_DispObject_Show
{
  hide,
  show
};

typedef struct _DISP_OBJ_MCLOCK : DISP_OBJ
{
  wchar_t *ImagesID_cur;
  char hour;
  char min;
  char hour_old;
  char min_old;
  int cnt_hour;
  int cnt;
}DISP_OBJ_MCLOCK;

typedef struct
{
  int i_w;
  int i_h;
  int x;
  int y;
}IMGCOORD;

u16 timer,draw_timer,draw_hour_timer;

int TerminateElf(void * ,BOOK *book)
{
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK *book)
{
  MessageBox(EMPTY_TEXTID,STR("Manila Clock\n\n(c) Joker XT"), NOIMAGE, 1 ,10000,0);
  return(1);
}

static const char MyGuiName[]="MClockGUI";

GUI *BookObj_GetGUIbyName(BOOK *b, char *my_gui_name)
{
  for(int i=0;i<(b->xguilist->guilist->FirstFree);i++)
  {
    GUI *gui = (GUI *)List_Get(b->xguilist->guilist,i);
    char *cur_name = DispObject_GetName(GUIObject_GetDispObject(gui));
    if(!strcmp(cur_name,my_gui_name)) return(gui);
  }
  return(0);
}

wchar_t RegisterImage(wchar_t *path, wchar_t *fname)
{
  IMAGEID ImageID = NOIMAGE;
  if(!fstat(path,fname,0))
    if(ImageID_Get(path,fname,&ImageID)<0) ImageID = NOIMAGE;
  return(ImageID);
}

void _RegisterImage_(DISP_OBJ_MCLOCK *db, int num, wchar_t *name)
{
  if(db->ImagesID_cur[num] != NOIMAGE) ImageID_Free(db->ImagesID_cur[num]);
  db->ImagesID_cur[num] = RegisterImage((wchar_t *)IMAGE,name);
}

void onDrawTimer_hour(u16 unk , LPARAM data)
{
  DISP_OBJ_MCLOCK *db = (DISP_OBJ_MCLOCK *)data;

  db->cnt_hour++;
  if(db->cnt_hour == 10)
  {
    ImageID_Free(db->ImagesID_cur[6]);
    ImageID_Free(db->ImagesID_cur[7]);
    db->ImagesID_cur[6] = NOIMAGE;
    db->ImagesID_cur[7] = NOIMAGE;
    db->hour_old = db->hour;
  }
  else
  {
    wchar_t ws[256];

    snwprintf(ws,255,L"bg_clock_assets_000%d.png",db->cnt_hour);
    _RegisterImage_(db,0,ws);

    //верхние части цифр
    snwprintf(ws,255,L"under_%d_assets_000%d.png",db->hour/10,db->cnt_hour);
    _RegisterImage_(db,1,ws);

    snwprintf(ws,255,L"under_%d_assets_000%d.png",db->hour%10,db->cnt_hour);
    _RegisterImage_(db,2,ws);

    //нижние части цифр
    int num;
    if(db->hour/10 == db->hour_old/10) num = db->hour/10;
    else num = db->hour_old/10;

    snwprintf(ws,255,L"up_%d_assets_000%d.png",num,db->cnt_hour);
    _RegisterImage_(db,6,ws);

    if(db->hour%10 == db->hour_old%10) num = db->hour%10;
    else num = db->hour_old%10;

    snwprintf(ws,255,L"up_%d_assets_000%d.png",num,db->cnt_hour);
    _RegisterImage_(db,7,ws);

    Timer_ReSet(&draw_hour_timer,TIME_R,onDrawTimer_hour,data);
  }
}

void onDrawTimer(u16 unk , LPARAM data)
{
  DISP_OBJ_MCLOCK *db = (DISP_OBJ_MCLOCK *)data;

  db->cnt++;
  if(db->cnt == 10)
  {
    ImageID_Free(db->ImagesID_cur[8]);
    ImageID_Free(db->ImagesID_cur[9]);
    db->ImagesID_cur[8] = NOIMAGE;
    db->ImagesID_cur[9] = NOIMAGE;
    db->min_old = db->min;
  }
  else
  {
    wchar_t ws[256];

    snwprintf(ws,255,L"bg_clock_assets_000%d.png",db->cnt);
    _RegisterImage_(db,3,ws);

    //верхние части цифр
    snwprintf(ws,255,L"under_%d_assets_000%d.png",db->min/10,db->cnt);
    _RegisterImage_(db,4,ws);

    snwprintf(ws,255,L"under_%d_assets_000%d.png",db->min%10,db->cnt);
    _RegisterImage_(db,5,ws);

    //нижние части цифр
    int num;
    if(db->min/10 == db->min_old/10) num = db->min/10;
    else num = db->min_old/10;

    snwprintf(ws,255,L"up_%d_assets_000%d.png",num,db->cnt);
    _RegisterImage_(db,8,ws);

    if(db->min%10 == db->min_old%10) num = db->min%10;
    else num = db->min_old%10;

    snwprintf(ws,255,L"up_%d_assets_000%d.png",num,db->cnt);
    _RegisterImage_(db,9,ws);

    Timer_ReSet(&draw_timer,TIME_R,onDrawTimer,data);
  }
}

void onTimer(u16 unk , LPARAM data)
{
  BOOK *stby = Find_StandbyBook();
  if((Display_GetTopBook(0) == stby)||(Display_GetTopBook(0) == FindBook(get_IsScreenSaverBook())))
  {
    DISP_OBJ_MCLOCK *db = (DISP_OBJ_MCLOCK *)data;
    DispObject_InvalidateRect((DISP_OBJ *)db,0);

    DATETIME dt;
    REQUEST_DATEANDTIME_GET(0,&dt);

    if(db->hour != dt.time.hour)
    {
      db->cnt_hour = 0;
      db->hour = dt.time.hour;
      Timer_ReSet(&draw_hour_timer,1,onDrawTimer_hour,data);
    }
    if(db->min != dt.time.min)
    {
      db->cnt = 0;
      db->min = dt.time.min;
      Timer_ReSet(&draw_timer,1,onDrawTimer,data);
    }
  }
  Timer_ReSet(&timer,100,onTimer,data);
}

void DestroyImage(wchar_t *ImgsID)
{
  for(int i=0;i<COUNT_IMAGES;i++)
  {
    if(ImgsID[i] != NOIMAGE) ImageID_Free(ImgsID[i]);
  }
}

void InitImage(DISP_OBJ_MCLOCK *db, wchar_t *img_path)
{
    DATETIME dt;
    REQUEST_DATEANDTIME_GET(0,&dt);

    db->hour = dt.time.hour;
    db->min = dt.time.min;
    db->hour_old = dt.time.hour;
    db->min_old = dt.time.min;

    if(BG) db->ImagesID_cur[10] = RegisterImage(img_path,L"hh_hm_clock_bg.png");
    else db->ImagesID_cur[10] = NOIMAGE;

    wchar_t ws[256];
    snwprintf(ws,255,L"up_%d_assets_0000.png",dt.time.hour/10);
    db->ImagesID_cur[0] = RegisterImage(img_path,L"bg_clock_assets_0000.png");
    db->ImagesID_cur[1] = RegisterImage(img_path,ws);
    snwprintf(ws,255,L"up_%d_assets_0000.png",dt.time.hour%10);
    db->ImagesID_cur[2] = RegisterImage(img_path,ws);
    db->ImagesID_cur[3] = RegisterImage(img_path,L"bg_clock_assets_0000.png");
    snwprintf(ws,255,L"up_%d_assets_0000.png",dt.time.min/10);
    db->ImagesID_cur[4] = RegisterImage(img_path,ws);
    snwprintf(ws,255,L"up_%d_assets_0000.png",dt.time.min%10);
    db->ImagesID_cur[5] = RegisterImage(img_path,ws);
    db->ImagesID_cur[6] = NOIMAGE;
    db->ImagesID_cur[7] = NOIMAGE;
    db->ImagesID_cur[8] = NOIMAGE;
    db->ImagesID_cur[9] = NOIMAGE;

}

static int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (!wstrcmpi(reconf->path,successed_config_path) && !wstrcmpi(reconf->name,successed_config_name))
  {
    InitConfig();
    DISP_OBJ_MCLOCK *dbmc = (DISP_OBJ_MCLOCK *)GUIObject_GetDispObject(BookObj_GetGUIbyName(Find_StandbyBook(),(char *)MyGuiName));
    DestroyImage(dbmc->ImagesID_cur);
    InitImage(dbmc,(wchar_t *)IMAGE);
    result=1;
  }
  return(result);
}

void GetCoordImage(IMGCOORD *crd, wchar_t ImgID, int x1, int y1)
{
  int w = crd->i_w = (GetImageWidth(ImgID)*scale)/100;
  int h = crd->i_h = (GetImageHeight(ImgID)*scale)/100;
  crd->x = x1 - w/2;
  crd->y = y1 - h/2;
}

void MyGuiOnRedraw(DISP_OBJ_MCLOCK *db,int ,int,int)
{
  if(!DispObject_GetVisible(GUIObject_GetDispObject(SBY_GetMainInput(Find_StandbyBook()))))
  {
    if((Display_GetTopBook(0) == Find_StandbyBook())||(Display_GetTopBook(0) == FindBook(get_IsScreenSaverBook())))
    {
      //if(db->ImageID != NOIMAGE)
      {
        GC *gc = get_DisplayGC();

        int sp[4];
        IMGCOORD *crd = (IMGCOORD *)sp;

        if(db->ImagesID_cur[10] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[10],X0,Y0);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[10]);
        }

        if(db->ImagesID_cur[0] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[0],X1,Y1);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[0]);
        }

        if(db->ImagesID_cur[3] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[3],X2,Y2);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[3]);
        }

        if(db->ImagesID_cur[1] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[1],X3,Y3);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[1]);
          if(db->ImagesID_cur[6] != NOIMAGE) GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[6]);
        }

        if(db->ImagesID_cur[2] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[2],X4,Y4);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[2]);
          if(db->ImagesID_cur[7] != NOIMAGE) GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[7]);
        }

        if(db->ImagesID_cur[4] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[4],X5,Y5);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[4]);
          if(db->ImagesID_cur[8] != NOIMAGE) GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[8]);
        }

        if(db->ImagesID_cur[5] != NOIMAGE)
        {
          GetCoordImage(crd,db->ImagesID_cur[5],X6,Y6);
          GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[5]);
          if(db->ImagesID_cur[9] != NOIMAGE) GC_PutChar(gc,crd->x,crd->y,crd->i_w,crd->i_h,db->ImagesID_cur[9]);
        }
      }
    }
  }
}

int MyGuiOnCreate(DISP_OBJ_MCLOCK *db)
{
  db->ImagesID_cur = new wchar_t[COUNT_IMAGES];
  InitImage(db,(wchar_t *)IMAGE);
  DispObject_Show(db,show);
  timer = Timer_Set(250,onTimer,(LPARAM)db);
  return(1);
}

void MyGuiOnClose(DISP_OBJ_MCLOCK *db)
{
  DestroyImage(db->ImagesID_cur);
  mfree(db->ImagesID_cur);
}

void MyGuiOnLayout(DISP_OBJ_MCLOCK *db)
{
  DispObject_SetLayerColor(db,0x00000000);
}

void MyGui_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,MyGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ_MCLOCK));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw);
  DISP_DESC_SetOnLayout(desc,(DISP_OBJ_ONLAYOUT_METHOD)MyGuiOnLayout);
}

void MyGui_destr(GUI *desc)
{
}

GUI *CreateGUI(BOOK *book)
{
  GUI *gui = new GUI;
  if(!GUIObject_Create(gui,MyGui_destr,MyGui_constr, book,0,0,0))
  {
    delete gui;
    return(0);
  }
  if(book) BookObj_AddGUIObject(book,gui);
  GUIObject_SetStyle(gui,UI_OverlayStyle_TrueFullScreen);
  GUIObject_SetTitleType(gui,UI_TitleMode_None);
  GUIObject_SoftKeys_Hide(gui);
  return(gui);
}

typedef struct
{
  BOOK* book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

#define FREE(a) if(a){mfree(a);a=0;}
wchar_t *bcfg;

int onBcfgConfig(void *mess, BOOK *book)
{
  MSG_BCFG *msg = (MSG_BCFG *)mess;
  FREE(bcfg);
  bcfg = new wchar_t[wstrlen(msg->bcfg_p)+1+wstrlen(msg->bcfg_n)+1];
  wstrcpy(bcfg,msg->bcfg_p);
  wstrcat(bcfg,L"/");
  wstrcat(bcfg,msg->bcfg_n);
  elfload(bcfg,(void *)successed_config_path,(void *)successed_config_name,0);
  return(1);
}

void onMyBookClose(BOOK * book)
{
  FREE(bcfg);
  if(timer) Timer_Kill(&timer);
  GUI *g = BookObj_GetGUIbyName(Find_StandbyBook(),(char *)MyGuiName);
  if(g) GUIObject_Destroy(g);
  SUBPROC(elf_exit);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT,        TerminateElf,
  ELF_SHOW_INFO_EVENT,        ShowAuthorInfo,
  ELF_RECONFIG_EVENT,         ReconfigElf,
  ELF_BCFG_CONFIG_EVENT,      onBcfgConfig,
  NIL_EVENT_TAG,              NULL
};

const PAGE_DESC bk_base = {"MClock_Base_Page",0,bk_msglst_base};

int main()
{
  bcfg = NULL;
  timer = NULL;
  BOOK *bk = new BOOK;
  memset(bk,0,sizeof(BOOK));
  if(CreateBook(bk,onMyBookClose,&bk_base,"ManilaClock",-1,0))
  {
    InitConfig();
    if(!CreateGUI(Find_StandbyBook())) FreeBook(bk);
  }
  else
  {
    mfree(bk);
    SUBPROC(elf_exit);
  }
  return(0);
}
