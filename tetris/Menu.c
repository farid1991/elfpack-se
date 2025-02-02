//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Button.h"
#include "Menu.h"
#include "MyFUNC.h"
#include "Game.h"
#include "ErrorIAR.h"
#include "Rec_Check.h"
#include "Pages.h"
#include "Color.h"
wchar_t*butname[9]={
#ifdef ENG_LANG
  L"Влево", L"Вправо", L"Ускорение", L"Мгновенное падение", L"Поворот по часовой", L"Поворот против часовой", L"Свернуть", L"Вкл/Откл звук", L"Пауза/Выход"
#else
  L"Left", L"Right", L"Speedup", L"Instant fall", L"Hour Tumbling", L"Tumbling counterclockwise", L"Hide", L"On/Off Sound", L"Pause/Exit"
#endif
};




void ExitBut1(BOOK*bk,GUI*)
{
  CreateButMenu(1);
}

int SetTitlePunktsBut1(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    iconidname2id(L"WV_MSN_BUDDYLIST_CONTACT_AVAILABLE_MOBILE_ICN", -1, &ID);
    SetListObjectItemIcon(msg, 0, ID);
    SetMenuItemText0(msg,Str2ID(butname[item],ENC_UCS2,TEXTID_ANY_LEN));
    int met[2];
#ifdef ENG_LANG
      met[0]=STR("Клавиша: ");
#else
      met[0]=STR("Key: ");
#endif
    met[1]=KeyCode2Name(MyBK()->nast->buttons[1][item]);
    SetMenuItemText1(msg,Str2ID(met,ENC_TEXTID,2));
  }
  return 1;
}

void OnEntBut1(BOOK * bk, GUI *)
{
  int ID[2];
  ID[0]=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);;
  ID[1]=1;
  ColorChange(ID, (BOOK*)(MyBK()));
}

void CreateDigButtonsMenu(int x)
{
  if(MyBK()->gui) GUI_Free(MyBK()->gui);
  MyBK()->gui=0;
  GUI_LIST*MNM=CreateListObject((BOOK*)(MyBK()),0);
  ((MyBK()))->gui=MNM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(MNM, Str2ID(L"Джойстик/Софты",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(MNM, Str2ID(L"Joystick/Soft",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(MNM, 9);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)MNM,SetTitlePunktsBut1);
  SetCursorToItem(MNM,x);
  SetMenuItemStyle(MNM,3);
  GUIObject_Softkey_SetAction(MNM,ACTION_BACK,ExitBut1);
  GUIObject_Softkey_SetAction(MNM,ACTION_LONG_BACK,ExitBut1);
  GUIObject_Softkey_SetAction(MNM,ACTION_SELECT1,OnEntBut1);
  ShowWindow(MNM);
}

void ExitBut0(BOOK*bk,GUI*)
{
  CreateButMenu(0);
}

int SetTitlePunktsBut0(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    iconidname2id(L"WV_MSN_BUDDYLIST_CONTACT_AVAILABLE_MOBILE_ICN", -1, &ID);
    SetListObjectItemIcon(msg, 0, ID);
    SetMenuItemText0(msg,Str2ID(butname[item],ENC_UCS2,TEXTID_ANY_LEN));
    int met[2];
    if(TextGetLength(KeyCode2Name(MyBK()->nast->buttons[0][item]))<=5)
    {
#ifdef ENG_LANG
      met[0]=STR("Клавиша: ");
#else
      met[0]=STR("Key: ");
#endif
    }
    else
    {
      met[0]=STR("");
    }
    met[1]=KeyCode2Name(MyBK()->nast->buttons[0][item]);
    SetMenuItemText1(msg,Str2ID(met,ENC_TEXTID,2));
  }
  return 1;
}


void OnEntBut0(BOOK * bk, GUI *)
{
  int ID[2];
  ID[0]=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);;
  ID[1]=0;
  ColorChange(ID, (BOOK*)(MyBK()));
}

