#define _SOURCE_NAME_ "config_data.c"
#define NAME_PREFIX conf_
#include "confitems.h"

CBOX(LANG("Мова(Язык)","Язык(Мова)"), Language, 2, 0, "Русский", "Українська")

LEVEL(LANG("Ресурсы","Ресурси") )
  CBOX( LANG("Использовать", "Використати") , resource, 2, 0, LANG("внутренние", "внутрішні"), LANG("внешние", "зовнішні") )
  UTF16_STRING( LANG("Папка с ресурсами", "Папка з ресурсами"), resource_path, "%ELFS_INT%/Lines/Skins/default", 0, 255)
LEVELUP
LEVEL(LANG("Звуки","Звуки") )
  CHECKBOX(LANG("Выбранный шар", "Вибрана кулька"), cfg_snd_jump, 0);
  CHECKBOX(LANG("Уничтожение", "Знищення"), cfg_snd_destroy, 0);
  CHECKBOX(LANG("Перемещение", "Переміщення"), cfg_snd_move, 0);
  CHECKBOX(LANG("Невозможный ход", "Неможливий хід"), cfg_snd_cantmove, 0);
  UTF16_STRING( LANG("Папка со звуками", "Папка зі звуками"), cfg_snd_path, "%ELFS_INT%/Lines/Sound", 0, 255)
  CHECKBOX(LANG("В режиме без звука", "В беззвучному режимі"), cfg_silent, 0);
  UINT(LANG("Уровень громкости", "Рівень гучності"), cfg_vol, 0, 8, 2);
LEVELUP

#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#define CD_CLEAN
#include "confitems.h"
#undef CD_CLEAN
#undef _SOURCE_NAME_
#endif
