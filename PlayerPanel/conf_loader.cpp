#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "migrate.h"

wchar_t successed_config_path[512];
wchar_t successed_config_name[512];

int SaveConfigData(const wchar_t *path,const wchar_t *fname);

#define BCFGLOADER_COMPLETE 0
#define BCFGLOADER_CONFIG_OLD_OR_CORRUPTED -1
#define BCFGLOADER_NOT_FOUND -2
#define BCFGLOADER_MEMORY_NOT_ALLOCATED -3
#define BCFGLOADER_PATH_OR_NAME_CORRUPTED -4
#define BCFGLOADER_UNKNOWN_ERROR -5
#define BCFGLOADER_SUCCESSED(a) a>=0 ? 1 : 0
#define BCFGLOADER_FAILED(a) a<0 ? 1 : 0
#pragma segment="CONFIG_C"
int LoadConfigData(const wchar_t *path,const wchar_t *fname)
{
  if (path==0 || fname==0)
  return BCFGLOADER_PATH_OR_NAME_CORRUPTED;
  int f;
  int result=BCFGLOADER_UNKNOWN_ERROR;
  void *cfg;
  FSTAT _fstat;
  unsigned int rlen;

  cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");
  if (fstat(path,fname,&_fstat)!=-1)
  {
    if (_fstat.fsize==len)
    {
      if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        rlen=fread(f, cfg, len);
        fclose(f);
        result=1;
      }
    }
    else
    {
      char *buf=new char[_fstat.fsize];
      if (buf)
      {
        if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
        {
          rlen=fread(f, buf, _fstat.fsize);
          fclose(f);
        }
        if (rlen==sizeof(CFG_test5))
        {
          MigrateTest5ToRelease((CFG_test5*)buf,(CFG_Release*)cfg);
          SaveConfigData(path,fname);
		  delete(buf);
          return 1;
        }
        delete(buf);
      }
      else
      {
        result=BCFGLOADER_MEMORY_NOT_ALLOCATED;
      }
    }
  }
  if (BCFGLOADER_SUCCESSED(result))
  {
    wstrcpy(successed_config_path,path);
    wstrcpy(successed_config_name,fname);
  }
  return(result);
};

int SaveConfigData(const wchar_t *path,const wchar_t *fname)
{
  void *cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");
  int f;
  int result=BCFGLOADER_UNKNOWN_ERROR;
  if ((f=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    if (fwrite(f,cfg,len)==len) result=BCFGLOADER_COMPLETE;
    fclose(f);
  }
  if (BCFGLOADER_SUCCESSED(result))
  {
    wstrcpy(successed_config_path,path);
    wstrcpy(successed_config_name,fname);
  }
  return result;
};

extern wchar_t dir[512];

void InitConfig(void)
{
 if (BCFGLOADER_FAILED(LoadConfigData(dir,L"PlayerPanel.bcfg")))
  {
   SaveConfigData(dir,L"PlayerPanel.bcfg");
  }
};
