#ifndef _GUI_H_
#define _GUI_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "..\include\var_arg.h"
#include "NOfMany.h"
#include "file.h"

void CreateNameInput( BOOK *book, GUI *);
void CreateSizeInput( BOOK *book, GUI *);


extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);

#endif
