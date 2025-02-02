#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
#include "CFile.h"

wchar_t space[]=L" ";

void Menu_Free(MENU*menu)
{
  if (menu->list)
  {
    while (menu->list->FirstFree)
    {
      ITEM* it=(ITEM*)List_RemoveAt(menu->list,0);
      Icon_Unreg(it,0);
      Icon_Unreg(it,1);
      DELETE(it->name);
      DELETE(it->link);
      DELETE(it->img);
      DELETE(it->simg);
      Menu_Free(&it->menu);
      DELETE(it);
    }
    LIST_FREE(menu->list);
    IDFREE(menu->cursorID);
  }
  DELETE(menu->cstyle);
  DELETE(menu->cursor_img);
  menu->last=0;
  menu->style=0;

}


//сигнатура
#define SIGN 0x7
//текущая версия конфига
#define VERSION 0x1

char digit[]="%d";
wchar_t separator=L'\n';

void MenuItem_Read(wchar_t*_buffer,W_FSTAT _fstat,int*x,MENU*menu)
{
  menu->list=List_Create();
  menu->style=0;
  menu->last=0;
  int cnt;
  int max=0;
  menu->cursorID=NOIMAGE;
  max=GetInt(_buffer,x,separator);
  menu->style_type=GetInt(_buffer,x,separator);
  menu->style=GetInt(_buffer,x,separator);
  menu->is_sicon=GetInt(_buffer,x,separator);
  menu->gui_style=GetInt(_buffer,x,separator);
  menu->cstyle=GetWchar(_buffer,x,separator);
  menu->cursor_img=GetWchar(_buffer,x,separator);
  menu->last=GetInt(_buffer,x,separator);
  for(cnt=0;cnt<max;cnt++)
  {
    ITEM *item=new ITEM;
    memset(item,0,sizeof(ITEM));
    item->name=GetWchar(_buffer,x,separator);
    item->link=GetWchar(_buffer,x,separator);
    item->img=GetWchar(_buffer,x,separator);
    item->simg=GetWchar(_buffer,x,separator);
    item->type=GetInt(_buffer,x,separator);
    Icon_Reg(item,item->img,0);
    Icon_Reg(item,item->simg,1);
    if(item->type==3)
    {
      MenuItem_Read(_buffer,_fstat,x,&item->menu);
    }
    List_InsertLast(menu->list, item);
  }
}

void Menu_Read()
{
  wchar_t * path=GetPath_Menu();
  CFile * file=new CFile(path);
  int position=0;
  if(file->Stat_Result()==0)
  {
    if(file->Open(WA_Read)>=0)
    {
      wchar_t * buf=file->ReadUTF8();
      if((buf[0]==SIGN)&&(buf[1]==VERSION))
      {
        position=3;
        MenuItem_Read(buf,file->FStat(),&position,Menu_Get());
      }
      else
      {
        MESSAGEBOX(STR("Error reading skin!"));
      }
      DELETE(buf);
      file->Close();
    }
  }
  DELETE(path);
  DELETE(file);
  Menu_Check();
}
void MenuItem_Save(MENU menu,CFile * file)
{
  file->WriteNum(menu.list->FirstFree);
  file->Write(&separator,1);

  file->WriteNum(menu.style_type);
  file->Write(&separator,1);

  file->WriteNum(menu.style);
  file->Write(&separator,1);

  file->WriteNum(menu.is_sicon);
  file->Write(&separator,1);

  file->WriteNum(menu.gui_style);
  file->Write(&separator,1);

  if(menu.cstyle)
  {
    file->WriteUTF8(menu.cstyle);
  }
  else
  {
    file->WriteUTF8(names[2]);
  }
  file->Write(&separator,1);
  if(menu.cursor_img)
  {
    file->WriteUTF8(menu.cursor_img);
  }
  else
  {
    file->WriteUTF8(names[2]);
  }
  file->Write(&separator,1);

  file->WriteNum(menu.last);
  file->Write(&separator,1);

  int i;
  for(i=0;i<menu.list->FirstFree;i++)
  {
    if(menu.list)
    {
      ITEM *it=(ITEM *)List_Get(menu.list, i);

      if(it->name)
        file->WriteUTF8(it->name);
      else
        file->WriteUTF8(names[2]);

      file->Write(&separator,1);

      if(it->link)
        file->WriteUTF8(it->link);
      else
        file->WriteUTF8(names[2]);

      file->Write(&separator,1);

      if(it->img)
        file->WriteUTF8(it->img);
      else
        file->WriteUTF8(names[2]);

      file->Write(&separator,1);
      if(it->simg)
        file->WriteUTF8(it->simg);
      else
        file->WriteUTF8(names[2]);

      file->Write(&separator,1);
      file->WriteNum(it->type);
      file->Write(&separator,1);
      if(it->type==3)
      {
        MenuItem_Save(it->menu,file) ;
      }
    }
  }
}
void Menu_Save()
{
  wchar_t*path=GetPath_Menu();
  CFile * file=new CFile(path);
  if(file->Open(WA_Read+WA_Write+WA_Create+WA_Truncate)>=0)
  {
    if(history)
    {
      //запоминаем положение курсора
      //Menu_Get()->last=GET_SELECTED();
      file->WriteUTF8_BOM();
      char sign[3];
      sign[0]=SIGN;
      sign[1]=VERSION;
      sign[2]='\n';
      file->Write(sign,3);
      ITEM*item=(ITEM*)List_Get(history, 0);
      MenuItem_Save(item->menu,file);
    }

    file->Close();
  }
  DELETE(file);
  DELETE(path);
}

