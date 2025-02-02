#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr_9={CFG_INT,"Скорость перелистывания",10,700};
__root const int TIME_R=50;

__root const CFG_HDR cfghdr88={CFG_CHECKBOX,"Отображение подложки",0,0};
__root const int BG = 0;

__root const CFG_HDR cfghdr1={CFG_COORDINATES,"Координаты подложки",0,0};
__root const int X0=120;
__root const int Y0=104;

__root const CFG_HDR cfghdr0={CFG_COORDINATES,"Координаты фона часы",0,0};
__root const int X1=74;
__root const int Y1=74;

__root const CFG_HDR cfghdr2={CFG_COORDINATES,"Координаты фона минуты",0,0};
__root const int X2=165;
__root const int Y2=74;

__root const CFG_HDR cfghdr3={CFG_COORDINATES,"Координаты часы 1",0,0};
__root const int X3=56;
__root const int Y3=72;

__root const CFG_HDR cfghdr4={CFG_COORDINATES,"Координаты часы 2",0,0};
__root const int X4=91;
__root const int Y4=72;

__root const CFG_HDR cfghdr5={CFG_COORDINATES,"Координаты минуты 1",0,0};
__root const int X5=147;
__root const int Y5=72;

__root const CFG_HDR cfghdr6={CFG_COORDINATES,"Координаты минуты 2",0,0};
__root const int X6=183;
__root const int Y6=72;

__root const CFG_HDR cfghdr_civ={CFG_LEVEL,"Изображения",1,0};
 __root const CFG_HDR cfghdr_images_1={CFG_UTF16_STRING,"Папка",0,255};
 __root const wchar_t IMAGE[255+1]=L"/usb/other/ZBin/Config/Manila";

 __root const CFG_HDR cfghdr_sc={CFG_INT,"Масштаб в %",0,1000};
 __root const int scale=100;
__root const CFG_HDR cfghdr_civ_end={CFG_LEVEL,"",0,0};


