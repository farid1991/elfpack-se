#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"


wchar_t * cur_title;
int move=0;
int move_index=0;

void Item_Init(ITEM * it, int type)
{
  it->name=   mstr(names[2]);;
  it->img=    mstr(names[2]);;
  it->simg=   mstr(names[2]);
  it->link=   mstr(names[2]);
  it->image=  NOIMAGE;
  it->simage= NOIMAGE;
  it->type=   type;
}

void Menu_Init(MENU * menu)
{
  menu->list=       List_Create();
  menu->style=      NULL;
  menu->style_type= NULL;
  menu->gui_style=  NULL;
  menu->is_sicon=   NULL;
  menu->last=       NULL;
  menu->cstyle=     mstr(names[2]);
  menu->cursor_img= mstr(names[2]);
  menu->cursorID=   NOIMAGE;
}
//функция возвращает текущее меню (всякая рекурсия, из-за которой сплошные баги, маст дай)
//ведь в сто раз лучше, пусть даже чуть чуть сложнее (?)
MENU * Menu_Get()
{
  ITEM*item=(ITEM*)List_Get(history, history->FirstFree-1);
  return(&item->menu);
}

int GET_SELECTED()
{
  int res=0;
  if(Menu_Get()->style_type==0)
  {
    res=ListMenu_GetSelectedItem(mbk->menu);
  }
  else
  {
    res=cursor;
  }
  return(res);
}

ITEM * Menu_GetSelected()
{
  ITEM *it=(ITEM *)List_Get(Menu_Get()->list, GET_SELECTED());
  return(it);
}


void Menu_Check()
{
  if(!Menu_Get()->list)
  {
    MESSAGEBOX(STR("Warning! List is undefined!"));
    Menu_Create();
  }
  if(Menu_Get()->last>=Menu_Get()->list->FirstFree)
  {
    MESSAGEBOX(STR("Warning! Index of last selected item bigger than number of elements"));
    Menu_Get()->last=0;
  }
  if(Menu_Get()->list->FirstFree==0 && Menu_Get()->list)
  {
    MESSAGEBOX(STR("Warning! List is empty!"));
    ITEM *item=new ITEM;
    memset(item,0,sizeof(ITEM));
    Item_Init(item,0);
    List_InsertLast(Menu_Get()->list, item);
  }
}

void Menu_Create()
{
  ITEM *item=new ITEM;
  memset(item,0,sizeof(ITEM));
  Item_Init(item, 0);
  Menu_Init(Menu_Get());
  List_InsertLast(Menu_Get()->list, item);
}

int Menu_CheckStyle()
{
  return(Menu_Get()->is_sicon);
}
