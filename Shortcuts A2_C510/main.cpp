#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

typedef struct
{
  BOOK book;
  char _unk;
  void *gui;
  char dummy[0x2C];
  wchar_t _unk2;
  char flag;
}MAIN_MENU_BOOK;

typedef struct
{
  char dummy[0x8];
  wchar_t *shortcut;
}LIST_ELEMENT;

typedef struct
{
  BOOK MsgBook;
  void *MsgGui;
}MSG_BOOK;

enum Keys_elfID{
  Left_ID=0,
  Central_ID=1,
  Right_ID=2,
};

enum Keys_stbID{
  Left_StbID=1004,
  Central_StbID=1003,
  Right_StbID=1005,
};

#define SHORTCUT_MAIN_MENU L"MainMenu"

#define KEY_SOFTKEY_LEFT 2
#define KEY_SOFTKEY_RIGHT 3
#define KEY_JOYSTICK_SELECT 8

#define KEY_SEND 6

#define __ELEMENT__(a,b) if(!((MAIN_MENU_BOOK*)a)->flag) b=*(short int*)((int)((MAIN_MENU_BOOK*)a)+((int)((MAIN_MENU_BOOK*)a)->_unk*10)+0x50); else b=*(short int*)((int)a+(int)((MAIN_MENU_BOOK*)a)->_unk*10+(int)((MAIN_MENU_BOOK*)a)->_unk2*2+0x5A)
#define __LIST__(a,b) b=(LIST*)(*(int*)(((int)((int)a+(((MAIN_MENU_BOOK*)a)->_unk2*4)))+0x2C))

int Key_elfID=0x0;
MAIN_MENU_BOOK *main_menu;

wchar_t *MenuBook_GetSelected(MAIN_MENU_BOOK *main)
{
  int element;
  LIST *lst;
  __ELEMENT__(main,element);
  __LIST__(main,lst);
  return (((LIST_ELEMENT*)List_Get(lst,element))->shortcut);
}

wchar_t *path[]={L"Left.txt",L"Central.txt",L"Right.txt"};

void Save_Element(wchar_t *buf,int elementID)
{
  int file;
  if((file=_fopen(L"/card/other/Shortcuts_A2",path[elementID],FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    fwrite(file,buf,wstrlen(buf)*2);
    fwrite(file,"\0\0",2);
    fclose(file);
  }
}

wchar_t* Read_Element(int elementID)
{
  int file;
  FSTAT _fstat;
  wchar_t *buf=0;
  if (fstat(L"/card/other/Shortcuts_A2",path[elementID],&_fstat)>=0)
  {
    if((file=_fopen(L"/card/other/Shortcuts_A2",path[elementID],FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      buf=new wchar_t[_fstat.fsize+1];
      fread(file,buf,_fstat.fsize);
      fclose(file);
    }
  }
  return buf;
}

void sh_run(BOOK *bk,GUI *)
{
  switch(SoftKeys_GetSelectedAction())
  {
  case Left_StbID:
    Shortcut_Run(Read_Element(Left_ID));
    break;
  case Central_StbID:
    Shortcut_Run(Read_Element(Central_ID));
    break;
  case Right_StbID:
    Shortcut_Run(Read_Element(Right_ID));
    break;
  }
}

void Update_Softkeys(void)
{
  GUI *gui=SBY_GetMainInput(Find_StandbyBook());
  GUIObject_SoftKeys_SetAction(gui,Left_StbID,sh_run);
  GUIObject_SoftKeys_SetText(gui,Left_StbID,Shortcut_Get_MenuItemName(Read_Element(Left_ID)));
  GUIObject_SoftKeys_SetAction(gui,Central_StbID,sh_run);
  GUIObject_SoftKeys_SetText(gui,Central_StbID,Shortcut_Get_MenuItemName(Read_Element(Central_ID)));
  GUIObject_SoftKeys_SetAction(gui,Right_StbID,sh_run);
  GUIObject_SoftKeys_SetText(gui,Right_StbID,Shortcut_Get_MenuItemName(Read_Element(Right_ID)));
}


int NewKey(int key,int r1,int mode,LPARAM,DISP_OBJ*)
{
  if(key==KEY_SEND && mode==KBD_LONG_RELEASE)
  {
    Save_Element(SHORTCUT_MAIN_MENU,Key_elfID);
    goto _msg;
  }
  if(key==KEY_SEND && mode==KBD_SHORT_RELEASE)
  {
    Save_Element(MenuBook_GetSelected(main_menu),Key_elfID);
  _msg:
    if(Key_elfID==0)
    {
    MessageBox(EMPTY_TEXTID,STR("Saved,select action for central softkey"),NOIMAGE,1,0,0);
    Key_elfID++;
    return 1;
    }
    if(Key_elfID==1)
    {
    MessageBox(EMPTY_TEXTID,STR("Saved,select action for right softkey"),NOIMAGE,1,0,0);
    Key_elfID++;
    return 1;
    }
    if(Key_elfID==2)
    {
    MessageBox(EMPTY_TEXTID,STR("Settings saved"),NOIMAGE,1,0,0);
    ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
    Update_Softkeys();
    return 1;
    }
  }
  return 1;
}

int main()
{
  main_menu=(MAIN_MENU_BOOK*)MenuBook_Desktop(0,-1);
  int MsgBookID=MessageBox(EMPTY_TEXTID,STR("Select action for left softkey"),NOIMAGE,1,0,0);
  ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
  Key_elfID=0;
  return 0;
}
