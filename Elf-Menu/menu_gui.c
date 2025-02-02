#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

void MenuCreateGuiList(void * r0, BOOK * bk);

void Menu_TitleRefresh()
{
  if(Menu_Get()->style_type==0)
  {
  ITEM*it=Menu_GetSelected();
  int str=wstr2id(it->name);
  GUIObject_SetTitleText(mbk->menu,str);
  }
}

void MENU_CREATE_GUI()
{
  if(Menu_Get()->style_type==0)
  {
    MenuCreateGuiList(0, &mbk->book);
  }
  else
  {
    Style_Read(Menu_Get()->cstyle, Menu_Get());
    BookObj_CallPage(&mbk->book,&bk_GMenu);
  }
}

void Menu_Refresh()
{
  //Menu_Get()->last=GET_SELECTED();
  FREE_GUI(mbk->menu);
  FREE_GUI(mbk->gmenu);
  MENU_CREATE_GUI();
}

void MenuBack(BOOK *bk, GUI *lt)
{
  Menu_Get()->last=GET_SELECTED();
  if(!move)
  {
    if(history->FirstFree>1)
    {
      ITEM*ditem=(ITEM*)List_RemoveAt(history,history->FirstFree-1);
      Menu_Refresh();
    }
    else
    {
      FREE_GUI(mbk->menu);
      FREE_GUI(mbk->gmenu);
      BookObj_Hide((BOOK*)mbk,0);
    }
  }
  else
  {
    move=0;
    Book_Refresh();
  }
}



void Return_ToStandby()
{
  History_Get();
  FREE_GUI(mbk->menu);
  FREE_GUI(mbk->gmenu);
  BookObj_Hide((BOOK*)mbk,0);
}

void MenuOnEnterPressed(BOOK *bk, GUI *lt)
{
  Link_Execute();
}

int MenuOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    ITEM *it=(ITEM *)List_Get(Menu_Get()->list, item);
    TEXTID str=wstr2id(it->name);
    GUIonMessage_SetMenuItemText(msg,str);

    GUIonMessage_SetMenuItemIcon(msg,0,it->image);
    if(Menu_CheckStyle())
      GUIonMessage_SetMenuItemIcon(msg,1,it->simage);
    if(move)
    {
      if(item==ListMenu_GetSelectedItem(mbk->menu))
      {
        ITEM *it2=(ITEM *)List_Get(Menu_Get()->list, move_index);
        str=wstr2id(it2->name);
        GUIonMessage_SetMenuItemText(msg,str);
        GUIonMessage_SetMenuItemIcon(msg,0,it2->image);
        if(Menu_CheckStyle())
          GUIonMessage_SetMenuItemIcon(msg,1,it2->simage);
      }
    }
  }
  return(1);
};

void Add_Item(int type)
{
  if(!move)
  {
    if(Menu_Get()->list->FirstFree<100)
    {
      if(((Menu_CheckStyle())&&(Menu_Get()->list->FirstFree<12))||(!Menu_CheckStyle()))
      {
        ITEM *item=new ITEM;
        memset(item,0,sizeof(ITEM));
        Item_Init(item,type);
        List_InsertLast(Menu_Get()->list, item);
        ListMenu_SetItemCount(mbk->menu,Menu_Get()->list->FirstFree);
        Book_Refresh();
        Menu_Save();
        if(IS_EDITOR)
        {
          ListMenu_SetCursorToItem(mbk->menu,Menu_Get()->list->FirstFree-1);
          EditCreateGuiList(0,(BOOK*)mbk);
        }
        Menu_TitleRefresh();
      }
    }
  }

}

void MenuAdd(BOOK * book,GUI * lt)
{
  Add_Item(0);
}

void SubMenuAdd(BOOK * book, GUI * lt)
{
  Add_Item(3);
}

void OnYesExitGui(BOOK * bk, GUI *)
{
  FREE_GUI(mbk->g_del);
  int item=GET_SELECTED();
  if(Menu_Get()->list->FirstFree>1)
  {
    ITEM *it=(ITEM*)List_RemoveAt(Menu_Get()->list,item);
    Menu_Free(&it->menu);
    DELETE(it->name);
    DELETE(it->link);
    Icon_Unreg(it,0);
    Icon_Unreg(it,1);
    DELETE(it->img);
    DELETE(it->simg);
    DELETE(it);
    if(Menu_Get()->style_type==0)
    {
      ListMenu_SetItemCount(mbk->menu,Menu_Get()->list->FirstFree);
    }
  }
  else
  {
    ITEM *it=Menu_GetSelected();
    Menu_Free(&it->menu);
    Icon_Unreg(it,0);
    Icon_Unreg(it,1);
    DELETE(it->name);
    DELETE(it->link);
    DELETE(it->img);
    DELETE(it->simg);
    it->type=0;
  }
  if(item>0)
  {
    if(Menu_Get()->style_type==0)
    {
      ListMenu_SetCursorToItem(mbk->menu,item-1);
    }
    else
    {
      cursor=item-1;
      set_nav();
    }
  }
  else
  {
    if(Menu_Get()->style_type==0)
    {
      ListMenu_SetCursorToItem(mbk->menu,0);
    }
    else
    {
      cursor=0;
      set_nav();
    }
  }
  Menu_TitleRefresh();
  //History_Refresh();
  Book_Refresh();
  Menu_Save();
}

