#ifndef HexEditor_H
#define HexEditor_H

#include "main.h"

enum MENU_ITEMS_T
{
    MENU_FILE_OPEN = 0,
    MENU_RAM_OPEN,
    MENU_FIND,
    MENU_GOTO_OFFSET,
    MENU_GOTO_PAGE,
    MENU_SETTINGS,
    MENU_SAVE_FILE,
    MENU_CLOSE_FILE,
    MENU_CLOSE_VIEW_RAM,
    MENU_EXIT,
    MENU_MAX = MENU_EXIT
};

typedef enum 
{
    HEX_MODE = 0,
    DEC_MODE,
    TEXT_MODE,
    MAX_MODE = TEXT_MODE
} VIEW_MODES_T;

typedef enum
{
  NOT_OPEN,
  OPEN_FILE,
  OPEN_RAM,
  NEW_WINDOW
} OPEN_MODE_T;

#endif
