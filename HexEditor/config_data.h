#ifndef _CONFIGDATA_H_
  #define _CONFIGDATA_H_

#define EXTVAR _Pragma("location=\"CONFIG_C\"") extern const

EXTVAR int HEADER_X;
EXTVAR int HEADER_Y;
EXTVAR int HEADER_FONT;
EXTVAR int HEADER_COLOR;

EXTVAR int DATA_FONT;
EXTVAR int DATA_COLOR;
EXTVAR int DATA_CORRECT_COLOR;

EXTVAR int OFFSET_FONT;
EXTVAR int OFFSET_COLOR;

EXTVAR int ADDRESS_FONT;
EXTVAR int ADDRESS_COLOR;

EXTVAR int HL_CELL_COLOR;

EXTVAR int MENU_ENABLE_TX_COLOR;
EXTVAR int MENU_N_ENABLE_TX_COLOR;
EXTVAR int MENU_BG_COLOR;
EXTVAR int MENU_CUR_COLOR;
EXTVAR int INDENT_H;
EXTVAR int INDENT_W;

EXTVAR int BACKGROUND_COLOR;
EXTVAR int GRID_COLOR;

EXTVAR int INFO_FONT;
EXTVAR int INFO_COLOR;

EXTVAR int COLUMN_FONT;
EXTVAR int COLUMN_COLOR;

EXTVAR int GRID_X;
EXTVAR int GRID_Y;
EXTVAR int MENU_FONT;
EXTVAR int LINES_COUNT;
EXTVAR int COLUMNS_COUNT;
EXTVAR int LINE_H;
EXTVAR int COLUMN_W;
EXTVAR wchar_t PATH_BCFGEDIT[256];

#endif

