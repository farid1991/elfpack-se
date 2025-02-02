#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr_1 = {CFG_CHECKBOX,"Автоблокировка",0,0};
CFGVAR int auto_keylock = 0;

CFGVAR CFG_HDR cfghdr_2 = {CFG_CHECKBOX,"Блокировка кнопки Отбой",0,0};
CFGVAR int lock_key_call_end = 0;

CFGVAR CFG_HDR cfghdr_3 = {CFG_CHECKBOX,"Блокировка Активити меню",0,0};
CFGVAR int lock_key_task = 0;
