#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\classlib\cl.h"
#include "conf_loader.h"
#include "mybook.h"
#include "rand.h"

wchar_t dir[512];

void elf_exit(void)
{
	trace_done();
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int cmp_proc(BOOK* book, int* param )
{
  if(!strcmp(book->xbook->name,(char*)param)) return 1;
  return 0;
}

int main (void)
{
	if(!FindBookEx(cmp_proc,(int*)"PlayerPanel"))
	{
		trace_init();
		wstrcpy( dir, GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL) );
		wstrcat( dir, L"/PlayerPanel" );
		InitConfig();
		DATETIME dt;
		REQUEST_DATEANDTIME_GET(0,&dt);
		srand((dt.time.sec<<16) | (dt.time.min<<8) | (dt.time.hour));
		FreeBook(FindBookEx(cmp_proc,(int*)"FeedbackTickerBook"));
		new CMyBook();
	}
	else
	{
		UI_Event_toBookID(ELF_TERMINATE_EVENT,BookObj_GetBookID(FindBookEx(cmp_proc,(int*)"PlayerPanel")));
		MessageBox(EMPTY_TEXTID,STR("Ёльф выгружен!"),NOIMAGE,1,5000,0);
	}
	return 0;
}
