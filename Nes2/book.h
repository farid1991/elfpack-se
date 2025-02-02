#ifndef BOOK_H
#define BOOK_H

struct MyBOOK : BOOK
{

  wchar_t *fname;
  wchar_t *fdir;
};

extern MyBOOK *MyBook;

BOOK * Book_Create();
void Book_GoMainPage();

#define ELF_BCFG_CONFIG_EVENT 994

#endif
