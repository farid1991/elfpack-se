#ifndef _MAIN_H_
#define _MAIN_H_

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

#define ELF_BCFG_CONFIG_EVENT 994

#define BOOKNAME    "WallpaperChanger"
#define DATE        __DATE__

#define DISPLAY                0
#define RESULT_OK              0
#define RESULT_FAIL           -1
#define FILE_HANDLE_INVALID   -1
#define FILE_INVALID          -1

#define FILENAME_MAX_LEN      255

typedef struct
{
  BOOK *book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
} MSG_BCFG;

#undef _MAIN_H_
#endif
