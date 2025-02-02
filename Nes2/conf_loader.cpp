#include "../include/Lib_Clara.h"
#include "../include/Dir.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

#include "conf_loader.h"
#include "lang.h"

const wchar_t *successed_config_path=L"";
const wchar_t *successed_config_name=L"";

const wchar_t cname[] = { L"nes9x.bcfg" };

#pragma segment="CONFIG_C"
/*
unsigned long Crc32(unsigned char *buf, unsigned long len)
{
  unsigned long crc_table[256];
  unsigned long crc;
  for (int i = 0; i < 256; i++)
  {
    crc = i;
    for (int j = 0; j < 8; j++)
      crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
    crc_table[i] = crc;
  }
  crc = 0xFFFFFFFFUL;
  while (len--)
    crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
  return crc ^ 0xFFFFFFFFUL;
}

unsigned long ConfigCrc32()
{
  unsigned char *cfg;
  cfg=(unsigned char *)__segment_begin("CONFIG_C");
  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");
  return Crc32(cfg, len);
}*/

int SaveConfigData(const wchar_t *path,const wchar_t *fname)
{
  int f;
  char *buf;
  int result = -1;
  void *cfg;

  cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((buf=new char[len]))
  {
    if ((f=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0)) >= 0)
    {
      if (fwrite(f,cfg,len)==len) result = 0;
      fclose(f);
    }
    delete buf;
  }
  if (result >= 0)
  {
    successed_config_path=path;
    successed_config_name=fname;
  }
  return(result);
}

int LoadConfigData(const wchar_t *path,const wchar_t *fname)
{
  int f;
  char *buf;
  int result=-1;
  void *cfg;
  FSTAT _fstat;
  unsigned int rlen;

  cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((buf=new char[len]))
  {
    if (fstat(path,fname,&_fstat)!=-1)
    {
      if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        rlen=fread(f,buf,len);
        fclose(f);
        if (rlen!=_fstat.fsize || rlen!=len)  goto L_SAVENEWCFG;
        memcpy(cfg,buf,len);
        result=0;
      }
    }
    else
    {
    L_SAVENEWCFG:
      if ((f=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        if (fwrite(f,cfg,len)==len) result=0;
        fclose(f);
      }
    }
    delete buf;
  }
  if (result>=0)
  {
    successed_config_path=path;
    successed_config_name=fname;
    //UpdateLanguage();
  }
  return(result);
}


void InitConfig(void)
{
  if (LoadConfigData(GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL),cname)<0)
  {
    LoadConfigData(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),cname);
  }
}

