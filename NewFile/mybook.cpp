#include "mybook.h"

extern void elf_exit(void);

GUI * BookObjGetGui(BOOK *book)
{
	return((GUI*)List_Get(book->xguilist->guilist,0));
}

int PAGE_ENTER_EVENT_PROC(void* msg, BOOK* book, PAGE_DESC* page_desc, LPARAM ClientData, u16 event)
{
	if(book && !strcmp(page_desc->name,"DataBrowser_Main_Page"))
	{
		int FileStrid;
		textidname2id(L"DB_MANAGE_FILE_TXT", TEXTID_ANY_LEN, &FileStrid );
		int FileSoftkeyAction=Softkeys_FindAction(BookObjGetGui(book),FileStrid);
		if(FileSoftkeyAction!=0xFFFF)
		{
			int MySoftkeyAction=Softkeys_GetFreeAction(BookObjGetGui(book));
			GUIObject_SoftKeys_SetAction(BookObjGetGui(book),MySoftkeyAction, CreateNameInput );
			GUIObject_SoftKeys_SetText(BookObjGetGui(book),MySoftkeyAction,STR("Создать"));
			GUIObject_SoftKeys_SetItemAsSubItem(BookObjGetGui(book),FileSoftkeyAction, MySoftkeyAction );
			GUIObject_SoftKeys_SetVisible(BookObjGetGui(book),MySoftkeyAction,true);
		}
	}
   return(0);
}

int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
        book->Free();
        return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
        MessageBox(EMPTY_TEXTID,STR("NewFile\n© MaPkiZzz"),NOIMAGE, 1 ,5000, *pbookmanbook);
        return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "NewFile_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END

//бука создаётся
CMyBook::CMyBook()  :CBook("NewFile",&base_page)
{
	ModifyUIPageHook( PAGE_ENTER_EVENT, PAGE_ENTER_EVENT_PROC,0,PAGE_HOOK_ADD_AFTER);
}

//бука убивается
CMyBook::~CMyBook()
{
	ModifyUIPageHook( PAGE_ENTER_EVENT, PAGE_ENTER_EVENT_PROC,0,PAGE_HOOK_REMOVE);
	//убийство книги приводит к убийству эльфа
	SUBPROC(elf_exit);
}

