#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

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

wchar_t *path[]={L"Left.txt",L"Central.txt",L"Right.txt"};

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
  GUI* gui=SBY_GetMainInput(Find_StandbyBook());
  GUIObject_SoftKeys_SetAction(gui,Left_StbID,sh_run);
  GUIObject_SoftKeys_SetText(gui,Left_StbID,Shortcut_Get_MenuItemName(Read_Element(Left_ID)));
  GUIObject_SoftKeys_SetAction(gui,Central_StbID,sh_run);
  GUIObject_SoftKeys_SetText(gui,Central_StbID,Shortcut_Get_MenuItemName(Read_Element(Central_ID)));
  GUIObject_SoftKeys_SetAction(gui,Right_StbID,sh_run);
  GUIObject_SoftKeys_SetText(gui,Right_StbID,Shortcut_Get_MenuItemName(Read_Element(Right_ID)));
}

int main()
{
  Update_Softkeys();
  return 0;
}
