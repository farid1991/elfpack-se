#ifndef __cplusplus
#define __extfn
#define __export
#else
#define __extfn extern "C"
#define __export extern "C"
#endif

__extfn int biosInstall(char* name,int(*)(int) ,int num);
__extfn int biosOpen(char* name);
__extfn int biosUninstall(char* name);
__extfn int callbios(int b,int fn);
__extfn int rightspatch(int);

__extfn void __copytbl(void*);
__extfn void __copydata(void*,void*,long);
__extfn long __getdword(void*);
__extfn long __realaddr2(void* addr);
__extfn void __someend();


void (*copytbl)(void*);
void (*copydata)(void*,void*,long);
long (*getdword)(void*);
long (*realaddr2)(void* addr);


SIGNAL* ptr;
void* freebuff;
const int addlen = 4096;
bool init()
{
	long fnsz = ((char*)__someend)-((char*)__copytbl);
	
	ptr = alloc(fnsz+addlen+3,0);
	if(!ptr)
		return false;
	
	void* aligned = (void*)( (((long)ptr)+3)&~3);
	
	memcpy(aligned, (void*)__copytbl, fnsz);
	
	freebuff = fnsz+(char*)aligned;
	
	copytbl = (void(*)(void*))aligned;
	
	copydata = (void(*)(void*,void*,long))  ((char*)copytbl + ((char*)__copydata-(char*)__copytbl));
	getdword =  (long(*)(void*))  ((char*)copytbl + ((char*)__getdword-(char*)__copytbl));
	realaddr2 =  (long(*)(void*))  ((char*)copytbl + ((char*)__realaddr2-(char*)__copytbl));
	
	IMB();
	
	return true;
}

void deinit()
{
	free_buf(&ptr);
}

static void copyglobal(void* dst, long src, int sz)
{
	biosInstall("rightspatch",rightspatch,3);
	int bios = biosOpen("rightspatch");
	
	for(int i=0;i<sz;i+=addlen)
	{
		int tmpsz=sz-i<addlen ? (sz-i+3) : addlen;
		
		int temp = callbios(bios,0);
		
		copydata(freebuff, (void*)(src|i),(tmpsz+3)/4);			
		memcpy(i+(char*)dst,freebuff,tmpsz);
	}
	
	
	biosUninstall("rightspatch");
}


//typedef unsigned long taddr;
//#define TPTR(v) ((void*)v)
//#define TADDR(v) ((taddr)v)
//#define SMALLPAGEADDR(v) (v & ~0xFFF)
//#define SEGMENTADDR(v) (v & ~0xFFFFF)

//int smallpagecmp(void* a1,void* a2)
//{
//	return TADDR(a1)-TADDR(a2);
//}

