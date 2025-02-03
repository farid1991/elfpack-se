#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\classlib\cl.h"
#include "CFile.h"

class CMyBook:public CBook
{

  DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
  ~CMyBook();
public:
  CMyBook();

  static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
  static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);
};

#endif

