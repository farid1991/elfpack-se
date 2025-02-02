#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "cfg_v2_18.h"
#include "merge.h"

int tryMerge(void *cfg, const wchar_t *path, const wchar_t *fname)
{
  int f;
  char *buf;
  int result = -1;
  FSTAT _fstat;
  unsigned int rlen;

  if (fstat(path,fname,&_fstat) != -1)
  {
    if ((buf=new char[_fstat.fsize]))
    {
      if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0)) >= 0)
      {
        rlen=fread(f,buf,_fstat.fsize);
        fclose(f);
        if (rlen==_fstat.fsize)
        {
          merge_from_v2_17((v2_18_config_t *)cfg, buf, _fstat.fsize);
          result=0;
        }
      }
    }
    delete buf;
  }
  return(result);
}
