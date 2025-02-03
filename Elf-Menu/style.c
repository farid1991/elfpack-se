#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
#include "CFile.h"

LIST * style_list;
typedef struct
{
  wchar_t*name;
  wchar_t*path;
}F_STYLE;

//ввод имени стиля
STYLE_ITEM * cur_style_item;
int _state=0;
void Ask_Gui_OnBack(BOOK * bk, GUI *gui)
{
  if(_state==0)
  {
    cur_style_item->is_second_icon=0;
  }
  else
  {
    cur_style_item->is_second_icon=1;
  }
  FREE_GUI(mbk->sn_input);
  SetRefresh();
}

void Ask_Gui_OnYes(BOOK * bk, GUI *g)
{
  _state=1;
  Ask_Gui_OnBack(bk,g);
}

void  Ask_Gui(BOOK *bk, GUI *)
{
    _state=0;
    TEXTID text=TextID_Create(names[23],ENC_UCS2,TEXTID_ANY_LEN);
    mbk->sn_input=CreateYesNoQuestionVA(0,
                                     VAR_BOOK((BOOK *)mbk),
                                     VAR_YESNO_QUESTION(text),
                                     0);
    GUIObject_SoftKeys_SetAction(mbk->sn_input,ACTION_YES,Ask_Gui_OnYes);
    GUIObject_SoftKeys_SetAction(mbk->sn_input,ACTION_NO,Ask_Gui_OnBack);
    GUIObject_SoftKeys_SetAction(mbk->sn_input,ACTION_BACK,Ask_Gui_OnBack);
}

int StyleNumInput_Destroy()
{
  FREE_GUI(mbk->sn_input);
  Ask_Gui(0,0);
  return(0);
}

void StyleNumInput_OnOkAction(BOOK *,wchar_t * string,int len)
{
  kickbadsymbols(string,len);
  wtoi(string,wstrlen(string),&cur_style_item->list);
  StyleNumInput_Destroy();
}

void StyleNumInput_Create()
{
  TEXTID text=TextID_CreateIntegerID(cur_style_item->list);
  TEXTID fixed=TextID_Create(names[6],ENC_UCS2,TEXTID_ANY_LEN);
  mbk->sn_input=CreateStringInputVA(0,
                                    VAR_PREV_ACTION_PROC(StyleNumInput_Destroy),
                                    VAR_LONG_BACK_PROC(StyleNumInput_Destroy),
                                    VAR_BOOK((BOOK*)mbk),
                                    VAR_OK_PROC(StyleNumInput_OnOkAction),
                                    VAR_STRINP_FIXED_TEXT(fixed),
                                    VAR_STRINP_MODE(9),
                                    VAR_STRINP_TEXT(text),
                                    VAR_STRINP_MAX_LEN(128),
                                    0);
}

int StyleNameInput_Destroy()
{
  FREE_GUI(mbk->sn_input);
  StyleNumInput_Create();
  return(0);
}

void StyleNameInput_OnOkAction(BOOK *,wchar_t * string,int len)
{
  kickbadsymbols(string,len);
  DELETE(cur_style_item->name);
  cur_style_item->name=new wchar_t[len+1];
  wstrncpy(cur_style_item->name,string,len);
  cur_style_item->name[len]=L'\0';
  StyleNameInput_Destroy();
}

void StyleNameInput_Create()
{
  TEXTID text=TextID_Create(cur_style_item->name,ENC_UCS2,TEXTID_ANY_LEN);
  TEXTID fixed=TextID_Create(names[7],ENC_UCS2,TEXTID_ANY_LEN);
  mbk->sn_input=CreateStringInputVA(0,
                                    VAR_PREV_ACTION_PROC(StyleNameInput_Destroy),
                                    VAR_LONG_BACK_PROC(StyleNameInput_Destroy),
                                    VAR_BOOK((BOOK*)mbk),
                                    VAR_OK_PROC(StyleNameInput_OnOkAction),
                                    VAR_STRINP_FIXED_TEXT(fixed),
                                    VAR_STRINP_TEXT(text),
                                    VAR_STRINP_MIN_LEN(1),
                                    VAR_STRINP_MAX_LEN(128),
                                    0);
}

