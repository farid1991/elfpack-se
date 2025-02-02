#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

//список процессов. прошивкозависимый.

wchar_t log[65536];
int logpos=0;

struct OS_pid_list
{
	OSBUFSIZE count;
	PROCESS list[1];
};

struct OS_pcb
{
	OSADDRESS type; /* should be cast to enum PROCESS_TYPE */
	OSADDRESS status; /*should be cast to enum PROCESS_STATUS */
	OSADDRESS priority; /*should be cast to OSPRIORITY*/
	OSUSER user;
	OSFSEMVAL fsemvalue;
	OSADDRESS sigqueye;
	OSADDRESS attach_list;
	OSADDRESS stack_top;
	OSADDRESS stack_limit;
	PROCESS remote_server;
	OSADDRESS sig_cnt_in_q;
	OSADDRESS sig_cnt_owned;
	OSADDRESS max_sigsize;
	OSADDRESS sigsel_size;
	OSADDRESS line;
	OSADDRESS file;
	OSADDRESS name;
	OSADDRESS cpuregs;
	OSADDRESS wanted;
	char strings[1];
};

/*
typedef enum PROCESS_STATUS
{
OS_RECEIVE=1,
OS_DELAY=2,
OS_SEMAPHORE=4,
OS_FSEMAPHORE=8,
OS_REMOTE=16,
OS_STOPPED=32,
OS_INTERCEPTED=64
}PROCESS_STATUS;
*/
struct STATUSES
{
	char* name;
	int st;
}statuses[]={
	"RECEIVE",1,
	"DELAY",2,
	"SEMAPHORE",4,
	"FSEMAPHORE",8,
	"REMOTE",16,
	"STOPPED",32,
	"INTERCEPTED",64
};

char* getstatusname(int status)
{
	static char tmp[256];
	int tmpsize=0;
	tmp[tmpsize]=0;
	for(int i=0;i<MAXELEMS(statuses);i++)
	{
		if(status & statuses[i].st)
		{
			if(tmpsize)
				tmp[tmpsize++]=' ';
			tmpsize+=sprintf(&tmp[tmpsize],"%s",statuses[i].name);
		}
	}
	return tmp;
}


//typedef OSBOOLEAN (*GET_MEM)(PROCESS pid, OSADDRESS from, void* to,OSADDRESS size);
//GET_MEM get_mem=(GET_MEM)0x4442B210;

typedef OSUSER (*GET_UID)(PROCESS pid);
GET_UID get_uid=(GET_UID)0x4442B284;

typedef struct OS_pid_list* (*GET_BID_LIST)(OSUSER user);
GET_BID_LIST get_bid_list=(GET_BID_LIST)0x4442B1D4;
GET_BID_LIST get_pid_list=(GET_BID_LIST)0x4442B22C;

typedef struct OS_pcb* (*GET_PCB)(PROCESS pid);
GET_PCB get_pcb=(GET_PCB)0x4442B220;

void elf_exit(void)
{
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

/////////////////////////////////////////////////////////////////////////
void exitproc()
{
	MessageBox(EMPTY_TEXTID,STR("elf stopped"),NOIMAGE,1,1000,0);
	elf_exit();
}

void msg()
{
	MessageBox(EMPTY_TEXTID,TextID_Create(log,ENC_UCS2,logpos),NOIMAGE, 2,0,NULL);
}

char* gettypename(int ptype)
{
	switch(ptype)
	{
	case OS_PRI_PROC : return "OS_PRI_PROC";
	case OS_BG_PROC  : return "OS_BG_PROC ";
	case OS_INT_PROC : return "OS_INT_PROC";
	case OS_TI_PROC  : return "OS_TI_PROC ";
	case OS_PHANTOM  : return "OS_PHANTOM ";
	case OS_BLOCK    : return "OS_BLOCK   ";
	case OS_ZOOMBIE  : return "OS_ZOOMBIE ";
	case OS_ILLEGAL  : return "OS_ILLEGAL ";
	}
	return "unknown";
}

int main (void)
{
	OSUSER id= get_uid( current_process() );

	logpos+=snwprintf(&log[logpos],256,_T("current user id=%x\n"),id);

	OS_pid_list* bidlist=get_bid_list(id);
	if(!bidlist)
	{
		logpos+=snwprintf(&log[logpos],256,_T("bidlist==NULL\n"));
	}else
	{

		logpos+=snwprintf(&log[logpos],256,_T("numblocks=%d\n"),bidlist->count);
		for(int i=0;i<bidlist->count;i++)
		{
			//logpos+=snwprintf(&log[logpos],256,_T("bid=%x\n"),bidlist->list[i]);
			struct OS_pcb* ppcb=get_pcb(bidlist->list[i]);
			if(ppcb!=NULL)
			{
				logpos+=snwprintf(&log[logpos],256,_T("block %08X:%s   t %08x\n"),bidlist->list[i],&ppcb->strings[ppcb->name],ppcb->type);
				OS_pid_list* pidlist=get_pid_list(bidlist->list[i]);
				if(pidlist)
				{
					//          logpos+=snwprintf(&log[logpos],256,_T("  processes=%d\n"),pidlist->count);
					for(int j=0;j<pidlist->count;j++)
					{
						struct OS_pcb* ppcb2=get_pcb(pidlist->list[j]);
						if(ppcb2!=NULL)
						{
							ppcb2->strings[0]=0;
							logpos+=snwprintf(&log[logpos],256,_T("  process: %08X:%-40.40s\tt %s status(%s) priority %02d user %d\n")
											  ,pidlist->list[j]
												  ,&ppcb2->strings[ppcb2->name]
													  ,gettypename(ppcb2->type)
														  ,getstatusname(ppcb2->status)
															  ,ppcb2->priority
																  ,get_uid(pidlist->list[j])
																	  );
							free_buf((union SIGNAL**)&ppcb2);
						}
					}

					free_buf((union SIGNAL**)&pidlist);
				}
				free_buf((union SIGNAL**)&ppcb);
			}
		}
		free_buf((union SIGNAL**)&bidlist);
	}

	int f=_fopen( GetDir(DIR_OTHER|MEM_EXTERNAL), _T("plist.txt") , FSX_O_CREAT|FSX_O_WRONLY, FSX_S_IWRITE|FSX_S_IREAD, 0);
	int z=0xFEFF;
	fwrite(f,(void*)&z,2);
	fwrite(f,(void*)&log,logpos*2);
	fclose(f);


	//  MessageBox(EMPTY_TEXTID,TextID_Create(log,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 2,0,NULL);

	elf_exit();
	return 0;
}
