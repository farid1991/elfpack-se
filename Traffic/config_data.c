#define _SOURCE_NAME_ "config_data.c"
#define NAME_PREFIX conf_
#include "confitems.h"

LEVEL( LANG("Входящий","Input") )

 CHECKBOX( LANG("Показывать","Show") ,inshow,1)
 COORDINATES( LANG("Координаты","Coordinates") ,inxpos,inypos,3,90)
 FONT( LANG("Размер шрифта", "Font size") ,infont,0xC)
 CBOX( LANG("Выравнивание", "Align") ,align_in, 3, 0, LANG("Слева", "Left") , LANG("Справа", "Right") , LANG("По центру", "Center") )
 COLOR_INT( LANG("Цвет шрифта", "Font color") ,infontcolor, 0xFFFFFFFF)
 COLOR_INT( LANG("Цвет обводки", "Border color") ,inbordercolor,0xFF000000)
 UTF16_STRING( LANG("Подпись","Description"), indesc, "Входящий: ",0,20)

LEVELUP

LEVEL( LANG("Исходящий", "Output") )

 CHECKBOX( LANG("Показывать", "Show") ,outshow,1)
 COORDINATES( LANG("Координаты", "Coordinates") ,outxpos,outypos,3,105)
 FONT( LANG("Размер шрифта", "Font size") ,outfont,0xC)
 CBOX( LANG("Выравнивание", "Align") ,align_out,3, 0 , LANG("Слева", "Left") , LANG("Справа", "Right") , LANG("По центру", "Center") )
 COLOR_INT( LANG("Цвет шрифта", "Font color") ,outfontcolor,0xFFFFFFFF)
 COLOR_INT( LANG("Цвет обводки", "Border color") ,outbordercolor,0xFF000000)
 UTF16_STRING( LANG("Подпись", "Description"), outdesc, "Исходящий: " ,0,20)

LEVELUP

LEVEL( LANG("Общий", "Total") )

 CHECKBOX( LANG("Показывать", "Show") ,totalshow,1)
 COORDINATES( LANG("Координаты", "Coordinates") ,totalxpos,totalypos, 3, 120)
 FONT( LANG("Размер шрифта", "Font size") ,totalfont,0xC)
 CBOX( LANG("Выравнивание", "Align") ,align_total,3, 0 , LANG("Слева", "Left") , LANG("Справа", "Right") , LANG("По центру", "Center") )
 COLOR_INT( LANG("Цвет шрифта", "Font color") ,totalfontcolor,0xFFFFFFFF)
 COLOR_INT( LANG("Цвет обводки", "Border color") ,totalbordercolor,0xFF000000)
 UTF16_STRING( LANG("Подпись", "Description"), totaldesc, "Общий: " ,0,20)

LEVELUP

LEVEL( LANG("Время соединения", "Connection time") )

 CHECKBOX( LANG("Показывать", "Show") ,timeshow,1)
 COORDINATES( LANG("Координаты", "Coordinates") ,timexpos,timeypos, 3, 135)
 FONT( LANG("Размер шрифта", "Font size") ,timefont,0xC)
 CBOX( LANG("Выравнивание", "Align") ,align_time,3, 0 , LANG("Слева", "Left") , LANG("Справа", "Right") , LANG("По центру", "Center") )
 COLOR_INT( LANG("Цвет шрифта", "Font color") ,timefontcolor,0xFFFFFFFF)
 COLOR_INT( LANG("Цвет обводки", "Border color") ,timebordercolor,0xFF000000)
 UTF16_STRING( LANG("Подпись", "Description"), timedesc, "Время: ",0,20)

LEVELUP

LEVEL( LANG("Ограничение трафика","Traffic restriction") )

 CHECKBOX( LANG("Отключать при привышении","Turn off") ,set_size,1)
 UINT( LANG("Лимит трафика","Limit traffic") ,max_size,10,10000,100)
 CBOX( LANG("Единица измерения", "Units") ,size_unit,2, 0, LANG("Кбайт", "KB"), LANG("Мбайт", "MB") )

LEVELUP

LEVEL( LANG("Дополнительно", "Other") )

 CHECKBOX( LANG("Только в режиме ожидания", "Only in standby") ,standby_only,1)
 CBOX( LANG("Показывать", "Show") ,activ_session,2, 0, LANG("При активной сесии", "When session is active"), LANG("Всегда", "Always") )

LEVELUP

CBOX(LANG("Language(Язык)","Язык(Language)"), Language, 2, 0, "Русский", "English")


#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#define CD_CLEAN
#include "confitems.h"
#undef CD_CLEAN
#undef _SOURCE_NAME_
#endif
