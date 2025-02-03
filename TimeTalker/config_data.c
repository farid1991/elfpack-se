#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdrd={CFG_UINT,"���� ���� (��)",0,10000};
CFGVAR unsigned int DELAY=800;

CFGVAR CFG_HDR cfghdrdv={CFG_UINT,"���������",0,15};
CFGVAR unsigned int VOLUME=0;

CFGVAR CFG_HDR cfghdr_s0={CFG_LEVEL,"����������",1,0};
	CFGVAR CFG_HDR cfghdr_s_1={CFG_UTF16_STRING,"����",0,255};
	CFGVAR wchar_t DIRSOUNDS[256]=L"/card/MSSEMC/Media files/other/ZBin/TimeTalker";
	CFGVAR CFG_HDR cfghdr_s_2={CFG_UTF16_STRING,"����������",0,10};
	CFGVAR wchar_t EXTENTION[11]=L"wav";
CFGVAR CFG_HDR cfghdr_s1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_k0={CFG_LEVEL,"������� ���������������",1,0};
	CFGVAR CFG_HDR cfghdr_k_1={CFG_CHECKBOX,"1: ������������",0,0};
	CFGVAR int KEYUSE1=0;
	CFGVAR CFG_HDR cfghdr_k_2={CFG_KEYCODE,"1: �������",0,0};
	CFGVAR int KEY1=0;
	CFGVAR int KEYMODE1=0;
	CFGVAR CFG_HDR cfghdr_k_3={CFG_CHECKBOX,"2: ������������",0,0};
	CFGVAR int KEYUSE2=0;
	CFGVAR CFG_HDR cfghdr_k_4={CFG_KEYCODE,"2: �������",0,0};
	CFGVAR int KEY2=0;
	CFGVAR int KEYMODE2=0;
CFGVAR CFG_HDR cfghdr_k1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_kp0={CFG_LEVEL,"�������� (����.)",1,0};
	CFGVAR CFG_HDR cfghdr_kp_1={CFG_CHECKBOX,"������",0,0};
	CFGVAR int CAMERA=1;
	CFGVAR CFG_HDR cfghdr_kp_1_={CFG_CHECKBOX,"��������",0,0};
	CFGVAR int SRECODER=1;
	CFGVAR CFG_HDR cfghdr_kp_2={CFG_CHECKBOX,"����������",0,0};
	CFGVAR int MPLAYER=1;
	CFGVAR CFG_HDR cfghdr_kp_3={CFG_CHECKBOX,"����������",0,0};
	CFGVAR int VPLAYER=1;
	CFGVAR CFG_HDR cfghdr_kp_4={CFG_CHECKBOX,"�����",0,0};
	CFGVAR int FMRADIO=1;
	CFGVAR CFG_HDR cfghdr_kp_5={CFG_CHECKBOX,"��� �����",0,0};
	CFGVAR int SILENCE=1;
CFGVAR CFG_HDR cfghdr_kp1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr6={CFG_CBOX,"�������������������",0,14};
CFGVAR int REPEAT=0;
CFGVAR CFG_CBOX_ITEM cfgcbox1[14]={"���������","1 ������","2 ������","5 �����","10 �����","15 �����","20 �����","30 �����","1 ���","2 ����","3 ����","4 ����","6 �����","12 �����"};

CFGVAR CFG_HDR cfghdr_ap0={CFG_LEVEL,"�������� (����)",1,0};
	CFGVAR CFG_HDR cfghdr_ap_1={CFG_CHECKBOX,"������",0,0};
	CFGVAR int AP_CAMERA=1;
	CFGVAR CFG_HDR cfghdr_ap_1_={CFG_CHECKBOX,"��������",0,0};
	CFGVAR int AP_SRECODER=1;
	CFGVAR CFG_HDR cfghdr_ap_2={CFG_CHECKBOX,"����������",0,0};
	CFGVAR int AP_MPLAYER=1;
	CFGVAR CFG_HDR cfghdr_ap_3={CFG_CHECKBOX,"����������",0,0};
	CFGVAR int AP_VPLAYER=1;
	CFGVAR CFG_HDR cfghdr_ap_4={CFG_CHECKBOX,"�����",0,0};
	CFGVAR int AP_FMRADIO=1;
	CFGVAR CFG_HDR cfghdr_ap_5={CFG_CHECKBOX,"��� �����",0,0};
	CFGVAR int AP_SILENCE=1;
CFGVAR CFG_HDR cfghdr_ap1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr_sl0={CFG_LEVEL,"��� (����)",1,0};
	CFGVAR CFG_HDR cfghdr_sl_1={CFG_CHECKBOX,"�����",0,0};
	CFGVAR int SLEEP=1;
	CFGVAR CFG_HDR cfghdrt_sl_2={CFG_TIME,"������ ���",0,0};
	CFGVAR TIME SLEEP_START={23,0,0};
	CFGVAR CFG_HDR cfghdrt_sl_3={CFG_TIME,"����� ���",0,0};
	CFGVAR TIME SLEEP_END={8,30,0};
CFGVAR CFG_HDR cfghdr_sl1={CFG_LEVEL,"",0,0};
