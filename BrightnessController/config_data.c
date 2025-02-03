#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

//Manual control
__root const CFG_HDR cfghdr1={CFG_LEVEL,"Manual Control",1,0};
__root const CFG_HDR cfghdr2={CFG_CHECKBOX,"Enable MSGBox",0,0};
__root const int IS_ENABLED=0;
__root const CFG_HDR cfghdr3={CFG_CHECKBOX,"Disable when MP etc.",0,0};
__root const int DISABLE_CONTROL=1;

//Keys Settings
__root const CFG_HDR cfghdr5={CFG_LEVEL,"Keys settings",1,0};
__root const CFG_HDR cfghdr6={CFG_KEYCODE,"Increasing key",0,0};
__root const int KEY1=0x16;
__root const int KEYMODE1=0;
__root const CFG_HDR cfghd7={CFG_KEYCODE,"Decreasing key",0,0};
__root const int KEY2=0x17;
__root const int KEYMODE2=0;
__root const CFG_HDR cfghdr8={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr8_11={CFG_LEVEL,"",0,0};
//End of General Settings


//Automatic mode
__root const CFG_HDR cfghdr9_1={CFG_LEVEL,"Automatic Mode",1,0};

//EnergySaver System Settings
__root const CFG_HDR cfghdr9={CFG_LEVEL,"EnergySaver System",1,0};
__root const CFG_HDR cfghdr10={CFG_CHECKBOX,"Enable?",0,0};
__root const int EnergySaver=1;
__root const CFG_HDR cfghdr11={CFG_INT,"Battery percent",1,100};
__root const int BattPercent=10;
__root const CFG_HDR cfghdr12={CFG_INT,"Decrease to",10,100};
__root const int DecreaseTo=20;
__root const CFG_HDR cfghdr13={CFG_LEVEL,"",0,0};

//Inteligence AutoBacklight System
__root const CFG_HDR cfghdr14={CFG_LEVEL,"Inteligence AutoBacklight",1,0};

__root const CFG_HDR cfghdr15={CFG_CHECKBOX,"Enable?",0,0};
__root const int SmartAB=1;

//First Interval
__root const CFG_HDR cfghdr14_0={CFG_LEVEL,"First Interval",1,0};
__root const CFG_HDR cfghdr16_0={CFG_INT,"00:00 to x (set x here)",0,23};
__root const int EndFirstInt=7;
__root const CFG_HDR cfghdr18_0={CFG_INT,"Brightness level:",10,100};
__root const int ValueOne=30;
__root const CFG_HDR cfghdr19_0={CFG_LEVEL,"",0,0};

//Second Interval
__root const CFG_HDR cfghdr14_1={CFG_LEVEL,"Second Interval",1,0};
__root const CFG_HDR cfghdr16_1={CFG_INT,"From x to y (set y here)",0,23};
__root const int EndSecInt=11;
__root const CFG_HDR cfghdr18_1={CFG_INT,"Brightness level:",10,100};
__root const int ValueTwo=50;
__root const CFG_HDR cfghdr19_1={CFG_LEVEL,"",0,0};

//Third Interval
__root const CFG_HDR cfghdr14_2={CFG_LEVEL,"Third Interval",1,0};
__root const CFG_HDR cfghdr16_2={CFG_INT,"From y to z (set z here)",0,23};
__root const int EndThirdInt=20;
__root const CFG_HDR cfghdr18_2={CFG_INT,"Brightness level:",10,100};
__root const int ValueThree=70;
__root const CFG_HDR cfghdr19_2={CFG_LEVEL,"",0,0};

//Fourth Interval
__root const CFG_HDR cfghdr14_3={CFG_LEVEL,"Fourth Interval",1,0};
__root const CFG_HDR cfghdr18_3={CFG_INT,"Level from z to 00:00",10,100};
__root const int ValueFour=30;
__root const CFG_HDR cfghdr19_3={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr19={CFG_LEVEL,"",0,0}; //End of InteligenceAutoBacklight level.

//Camera Settings
__root const CFG_HDR cfghdr20={CFG_LEVEL,"Camera settings",1,0};
__root const CFG_HDR cfghdr21={CFG_CHECKBOX,"Change brighntess in cam.?",0,0};
__root const int CamChange=1;
__root const CFG_HDR cfghdr22={CFG_INT,"Change to:",10,100};
__root const int CamLevel=100;
__root const CFG_HDR cfghdr23={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr19_11={CFG_LEVEL,"",0,0}; //End of Automatic Mode