void CreateButtonsMenu(int x)
{
  if(MyBK()->gui) GUI_Free(MyBK()->gui);
  MyBK()->gui=0;
  GUI_LIST*MNM=CreateListObject((BOOK*)(MyBK()),0);
  ((MyBK()))->gui=MNM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(MNM, Str2ID(L"Джойстик/Софты",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(MNM, Str2ID(L"Joystick/Soft",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(MNM, 9);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)MNM,SetTitlePunktsBut0);
  SetCursorToItem(MNM,x);
  SetMenuItemStyle(MNM,3);
  GUIObject_Softkey_SetAction(MNM,ACTION_BACK,ExitBut0);
  GUIObject_Softkey_SetAction(MNM,ACTION_LONG_BACK,ExitBut0);
  GUIObject_Softkey_SetAction(MNM,ACTION_SELECT1,OnEntBut0);
  ShowWindow(MNM);
}

void OnEntBut(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    CreateButtonsMenu(0);
  }
  if(item==1)
  {
    CreateDigButtonsMenu(0);
  }
  if(item==2)
  {
    DefaultButtons(MyBK());
    CreateButMenu(2);
  }
}

void ExitBut(BOOK*bk,GUI*)
{
  CreateNM((MyBook*)bk,3);
}

int SetTitlePunktsBut(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==1)
      {
        iconidname2id(L"WV_MSN_BUDDYLIST_CONTACT_AVAILABLE_MOBILE_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Цифровая клавиатура",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Digital keyboard",ENC_UCS2,TEXTID_ANY_LEN));
#endif
      }
      if(item==0)
      {
        iconidname2id(L"WV_MSN_BUDDYLIST_CONTACT_AVAILABLE_MOBILE_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Джойстик/Софты",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Joystick/Soft",ENC_UCS2,TEXTID_ANY_LEN));
#endif
      }
      if(item==2)
      {
        iconidname2id(L"RDJ_BR_WISHLIST_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Сброс",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Reset",ENC_UCS2,TEXTID_ANY_LEN));
#endif
      }
  }
  return 1;
}


void CreateButMenu(int x)
{
  if(MyBK()->gui) GUI_Free(MyBK()->gui);
  MyBK()->gui=0;
  GUI_LIST*MNM=CreateListObject((BOOK*)(MyBK()),0);
  ((MyBK()))->gui=MNM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(MNM, Str2ID(L"Управление",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(MNM, Str2ID(L"Management",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(MNM, 3);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)MNM,SetTitlePunktsBut);
  SetCursorToItem(MNM,x);
  SetMenuItemStyle(MNM,0);
  GUIObject_Softkey_SetAction(MNM,ACTION_BACK,ExitBut);
  GUIObject_Softkey_SetAction(MNM,ACTION_LONG_BACK,ExitBut);
  GUIObject_Softkey_SetAction(MNM,ACTION_SELECT1,OnEntBut);
  ShowWindow(MNM);
}

wchar_t*NamesOfPunktsPicture[8]={
#ifdef ENG_LANG
  L"Цвет1 фигуры", L"Цвет2 фигуры", L"Цвет3 фигуры", L"Цвет4 фигуры", L"Цвет сл.фигуры", L"Цвет фона1", L"Цвет фона2", L"Цвет текста"
#else
  L"Color1", L"Color2", L"Color3", L"Color4", L"Color next figure", L"Color background", L"Color game background", L"Color text"
#endif
};



void ExitCol(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateNM((MyBook*)bk,2);
}
void OnEntCol(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    if(((MyBook*)bk)->nast->coloron==0) ((MyBook*)bk)->nast->coloron=1;
    else ((MyBook*)bk)->nast->coloron=0;
    GUI_Free(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    CreateColorsMenu(item);
  }
  if(item>0)
  {
    ColorChange((item-1), bk);
  }
}

int SetTitlePunktsCol(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        iconidname2id(L"DB_LIST_THEME_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Разные цвета",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Different colors",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        SetMenuItemText1(msg,GetStrokaIMG(StatusSettings(((MyBook*)isBookX(NameMyElf, 0))->nast->coloron),((MyBook*)isBookX(NameMyElf, 0))->nast->coloron));
      }
      int r, g, b, c;
      wchar_t buf[32];
      for(int i=0; i<8; i++)
      {
        if(item==i+1)
        {
          SetMenuItemText0(msg,Str2ID(NamesOfPunktsPicture[i],ENC_UCS2,TEXTID_ANY_LEN));
          c=MyBK()->nast->colors[i]-0xFF000000;
          r=c/0x10000;
          g=(c-r*0x10000)/0x100;
          b=(c-r*0x10000-g*0x100);
          snwprintf(buf, MAXELEMS(buf)-1,L"0x%02X%02X%02X",r,g,b);
          SetMenuItemText1(msg,Str2ID(buf,ENC_UCS2,TEXTID_ANY_LEN));
          iconidname2id(L"DB_LIST_THEME_ICN", -1, &ID);
          SetListObjectItemIcon(msg, 0, ID);
        }
      }
  }
  return(1);
}

void Setp(int item)
{
  if(item==0)
  {
    GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, Str2ID(StatusSettings1(((MyBook*)isBookX(NameMyElf, 0))->nast->coloron),ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_Softkey_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, OnEntCol);
    //SetListObjectItemIcon(msg, 0, IconID->I1);
  }
  for(int i=0; i<8; i++)
  {
    if(item==i+1)
    {
#ifdef ENG_LANG
      GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, Str2ID(L"Изменить",ENC_UCS2,TEXTID_ANY_LEN));
#else
      GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, Str2ID(L"Edit",ENC_UCS2,TEXTID_ANY_LEN));
#endif
      GUIObject_Softkey_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, OnEntCol);
      //SetListObjectItemIcon(msg, 0, IconID->I2);
    }
  }
}

