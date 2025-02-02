#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "Pages.h"
#include "StructBook.h"
#include "MyFUNC.h"

int SetTitlePunktsAL(GUI_MESSAGE * msg)
{
  int item;
  int ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
#ifdef ENG_LANG
        ID=Str2ID(L"Продолжить",ENC_UCS2,TEXTID_ANY_LEN);;
#else
        ID=Str2ID(L"Continue",ENC_UCS2,TEXTID_ANY_LEN);;
#endif
        SetMenuItemText0(msg,ID);
        iconidname2id(L"PROFILE_INCAR_LIST_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
#ifdef ENG_LANG
        ID=Str2ID(L"Удалить",ENC_UCS2,TEXTID_ANY_LEN);;
#else
        ID=Str2ID(L"Delete",ENC_UCS2,TEXTID_ANY_LEN);;
#endif
        SetMenuItemText0(msg,ID);
        iconidname2id(L"RDJ_RATE_DISLIKE_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
  }
  return 0;
}


void OnEntAL(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    GUI_Free(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    BookObj_GotoPage(bk, &bk_gs);
  }
  if(item==1)
  {
    int err;
    FileDelete(MyBK()->path, MyBK()->name, &err);
    GUI_Free(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    BookObj_GotoPage(bk, &bk_db);
  }
}

void ExitAL(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  BookObj_GotoPage(bk, &bk_db);
}

int ActionListCreate(void*,BOOK*bk)
{
  GUI_LIST*AL=CreateListObject((BOOK*)bk,0);
  if(((MyBook*)bk)->gui) GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  ((MyBook*)bk)->gui=AL;
#ifdef ENG_LANG
  GuiObject_SetTitleText(AL, STR("Список действий"));
#else
  GuiObject_SetTitleText(AL, STR("Action List"));
#endif
  SetNumOfMenuItem(AL, 2);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)AL,SetTitlePunktsAL);
  SetCursorToItem(AL,0);
  SetMenuItemStyle(AL,0);
  GUIObject_Softkey_SetAction(AL,ACTION_BACK,ExitAL);
  GUIObject_Softkey_SetAction(AL,ACTION_LONG_BACK,ExitAL);
  GUIObject_Softkey_SetAction(AL,ACTION_SELECT1,OnEntAL);
  ShowWindow(AL);
  return 0;
}
