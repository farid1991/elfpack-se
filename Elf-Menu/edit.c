#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

void RefreshEdList(GUI_LIST * g,int itemcount)
{
  int pos = ListMenu_GetSelectedItem(g);
  ListMenu_DestroyItems(g);
  ListMenu_SetItemCount(g,itemcount);
  ListMenu_SetCursorToItem(g,pos);
  GUIObject_SoftKeys_SetVisible((GUI*)g, ACTION_SELECT1, 1 );
}

void SetRefresh()
{
  if((Menu_CheckStyle()&&(Menu_Get()->style_type==0))||((Menu_Get()->style_type==1)&&(Menu_Get()->st.is_selected)))
  {
      RefreshEdList(mbk->edit,5);
  } else {
      RefreshEdList(mbk->edit,4);
  }
}

//ввод имени
int DestroyGui()
{
  FREE_GUI(mbk->str_input);
  SetRefresh();
  return(0);
}

void OnOkAction(BOOK *,wchar_t * string,int len)
{
  ITEM *it=Menu_GetSelected();
  DELETE(it->name);
  kickbadsymbols(string,len);
  it->name=new wchar_t[len+1];
  wstrncpy(it->name,string,len);
  it->name[len]=L'\0';
  GUIObject_SetTitleText(mbk->edit,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
  DestroyGui();
}

void CreateSInput()
{
  ITEM *it=Menu_GetSelected();
  int str=wstr2id(it->name);
  mbk->str_input=CreateStringInputVA(0,
                                   VAR_PREV_ACTION_PROC(DestroyGui),
                                   VAR_LONG_BACK_PROC(DestroyGui),
                                   VAR_BOOK((BOOK*)mbk),
                                   VAR_OK_PROC(OnOkAction),
                                   VAR_STRINP_FIXED_TEXT(TextID_Create(names[7],ENC_UCS2,TEXTID_ANY_LEN)),
                                   VAR_STRINP_TEXT(str),
                                   VAR_STRINP_MIN_LEN(1),
                                   VAR_STRINP_MAX_LEN(128),
                                   0);
}


void TypeBack(BOOK *bk, GUI*lt)
{
  FREE_GUI(mbk->type);
}

void TypeOnEnterPressed(BOOK *bk, GUI*lt)
{
  int item=ListMenu_GetSelectedItem(mbk->type);
  ITEM *it=Menu_GetSelected();
  DELETE(it->link);
  it->type=item;
  FREE_GUI(mbk->type);
  SetRefresh();
}

int TypeOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    int icid;
    if(item==0)
    {
      icid=GetIconID(L"RN_SHORTCUT_LIST_GENERIC_SHORTCUT_ICN");
    }
    else if(item==1)
    {
      icid=GetIconID(L"RN_SHORTCUT_LIST_ENTERTAINMENT_ICN");
    }
    else if(item==2)
    {
      icid=GetIconID(L"ORGANIZER_MENU_APPLICATIONS_ICN");
    }
    else if(item==3)
    {
      icid=GetIconID(L"RN_SHORTCUT_LIST_MENU_ICN");
    }
    else if(item==4)
    {
      icid=GetIconID(L"GUI_UNAVAILABLE_ICN");
    }
    else if(item==5)
    {
      icid=GetIconID(L"CALE_TASK_PHONECALL_ICN");
    }
    else if(item==6)
    {
      icid=GetIconID(L"DB_LIST_FOLDER_ICN");
    }
    if(item<3)GUIonMessage_SetMenuItemText(msg,TextID_Create(names[8+item],ENC_UCS2,TEXTID_ANY_LEN));
    else if(item==3)GUIonMessage_SetMenuItemText(msg,TextID_Create(names[1],ENC_UCS2,TEXTID_ANY_LEN));
    else if(item==4)GUIonMessage_SetMenuItemText(msg,TextID_Create(names[47],ENC_UCS2,TEXTID_ANY_LEN));
    else if(item==5)GUIonMessage_SetMenuItemText(msg,TextID_Create(names[48],ENC_UCS2,TEXTID_ANY_LEN));
    else if(item==6)GUIonMessage_SetMenuItemText(msg,TextID_Create(names[49],ENC_UCS2,TEXTID_ANY_LEN));


    GUIonMessage_SetMenuItemIcon(msg,0,icid);
  }
  return(1);
};