void OnBackExitGui(BOOK * bk, GUI *)
{
  FREE_GUI(mbk->g_del);
}

void OnDelGui(BOOK *bk, GUI *)
{
  if(!move)
  {
    TEXTID strid=TextID_Create(names[27],ENC_UCS2,TEXTID_ANY_LEN);
    mbk->g_del=CreateYesNoQuestionVA(0,
                                     VAR_BOOK((BOOK *)mbk),
                                     VAR_YESNO_QUESTION(strid),
                                     0);
    GUIObject_SoftKeys_SetAction(mbk->g_del,ACTION_YES,OnYesExitGui);
    GUIObject_SoftKeys_SetAction(mbk->g_del,ACTION_NO,OnBackExitGui);
    GUIObject_SoftKeys_SetAction(mbk->g_del,ACTION_BACK,OnBackExitGui);
  }
}

void Move(BOOK * book,GUI * lt)
{
  move_index=GET_SELECTED();
  move=1;
}
void Style_(BOOK * book,GUI * lt)
{
  if(!move)
  {
    Menu_Get()->last=GET_SELECTED();
    StyleCreateGuiList(0, book);
  }
}
void Edit_(BOOK * book,GUI * lt)
{
  if(!move)
    EditCreateGuiList(0, book);
}
void Options_(BOOK * book,GUI * lt)
{
  if(!move)
    SetCreateGuiList(0, book);
}

void Skin_(BOOK * book,GUI * lt)
{
  if(!move)
    SkinCreateGuiList(0, book);
}

void MenuCreateGuiList(void * r0, BOOK * bk)
{
  mbk->menu=CreateListMenu(bk,0);
  Menu_Check();
  ITEM *it=(ITEM *)List_Get(Menu_Get()->list, Menu_Get()->last);

  if((!IS_DYNTITLE)&&(history->FirstFree>1))
  {
    GUIObject_SetTitleText(mbk->menu,TextID_Create(cur_title,ENC_UCS2,TEXTID_ANY_LEN));
  }
  else if((IS_DYNTITLE)||((!IS_DYNTITLE)&&(history->FirstFree==1)&&(IS_MAIN_DYNTITLE)))
  {
    TEXTID str=wstr2id(it->name);
    GUIObject_SetTitleText(mbk->menu,str);
  }
  else if((!IS_MAIN_DYNTITLE)&&(history->FirstFree==1))
  {
    TEXTID str=TextID_Create(L"Μενώ",ENC_UCS2,TEXTID_ANY_LEN);
    GUIObject_SetTitleText(mbk->menu,str);
  }
  ListMenu_SetItemCount(mbk->menu,Menu_Get()->list->FirstFree);
  ListMenu_SetCursorToItem(mbk->menu,Menu_Get()->last);
  ListMenu_SetItemStyle(mbk->menu,Menu_Get()->style);

  ListMenu_SetOnMessage(mbk->menu,MenuOnLBMessage);
  GUIObject_SetStyle(mbk->menu,Menu_Get()->gui_style);
  if(IS_NOSOFTKEYS)GUIObject_SoftKeys_Hide(mbk->menu);
  DISP_OBJ*dobj=GUIObject_GetDispObject(mbk->menu);
  mbk->oldOnKey=(void*)DispObject_GetOnKey(dobj);
  DISP_DESC_SetOnKey( DispObject_GetDESC (dobj), (DISP_OBJ_ONKEY_METHOD)myOnKey );

  GUIObject_SoftKeys_SetAction(mbk->menu,0,Edit_);
  GUIObject_SoftKeys_SetText(mbk->menu,0,TextID_Create(names[4],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,1,MenuAdd);
  GUIObject_SoftKeys_SetText(mbk->menu,1,TextID_Create(names[5],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,2,SubMenuAdd);
  GUIObject_SoftKeys_SetText(mbk->menu,2,TextID_Create(names[29],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,3,Move);
  GUIObject_SoftKeys_SetText(mbk->menu,3,TextID_Create(names[46],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,4,Style_);
  GUIObject_SoftKeys_SetText(mbk->menu,4,TextID_Create(names[6],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,5,Options_);
  GUIObject_SoftKeys_SetText(mbk->menu,5,TextID_Create(names[0],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,6,Skin_);
  GUIObject_SoftKeys_SetText(mbk->menu,6,TextID_Create(names[31],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_DELETE, OnDelGui);
  GUIObject_SoftKeys_SetText(mbk->menu,ACTION_DELETE,TextID_Create(names[25],ENC_UCS2,TEXTID_ANY_LEN));

  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_BACK, MenuBack);
  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_LONG_BACK, MenuBack);
  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_SELECT1,MenuOnEnterPressed);

  GUIObject_Show(mbk->menu);
};
