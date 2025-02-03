#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

wchar_t * names[LCNT];

int VIBRA=0;
int IS_FULLSCREEN=0;
int IS_NOSOFTKEYS=0;
int IS_DYNTITLE=0;
int IS_EDITOR=0;
int IS_JOY=0;
int IS_MOVE=0;
int IS_EXIT=0;
int IS_MAIN_DYNTITLE=0;
void SetBack(BOOK *bk, GUI*lt)
{
  FREE_GUI(mbk->set);
  Menu_Refresh();
}

void SetOnEnterPressed(BOOK *bk, GUI*lt)
{
  int item=ListMenu_GetSelectedItem(mbk->set);
  if(item==0)
  {
    if(IS_NOSOFTKEYS==0)IS_NOSOFTKEYS=1;
    else IS_NOSOFTKEYS=0;
  }
  else if(item==1)
  {
    if(VIBRA==0)VIBRA=1;
    else VIBRA=0;
  }
  else if(item==2)
  {
    if(IS_FULLSCREEN==0)IS_FULLSCREEN=1;
    else IS_FULLSCREEN=0;
  }
  else if(item==3)
  {
    if(IS_DYNTITLE==0)IS_DYNTITLE=1;
    else IS_DYNTITLE=0;
  }
  else if(item==4)
  {
    if(IS_EDITOR==0)IS_EDITOR=1;
    else IS_EDITOR=0;
  }
  else if(item==5)
  {
    if(IS_JOY==0)IS_JOY=1;
    else IS_JOY=0;
    Softkey_SetAction(1);
  }
  else if(item==6)
  {
    if(IS_MOVE==0)IS_MOVE=1;
    else IS_MOVE=0;
  }
  else if(item==7)
  {
    if(IS_EXIT==0)IS_EXIT=1;
    else IS_EXIT=0;
  }
  else if(item==8)
  {
    if(IS_MAIN_DYNTITLE==0)IS_MAIN_DYNTITLE=1;
    else IS_MAIN_DYNTITLE=0;
  }
  Config_Save();
  Book_Refresh();
  RefreshEdList(mbk->set,9);
}

int SetOnLBMessage(GUI_MESSAGE * msg)
{
  int item;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    int strid;
    if(item==0)
    {
      strid=TextID_Create(names[26],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_NOSOFTKEYS)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==1)
    {
      strid=TextID_Create(names[28],ENC_UCS2,TEXTID_ANY_LEN);
      if(VIBRA)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==2)
    {
      strid=TextID_Create(names[45],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_FULLSCREEN)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==3)
    {
      strid=TextID_Create(names[41],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_DYNTITLE)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==4)
    {
      strid=TextID_Create(names[42],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_EDITOR)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==5)
    {
      strid=TextID_Create(names[34],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_JOY)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==6)
    {
      strid=TextID_Create(names[24],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_MOVE)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==7)
    {
      strid=TextID_Create(names[35],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_EXIT)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    else if(item==8)
    {
      strid=TextID_Create(names[30],ENC_UCS2,TEXTID_ANY_LEN);
      if(IS_MAIN_DYNTITLE)
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKMARK_IN_BOX_ICN"));
      else GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"CHECKBOX_ICN"));
    }
    GUIonMessage_SetMenuItemText(msg,strid);
  }
  return(1);
};

void SetCreateGuiList(void * r0, BOOK * bk)
{
  mbk->set=CreateListMenu(bk,0);
  GUIObject_SetTitleText(mbk->set,TextID_Create(names[0],ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(mbk->set,9);
  ListMenu_SetOnMessage(mbk->set,SetOnLBMessage);
  ListMenu_SetCursorToItem(mbk->set,0);
  ListMenu_SetItemStyle(mbk->set,0);
  GUIObject_SoftKeys_SetAction(mbk->set,ACTION_BACK, SetBack);
  GUIObject_SoftKeys_SetAction(mbk->set,ACTION_LONG_BACK, SetBack);
  GUIObject_SoftKeys_SetAction(mbk->set,ACTION_SELECT1,SetOnEnterPressed);
  GUIObject_Show(mbk->set);
};
