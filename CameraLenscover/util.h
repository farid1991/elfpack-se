#ifndef _UTIL_H_
#define _UTIL_H_

struct
{
  char* name;
  int value;
}cfgmacro []=
{
  "AUDIO", DIR_AUDIO,
  "IMAGE", DIR_IMAGE,
  "VIDEO", DIR_VIDEO,
  "THEME", DIR_THEME,
  "OTHER", DIR_OTHER,
  "ELFS", DIR_ELFS,
  "ELFS_DAEMONS", DIR_ELFS_DAEMONS,
  "ELFS_CONFIG", DIR_ELFS_CONFIG,
  "INI", DIR_INI
};

void splitfilename(wchar_t* src, wchar_t* destpath, wchar_t* destname)
{
  wchar_t* fname = wstrrchr(src, '/');
  wstrncpy(destpath, src, fname-src);
  destpath[fname - src] = 0;
  wstrcpy(destname, fname + 1);
}

void extractdir(wchar_t* path, wchar_t* name, const wchar_t* src)
{
  int dir = -1;
  wchar_t dest[512];

  for(int i = 0; i < sizeof(cfgmacro) / sizeof(cfgmacro[0]); i++)
  {
    wchar_t tmp[32];
    int sz;
    sz = snwprintf(tmp, MAXELEMS(tmp), L"%%%s_INT%%", cfgmacro[i].name);
    if(!wstrcmpni(src, tmp, sz))
    {
      dir = cfgmacro[i].value|MEM_INTERNAL;
      src += sz;
      break;
    }
    sz=snwprintf(tmp, MAXELEMS(tmp), L"%%%s_EXT%%", cfgmacro[i]. name);
    if(!wstrcmpni(src, tmp, sz))
    {
      dir = cfgmacro[i].value|MEM_EXTERNAL;
      src += sz;
      break;
    }
  }
  
  if(dir != -1) wstrcpy(dest, GetDir(dir));
  else dest[0] = 0;
  
  wstrcat(dest + wstrlen(dest), src);
  splitfilename(dest, path, name);
}

#endif
