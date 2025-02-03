#define _SOURCE_NAME_ "config_data.c"
#define NAME_PREFIX conf_
#include "confitems.h"

CBOX(LANG("����(����)","����(����)"), Language, 2, 0, "�������", "���������")

LEVEL(LANG("�������","�������") )
  CBOX( LANG("������������", "�����������") , resource, 2, 0, LANG("����������", "�������"), LANG("�������", "������") )
  UTF16_STRING( LANG("����� � ���������", "����� � ���������"), resource_path, "%ELFS_INT%/Lines/Skins/default", 0, 255)
LEVELUP
LEVEL(LANG("�����","�����") )
  CHECKBOX(LANG("��������� ���", "������� ������"), cfg_snd_jump, 0);
  CHECKBOX(LANG("�����������", "��������"), cfg_snd_destroy, 0);
  CHECKBOX(LANG("�����������", "����������"), cfg_snd_move, 0);
  CHECKBOX(LANG("����������� ���", "���������� ���"), cfg_snd_cantmove, 0);
  UTF16_STRING( LANG("����� �� �������", "����� � �������"), cfg_snd_path, "%ELFS_INT%/Lines/Sound", 0, 255)
  CHECKBOX(LANG("� ������ ��� �����", "� ����������� �����"), cfg_silent, 0);
  UINT(LANG("������� ���������", "г���� �������"), cfg_vol, 0, 8, 2);
LEVELUP

#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#define CD_CLEAN
#include "confitems.h"
#undef CD_CLEAN
#undef _SOURCE_NAME_
#endif
