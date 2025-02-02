#include "../include/Lib_Clara.h"

#include "conf_loader.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

#include "lang.h"

#include <stdarg.h>

void SetCBox(const CFG_CBOX_ITEM *item, int num, ...)
{
  va_list marker;

  va_start( marker, num );
  for( int i = 0; i < num; i++ )
  {
    char *str = va_arg( marker, char *);

    strcpy( (char *)item[i].cbox_text, str );
  }
  va_end( marker );
}

#define CD_LANG_MODE

void RusConfig()
{
#define CD_LANG_RUS
#include "config_data.c"
#undef CD_LANG_RUS
}


void EngConfig()
{
#define CD_LANG_ENG
#include "config_data.c"
#undef CD_LANG_ENG
}

#undef CD_LANG_MODE

void UpdateLanguage()
{
  FSTAT stat;

  if( Language == 0 )
    RusConfig();
  else
    EngConfig();

  SaveConfigData(successed_config_path,successed_config_name);
}
