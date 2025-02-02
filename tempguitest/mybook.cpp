#include "..\deleaker\mem2.h"
#include "mybook.h"


extern void elf_exit(void);


int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(0x6fFFFFFF,STR("Myelf\n\n(c) mcming1989 "),0, 1 ,5000, *pbookmanbook);
	return 1;
}



DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "MY_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END

//первый вызов из pg_UICLH_OGCallBook_OngoingCall::PAGE_ENTER_EVENT
void (*test1)(GUI *)=(void(*)(GUI *))0x10A40E49;

CMyBook::CMyBook()  :CBook("MY_Book",&base_page)
{
  gui1 = new CGuiT<CFirstDisp>(this,0);
  gui1->Show();
  gui1->SetStyle(4);
  gui1->SoftKeys_Hide();
  gui1->SetTitleType(1);

  gui2 = new CGuiT<CSecondDisp>(this,0);
  gui2->Show();
  gui2->SetStyle(4);
  gui2->SoftKeys_Hide();
  gui2->SetTitleType(1);

  gui3 = new CGuiT<CThirdDisp>(this,0);
  gui3->Show();
  gui3->SetStyle(4);
  gui3->SoftKeys_Hide();
  gui3->SetTitleType(1);

  test1(gui1);
}


CMyBook::~CMyBook()
{
   if(gui1) gui1->Free();
   if(gui2) gui1->Free();
   if(gui3) gui1->Free();
   elf_exit();
}
