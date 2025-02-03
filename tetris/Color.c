//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Menu.h"
#include "MyFUNC.h"
#include "rand.h"
#include "NewRecord.h"
#include "Pages.h"
#include "Save.h"

int col,xs,ys,newcol,numb;
int ColorsTab[4][4]=
{
  {0xff000000, 0xff0000ff, 0xff0080ff, 0xff8000ff},
  {0xff808080, 0xff00ff00, 0xff00ff80, 0xff80ff00},
  {0xffcccccc, 0xffff0000, 0xffff0080, 0xffff8000},
  {0xffffffff, 0xffffff00, 0xffff00ff, 0xff00ffff}
};

typedef GUI GUI_CUSTOM;
DISP_DESC *pdd;
GUI_CUSTOM*pnp;

void DrawSel(int x, int y)
{
  int ID;
  int cub=MyBK()->dis[GetDis()]->x/4;
  iconidname2id(L"CHECKMARK_IN_BOX_ICN", -1, &ID);
  putchar(get_DisplayGC() ,x*cub, y*cub , 0, 0, ID);
}

void DrawCol(int x, int y, int color)
{
  int cub=MyBK()->dis[GetDis()]->x/4;
  DrawRect(x*cub, y*cub, x*cub+cub,  y*cub+cub,0xff000000, color);
}


void ColorOnRedraw(DISP_OBJ *DO, int a, int b, int c)
{
  DrawRect(0,0,MyBK()->dis[GetDis()]->x,MyBK()->dis[GetDis()]->y, 0xFF000000, 0xFF000000);
  for(int i=0; i<4; i++)
  {
    for(int j=0; j<4; j++)
    {
      DrawCol(i,j,ColorsTab[i][j]);
    }
  }
  DrawSel(xs,ys);
}

static const char pcustom_guiname[]="gui_custom";

int pcustom_onCreate(DISP_OBJ *DO)
{
  return 1;
};

void pcustom_onClose(DISP_OBJ *DO)
{
  return;
};

void pcustom_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,pcustom_guiname);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)pcustom_onCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)pcustom_onClose);
};

void pcustom_destr(GUI *desc)
{
  return;
};

void OnEnterCol(BOOK * bk, GUI *)
{
  MyBK()->nast->colors[numb]=ColorsTab[xs][ys];
  GUI_Free((GUI*)pnp);
  CreateColorsMenu(numb+1);
}

void OnExitCol(BOOK * bk, GUI *)
{
  GUI_Free((GUI*)pnp);
  CreateColorsMenu(numb+1);
}

GUI_CUSTOM *pcustom_create(BOOK *bk)
{
  GUI_CUSTOM *gui_cs=new GUI_CUSTOM;
  if (!CreateObject((GUI*)gui_cs,pcustom_destr, pcustom_constr,bk,0,0,0))
  {
    delete gui_cs;
    return 0;
  }
  if (bk) addGui2book(bk,(GUI*)gui_cs);
  GUI_SetStyle((GUI*)gui_cs, 4);
  GuiObject_SetTitleType((GUI*)gui_cs, 1);
  GUIObject_Softkey_SetText((GUI*)gui_cs, ACTION_SELECT1, Str2ID(L"Выбор",ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_Softkey_SetAction((GUI*)gui_cs, ACTION_SELECT1, OnEnterCol);
  GUIObject_Softkey_SetAction((GUI*)gui_cs, ACTION_BACK, OnExitCol);
  //GUIObject_HideSoftkeys((GUI*)gui_cs);
  return gui_cs;
};

void ColorOnKey(DISP_OBJ *db,int key,int a,int b,int type)
{
  if(key==KEY_ESC && type==KBD_SHORT_RELEASE)
  {
    GUI_Free((GUI*)pnp);
    CreateColorsMenu(numb+1);
    return;
  }
  if(key==KEY_LEFT && type==KBD_SHORT_RELEASE) if(xs>0) xs+=-1; else xs=3;
  if(key==KEY_RIGHT && type==KBD_SHORT_RELEASE) if(xs<3) xs++; else xs=0;
  if(key==KEY_UP && type==KBD_SHORT_RELEASE) if(ys>0) ys+=-1; else ys=3;
  if(key==KEY_DOWN && type==KBD_SHORT_RELEASE) if(ys<3) ys++; else ys=0;
  DispObject_InvalidateRect(GUIObject_GetDispObject(pnp),0);
}

int ColorChange(int ncolor, BOOK*bk)
{
  numb=ncolor;
  col=MyBK()->nast->colors[numb];
  xs=0;
  ys=0;
  for(int i=0; i<4; i++)
  {
    for(int j=0; j<4; j++)
    {
      if(col==ColorsTab[i][j])
      {
        xs=i;
        ys=j;
      }
    }
  }
  pnp=pcustom_create(bk);
  pdd=DISP_OBJ_GetDESC (GUIObject_GetDispObject(pnp));
  DISP_DESC_SetOnKey(pdd, ColorOnKey);
  DISP_DESC_SetOnRedraw(pdd, ColorOnRedraw);
  ShowWindow((GUI*)pnp);
  BookObj_Show(bk, 0);
  BookObj_SetFocus(bk, 0);
  return 0;
}

