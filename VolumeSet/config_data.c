#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr1_0 = {CFG_CBOX, "Режим громкости", 0, 2};
  CFGVAR int Mode = 0;
  CFGVAR CFG_CBOX_ITEM cfghdr1_1[2] = {"Последняя", "Постоянная"};
CFGVAR CFG_HDR cfghdr1 = {CFG_LEVEL, "", 0, 0};

CFGVAR CFG_HDR cfghdr2_0 = {CFG_UINT, "Громкость", 0, 15};
  CFGVAR unsigned int Volume = 10;
CFGVAR CFG_HDR cfghdr2 = {CFG_LEVEL, "", 0, 0};
