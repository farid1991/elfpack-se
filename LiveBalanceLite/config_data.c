#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

  __root const CFG_HDR cfghdr_check={CFG_LEVEL,"�������� �����",1,0};
  __root const CFG_HDR cfghdrC={CFG_CHECKBOX,"��������� ������",0,0};
  __root const int on_call=1;

  __root const CFG_HDR cfghdrIn={CFG_CHECKBOX,"�������� ������",0,0};
  __root const int on_con=1;

  __root const CFG_HDR cfghdrS={CFG_CHECKBOX,"�������� SMS",0,0};
  __root const int on_sms=1;

  __root const CFG_HDR cfghdrM={CFG_CHECKBOX,"�������� MMS",0,100};
  __root const int on_mms=1;

  __root const CFG_HDR cfghdrI={CFG_CHECKBOX,"�������� (���������� ������)",0,100};
  __root const int on_net=1;

  __root const CFG_HDR cfghdrIen={CFG_CHECKBOX,"�������� (�������� ������)",0,100};
  __root const int on_net_est=1;

  __root const CFG_HDR cfghdr_checkx={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr0={CFG_STR_WIN1251,"����� �������",0,31};
  __root const char CASHREQNUM[32]="*160*1*1#";

  __root const CFG_HDR cfghdr000={CFG_UINT,"������������ (������)",0,1440};
  __root const unsigned int CHECK_MIN=0;

  __root const CFG_HDR cfghdr02={CFG_STR_WIN1251,"������� ��� + �������",0,31};
  __root const char CashPatPlus[32]="������= # ���.";

   __root const CFG_HDR cfghdr03={CFG_STR_WIN1251,"������� ��� - �������",0,31};
  __root const char CashPatMinus[32]="������= -# ���.";

  __root const CFG_HDR cfghdr_src={CFG_UTF16_STRING,"��� �����",0,11};
  __root const wchar_t src[12]=L" ���";

  __root const CFG_HDR cfghdr_playerOn={CFG_CBOX,"��� ����������",0,2};
  __root const int set_text=0;
  __root const CFG_CBOX_ITEM cfgcbox_playerOn[2]={"�������� �����","�������"};

  __root const CFG_HDR cfghdr_m12={CFG_LEVEL,"�������������",1,0};

  __root const CFG_HDR cfghdr_standby_only={CFG_CHECKBOX,"������ � ������ ��������",0,0};
  __root const int standby_only = 1;

   __root const CFG_HDR cfghdr_log0b={CFG_LEVEL,"��������� ����",1,0};

  __root const CFG_HDR cfghdr_log1={CFG_CHECKBOX,"����� ���",0,0};
  __root const int log = 0;

  __root const CFG_HDR cfghdr_log2={CFG_UTF16_STRING,"����� ��� ����",0,255};
  __root const wchar_t log_path[256]=L"";

   __root const CFG_HDR cfghdr_log0e={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_pr={CFG_LEVEL,"�������� ���",1,0};

  __root const CFG_HDR cfghdr_pr1={CFG_COORDINATES,"����������",0,0};
  __root const unsigned int IDLE_X_BAR=0;
  __root const unsigned int IDLE_Y_BAR=65;

  __root const CFG_HDR cfghdr_pr2={CFG_COLOR_INT,"���� �����",0,0};
  __root const int COLOR_BORDER_BAR=0xFFFFFFFF;

  __root const CFG_HDR cfghdr_pr3={CFG_COLOR_INT,"���� �������",0,0};
  __root const int COLOR_FILL_BAR=0xFF0000FF;

  __root const CFG_HDR cfghdr_pr4={CFG_COLOR_INT,"���� ������",0,0};
  __root const int COLOR_TEXT_BAR=0xFFFFFFFF;

  __root const CFG_HDR cfghdr_pr5={CFG_FONT,"������ ������",0,100};
  __root const unsigned int FONT_BAR=0x18;

  __root const CFG_HDR cfghdr_prx={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_ts={CFG_LEVEL,"�����",1,0};

  __root const CFG_HDR align_ts1={CFG_CBOX,"������������",0,3};
  __root const int align=1;
  __root const CFG_CBOX_ITEM align_ts1x[3]={"�����","������","�� ������"};

  __root const CFG_HDR cfghdr_ts2={CFG_COORDINATES,"����������",0,0};
  __root const unsigned int IDLE_X_TEXT=173;
  __root const unsigned int IDLE_Y_TEXT=65;

  __root const CFG_HDR cfghdr_ts3={CFG_COLOR_INT,"���� ������",0,0};
  __root const int COLOR_TEXT=0xFFFFFFFF;

  __root const CFG_HDR cfghdr_ts4={CFG_COLOR_INT,"���� �������",0,0};
  __root const int COLOR_BORDER_TEXT=0xFF000000;

  __root const CFG_HDR cfghdr_bord_tipe={CFG_CBOX,"��� �������",0,4};
  __root const int TIPE_BORDER = 0;
  __root const CFG_CBOX_ITEM cfgcbox_bord_tipe_x[4]={"���","������ V.1","������ V.2","����"};

  __root const CFG_HDR cfghd_ts5={CFG_FONT,"������ ������",0,100};
  __root const unsigned int FONT_TEXT=0x18;

  __root const CFG_HDR cfghdr_tsx={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_fx={CFG_LEVEL,"",0,0};

 __root const CFG_HDR cfghdr_m13={CFG_LEVEL,"",0,0};
