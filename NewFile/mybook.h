#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "softkeys.h"
#include "gui.h"

class CMyBook:public CBook
{
  CGuiBase* gui;

  DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
  ~CMyBook();
public:
  static void onTimer (u16 unk , void * data);
  CMyBook();

  static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
  static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);
  static int Accept(void *msg, CMyBook* book);
  static int Cancel(void *msg, CMyBook* book);

};

#endif
