#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "book.h"

#include "conf_loader.h"

int main(wchar_t *elfpath, wchar_t *fdir, wchar_t *fname)
{
  InitConfig();

  Book_Create();

  MyBook->fname = fname;
  MyBook->fdir = fdir;

  if( !fname )
    MyBook->fname = L"Mario.nes";

  if( !fdir )
    MyBook->fdir = L"/card/other";

  Book_GoMainPage();

  return 0;
}
