#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "mydisp.h"

class CMyBook:public CBook
{
	CGuiBase* gui1;
        CGuiBase* gui2;
        CGuiBase* gui3;
	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();
public:
	CMyBook();
	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);
};

#endif