wchar_t * Styles_List_Path()
{
  wchar_t*temp=get_path(0);
  wchar_t temp2[]=L"styles.ini";
  wchar_t*path=new wchar_t[wstrlen(temp)+wstrlen(temp2)+2];
  wstrcpy(path,temp);
  wstrcat(path,Slash);
  wstrcat(path,temp2);
  DELETE(temp);
  return(path);
}

void Styles_List_Save()
{
  wchar_t*path=Styles_List_Path();
  CFile * file=new CFile(path);
  if(file->Open(WA_Read+WA_Write+WA_Create+WA_Truncate)>=0)
  {
    if(mbk->style_list)
    {
      file->WriteUTF8_BOM();
      file->WriteNum(mbk->style_list->FirstFree);
      file->Write(&separator,1);
      int i;
      for(i=0;i<mbk->style_list->FirstFree;i++)
      {
        STYLE_ITEM*it=(STYLE_ITEM*)List_Get(mbk->style_list, i);
        file->WriteUTF8(it->name);
        file->Write(&separator,1);
        file->WriteNum(it->list);
        file->Write(&separator,1);
        file->WriteNum(it->is_second_icon);
        file->Write(&separator,1);
      }
    }
    file->Close();
  }
  DELETE(path);
  DELETE(file);
}

void Styles_List_Load()
{
  if(!mbk->style_list)
  {
    mbk->style_list=List_Create();
  }
  wchar_t*path=Styles_List_Path();
  CFile * file=new CFile(path);
  int position=0;
  if(file->Open(WA_Read)>=0)
  {
    wchar_t * buf=file->ReadUTF8();
    int max=GetInt(buf,&position,separator);
    for(int i=0;i<max;i++)
    {
      STYLE_ITEM *item=new STYLE_ITEM;
      memset(item,0,sizeof(STYLE_ITEM));
      item->name=GetWchar(buf,&position,separator);
      item->list=GetInt(buf,&position,separator);
      item->is_second_icon=GetInt(buf,&position,separator);
      List_InsertLast(mbk->style_list, item);
    }
    file->Close();
    DELETE(buf);
  }
  DELETE(file);
  DELETE(path);
}

void Styles_List_Destroy()
{
  if (mbk->style_list)
  {
    while (mbk->style_list->FirstFree)
    {
      STYLE_ITEM* it=(STYLE_ITEM*)List_RemoveAt(mbk->style_list,0);
      DELETE(it->name);
      DELETE(it);
    }
    LIST_FREE(mbk->style_list);
  }
}

void CustomStyles_List_Destroy()
{
  if(style_list)
  {
    while(style_list->FirstFree)
    {
      F_STYLE* it=(F_STYLE*)List_RemoveAt(style_list,0);
      DELETE(it->name);
      DELETE(it->path);
      DELETE(it);
    }
    LIST_FREE(style_list);
  }
}
void StyleAdd(BOOK *bk, GUI *lt)
{
  STYLE_ITEM *item=new STYLE_ITEM;
  memset(item,0,sizeof(STYLE_ITEM));
  item->name=mstr(names[2]);
  item->list=NULL;
  List_InsertLast(mbk->style_list, item);
  ListMenu_SetItemCount(mbk->style,mbk->style_list->FirstFree);
  cur_style_item=item;
  StyleNameInput_Create();
}

void StyleEdit(BOOK *bk, GUI *lt)
{
  if(mbk->style_list->FirstFree>0)
  {
  int item=ListMenu_GetSelectedItem(mbk->style);
  STYLE_ITEM*it=(STYLE_ITEM*)List_Get(mbk->style_list, item);
  cur_style_item=it;
  StyleNameInput_Create();
  }
}

void StyleDel(BOOK *bk, GUI *lt)
{
  int item=ListMenu_GetSelectedItem(mbk->style);
  if(mbk->style_list->FirstFree>0)
  {
    STYLE_ITEM* it=(STYLE_ITEM*)List_RemoveAt(mbk->style_list,item);
    DELETE(it->name);
    DELETE(it);
    ListMenu_SetItemCount(mbk->style,mbk->style_list->FirstFree);
  }
  if(item>0)
  {
    ListMenu_SetCursorToItem(mbk->style,item-1);
  }
  else
  {
    ListMenu_SetCursorToItem(mbk->style,0);
  }
  Book_Refresh();
}

