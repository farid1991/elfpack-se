#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

LIST *skl;
typedef struct
{
  u16 action;
  u16 unk;
  BOOK * book;
  DISP_OBJ * DISP_OBJ1;
  u16 unk2;
  u16 unk3;
  int text;
  int text_mini;
  int text_disabled;
  int text_help;
  u16 parent_action;
  u16 unk4;
  LIST * list;
  void (*proc)(BOOK *,GUI*);
  char unk5[12];
  char enable;
  char visible;
  char unk6[6];
}SOFTKEY;

SOFTKEY*sk;

void (*action_std)(BOOK *,GUI*);

int getelemindex(LIST *lst, int act)
{
  int x;
  for (x=0;x<lst->FirstFree;x++)
  {
    SOFTKEY *sk=(SOFTKEY*)List_Get(lst,x);
    if (sk->action==act)
    {
      return x;
    }
  }
  return -1;
};


void MenuAction(BOOK *bk,GUI*r1)
{
  if(FindBook(isMyBook))
  {
    if(!mbk->menu)
    {
      MENU_CREATE_GUI();
    }
    BookObj_Show((BOOK*)mbk,0);
  }
  else Shortcut_Run(L"MainMenu");

}
void Softkey_SetAction(int mode)
{
  wchar_t act=4;
  GUI*gui=SBY_GetMainInput(Find_StandbyBook());
  skl=DispObject_SoftKeys_GetList(GUIObject_GetDispObject(gui),Find_StandbyBook(),0);
  if(IS_JOY)
  {
    act=0xFFF;
  }
  else
  {
    if(Platform == 200)
    {
      act=1003;
    }
    else
    {
      act=4;
    }
  }
  int i=getelemindex(skl, act);
  sk=(SOFTKEY*)List_Get(skl,i);
  if(mode==0)
  {
    sk->proc=action_std;
  }
  else
  {
    action_std=sk->proc;
    sk->proc=MenuAction;
  }
}
