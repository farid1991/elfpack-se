#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

void SetCursor(int to)
{
  if(Menu_Get()->style_type==0)
  {
    ListMenu_SetCursorToItem(mbk->menu,to);
  }
  else cursor=to;
}

void HotKey(int i1)
{
  if((i1>=KEY_DIGITAL_0)&&(i1<=KEY_DIEZ))
  {
    if(i1==KEY_DIGITAL_0)
    {
      if(Menu_Get()->list->FirstFree>10)
      {
        SetCursor(10);
        Link_Execute();
      }
    }
    else if(i1==KEY_STAR)
    {
      if(Menu_Get()->list->FirstFree>9)
      {
        SetCursor(9);
        Link_Execute();
      }
    }
    else if(i1==KEY_DIEZ)
    {
      if(Menu_Get()->list->FirstFree>11)
      {
        SetCursor(11);
        Link_Execute();
      }
    }
    else
    {
      if(Menu_Get()->list->FirstFree>(i1-KEY_DIGITAL_0-1))
      {
        SetCursor(i1-KEY_DIGITAL_0-1);
        Link_Execute();
      }
    }
  }
}

void myOnKey(void *p, int i1, int i2, int i3, int i4)
{
  void (*OldOnKey)(void *, int, int, int, int);
  OldOnKey=(void(*)(void *,int,int,int,int))mbk->oldOnKey;
  OldOnKey(p,i1,i2,i3,i4);
  if ((i4==KBD_SHORT_RELEASE))
  {
    HotKey(i1);
  }
  if ((i4==KBD_SHORT_RELEASE)||(i4==KBD_REPEAT))
  {
    if((i1==KEY_LEFT)||(i1==KEY_RIGHT)||(i1==KEY_UP)||(i1==KEY_DOWN))
    {
      if(!move)
      {
        //MENU mn=Menu_Get();
        Menu_Get()->last=ListMenu_GetSelectedItem(mbk->menu);
        if((IS_DYNTITLE)||((!IS_DYNTITLE)&&(history->FirstFree==1)&&(IS_MAIN_DYNTITLE)))
        {
          Menu_TitleRefresh();
        }
      }
      else
      {
        Book_Refresh();
      }
      if(VIBRA)AudioControl_Vibrate(pAC,70,70,70);
    }
  }
}
void myOnKey3(void *p, int i1, int i2, int i3, int i4)
{
  void (*OldOnKey)(void *, int, int, int, int);
  OldOnKey=(void(*)(void *,int,int,int,int))mbk->oldOnKey2;
  OldOnKey(p,i1,i2,i3,i4);
  if ((i4==KBD_SHORT_RELEASE))
  {
    if(i1==KEY_DIGITAL_0+1)
    {
      ListMenu_SetCursorToItem(mbk->edit,0);
      EditOnEnterPressed((BOOK*)mbk, 0);
    }
    if(i1==KEY_DIGITAL_0+2)
    {
      ListMenu_SetCursorToItem(mbk->edit,1);
      EditOnEnterPressed((BOOK*)mbk, 0);
    }
    if(i1==KEY_DIGITAL_0+3)
    {
      ListMenu_SetCursorToItem(mbk->edit,2);
      EditOnEnterPressed((BOOK*)mbk, 0);
    }
    if(i1==KEY_DIGITAL_0+4)
    {
      ListMenu_SetCursorToItem(mbk->edit,3);
      EditOnEnterPressed((BOOK*)mbk, 0);
    }
    if(i1==KEY_DIGITAL_0+5)
    {
      if(Menu_CheckStyle())
      {
        ListMenu_SetCursorToItem(mbk->edit,4);
        EditOnEnterPressed((BOOK*)mbk, 0);
      }
    }
  }
}