void StyleBack(BOOK *bk, GUI *lt)
{
  Styles_List_Save();
  Styles_List_Destroy();
  CustomStyles_List_Destroy();
  FREE_GUI(mbk->style_tab);
  FREE_GUI(mbk->cstyle);
  FREE_GUI(mbk->style);
  FREE_GUI(mbk->gstyle);
  Menu_Refresh();
}

void StyleOnEnterPressed(BOOK *bk, GUI *lt)
{
  if(mbk->style_list&&mbk->style_list->FirstFree>0)
  {
    int item=ListMenu_GetSelectedItem(mbk->style);
    STYLE_ITEM*it=(STYLE_ITEM*)List_Get(mbk->style_list, item);
    Menu_Get()->style=it->list;
    Menu_Get()->style_type=0;
    Menu_Get()->is_sicon=it->is_second_icon;
    ListMenu_SetItemStyle(mbk->menu,Menu_Get()->style);
    StyleBack(bk, lt);
    RefreshEdList(mbk->menu,Menu_Get()->list->FirstFree);
  }
}

int StyleOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    if(mbk->style_list&&mbk->style_list->FirstFree>0)
    {
      STYLE_ITEM*it=(STYLE_ITEM*)List_Get(mbk->style_list, item);
      GUIonMessage_SetMenuItemText(msg,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
    }
  }
  return(1);
};


void CStyleOnEnterPressed(BOOK *bk, GUI *lt)
{
  int item=ListMenu_GetSelectedItem(mbk->cstyle);
  F_STYLE *it=(F_STYLE *)List_Get(style_list, item);
  wchar_t*path=new wchar_t[wstrlen(it->name)+wstrlen(it->path)+2];
  wstrcpy(path,it->path);
  wstrcat(path,Slash);
  wstrcat(path,it->name);
  Style_Read(path,Menu_Get());
  Menu_Get()->style_type=1;    //тип стиля(стандартный или свой)
  Menu_Get()->cstyle=new wchar_t[wstrlen(path)+1];
  wstrcpy(Menu_Get()->cstyle,path);
  DELETE(path);
  StyleBack(bk, lt);
}
int CStyleOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    F_STYLE *it=(F_STYLE *)List_Get(style_list, item);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
  }
  return(1);
};

void GuiStyleOnEnterPressed(BOOK *bk, GUI *lt)
{
    Menu_Get()->gui_style=ListMenu_GetSelectedItem(mbk->gstyle)+1;
    StyleBack(bk,lt);
}

int GuiStyleOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    TEXTID strid=TextID_Create(names[12+item],ENC_UCS2,TEXTID_ANY_LEN);
    GUIonMessage_SetMenuItemText(msg,strid);
  }
  return(1);
};

