#ifndef _MYBOOK_H_
	#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\\classlib\cl.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "rand.h"
#include "mydisp.h"

class CMyBook:public CBook
{
public:
	CGuiBase* gui;
	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();
public:
	CMyBook();
	static int Cancel(void *msg, CMyBook* book);
	static int Accept(void *msg, CMyBook* book);
};

#endif
