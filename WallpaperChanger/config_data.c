#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr_01={CFG_CHECKBOX,"�������� ����� ��������",0,0};
CFGVAR int IS_ENABLED = 0;

CFGVAR CFG_HDR cfghdr_03={CFG_CHECKBOX,"��������� �����",0,0};
CFGVAR int IS_RANDOM = 0;

CFGVAR CFG_HDR cfghdr_04={CFG_CHECKBOX,"������ ���� Standby � ������",0,0};
CFGVAR int IS_STANDBY_FOCUS = 0;

CFGVAR CFG_HDR cfghdr_06={CFG_CHECKBOX,"������ � ������ ������",0,0};
CFGVAR int IS_SLEEP_ACTIVATED = 0;

CFGVAR CFG_HDR cfghdr_02={CFG_UTF16_STRING,"���� � ����� � ����������",0,255};
CFGVAR wchar_t PATH[256] = L"/";

CFGVAR CFG_HDR cfghdr_07={CFG_CHECKBOX,"�� ��������� �����",0,0};
CFGVAR int IS_IN_SUBFOLDERS = 0;
 
CFGVAR CFG_HDR level_open_1={CFG_LEVEL,"��������� �������",1,0};
  
  CFGVAR CFG_HDR cfghdr_11={CFG_UINT,"����� ����������",0,59};
  CFGVAR unsigned int REFRESH_TIME = 20;
  
  CFGVAR CFG_HDR cfghdr_12={CFG_CBOX,"������� ���������",0,3};
  CFGVAR int TIME_UNITS = 0;
  CFGVAR CFG_CBOX_ITEM cfgcbox_11[3]={"���","���","�"};
  
CFGVAR CFG_HDR level_close_1={CFG_LEVEL,"",0,0};

