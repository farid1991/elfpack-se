#ifndef _FILE_H_
#define _FILE_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"

int CreateNewFile(wchar_t *name,int size);
int FolderList_CreateFile(LIST *lst,wchar_t *name,int size);

#endif
