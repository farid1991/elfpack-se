#ifndef LNGPARSER_H
#define LNGPARSER_H

typedef enum
{
    LNG_OPEN_FILE,
    LNG_OPEN_RAM,
    LNG_FIND,
    LNG_GOTO_OFFSET,
    LNG_GOTO_PAGE,
    LNG_SETTINGS,
    LNG_SAVE_FILE,
    LNG_CLOSE_FILE,
    LNG_CLOSE_VIEW_RAM,
    LNG_EXIT,
    
    LNG_SELECT_FILE,
    LNG_E_ENTER_TEXT,
    LNG_E_ILLEGAL_SYMB,
    LNG_E_OFFSET_NOT_AV,
    LNG_E_NOT_FOUNT,
    LNG_Q_SAVE_CHANGES,
    LNG_Q_NEW_WINDOW,
    LNG_MAX
} LNG_RES_ID_T;

typedef struct
{
    wchar_t *res;
    wchar_t *str[LNG_MAX];  
} LNG_RES_T;

extern LNG_RES_T Lang; 

#include "main.h"

int ReadLang(wchar_t *file);
void CloseLang(void);
wchar_t *RecLang(LNG_RES_ID_T num);

#endif

