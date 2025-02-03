#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr1_0 = {CFG_KEYCODE, "������ �������", 1, 0};
  CFGVAR int KeyStart = 0x53;
  CFGVAR int KeyStartPressMode = 0;
CFGVAR CFG_HDR cfghdr1 = {CFG_LEVEL, "", 0, 0};

CFGVAR CFG_HDR cfghdr2_0 = {CFG_CBOX, "������", 0, 4};
  CFGVAR int Start = 0;
  CFGVAR CFG_CBOX_ITEM cfghdr2_1[4] = {"����", "����������", "�����������", "������"};
CFGVAR CFG_HDR cfghdr2 = {CFG_LEVEL, "", 0, 0};

CFGVAR CFG_HDR cfghdr3_0 = {CFG_LEVEL, "�����", 1, 0};
  CFGVAR CFG_HDR cfghdr3_1 = {CFG_CHECKBOX, "��������������", 0, 2};
  CFGVAR int Sound = 1;
  CFGVAR CFG_HDR cfghdr3_2 = {CFG_CHECKBOX, "� ������ ��� �����", 0, 2};
  CFGVAR int Silent = 0;
  CFGVAR CFG_HDR cfghdr3_3 = {CFG_UINT, "���������", 0, 15};
  CFGVAR unsigned int Volume = 10;
  CFGVAR CFG_HDR cfghdr3_4 = {CFG_UTF16_STRING, "���� ��������", 0, 511};
  CFGVAR wchar_t SoundOpen[512] = L"/card/other/ZBin/Config/CameraLenscover/cameraopen.mp3";
  CFGVAR CFG_HDR cfghdr3_5 = {CFG_UTF16_STRING, "���� ��������", 0, 511};
  CFGVAR wchar_t SoundClose[512 ]= L"/card/other/ZBin/Config/CameraLenscover/cameraclose.mp3";
CFGVAR CFG_HDR cfghdr3 = {CFG_LEVEL, "", 0, 0};

CFGVAR CFG_HDR cfghdr4_0 = {CFG_CHECKBOX, "��������� ���� ���������", 0, 2};
  CFGVAR int UserInactivity = 1;
CFGVAR CFG_HDR cfghdr4 = {CFG_LEVEL, "", 0, 0};

CFGVAR CFG_HDR cfghdr5_0 = {CFG_LEVEL, "�������������� ������", 1, 0};
  CFGVAR CFG_HDR cfghdr5_1 = {CFG_CHECKBOX, "�������������� ������", 0, 0};
  CFGVAR int AutoLEDOff = 1;
  CFGVAR CFG_HDR cfghdr5_2 = {CFG_UINT, "����� �� ���������� (���)", 1, 1000};
  CFGVAR unsigned int AutoLEDOffTime = 60;
CFGVAR CFG_HDR cfghdr5 = {CFG_LEVEL, "", 0, 0};

CFGVAR CFG_HDR cfghdr6_0 = {CFG_LEVEL, "GUI", 1, 0};
  CFGVAR CFG_HDR cfghdr6_1 = {CFG_CBOX, "����� �������", 0, 3};
  CFGVAR int Focus = 0;
  CFGVAR CFG_CBOX_ITEM cfgcbox6_2[3] = {"����������", "�����������", "������"};
  CFGVAR CFG_HDR cfghdr6_3 = {CFG_CHECKBOX, "�������� ����", 0, 2};
  CFGVAR int Animation = 0;
  CFGVAR CFG_HDR cfghdr6_4 = {CFG_CBOX, "����� �������", 0, 3};
  CFGVAR int ItemStyle = 1;
  CFGVAR CFG_CBOX_ITEM cfgcbox6_5[3] = {"����� ������ ������", "������ ������ � ������", "��� ������ ������"};
CFGVAR CFG_HDR cfghdr6 = {CFG_LEVEL, "", 0, 0};