void Menu_Copy(MENU*dest,MENU src)
{
  Menu_Free(dest);
  dest->style=0;
  dest->last=0;
  dest->style_type=0;
  dest->gui_style=0;
  dest->cstyle=mstr(src.cstyle);
  dest->cursor_img=mstr(src.cursor_img);
  wstrcpy(dest->cstyle,src.cstyle);
  wstrcpy(dest->cursor_img,src.cursor_img);
  if(src.list)
  {
    dest->list=List_Create();
    for(int i=0;i<src.list->FirstFree;i++)
    {
      ITEM*it=(ITEM *)List_Get(src.list, i);

      ITEM *item=new ITEM;
      memset(item,0,sizeof(ITEM));
      item->name=mstr(it->name);
      item->link=mstr(it->link);
      item->img=mstr(it->img);
      item->simg=mstr(it->simg);
      item->type=it->type;
      Icon_Reg(item,item->img,0);
      Icon_Reg(item,item->simg,1);
      if(it->type==3)Menu_Copy(&item->menu,it->menu);
      List_InsertLast(dest->list, item);
    }
  }
  if(src.style>0)dest->style=src.style;
  if(src.gui_style>0)dest->gui_style=src.gui_style;
  if(src.last>0)dest->last=src.last;
  if(src.style_type>0)dest->style_type=src.style_type;
}

void Menu_Item_Move()
{
  int selected=GET_SELECTED();
  if(selected!=move_index)
  {
    if(!IS_MOVE)
    {
      ITEM *item=(ITEM *)List_RemoveAt(Menu_Get()->list,move_index);
      List_Insert(Menu_Get()->list, selected, item);
    }
    else
    {
      /*
      [0]->[1] && [1]->[0]
      [2]=[0];
      [0]=[1];
      [1]=[2];
      берем выделенный пункт, удаляем из списка
      берем перемещаемый пункт, удаляем из списка и вставляем на место выделенного, а на место перемещаемого - выделенный
      */
      if(move_index<selected)
      {
        ITEM *it=(ITEM *)List_RemoveAt(Menu_Get()->list,selected);
        ITEM *it2=(ITEM *)List_RemoveAt(Menu_Get()->list,move_index);
        List_Insert(Menu_Get()->list, move_index, it);
        List_Insert(Menu_Get()->list, selected, it2);
      }
      else
      {
        ITEM *it2=(ITEM *)List_RemoveAt(Menu_Get()->list,move_index);
        ITEM *it=(ITEM *)List_RemoveAt(Menu_Get()->list,selected);
        List_Insert(Menu_Get()->list, selected, it2);
        List_Insert(Menu_Get()->list, move_index, it);
      }
      /*
      //скопировали выделенный пункт в буффер item
      ITEM*item=new ITEM;
      memset(item,0,sizeof(ITEM));
      item->name=mstr(it->name);
      item->link=mstr(it->link);
      item->img=mstr(it->img);
      item->simg=mstr(it->simg);
      item->type=it->type;
      if(it->type==3)Menu_Copy(&item->menu,it->menu);
      Icon_Reg(item,item->img,0);
      Icon_Reg(item,item->simg,1);
      //очистили выделенный пункт
      Icon_Unreg(it,0);
      Icon_Unreg(it,1);
      DELETE(it->name);
      DELETE(it->link);
      DELETE(it->img);
      DELETE(it->simg);
      Menu_Free(&it->menu);
      //скопировали перемещаемый пункт в выделенный
      ITEM *it2=(ITEM *)List_Get(Menu_Get()->list, move_index);
      it->name=mstr(it2->name);
      it->link=mstr(it2->link);
      it->img=mstr(it2->img);
      it->simg=mstr(it2->simg);
      it->type=it2->type;
      if(it2->type==3)Menu_Copy(&it->menu,it2->menu);
      Icon_Reg(it,it->img,0);
      Icon_Reg(it,it->simg,1);
      //очистили перемещаемый пункт
      Icon_Unreg(it2,0);
      Icon_Unreg(it2,1);
      DELETE(it2->name);
      DELETE(it2->link);
      DELETE(it2->img);
      DELETE(it2->simg);
      Menu_Free(&it2->menu);
      //скопировали буффер в перемещаемый пункт
      it2->name=mstr(item->name);
      it2->link=mstr(item->link);
      it2->img=mstr(item->img);
      it2->simg=mstr(item->simg);
      it2->type=item->type;
      if(item->type==3)Menu_Copy(&it2->menu,item->menu);
      Icon_Reg(it2,it2->img,0);
      Icon_Reg(it2,it2->simg,1);
      //удалили буффер
      Icon_Unreg(item,0);
      Icon_Unreg(item,1);
      DELETE(item->name);
      DELETE(item->link);
      DELETE(item->img);
      DELETE(item->simg);
      Menu_Free(&item->menu);
      DELETE(item);*/
    }
  }
}
