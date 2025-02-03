//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "MyFUNC.h"
#include "Menu.h"
#include "Pages.h"

void OnEntII(BOOK * bk,wchar_t * string,int len)
{

  switch(((MyBook*)bk)->speed)
  {
  case 1:
    if(Perestava(((MyBook*)bk)->nast->rez->te, string, ((MyBook*)bk)->point, 10))
    {
    }
    break;
  case 2:
    if(Perestava(((MyBook*)bk)->nast->rez->tn, string, ((MyBook*)bk)->point, 10))
    {
    }
    break;
  case 3:
    if(Perestava(((MyBook*)bk)->nast->rez->th, string, ((MyBook*)bk)->point, 10))
    {
    }
    break;
  }
  GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  BookObj_GotoPage(isBookX(NameMyElf, 0),&bk_gm);
}

void CreateInputName(BOOK*bk)
{
  if(((MyBook*)bk)->gui1) GUI_Free(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  ((MyBook*)bk)->gui1=CreateStringInput(0,
                                       //VAR_PREV_ACTION_PROC(DestroyII),
                                       //VAR_LONG_BACK_PROC(DestroyII),
                                       VAR_BOOK((BOOK*)bk),
                                       VAR_STRINP_FIXED_TEXT(Str2ID(L"Вы установили рекорд\nвведите имя:",ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(Str2ID(L"Игрок", ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(8),
                                       VAR_STRINP_MIN_LEN(1),
                                       VAR_OK_PROC(OnEntII),
                                       0);
}

int CreateGO(void*, BOOK*bk)
{
  int result=0;
  switch(((MyBook*)bk)->speed)
  {
  case 1:
    if(Record(((MyBook*)bk)->nast->rez->te, ((MyBook*)bk)->point, 10))
    {
      result=1;
    }
  case 2:
    if(Record(((MyBook*)bk)->nast->rez->tn, ((MyBook*)bk)->point, 10))
    {
      result=1;
    }
  case 3:
    if(Record(((MyBook*)bk)->nast->rez->th, ((MyBook*)bk)->point, 10))
    {
      result=1;
    }
  }
  if(result)
  {
    PlayMyMusic(3);
    CreateInputName((BOOK*)bk);
    return result;
  }
  BookObj_GotoPage(bk,&bk_gm);
  return result;
}
