#define _SOURCE_NAME_ "config_data.c"
#define NAME_PREFIX conf_
#include "confitems.h"

LEVEL( LANG("��������","Input") )

 CHECKBOX( LANG("����������","Show") ,inshow,1)
 COORDINATES( LANG("����������","Coordinates") ,inxpos,inypos,3,90)
 FONT( LANG("������ ������", "Font size") ,infont,0xC)
 CBOX( LANG("������������", "Align") ,align_in, 3, 0, LANG("�����", "Left") , LANG("������", "Right") , LANG("�� ������", "Center") )
 COLOR_INT( LANG("���� ������", "Font color") ,infontcolor, 0xFFFFFFFF)
 COLOR_INT( LANG("���� �������", "Border color") ,inbordercolor,0xFF000000)
 UTF16_STRING( LANG("�������","Description"), indesc, "��������: ",0,20)

LEVELUP

LEVEL( LANG("���������", "Output") )

 CHECKBOX( LANG("����������", "Show") ,outshow,1)
 COORDINATES( LANG("����������", "Coordinates") ,outxpos,outypos,3,105)
 FONT( LANG("������ ������", "Font size") ,outfont,0xC)
 CBOX( LANG("������������", "Align") ,align_out,3, 0 , LANG("�����", "Left") , LANG("������", "Right") , LANG("�� ������", "Center") )
 COLOR_INT( LANG("���� ������", "Font color") ,outfontcolor,0xFFFFFFFF)
 COLOR_INT( LANG("���� �������", "Border color") ,outbordercolor,0xFF000000)
 UTF16_STRING( LANG("�������", "Description"), outdesc, "���������: " ,0,20)

LEVELUP

LEVEL( LANG("�����", "Total") )

 CHECKBOX( LANG("����������", "Show") ,totalshow,1)
 COORDINATES( LANG("����������", "Coordinates") ,totalxpos,totalypos, 3, 120)
 FONT( LANG("������ ������", "Font size") ,totalfont,0xC)
 CBOX( LANG("������������", "Align") ,align_total,3, 0 , LANG("�����", "Left") , LANG("������", "Right") , LANG("�� ������", "Center") )
 COLOR_INT( LANG("���� ������", "Font color") ,totalfontcolor,0xFFFFFFFF)
 COLOR_INT( LANG("���� �������", "Border color") ,totalbordercolor,0xFF000000)
 UTF16_STRING( LANG("�������", "Description"), totaldesc, "�����: " ,0,20)

LEVELUP

LEVEL( LANG("����� ����������", "Connection time") )

 CHECKBOX( LANG("����������", "Show") ,timeshow,1)
 COORDINATES( LANG("����������", "Coordinates") ,timexpos,timeypos, 3, 135)
 FONT( LANG("������ ������", "Font size") ,timefont,0xC)
 CBOX( LANG("������������", "Align") ,align_time,3, 0 , LANG("�����", "Left") , LANG("������", "Right") , LANG("�� ������", "Center") )
 COLOR_INT( LANG("���� ������", "Font color") ,timefontcolor,0xFFFFFFFF)
 COLOR_INT( LANG("���� �������", "Border color") ,timebordercolor,0xFF000000)
 UTF16_STRING( LANG("�������", "Description"), timedesc, "�����: ",0,20)

LEVELUP

LEVEL( LANG("����������� �������","Traffic restriction") )

 CHECKBOX( LANG("��������� ��� ����������","Turn off") ,set_size,1)
 UINT( LANG("����� �������","Limit traffic") ,max_size,10,10000,100)
 CBOX( LANG("������� ���������", "Units") ,size_unit,2, 0, LANG("�����", "KB"), LANG("�����", "MB") )

LEVELUP

LEVEL( LANG("�������������", "Other") )

 CHECKBOX( LANG("������ � ������ ��������", "Only in standby") ,standby_only,1)
 CBOX( LANG("����������", "Show") ,activ_session,2, 0, LANG("��� �������� �����", "When session is active"), LANG("������", "Always") )

LEVELUP

CBOX(LANG("Language(����)","����(Language)"), Language, 2, 0, "�������", "English")


#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#define CD_CLEAN
#include "confitems.h"
#undef CD_CLEAN
#undef _SOURCE_NAME_
#endif
