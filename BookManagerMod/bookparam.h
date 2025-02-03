#ifndef BOOKPARAM_H_
#define BOOKPARAM_H_

typedef struct
{
  char book[128];
  wchar_t name[128];
  wchar_t i_name[128];
}BOOK_PARAM_4_21;

typedef struct
{
  char book[32];
  wchar_t name[64];
  wchar_t i_name[64];
}BOOK_PARAM;

typedef struct
{
  char flag[2];
  char version[3];
}INFO;

void WritePARAM(BOOK_PARAM* data, const int n);
int GetPARAM(char* BookName, BOOK_PARAM* data);
#endif