void StyleCreateGuiList(void * r0, BOOK * bk)
{
  Styles_List_Destroy();
  Styles_List_Load();
  //стандартные стили
  mbk->style=CreateListMenu(bk,0);
  GUIObject_SetTitleText(mbk->style,TextID_Create(names[6],ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(mbk->style,mbk->style_list->FirstFree);

  ListMenu_SetOnMessage(mbk->style,StyleOnLBMessage);
  ListMenu_SetCursorToItem(mbk->style,0);
  ListMenu_SetItemStyle(mbk->style,0);

  GUIObject_SoftKeys_SetAction(mbk->style,0,StyleAdd);
  GUIObject_SoftKeys_SetText(mbk->style,0,TextID_Create(names[5],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(mbk->style,1,StyleEdit);
  GUIObject_SoftKeys_SetText(mbk->style,1,TextID_Create(names[4],ENC_UCS2,TEXTID_ANY_LEN));

  GUIObject_SoftKeys_SetAction(mbk->style,ACTION_DELETE, StyleDel);
  GUIObject_SoftKeys_SetText(mbk->style,ACTION_DELETE,TextID_Create(names[25],ENC_UCS2,TEXTID_ANY_LEN));

  GUIObject_SoftKeys_SetAction(mbk->style,ACTION_BACK, StyleBack);
  GUIObject_SoftKeys_SetAction(mbk->style,ACTION_LONG_BACK, StyleBack);
  GUIObject_SoftKeys_SetAction(mbk->style,ACTION_SELECT1,StyleOnEnterPressed);
  //стили окна (GUI)
  mbk->gstyle=CreateListMenu(bk,0);
  GUIObject_SetTitleText(mbk->gstyle,TextID_Create(names[22],ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(mbk->gstyle,10);

  ListMenu_SetOnMessage(mbk->gstyle,GuiStyleOnLBMessage);
  ListMenu_SetCursorToItem(mbk->gstyle,0);
  ListMenu_SetItemStyle(mbk->gstyle,0);

  GUIObject_SoftKeys_SetAction(mbk->gstyle,ACTION_BACK, StyleBack);
  GUIObject_SoftKeys_SetAction(mbk->gstyle,ACTION_LONG_BACK, StyleBack);
  GUIObject_SoftKeys_SetAction(mbk->gstyle,ACTION_SELECT1,GuiStyleOnEnterPressed);

  //ищем свои стили в папке
  FILELISTITEM * mem = (FILELISTITEM *)malloc(0x300);
  wchar_t*path=get_style_path(0);
  DIR_HANDLE * handle=AllocDirHandle(path);
  DELETE(path);
  FILELISTITEM * fli;
  style_list=List_Create();
  do
  {
    if(fli=GetFname(handle,mem))
    {
      if(wstrwstr(fli->fname,L"ms"))
      {
        F_STYLE *item=new F_STYLE;
        memset(item,0,sizeof(F_STYLE));
        item->name=new wchar_t[wstrlen(fli->fname)+1];
        wstrcpy(item->name,fli->fname);
        item->path=new wchar_t[wstrlen(fli->path)+1];
        wstrcpy(item->path,fli->path);
        List_InsertLast(style_list, item);
      }
    }
  }
  while(fli);
  if (handle) DestroyDirHandle(handle);
  mfree(mem);
  //нашли свои стили
  mbk->style_tab = CreateTabMenuBar(bk);

  if(style_list->FirstFree>0)
  {
    mbk->cstyle=CreateListMenu(bk,0);
    GUIObject_SetTitleText(mbk->cstyle,TextID_Create(names[43],ENC_UCS2,TEXTID_ANY_LEN));
    ListMenu_SetItemCount(mbk->cstyle,style_list->FirstFree);
    ListMenu_SetOnMessage(mbk->cstyle,CStyleOnLBMessage);
    ListMenu_SetCursorToItem(mbk->cstyle,Menu_Get()->style);
    ListMenu_SetItemStyle(mbk->cstyle,0);
    GUIObject_SoftKeys_SetAction(mbk->cstyle,ACTION_BACK, StyleBack);
    GUIObject_SoftKeys_SetAction(mbk->cstyle,ACTION_LONG_BACK, StyleBack);
    GUIObject_SoftKeys_SetAction(mbk->cstyle,ACTION_SELECT1,CStyleOnEnterPressed);
    TabMenuBar_SetTabCount(mbk->style_tab,3);
    TabMenuBar_SetTabGui(mbk->style_tab,2,mbk->cstyle);
    TabMenuBar_SetTabIcon(mbk->style_tab,2,GetIconID(L"WV_YAHOO_FRIENDS_TAB_INACTIVE_ICN"),0);
    TabMenuBar_SetTabIcon(mbk->style_tab,2,GetIconID(L"WV_YAHOO_FRIENDS_TAB_ICN"),1);
  }
  else
  {
    TabMenuBar_SetTabCount(mbk->style_tab,2);
  }
  TabMenuBar_SetFocusedTab(mbk->style_tab,Menu_Get()->style_type);
  TabMenuBar_SetTabGui(mbk->style_tab,0,mbk->style);
  TabMenuBar_SetTabGui(mbk->style_tab,1,mbk->gstyle);

  TabMenuBar_SetTabIcon(mbk->style_tab,0,GetIconID(L"HPB_INFO_TAB_INACTIVE_ICN"),0);
  TabMenuBar_SetTabIcon(mbk->style_tab,0,GetIconID(L"HPB_INFO_TAB_ICN"),1);

  TabMenuBar_SetTabIcon(mbk->style_tab,1,GetIconID(L"WV_YAHOO_CONVERSATION_TAB_INACTIVE_ICN"),0);
  TabMenuBar_SetTabIcon(mbk->style_tab,1,GetIconID(L"WV_YAHOO_CONVERSATION_TAB_ICN"),1);


  GUIObject_Show(mbk->style_tab);
};
