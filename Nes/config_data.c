#undef _SOURCE_NAME_
#undef NAME_PREFIX

#define _SOURCE_NAME_ "config_data.c"
#define NAME_PREFIX
#include "confitems.h"

LEVEL("Клавиши")

  KEYCODE("A", key_a, mode_a, 0x16, 3)
  KEYCODE("B", key_b, mode_b, 0x17, 3)
  KEYCODE("Select", key_select, mode_select, 0x8, 3)
  KEYCODE("Start", key_start, mode_start, 0x22, 3)

  KEYCODE("Up", key_up, mode_up, 0x9, 3)
  KEYCODE("Down", key_down, mode_down, 0xD, 3)
  KEYCODE("Left", key_left, mode_left, 0xF, 3)
  KEYCODE("Right", key_right, mode_right, 0xB, 3)

LEVELUP

LEVEL("Настройки")

  UINT("frame_skip_max", frame_skip_max, 0, 60, 2)
  CBOX("machine_type", machine_type, 2, 0, "MACHINE_TYPE_NTSC", "MACHINE_TYPE_PAL")

LEVELUP

//=================== Язык ===============================================
CBOX("Language", Language, 2, 0, "Русский", "English")


//Для возможности вложения нескольких конфиг-хидеров и избежания кофликтов...
#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#define CD_CLEAN
#include <confitems.h>
#undef CD_CLEAN
#endif

