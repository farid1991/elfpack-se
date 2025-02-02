//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "MyFUNC.h"
#include "Pages.h"
void CreateSaveName(wchar_t*string, MyBook*bk);
void ExitPressSave(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  BookObj_GotoPage((BOOK *)(MyBK()),&bk_gs);
}


void OnEntInputSave(BOOK * bk,wchar_t * string,int len)
{
  wchar_t name[128];
  wstrcpy(name, string);
  wstrcat(name, L".stg");
  if(!FindFile(GetConfigPath(), name))
  {
    CreateSaveName(string, (MyBook*)(MyBK()));
    MessageBox(0x6fffffff, STR("Файл уже существует, введите другое имя"),0,0,0,0);
    return;
  }
  else
  {
    rename(MyBK()->path, MyBK()->name, MyBK()->path, name, 0);
    BookObj_GotoPage((BOOK*)(MyBK()),&bk_gm);
  }
}




void CreateSaveName(wchar_t*name, MyBook*bk)
{
  if(bk->gui1) GUI_Free(bk->gui1);
  bk->gui1=0;
  bk->gui1=CreateStringInput(0,
                                       VAR_BOOK((BOOK*)bk),
#ifdef ENG_LANG
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Имя:",ENC_UCS2, TEXTID_ANY_LEN)),
#else
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Name:",ENC_UCS2, TEXTID_ANY_LEN)),
#endif
                                       VAR_STRINP_TEXT(Str2ID(name, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(127),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputSave),
                                       0);
}

void SaveGame()
{
  int i=1;
  wchar_t name[128];
  wstrcpy(name, L"TetrisGame");
  wstrcat(name, chislo(i));
  wstrcat(name, L".stg");
  while(FindFile(GetConfigPath(),name)==0)
  {
    i++;
    wstrcpy(name, L"TetrisGame");
    wstrcat(name, chislo(i));
    wstrcat(name, L".stg");
  }
  wstrcpy(name, L"TetrisGame");
  wstrcat(name, chislo(i));
  CreateSaveName(name, MyBK());
  //rename(MyBK()->path, MyBK()->name, MyBK()->path, name, 0);
}

void YesPressSave(BOOK*bk,GUI*)
{
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  SaveGame();
}

void NoPressSave(BOOK*bk,GUI*)
{
  int er;
  FileDelete(MyBK()->path, MyBK()->name, &er);
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  BookObj_GotoPage((BOOK*)(MyBK()),&bk_gm);
}

void YesNoSave(MyBook*bk)
{
  bk->gui1=CreateYesNoQuestionVA(0,
                                    VAR_BOOK(bk),
                                    VAR_YESNO_QUESTION(Str2ID(L"Сохранить?", ENC_UCS2, TEXTID_ANY_LEN)),
                                    0);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_YES, YesPressSave);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_NO,  NoPressSave);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_BACK, ExitPressSave);
  GUIObject_Softkey_SetAction(bk->gui1, ACTION_LONG_BACK, ExitPressSave);
  ShowWindow(((MyBook*)bk)->gui1);
}

int CreateAsk(void*, BOOK*bk)
{
  if(MyBK()->gui1) GUI_Free(MyBK()->gui1);
  MyBK()->gui1=0;
  YesNoSave(MyBK());
  return 0;
}
