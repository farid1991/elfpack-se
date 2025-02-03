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
#include "Button.h"
wchar_t*butname1[9]={
#ifdef ENG_LANG
  L"Влево", L"Вправо", L"Ускорение", L"Мгновенное падение", L"Поворот по часовой", L"Поворот против часовой", L"Свернуть", L"Вкл/Откл звук", L"Пауза/Выход"
#else
  L"Left", L"Right", L"Speedup", L"Instant fall", L"Hour Tumbling", L"Tumbling counterclockwise", L"Hide", L"On/Off Sound", L"Pause/Exit"
#endif
};

int KeysID, ButtonID, OldButton, NewButton;
u16 ButtonTimer;

typedef GUI GUI_CUSTOM;
DISP_DESC *bdd;
GUI_CUSTOM*bnp;


void butOnRedraw(DISP_OBJ *DO, int a, int b, int c)
{
  int Font;
  if(TextGetLength(KeyCode2Name(NewButton))>5)
  {
    Font=MyBK()->dis[GetDis()]->FS;
  }
  else
  {
    Font=MyBK()->dis[GetDis()]->FP;
  }
  int x1, x2, y1, y2;
  SetFont(Font);
  DrawRect(0,0,MyBK()->dis[GetDis()]->x,MyBK()->dis[GetDis()]->y, 0xFF000000, 0xFF000000);
  x1=(MyBK()->dis[GetDis()]->x-TextGetLength(KeyCode2Name(NewButton))*GetFSize(Font))/2;
  x2=x1+TextGetLength(KeyCode2Name(NewButton))*GetFSize(Font);
  y1=(MyBK()->dis[GetDis()]->y-GetFSize(Font))/2;
  y2=y1+GetFSize(Font);
  DrawString(KeyCode2Name(NewButton),2,x1,y1,x2,y2,1,1, 0xFFFFFFFF, 0xFFFFFFFF);
  SetFont(Font=MyBK()->dis[0]->FS);
  DrawString(Str2ID(butname1[ButtonID], ENC_UCS2, TEXTID_ANY_LEN),2,0,0,MyBK()->dis[GetDis()]->x,MyBK()->dis[GetDis()]->y,1,1, 0xFFFF0000, 0xFFFF0000);


}

static const char bcustom_guiname[]="gui_custom";

int bcustom_onCreate(DISP_OBJ *DO)
{
  return 1;
};

void bcustom_onClose(DISP_OBJ *DO)
{
  return;
};

void bcustom_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,bcustom_guiname);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)bcustom_onCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)bcustom_onClose);
};

void bcustom_destr(GUI *desc)
{
  return;
};

GUI_CUSTOM *bcustom_create(BOOK *bk)
{
  GUI_CUSTOM *gui_cs=new GUI_CUSTOM;
  if (!CreateObject((GUI*)gui_cs,bcustom_destr, bcustom_constr,bk,0,0,0))
  {
    delete gui_cs;
    return 0;
  }
  if (bk) addGui2book(bk,(GUI*)gui_cs);
  GUI_SetStyle((GUI*)gui_cs, 4);
  GuiObject_SetTitleType((GUI*)gui_cs, 1);
  GUIObject_HideSoftkeys((GUI*)gui_cs);
  return gui_cs;
};

void ButTimer (u16 tmr , LPARAM)
{
  MyBK()->nast->buttons[KeysID][ButtonID]=NewButton;
  GUI_Free((GUI*)bnp);
  if(KeysID) CreateDigButtonsMenu(ButtonID);
  else CreateButtonsMenu(ButtonID);
  Timer_Kill(&ButtonTimer);
}

int CorrectKeys(int x)
{
  if(KeysID && (x<=(KEY_DIGITAL_0+9) && x>=KEY_DIGITAL_0 || x==KEY_STAR || x==KEY_DIEZ)) return true;
  if(!KeysID && (x==KEY_UP || x==KEY_DOWN || x==KEY_LEFT || x==KEY_RIGHT || x==KEY_ENTER || x==KEY_ESC || x==KEY_LEFT_SOFT || x==KEY_RIGHT_SOFT || x==KEY_DEL)) return true;
  return false;
}

void ButOnKey(DISP_OBJ *db,int key,int a,int b,int type)
{
  if(CorrectKeys(key))
  {
    NewButton=key;
    DispObject_InvalidateRect(GUIObject_GetDispObject(bnp),0);
  }
  Timer_ReSet(&ButtonTimer, 5000, ButTimer, 0);
}

int ColorChange(int ID[2], BOOK*bk)
{
  ButtonID=ID[0];
  KeysID=ID[1];
  OldButton=MyBK()->nast->buttons[KeysID][ButtonID];
  NewButton=OldButton;
  bnp=bcustom_create(bk);
  bdd=DISP_OBJ_GetDESC (GUIObject_GetDispObject(bnp));
  DISP_DESC_SetOnKey(bdd, ButOnKey);
  DISP_DESC_SetOnRedraw(bdd, butOnRedraw);
  ShowWindow((GUI*)bnp);
  BookObj_Show(bk, 0);
  BookObj_SetFocus(bk, 0);
  ButtonTimer=Timer_Set(5000, ButTimer, 0);
  return 0;
}
