#ifndef _SOFTKEYS_H_
#define _SOFTKEYS_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "quicksort.h"

wchar_t Softkeys_GetFreeAction(GUI *gui);
wchar_t Softkeys_FindAction(GUI *gui, int strid);

#endif
