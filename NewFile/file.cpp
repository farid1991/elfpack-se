#include "file.h"

int CreateNewFile(wchar_t *name,int size)
{
	int f;
	W_FSTAT wfstat;
	char null=0;
	if((w_fstat(name,&wfstat))<0)
	{
		if ((f=w_fopen(name,WA_Create, 0x1FF, 0))>=0)
		{
			for(int i=0;i<size;i++) w_fwrite(f,&null,sizeof(char));
			w_fclose(f);
			return 0;
		}else return(-1);
	}else return(-2);
}

int FolderList_CreateFile(LIST *lst,wchar_t *name,int size)
{
	int result=0;
	for(int k=0;k<lst->FirstFree;k++)
	{
		wchar_t pathname[512];
		snwprintf(pathname,MAXELEMS(pathname),L"%ls/%ls",List_Get(lst,k),name);
		result=CreateNewFile(pathname,size);
	}
	return(result);
}
