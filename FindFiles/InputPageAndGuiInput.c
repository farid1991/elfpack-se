#include "mcl.h"

void CMyBook::OnBackPress_GuiInput(CMyBook*mbk,wchar_t*string,int len)
{
  FUNCTION
  mbk->Free();
  elf_exit();
};

void CMyBook::OnEnterPress_GuiInput(CMyBook*mbk,wchar_t*string,int len)
{
  FUNCTION
  mbk->SetFindStr(string);
  GUIFREE(mbk->gui_input);
  mbk->GotoPage(&folder_page);
};

int CMyBook::OnInputPage(void*,CMyBook*mbk)
{
  FUNCTION
  if(mbk->GetFindStr())
  {
    mbk->GotoPage(&mbk->folder_page);
    return 1;
  }
  GUIFREE(mbk->gui_input);
  int TextID;
  textidname2id(L"WAP_ENTER_SK",-1,&TextID);
  mbk->gui_input=CreateStringInputVA(0,
                               VAR_STRINP_MIN_LEN(1),
                               VAR_STRINP_MODE(1),
                               VAR_BOOK(mbk),
                               VAR_HEADER_TEXT(TextID),
                               VAR_STRINP_FIXED_TEXT(TextID),
                               VAR_STRINP_TEXT(Str2ID(mbk->GetFindStr(),ENC_UCS2,TEXTID_ANY_LEN)),
                               VAR_PREV_ACTION_PROC(mbk->OnBackPress_GuiInput),
                               VAR_LONG_BACK_PROC(mbk->OnBackPress_GuiInput),
                               VAR_OK_PROC(mbk->OnEnterPress_GuiInput),
                               0);
  ((CGuiBase*)mbk->gui_input)->Show();
  return 0;
};
