#include "..\\include\cfg_items.h"
#include "config_data.h"

#include "..\\include\Colors.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr_1={CFG_COLOR_INT,"Цвет фона",0,200};
CFGVAR int BACKGROUND_COLOR = clWhite;

CFGVAR CFG_HDR cfghdr_2={CFG_COLOR_INT,"Цвет выделенной ячейки",0,200};
CFGVAR int HL_CELL_COLOR = 0x600000FF;

//----------------------

CFGVAR CFG_HDR cfghdr_level_1_b={CFG_LEVEL,"Заголовок",1,0};

  CFGVAR CFG_HDR cfghdr_l1_1={CFG_COORDINATES,"Координаты",0,0};
  CFGVAR int HEADER_X = 1;
  CFGVAR int HEADER_Y = 1;
  
  CFGVAR CFG_HDR cfghdr_l1_2={CFG_FONT,"Шрифт",0,0};
  CFGVAR int HEADER_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l1_3={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int HEADER_COLOR = clBlack;

CFGVAR CFG_HDR cfghdr_level_1_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_2_b={CFG_LEVEL,"Данные",1,0};

  CFGVAR CFG_HDR cfghdr_l2_1={CFG_FONT,"Шрифт",0,0};
  CFGVAR int DATA_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l2_2={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int DATA_COLOR = clBlack;
  
  CFGVAR CFG_HDR cfghdr_l2_3={CFG_COLOR_INT,"Цвет исправления",0,200};
  CFGVAR int DATA_CORRECT_COLOR = clBlue;

CFGVAR CFG_HDR cfghdr_level_2_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_3_b={CFG_LEVEL,"Сетка",1,0};

  CFGVAR CFG_HDR cfghdr_l3_1={CFG_COORDINATES,"Координаты сетки без адресов",0,0};
  CFGVAR int GRID_X = 84;
  CFGVAR int GRID_Y = 44;
  
  CFGVAR CFG_HDR cfghdr_l1_1_1={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int GRID_COLOR = 0x80000000;
  
  CFGVAR CFG_HDR cfghdr_l3_2={CFG_INT,"Количество строк",0,1000};
  CFGVAR int LINES_COUNT = 12;

  CFGVAR CFG_HDR cfghdr_l3_3={CFG_INT,"Количество столбцов",0,1000};
  CFGVAR int COLUMNS_COUNT = 5;

  CFGVAR CFG_HDR cfghdr_l3_4={CFG_INT,"Высота строки",0,400};
  CFGVAR int LINE_H = 20;

  CFGVAR CFG_HDR cfghdr_l3_5={CFG_INT,"Ширина столбца",0,400};
  CFGVAR int COLUMN_W = 32;
  
CFGVAR CFG_HDR cfghdr_level_3_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_4_b={CFG_LEVEL,"Инфо",1,0};

  CFGVAR CFG_HDR cfghdr_l4_1={CFG_FONT,"Шрифт",0,0};
  CFGVAR int INFO_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l4_2={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int INFO_COLOR = clBlack;

CFGVAR CFG_HDR cfghdr_level_4_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_5_b={CFG_LEVEL,"Текущий оффсет",1,0};

  CFGVAR CFG_HDR cfghdr_l5_1={CFG_FONT,"Шрифт",0,0};
  CFGVAR int OFFSET_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l5_2={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int OFFSET_COLOR = 0x80000000;

CFGVAR CFG_HDR cfghdr_level_5_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_6_b={CFG_LEVEL,"Номера столбцов",1,0};

  CFGVAR CFG_HDR cfghdr_l6_1={CFG_FONT,"Шрифт",0,0};
  CFGVAR int COLUMN_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l6_2={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int COLUMN_COLOR = clBlack;

CFGVAR CFG_HDR cfghdr_level_6_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_7_b={CFG_LEVEL,"Адреса слева",1,0};

  CFGVAR CFG_HDR cfghdr_l7_1={CFG_FONT,"Шрифт",0,0};
  CFGVAR int ADDRESS_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l7_2={CFG_COLOR_INT,"Цвет",0,200};
  CFGVAR int ADDRESS_COLOR = clBlack;

CFGVAR CFG_HDR cfghdr_level_7_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_level_8_b={CFG_LEVEL,"Меню",1,0};

  CFGVAR CFG_HDR cfghdr_l8_1={CFG_FONT,"Шрифт",0,0};
  CFGVAR int MENU_FONT = 18;
  
  CFGVAR CFG_HDR cfghdr_l8_2={CFG_COLOR_INT,"Цвет активного пункта",0,200};
  CFGVAR int MENU_ENABLE_TX_COLOR = clWhite;
  
  CFGVAR CFG_HDR cfghdr_l8_3={CFG_COLOR_INT,"Цвет неактивного пункта",0,200};
  CFGVAR int MENU_N_ENABLE_TX_COLOR = 0x80888888;
  
  CFGVAR CFG_HDR cfghdr_l8_4={CFG_COLOR_INT,"Цвет фона",0,200};
  CFGVAR int MENU_BG_COLOR = clBlack;
  
  CFGVAR CFG_HDR cfghdr_l8_5={CFG_COLOR_INT,"Цвет курсора",0,200};
  CFGVAR int MENU_CUR_COLOR = 0x600000FF;
  
  CFGVAR CFG_HDR cfghdr_l8_6={CFG_INT,"Расстояние между пунктами",0,1000};
  CFGVAR int INDENT_H = 4;
  
  CFGVAR CFG_HDR cfghdr_l8_7={CFG_INT,"Расстояние до края меню",0,1000};
  CFGVAR int INDENT_W = 2;

CFGVAR CFG_HDR cfghdr_level_8_e={CFG_LEVEL,"",0,0};

//----------------------

CFGVAR CFG_HDR cfghdr_6={CFG_UTF16_STRING,"Путь к BcfgEdit.elf",0,255};
CFGVAR wchar_t PATH_BCFGEDIT[256]=L"";
