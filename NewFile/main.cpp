#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int cmp_proc( BOOK* book , int* param )
{
	return !strcmp( book->xbook->name, (char*)param );
}

int main (void)
{
	BOOK * myBook = FindBookEx( cmp_proc, (int*)"NewFile" );
	if(!myBook) new CMyBook();
	else
	{
		UI_Event_toBookID(ELF_TERMINATE_EVENT,BookObj_GetBookID(myBook));
		MessageBox(EMPTY_TEXTID,STR("Ёльф выгружен!"),NOIMAGE,1,5000,0);
	}
	return 0;
}
