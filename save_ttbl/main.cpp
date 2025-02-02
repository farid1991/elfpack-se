#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include <intrinsics.h>

void elf_exit(void)
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

#include "commonc.h"

long tbl[16384/4];

int main (wchar_t* filename)
{
	trace_init();
	
	
	if(init())
	{	
		{
			unsigned long ptbl = __MRC( 15, 0, 2, 0, 0 );
			copyglobal(tbl, ptbl, 16384);
			
			w_chdir(GetDir(DIR_OTHER|MEM_EXTERNAL));
			int f=w_fopen(_T("ttbl.bin"),WA_Write+WA_Create+WA_Truncate,0x1FF,0);
			w_fwrite(f, tbl, sizeof(tbl));
			w_fclose(f);
		}
		
		
		
		long addr=0;
		int size=1024*1024;
		
		char* destbuf = new char[size];
		if(destbuf)
		{		
			copyglobal(destbuf, addr, size);
			
			wchar_t fname[48];		
			snwprintf(fname, sizeof(fname)/sizeof(fname[0]), _T("dump_%x_%x.bin"), addr, size);
			
			w_chdir(GetDir(DIR_OTHER|MEM_EXTERNAL));
			int f=w_fopen(fname,WA_Write+WA_Create+WA_Truncate,0x1FF,0);
			w_fwrite(f, destbuf, size);
			w_fclose(f);
			
			delete[] destbuf;			
		}else
		{
			debug_printf("\n!!! can't allocate %d bytes\n", size);
		}
		
		deinit();
		
	}else
	{
		debug_printf("\n!!!! can't allocate\n");
	}
	
	
	elf_exit();
	
	return 0;
}