void TypeCreateGuiList(void * r0, BOOK * bk)
{
  mbk->type=CreateListMenu(bk,0);
  GUIObject_SetTitleText(mbk->type,TextID_Create(names[11],ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(mbk->type,7);
  ListMenu_SetOnMessage(mbk->type,TypeOnLBMessage);
  ListMenu_SetCursorToItem(mbk->type,0);
  ListMenu_SetItemStyle(mbk->type,0);
  GUIObject_SoftKeys_SetAction(mbk->type,ACTION_BACK, TypeBack);
  GUIObject_SoftKeys_SetAction(mbk->type,ACTION_LONG_BACK, TypeBack);
  GUIObject_SoftKeys_SetAction(mbk->type,ACTION_SELECT1,TypeOnEnterPressed);
  GUIObject_Show(mbk->type);
};

void EditBack(BOOK *bk, GUI*lt)
{
  FREE_GUI(mbk->edit);
  Book_Refresh();
  if((IS_DYNTITLE)||((!IS_DYNTITLE)&&(history->FirstFree==1)))
  Menu_TitleRefresh();
  ITEM *it=Menu_GetSelected();
  if(it->type!=3)Menu_Free(&it->menu);
  Menu_Save();
}


void EditOnEnterPressed(BOOK *bk, GUI*lt)
{
  int item=ListMenu_GetSelectedItem(mbk->edit);
  ITEM *it=Menu_GetSelected();
  if(item==0)CreateSInput();
  if(item==1)
  {
    if(it->type==0)CreateMainMenu(0, bk);
    else if(it->type==1)CreateJavaList(0, bk);
    else if(it->type==2)BookObj_CallPage(bk,&SelectElf_page);
    else if(it->type==3)
    {
      EditBack(bk, 0);
      /*HIS_ITEM*hitem=(HIS_ITEM*)List_Get(history, history->FirstFree-1);
      hitem->menu=mbk->cur_menu;*/

      ITEM *item=new ITEM;
      memset(item,0,sizeof(ITEM));
      //item->citem=it;
      item=it;
      List_InsertLast(history, item);
      //mbk->cur_menu=it->menu;
      MENU_CREATE_GUI();
    }
    else if(it->type==4)CreateEvInput();
    else if(it->type==5)CreateVcInput();
    else if(it->type==6)mbk->selectf=CreateFileFolderSelect(mbk, it->link);
  }
  if(item==2)
    TypeCreateGuiList(0, bk);
  if(item==3)
    Create_SelectImageDB(0);
  if(item==4)
    Create_SelectImageDB(1);
}


int EditOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    ITEM *it=Menu_GetSelected();

    if(item==0)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[36],ENC_UCS2,TEXTID_ANY_LEN));
      int str=wstr2id(it->name);
      GUIonMessage_SetMenuItemSecondLineText(msg,str);
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"DB_LIST_OTHER_ICN"));
    }
    else if(item==1)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[37],ENC_UCS2,TEXTID_ANY_LEN));
      if(it->type!=3)
      {
        int str=wstr2id(it->link);
        GUIonMessage_SetMenuItemSecondLineText(msg,str);
      }
      else
      {
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"...",ENC_UCS2,TEXTID_ANY_LEN));
      }
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"STATUSBAR_DOWNLOADING_DATA_ICN"));
    }
    else if(item==2)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[11],ENC_UCS2,TEXTID_ANY_LEN));
      //int strid;
      int icid;
      if(it->type==0)
      {
        icid=GetIconID(L"RN_SHORTCUT_LIST_GENERIC_SHORTCUT_ICN");
      }
      else if(it->type==1)
      {
        icid=GetIconID(L"RN_SHORTCUT_LIST_ENTERTAINMENT_ICN");
      }
      else if(it->type==2)
      {
        icid=GetIconID(L"ORGANIZER_MENU_APPLICATIONS_ICN");
      }
      else if(it->type==3)
      {
        icid=GetIconID(L"RN_SHORTCUT_LIST_MENU_ICN");
      }
      else if(it->type==4)
      {
        icid=GetIconID(L"GUI_UNAVAILABLE_ICN");
      }
      else if(it->type==5)
      {
        icid=GetIconID(L"CALE_TASK_PHONECALL_ICN");
      }
      else if(it->type==6)
      {
        icid=GetIconID(L"DB_LIST_FOLDER_ICN");
      }


      if(it->type<3)
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(names[8+it->type],ENC_UCS2,TEXTID_ANY_LEN));
      else if(it->type==3)
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(names[1],ENC_UCS2,TEXTID_ANY_LEN));
      else if(it->type==4)
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(names[47],ENC_UCS2,TEXTID_ANY_LEN));
      else if(it->type==5)
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(names[48],ENC_UCS2,TEXTID_ANY_LEN));
      else if(it->type==6)
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(names[49],ENC_UCS2,TEXTID_ANY_LEN));

      GUIonMessage_SetMenuItemIcon(msg,0,icid);
    }
    else if(item==3)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[38],ENC_UCS2,TEXTID_ANY_LEN));
      int str=wstr2id(it->img);
      GUIonMessage_SetMenuItemSecondLineText(msg,str);
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"DB_LIST_PICTURE_ICN"));
    }
    else if(item==4)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[39],ENC_UCS2,TEXTID_ANY_LEN));
      int str=wstr2id(it->simg);
      GUIonMessage_SetMenuItemSecondLineText(msg,str);
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"DB_LIST_PICTURE_ICN"));
    }
  }
  return(1);
};


void EditCreateGuiList(void * r0, BOOK * bk)
{
  ITEM *it=Menu_GetSelected();
  mbk->edit=CreateListMenu(bk,0);

  int str=wstr2id(it->name);

  GUIObject_SetTitleText(mbk->edit,str);
  if((Menu_CheckStyle()&&(Menu_Get()->style_type==0))||((Menu_Get()->style_type==1)&&(Menu_Get()->st.is_selected)))
  {
    ListMenu_SetItemCount(mbk->edit,5);
  }
  else
  {
    ListMenu_SetItemCount(mbk->edit,4);
  }
  ListMenu_SetOnMessage(mbk->edit,EditOnLBMessage);
  ListMenu_SetCursorToItem(mbk->edit,0);
  ListMenu_SetItemStyle(mbk->edit,1);
  DISP_OBJ*dobj=GUIObject_GetDispObject(mbk->edit);
  mbk->oldOnKey2=(void*)DispObject_GetOnKey(dobj);
  DISP_DESC_SetOnKey( DispObject_GetDESC (dobj), (DISP_OBJ_ONKEY_METHOD)myOnKey3 );

  GUIObject_SoftKeys_SetAction(mbk->edit,ACTION_BACK, EditBack);
  GUIObject_SoftKeys_SetAction(mbk->edit,ACTION_LONG_BACK, EditBack);
  GUIObject_SoftKeys_SetAction(mbk->edit,ACTION_SELECT1,EditOnEnterPressed);
  GUIObject_Show(mbk->edit);
};