void NewKey2(DISP_OBJ * p, int i1, int i2, int i3, int i4)
{
  void(*OldKey)(void*, int, int, int, int);
  OldKey=((void(*)(void*, int, int, int, int))((MyBook*)isBookX(NameMyElf, 0))->OldKey1);
  OldKey(p, i1, i2, i3, i4);
  int item=ListMenu_GetSelectedItem(MyBK()->gui);
  Setp(item);
}

void CreateColorsMenu(int x)
{
  if(MyBK()->gui) GUI_Free(MyBK()->gui);
  MyBK()->gui=0;
  GUI_LIST*MNM=CreateListObject((BOOK*)(MyBK()),0);
  ((MyBK()))->gui=MNM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(MNM, Str2ID(L"Цвета фигур",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(MNM, Str2ID(L"Colors",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(MNM, 9);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)MNM,SetTitlePunktsCol);
  SetCursorToItem(MNM,x);
  SetMenuItemStyle(MNM,3);
  GUIObject_Softkey_SetAction(MNM,ACTION_BACK,ExitCol);
  GUIObject_Softkey_SetAction(MNM,ACTION_LONG_BACK,ExitCol);
  Setp(x);
  //GUIObject_Softkey_SetAction(MNM,ACTION_SELECT1,OnEntMNM);
  MyBK()->OldKey1=DispObject_GetOnKey(GUIObject_GetDispObject(MNM));
  DISP_DESC_SetOnKey( DISP_OBJ_GetDESC (GUIObject_GetDispObject(MNM)), NewKey2);
  ShowWindow(MNM);
}

void ShowResults(TABL*t)
{
  int text[60];
  int i, j;
  j=0;
  for(i=0; i<10; i++)
  {
    text[j]=int2strID(i+1);
    j++;
    text[j]=STR(". ");
    j++;
    text[j]=Str2ID(t->name[i], ENC_UCS2, TEXTID_ANY_LEN);
    j++;
    text[j]=STR(" - ");
    j++;
    text[j]=int2strID(t->rezultat[i]);
    j++;
    text[j]=STR("\n");
    j++;
  }
  int ID=Str2ID(text, ENC_TEXTID, 60);
#ifdef ENG_LANG
  MessageBox(STR("Результаты"), ID, 0, 2, 0, 0);
#else
  MessageBox(STR("Results"), ID, 0, 2, 0, 0);
#endif
}

void DestroyInputI()
{
  GUI_Free(MyBK()->gui1);
  MyBK()->gui1=0;
  ShowWindow(MyBK()->gui);
}


void OnEntInputI(BOOK * bk,wchar_t * string,int len)
{
  wstrcpy(((MyBook*)bk)->nast->im->names[ListMenu_GetSelectedItem(((MyBook*)bk)->gui)], string);
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  ImagesDel(bk);
  CheckResurs(MyBK());
  ShowWindow(((MyBook*)bk)->gui);
}

void CreateInputI(int x, IMGN*i, MyBook*bk)
{
  if(bk->gui1) GUI_Free(bk->gui1);
  bk->gui1=0;
  bk->gui1=CreateStringInput(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputI),
                                       VAR_LONG_BACK_PROC(DestroyInputI),
                                       VAR_BOOK((BOOK*)bk),
#ifdef ENG_LANG
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Изменить имя:", ENC_UCS2, TEXTID_ANY_LEN)),
#else
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Edit name:", ENC_UCS2, TEXTID_ANY_LEN)),
#endif
                                       VAR_STRINP_TEXT(Str2ID(i->names[x], ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(127),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputI),
                                       0);
}

void OnEntINM(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item<7)
  {
    CreateInputI(item, ((MyBook*)bk)->nast->im, (MyBook*)bk);
  }
  else
  {
    if(MyBK()->nast->im->on) MyBK()->nast->im->on=0;
    else MyBK()->nast->im->on=1;
    GUI_Free(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    CreateINM((MyBook*)bk, 7);
  }
}

int SetTitlePunktsINM(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    for(int i=0; i<7; i++)
    {
      if(item==i)
      {
        SetMenuItemText0(msg,Str2ID(GetNamesOfIMN(i, 0),ENC_UCS2,TEXTID_ANY_LEN));
        SetMenuItemText1(msg,Str2ID(((MyBook*)isBookX(NameMyElf, 0))->nast->im->names[i],ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"DB_LIST_PICTURE_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
    }
    if(item==7)
    {
#ifdef ENG_LANG
      SetMenuItemText0(msg,Str2ID(L"Картинки:",ENC_UCS2,TEXTID_ANY_LEN));
#else
      SetMenuItemText0(msg,Str2ID(L"Images:",ENC_UCS2,TEXTID_ANY_LEN));
#endif

      if(MyBK()->nast->im->on)
      {
#ifdef ENG_LANG
        SetMenuItemText1(msg,Str2ID(L"Из ФС",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText1(msg,Str2ID(L"From FS",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"CALLS_INCOMING_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      else
      {
#ifdef ENG_LANG
        SetMenuItemText1(msg,Str2ID(L"Из эльфа",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText1(msg,Str2ID(L"From Elf",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"CALLS_OUTGOING_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
    }
  }
  return 0;
}

void ExitINM(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateNM((MyBook*)bk,1);
}

void CreateINM(MyBook*bk, int x)
{
  if(bk->gui) GUI_Free(bk->gui);
  bk->gui=0;
  GUI_LIST*INM=CreateListObject((BOOK*)bk,0);
  ((MyBook*)bk)->gui=INM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(INM, Str2ID(L"Картинки",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(INM, Str2ID(L"Images",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(INM, 8);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)INM,SetTitlePunktsINM);
  SetCursorToItem(INM,x);
  SetMenuItemStyle(INM,3);
  GUIObject_Softkey_SetAction(INM,ACTION_BACK,ExitINM);
  GUIObject_Softkey_SetAction(INM,ACTION_LONG_BACK,ExitINM);
  GUIObject_Softkey_SetAction(INM,ACTION_SELECT1,OnEntINM);
#ifdef ENG_LANG
  GUIObject_Softkey_SetText(INM,ACTION_SELECT1,STR("Изменить"));
#else
  GUIObject_Softkey_SetText(INM,ACTION_SELECT1,STR("Edit"));
#endif
  ShowWindow(INM);
}

void ExitMNM(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateNM((MyBook*)bk,0);
}

void OnMNM1(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  for(int i=0; i<6; i++)
  {
    if(item==i+1)
    {
      if(((MyBook*)bk)->nast->sound->status[i]==0) ((MyBook*)bk)->nast->sound->status[i]=1;
      else ((MyBook*)bk)->nast->sound->status[i]=0;
    }
  }
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateMNM(((MyBook*)bk), item);
}


void DestroyInput()
{
  GUI_Free(MyBK()->gui1);
  MyBK()->gui1=0;
  ShowWindow(MyBK()->gui);
}


void OnEntInput(BOOK * bk,wchar_t * string,int len)
{
  int x=ListMenu_GetSelectedItem(((MyBook*)bk)->gui)-1;
  wstrcpy(((MyBook*)bk)->nast->sound->names[x], string);
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  ShowWindow(((MyBook*)bk)->gui);
}




void CreateInput(int x, SOUND*s, MyBook*bk)
{
  if(bk->gui1) GUI_Free(bk->gui1);
  bk->gui1=0;
  bk->gui1=CreateStringInput(0,
                                       VAR_PREV_ACTION_PROC(DestroyInput),
                                       VAR_LONG_BACK_PROC(DestroyInput),
                                       VAR_BOOK((BOOK*)bk),
#ifdef ENG_LANG
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Изменить имя:", ENC_UCS2, TEXTID_ANY_LEN)),
#else
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Edit name:", ENC_UCS2, TEXTID_ANY_LEN)),
#endif
                                       VAR_STRINP_TEXT(Str2ID(s->names[x-1], ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(255),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInput),
                                       0);
}



void OnEntMNM(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    if(((MyBook*)bk)->nast->sound->sound==0) ((MyBook*)bk)->nast->sound->sound=1;
    else ((MyBook*)bk)->nast->sound->sound=0;
    GUI_Free(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    CreateMNM(((MyBook*)bk), item);
  }
  if(item>0)
  {
    CreateInput(item, ((MyBook*)bk)->nast->sound, (MyBook*)bk);
  }
}

int SetTitlePunktsMNM(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        iconidname2id(L"DB_LIST_SOUND_IMELODY_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Звук",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Sound",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        SetMenuItemText1(msg,GetStrokaIMG(StatusSettings(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->sound),((MyBook*)isBookX(NameMyElf, 0))->nast->sound->sound));
      }
      for(int i=0; i<6; i++)
      {
        if(item==i+1)
        {
          SetMenuItemText0(msg,Str2ID(NamesOfPunkts(i),ENC_UCS2,TEXTID_ANY_LEN));
          SetMenuItemText1(msg,GetStrokaIMG(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->names[i],((MyBook*)isBookX(NameMyElf, 0))->nast->sound->status[i]));
          iconidname2id(L"DB_LIST_SOUND_EMELODY_ICN", -1, &ID);
          SetListObjectItemIcon(msg, 0, ID);
        }
      }
  }
  return(1);
}

void Set(int item)
{
  if(item==0)
  {
    GUIObject_Softkey_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui,1,0);
    GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, 1, Str2ID(L" ",ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, Str2ID(StatusSettings1(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->sound),ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_Softkey_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, OnEntMNM);
    //SetListObjectItemIcon(msg, 0, IconID->I1);
  }
  for(int i=0; i<6; i++)
  {
    if(item==i+1)
    {
      GUIObject_Softkey_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui,1,OnMNM1);
      GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, 1, Str2ID(StatusSettings1(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->status[i]),ENC_UCS2,TEXTID_ANY_LEN));
#ifdef ENG_LANG
      GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, Str2ID(L"Изменить",ENC_UCS2,TEXTID_ANY_LEN));
#else
      GUIObject_Softkey_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, Str2ID(L"Edit",ENC_UCS2,TEXTID_ANY_LEN));
#endif
      GUIObject_Softkey_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, OnEntMNM);
      //SetListObjectItemIcon(msg, 0, IconID->I2);
    }
  }
}

void NewKey1(DISP_OBJ * p, int i1, int i2, int i3, int i4)
{
  void(*OldKey)(void*, int, int, int, int);
  OldKey=((void(*)(void*, int, int, int, int))((MyBook*)isBookX(NameMyElf, 0))->OldKey1);
  OldKey(p, i1, i2, i3, i4);
  int item=ListMenu_GetSelectedItem(MyBK()->gui);
  Set(item);
}

void CreateMNM(MyBook*bk, int x)
{
  if(bk->gui) GUI_Free(bk->gui);
  bk->gui=0;
  GUI_LIST*MNM=CreateListObject((BOOK*)bk,0);
  ((MyBook*)bk)->gui=MNM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(MNM, Str2ID(L"Музыка",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(MNM, Str2ID(L"Sound",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(MNM, 7);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)MNM,SetTitlePunktsMNM);
  SetCursorToItem(MNM,x);
  SetMenuItemStyle(MNM,3);
  GUIObject_Softkey_SetAction(MNM,ACTION_BACK,ExitMNM);
  GUIObject_Softkey_SetAction(MNM,ACTION_LONG_BACK,ExitMNM);
  Set(x);
  //GUIObject_Softkey_SetAction(MNM,ACTION_SELECT1,OnEntMNM);
  bk->OldKey1=DISP_OBJ_GetOnKey(GUIObject_GetDispObject(MNM));
  DISP_DESC_SetOnKey( DISP_OBJ_GetDESC (GUIObject_GetDispObject(MNM)), NewKey1);
  ShowWindow(MNM);
}
int last;
void ExitNM(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  if(!((MyBook*)bk)->pause) CreateGM(0, bk,last);
  //CreatePM(bk);
}

void OnEntGameM(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  ((MyBook*)bk)->speed=item+1;
  ((MyBook*)bk)->point=0;
  ((MyBook*)bk)->lvl=0;
  BookObj_GotoPage(bk,&bk_game);
  GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
}

void OnEntMR(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    ShowResults(((MyBook*)bk)->nast->rez->te);
  }
  if(item==1)
  {
    ShowResults(((MyBook*)bk)->nast->rez->tn);
  }
  if(item==2)
  {
    ShowResults(((MyBook*)bk)->nast->rez->th);
  }
}

void OnEntNM(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    CreateMNM((MyBook*)bk, 0);
  }
  if(item==1)
  {
    CreateINM((MyBook*)bk, 0);
  }
  if(item==2)
  {
    CreateColorsMenu(0);
  }
  if(item==3)
  {
    CreateButMenu(0);
  }
}

int SetTitlePunktsNM(GUI_MESSAGE * msg)
{
  int item, ID;
  switch(GUIonMessage_GetMsg(msg))
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Музыка",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Sound",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"ENTERTAINMENT_MENU_MIDI_COMP_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Картинки",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Images",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"ENTERTAINMENT_MENU_PICTUREEDITOR_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==2)
      {
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Разные цвета",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Different colors",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"DB_THEMES_DIRECTORY_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==3)
      {
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Управление",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Management",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"RN_SHORTCUT_LIST_ENTERTAINMENT_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
  }
  return(1);
}

void DeleteRes(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    DELTABL(((MyBook*)bk)->nast->rez->te);
  }
  if(item==1)
  {
    DELTABL(((MyBook*)bk)->nast->rez->tn);
  }
  if(item==2)
  {
    DELTABL(((MyBook*)bk)->nast->rez->th);
  }
}

int SetTitlePunktsMR(GUI_MESSAGE * msg)
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
        SetMenuItemText0(msg,Str2ID(L"Легко",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Easy",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"WV_BUDDYLIST_CONTACT_AVAILABLE_ASHAMED_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Средне",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Normal",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"WV_BUDDYLIST_CONTACT_AVAILABLE_ANXIOUS_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==2)
      {
#ifdef ENG_LANG
        SetMenuItemText0(msg,Str2ID(L"Тяжело",ENC_UCS2,TEXTID_ANY_LEN));
#else
        SetMenuItemText0(msg,Str2ID(L"Hard",ENC_UCS2,TEXTID_ANY_LEN));
#endif
        iconidname2id(L"WV_BUDDYLIST_CONTACT_AVAILABLE_ANGRY_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
  }
  return(1);
}

void MenuResultat(MyBook*bk)
{
  last=3;
  if(bk->gui) GUI_Free(bk->gui);
  bk->gui=0;
  GUI_LIST*MR=CreateListObject((BOOK*)bk,0);
  ((MyBook*)bk)->gui=MR;
#ifdef ENG_LANG
  GuiObject_SetTitleText(MR, Str2ID(L"Результаты",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(MR, Str2ID(L"Results",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(MR, 3);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)MR,SetTitlePunktsMR);
  SetCursorToItem(MR,0);
  SetMenuItemStyle(MR,0);
  GUIObject_Softkey_SetAction(MR,ACTION_BACK,ExitNM);
  GUIObject_Softkey_SetAction(MR,ACTION_LONG_BACK,ExitNM);
  GUIObject_Softkey_SetAction(MR,ACTION_SELECT1,OnEntMR);
  GUIObject_Softkey_SetAction(MR,1,DeleteRes);
#ifdef ENG_LANG
  GUIObject_Softkey_SetText(MR,1,STR("Сброс"));
  GUIObject_Softkey_SetText(MR,ACTION_SELECT1,STR("Просм."));
#else
  GUIObject_Softkey_SetText(MR,1,STR("Reset"));
  GUIObject_Softkey_SetText(MR,ACTION_SELECT1,STR("Open"));
#endif
  ShowWindow(MR);
}

void CreateNM(MyBook*bk, int x)
{
  last=2;
  if(bk->gui) GUI_Free(bk->gui);
  bk->gui=0;
  GUI_LIST*NM=CreateListObject((BOOK*)bk,0);
  ((MyBook*)bk)->gui=NM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(NM, Str2ID(L"Настройки",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(NM, Str2ID(L"Settings",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(NM, 4);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)NM,SetTitlePunktsNM);
  SetCursorToItem(NM,x);
  SetMenuItemStyle(NM,0);
  GUIObject_Softkey_SetAction(NM,ACTION_BACK,ExitNM);
  GUIObject_Softkey_SetAction(NM,ACTION_LONG_BACK,ExitNM);
  GUIObject_Softkey_SetAction(NM,ACTION_SELECT1,OnEntNM);
  ShowWindow(NM);
}


void YesPress(BOOK*bk,GUI*)
{
  FreeBook(bk);
}

void NoPress(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  ShowWindow(((MyBook*)bk)->gui);
}

void YesNo(MyBook*bk)
{
  bk->gui1=CreateYesNoQuestionVA(0,
                                    VAR_BOOK(bk),
#ifdef ENG_LANG
                                    VAR_YESNO_QUESTION(Str2ID(L"Выйти?", ENC_UCS2, TEXTID_ANY_LEN)),
#else
                                    VAR_YESNO_QUESTION(Str2ID(L"Exit?", ENC_UCS2, TEXTID_ANY_LEN)),
#endif
                                    0);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_YES, YesPress);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_NO,  NoPress);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_BACK, NoPress);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_LONG_BACK, NoPress);
  ShowWindow(((MyBook*)bk)->gui1);
}

void ExitGM(BOOK*bk,GUI*)
{
  YesNo((MyBook*)bk);
}



void CreateGameM(MyBook*bk)
{
  last=0;
  if(bk->gui) GUI_Free(bk->gui);
  bk->gui=0;
  GUI_LIST*GameM=CreateListObject((BOOK*)bk,0);
  ((MyBook*)bk)->gui=GameM;
#ifdef ENG_LANG
  GuiObject_SetTitleText(GameM, Str2ID(L"Выбор уровня",ENC_UCS2,TEXTID_ANY_LEN));
#else
  GuiObject_SetTitleText(GameM, Str2ID(L"Choice level",ENC_UCS2,TEXTID_ANY_LEN));
#endif
  SetNumOfMenuItem(GameM, 3);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)GameM,SetTitlePunktsMR);
  SetCursorToItem(GameM,0);
  SetMenuItemStyle(GameM,0);
  GUIObject_Softkey_SetAction(GameM,ACTION_BACK,ExitNM);
  GUIObject_Softkey_SetAction(GameM,ACTION_LONG_BACK,ExitNM);
  GUIObject_Softkey_SetAction(GameM,ACTION_SELECT1,OnEntGameM);
  ShowWindow(GameM);
}



void OnEntGM(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    CreateGameM((MyBook*)bk);
  }
  if(item==1)
  {
    GUI_Free(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    BookObj_GotoPage(bk, &bk_db);
  }
  if(item==2)
  {
    CreateNM((MyBook*)bk,0);
  }
  if(item==3)
  {
    MenuResultat((MyBook*)bk);
  }
  if(item==4)
  {
    ShowInfo();
  }
  if(item==5)
  {
    ExitGM(bk,0);
  }
}

int SetTitlePunktsGM(GUI_MESSAGE * msg)
{
  int item;
  int ID;
  switch(GUIonMessage_GetMsg(msg))
  {
#ifdef ENG_LANG
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        ID=Str2ID(L"Новая игра",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Начать игру!",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"PROFILE_INCAR_LIST_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
        ID=Str2ID(L"Продолжить",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Сохранения",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"DB_ENTERTAINMENT_DIRECTORY_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==2)
      {
        ID=Str2ID(L"Настройки",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Поковыряца",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"MENU_SETTINGS_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==3)
      {
        ID=Str2ID(L"Результаты",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Просмотр результатов",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"MISCELLANEOUS_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==4)
      {
        ID=Str2ID(L"Инфо",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Информация",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"HELP_TEXT_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==5)
      {
        ID=Str2ID(L"Выход",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Выйти из игры",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"FORBIDDEN_NETWORK_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
#else
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        ID=Str2ID(L"New game",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Start game!",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"PROFILE_INCAR_LIST_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
        ID=Str2ID(L"Continue",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Saves",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"DB_ENTERTAINMENT_DIRECTORY_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==2)
      {
        ID=Str2ID(L"Settings",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Rummage",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"MENU_SETTINGS_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==3)
      {
        ID=Str2ID(L"Results",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Viewing results",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"MISCELLANEOUS_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==4)
      {
        ID=Str2ID(L"Info",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Information",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"HELP_TEXT_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
      if(item==5)
      {
        ID=Str2ID(L"Exit",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText0(msg,ID);
        ID=Str2ID(L"Exit from game",ENC_UCS2,TEXTID_ANY_LEN);
        SetMenuItemText1(msg,ID);
        iconidname2id(L"FORBIDDEN_NETWORK_ICN", -1, &ID);
        SetListObjectItemIcon(msg, 0, ID);
      }
#endif
  }
  return(1);
}

int CreateGM(void*, BOOK*bk,int x)
{
  int ID;
  GUI_LIST*GM=CreateListObject((BOOK*)bk,0);
  if(((MyBook*)bk)->gui) GUI_Free(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  ((MyBook*)bk)->gui=GM;
  ID=Str2ID(L"Tetris",ENC_UCS2,TEXTID_ANY_LEN);
  GuiObject_SetTitleText(GM, ID);
  SetNumOfMenuItem(GM, 6);
  OneOfMany_SetonMessage((GUI_ONEOFMANY*)GM,SetTitlePunktsGM);
  SetCursorToItem(GM,x);
  SetMenuItemStyle(GM,3);
  GUIObject_Softkey_SetAction(GM,ACTION_BACK,ExitGM);
  GUIObject_Softkey_SetAction(GM,ACTION_LONG_BACK,ExitGM);
  GUIObject_Softkey_SetAction(GM,ACTION_SELECT1,OnEntGM);
  ShowWindow(GM);
  return 0;
}
int Create(void*, BOOK*bk)
{
  CreateGM(0,bk,0);
  return 0;
}
