#undef _SOURCE_NAME_
#undef NAME_PREFIX

#define _SOURCE_NAME_ "config_data.c"
#define NAME_PREFIX
#include "confitems.h"

LEVEL("�������")

  KEYCODE("A", key_a, mode_a, 0x14, 3)
  KEYCODE("B", key_b, mode_b, 0x16, 3)
  KEYCODE("Select", key_select, mode_select, 0x13, 3)
  KEYCODE("Start", key_start, mode_start, 0x18, 3)

  KEYCODE("Up", key_up, mode_up, 0x15, 3)
  KEYCODE("Down", key_down, mode_down, 0x1B, 3)
  KEYCODE("Left", key_left, mode_left, 0x17, 3)
  KEYCODE("Right", key_right, mode_right, 0x19, 3)

LEVELUP

LEVEL("���������")

  UINT("Frame Skip", frameskip, 0, 60, 2)

LEVELUP

//=================== ���� ===============================================
CBOX("Language", Language, 2, 0, "�������", "English")


//��� ����������� �������� ���������� ������-������� � ��������� ���������...
#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#define CD_CLEAN
#include <confitems.h>
#undef CD_CLEAN
#endif

