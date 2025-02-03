#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "mcl.h"
int main(wchar_t*elf,wchar_t*findstr,wchar_t**folders,int num)
{
  trace_init();
  FUNCTION
  new CMyBook(findstr,folders,num);
  return 0;
};
