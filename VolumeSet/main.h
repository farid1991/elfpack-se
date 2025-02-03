#ifndef _MAIN_H_
#define _MAIN_H_

#define BOOK_NAME "VolumeSet"
#define ABOUT_TEXT "VolumeSet v. 2.0 Fix\n\n(c) E1kolyan"

typedef struct
{
  BOOK* book;
}MSG;

#define ELF_BCFG_CONFIG_EVENT 994

typedef struct
{
  BOOK* book;
  wchar_t* bcfg_p;
  wchar_t* bcfg_n;
}MSG_BCFG;

#endif
