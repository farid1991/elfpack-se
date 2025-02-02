#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_LEVEL,"Общие настройки",1,0};
__root const CFG_HDR cfghdr1={CFG_KEYCODE,"Клавиша запуска",0,0};
__root const int KEY_START=0x2A;
__root const int KEY_START_MODE=0;

__root const CFG_HDR cfghdr2={CFG_CHECKBOX,"Поверх всех окон",0,0};
__root const int ONTOP=1;

__root const CFG_HDR cfghdr3={CFG_CHECKBOX,"Управл.плеером поверх всех окон",0,0};
__root const int ONPLAYERCONTROL=1;

__root const CFG_HDR cfghdr4={CFG_RECT,"Координаты фона",0,0};
__root const RECT BACKGROUND_RC={0,240,50,131};

__root const CFG_HDR cfghdr5={CFG_COLOR_INT,"Цвет фона",0,0};
__root const unsigned int BACKGROUND_COLOR=0x00000000;

__root const CFG_HDR cfghdr6={CFG_CHECKBOX,"Показывать при смене трека",0,0};
__root const int ON_TRACK_CHANGE=1;

__root const CFG_HDR cfghdr7={CFG_UINT,"Автоскрытие(мс.)",0,999999999};
__root const unsigned int AUTO_HIDE_TIME=3000;

__root const CFG_HDR cfghdr8={CFG_UINT,"Шаг анимации",0,320};
__root const unsigned int STEP=10;

__root const CFG_HDR cfghdr9={CFG_CBOX,"Анимация показа",0,9};
__root const int ANIMATION_SHOW=0;
__root const CFG_CBOX_ITEM cfgcbox10[9]={"Сверху","Снизу","Слева","Справа","Сверху|Слева","Сверху|Справа","Снизу|Слева","Снизу|Справа","Случайно"};

__root const CFG_HDR cfghdr11={CFG_CBOX,"Анимация скрытия",0,9};
__root const int ANIMATION_HIDE=0;
__root const CFG_CBOX_ITEM cfgcbox12[9]={"Сверху","Снизу","Слева","Справа","Сверху|Слева","Сверху|Справа","Снизу|Слева","Снизу|Справа","Случайно"};
__root const CFG_HDR cfghdr13={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr14={CFG_LEVEL,"Текстовая информация",1,0};
__root const CFG_HDR cfghdr15={CFG_CHECKBOX,"Поддержка WIN1251",0,0};
__root const int WIN1251_SUPPORT=0;
__root const CFG_HDR cfghdr16={CFG_LEVEL,"Исполнитель",1,0};
__root const CFG_HDR cfghdr17={CFG_CHECKBOX,"Включить",0,0};
__root const int ARTIST_EN=1;
__root const CFG_HDR cfghdr18={CFG_RECT,"Координаты",0,0};
__root const RECT ARTIST_RC={80,120,50,100};
__root const CFG_HDR cfghdr19={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int ARTIST_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr20={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int ARTIST_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr21={CFG_FONT,"Шрифт",0,0};
__root const int ARTIST_FONT=0x12;
__root const CFG_HDR cfghdr22={CFG_CBOX,"Выравнивание",0,3};
__root const int ARTIST_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox23[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr24={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr25={CFG_LEVEL,"Альбом",1,0};
__root const CFG_HDR cfghdr26={CFG_CHECKBOX,"Включить",0,0};
__root const int ALBUM_EN=1;
__root const CFG_HDR cfghdr27={CFG_RECT,"Координаты",0,0};
__root const RECT ALBUM_RC={80,120,50,100};
__root const CFG_HDR cfghdr28={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int ALBUM_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr29={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int ALBUM_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr30={CFG_FONT,"Шрифт",0,0};
__root const int ALBUM_FONT=0x12;
__root const CFG_HDR cfghdr31={CFG_CBOX,"Выравнивание",0,3};
__root const int ALBUM_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox32[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr33={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr34={CFG_LEVEL,"Название",1,0};
__root const CFG_HDR cfghdr35={CFG_CHECKBOX,"Включить",0,0};
__root const int TITLE_EN=1;
__root const CFG_HDR cfghdr36={CFG_RECT,"Координаты",0,0};
__root const RECT TITLE_RC={80,120,50,100};
__root const CFG_HDR cfghdr37={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int TITLE_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr38={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int TITLE_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr39={CFG_FONT,"Шрифт",0,0};
__root const int TITLE_FONT=0x12;
__root const CFG_HDR cfghdr40={CFG_CBOX,"Выравнивание",0,3};
__root const int TITLE_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox41[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr42={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr43={CFG_LEVEL,"Год",1,0};
__root const CFG_HDR cfghdr44={CFG_CHECKBOX,"Включить",0,0};
__root const int YEAR_EN=1;
__root const CFG_HDR cfghdr45={CFG_RECT,"Координаты",0,0};
__root const RECT YEAR_RC={80,120,50,100};
__root const CFG_HDR cfghdr46={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int YEAR_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr47={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int YEAR_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr48={CFG_FONT,"Шрифт",0,0};
__root const int YEAR_FONT=0x12;
__root const CFG_HDR cfghdr49={CFG_CBOX,"Выравнивание",0,3};
__root const int YEAR_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox50[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr51={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr52={CFG_LEVEL,"Жанр",1,0};
__root const CFG_HDR cfghdr53={CFG_CHECKBOX,"Включить",0,0};
__root const int GENRE_EN=1;
__root const CFG_HDR cfghdr54={CFG_RECT,"Координаты",0,0};
__root const RECT GENRE_RC={80,120,50,100};
__root const CFG_HDR cfghdr55={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int GENRE_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr56={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int GENRE_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr57={CFG_FONT,"Шрифт",0,0};
__root const int GENRE_FONT=0x12;
__root const CFG_HDR cfghdr58={CFG_CBOX,"Выравнивание",0,3};
__root const int GENRE_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox59[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr60={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr61={CFG_LEVEL,"Каналы",1,0};
__root const CFG_HDR cfghdr62={CFG_CHECKBOX,"Включить",0,0};
__root const int CHANNEL_EN=1;
__root const CFG_HDR cfghdr63={CFG_RECT,"Координаты",0,0};
__root const RECT CHANNEL_RC={80,120,50,100};
__root const CFG_HDR cfghdr64={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int CHANNEL_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr65={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int CHANNEL_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr66={CFG_FONT,"Шрифт",0,0};
__root const int CHANNEL_FONT=0x12;
__root const CFG_HDR cfghdr67={CFG_CBOX,"Выравнивание",0,3};
__root const int CHANNEL_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox68[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr69={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr70={CFG_LEVEL,"Битрейт",1,0};
__root const CFG_HDR cfghdr71={CFG_CHECKBOX,"Включить",0,0};
__root const int BITRATE_EN=1;
__root const CFG_HDR cfghdr72={CFG_RECT,"Координаты",0,0};
__root const RECT BITRATE_RC={80,120,50,100};
__root const CFG_HDR cfghdr73={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  BITRATE_MASK[64]=L"%d kb/s";
__root const CFG_HDR cfghdr74={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int BITRATE_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr75={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int BITRATE_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr76={CFG_FONT,"Шрифт",0,0};
__root const int BITRATE_FONT=0x12;
__root const CFG_HDR cfghdr77={CFG_CBOX,"Выравнивание",0,3};
__root const int BITRATE_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox78[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr79={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr80={CFG_LEVEL,"Частота дискретизации",1,0};
__root const CFG_HDR cfghdr81={CFG_CHECKBOX,"Включить",0,0};
__root const int FREQUENCY_EN=1;
__root const CFG_HDR cfghdr82={CFG_RECT,"Координаты",0,0};
__root const RECT FREQUENCY_RC={80,120,50,100};
__root const CFG_HDR cfghdr83={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  FREQUENCY_MASK[64]=L"%d Hz";
__root const CFG_HDR cfghdr84={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int FREQUENCY_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr85={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int FREQUENCY_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr86={CFG_FONT,"Шрифт",0,0};
__root const int FREQUENCY_FONT=0x12;
__root const CFG_HDR cfghdr87={CFG_CBOX,"Выравнивание",0,3};
__root const int FREQUENCY_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox88[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr89={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr90={CFG_LEVEL,"Текущее время",1,0};
__root const CFG_HDR cfghdr91={CFG_CHECKBOX,"Включить",0,0};
__root const int CURRENT_TIME_EN=1;
__root const CFG_HDR cfghdr92={CFG_RECT,"Координаты",0,0};
__root const RECT CURRENT_TIME_RC={80,120,50,100};
__root const CFG_HDR cfghdr93={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  CURRENT_TIME_MASK[64]=L"%02d:%02d";
__root const CFG_HDR cfghdr94={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int CURRENT_TIME_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr95={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int CURRENT_TIME_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr96={CFG_FONT,"Шрифт",0,0};
__root const int CURRENT_TIME_FONT=0x12;
__root const CFG_HDR cfghdr97={CFG_CBOX,"Выравнивание",0,3};
__root const int CURRENT_TIME_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox98[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr99={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr100={CFG_LEVEL,"Общее время",1,0};
__root const CFG_HDR cfghdr101={CFG_CHECKBOX,"Включить",0,0};
__root const int FULL_TIME_EN=1;
__root const CFG_HDR cfghdr102={CFG_RECT,"Координаты",0,0};
__root const RECT FULL_TIME_RC={80,120,50,100};
__root const CFG_HDR cfghdr103={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  FULL_TIME_MASK[64]=L"%02d:%02d";
__root const CFG_HDR cfghdr104={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int FULL_TIME_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr105={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int FULL_TIME_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr106={CFG_FONT,"Шрифт",0,0};
__root const int FULL_TIME_FONT=0x12;
__root const CFG_HDR cfghdr107={CFG_CBOX,"Выравнивание",0,3};
__root const int FULL_TIME_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox108[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr109={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr110={CFG_LEVEL,"Оставшееся время",1,0};
__root const CFG_HDR cfghdr111={CFG_CHECKBOX,"Включить",0,0};
__root const int LEFT_TIME_EN=1;
__root const CFG_HDR cfghdr112={CFG_RECT,"Координаты",0,0};
__root const RECT LEFT_TIME_RC={80,120,50,100};
__root const CFG_HDR cfghdr113={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  LEFT_TIME_MASK[64]=L"%02d:%02d";
__root const CFG_HDR cfghdr114={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int LEFT_TIME_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr115={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int LEFT_TIME_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr116={CFG_FONT,"Шрифт",0,0};
__root const int LEFT_TIME_FONT=0x12;
__root const CFG_HDR cfghdr117={CFG_CBOX,"Выравнивание",0,3};
__root const int LEFT_TIME_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox118[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr119={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr120={CFG_LEVEL,"Кол-во трэков",1,0};
__root const CFG_HDR cfghdr121={CFG_CHECKBOX,"Включить",0,0};
__root const int TRACK_COUNT_EN=1;
__root const CFG_HDR cfghdr122={CFG_RECT,"Координаты",0,0};
__root const RECT TRACK_COUNT_RC={80,120,50,100};
__root const CFG_HDR cfghdr123={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  TRACK_COUNT_MASK[64]=L"/%02d";
__root const CFG_HDR cfghdr124={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int TRACK_COUNT_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr125={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int TRACK_COUNT_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr126={CFG_FONT,"Шрифт",0,0};
__root const int TRACK_COUNT_FONT=0x12;
__root const CFG_HDR cfghdr127={CFG_CBOX,"Выравнивание",0,3};
__root const int TRACK_COUNT_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox128[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr129={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr130={CFG_LEVEL,"Текущий трэк",1,0};
__root const CFG_HDR cfghdr131={CFG_CHECKBOX,"Включить",0,0};
__root const int CURRENT_TRACK_EN=1;
__root const CFG_HDR cfghdr132={CFG_RECT,"Координаты",0,0};
__root const RECT CURRENT_TRACK_RC={80,120,50,100};
__root const CFG_HDR cfghdr133={CFG_UTF16_STRING,"Маска",0,63};
__root const wchar_t  CURRENT_TRACK_MASK[64]=L"%02d:%02d";
__root const CFG_HDR cfghdr134={CFG_COLOR_INT,"Цвет текста",0,0};
__root const unsigned int CURRENT_TRACK_NORAMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr135={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int CURRENT_TRACK_BORDER_COLOR=0xFF000000;
__root const CFG_HDR cfghdr136={CFG_FONT,"Шрифт",0,0};
__root const int CURRENT_TRACK_FONT=0x12;
__root const CFG_HDR cfghdr137={CFG_CBOX,"Выравнивание",0,3};
__root const int CURRENT_TRACK_ALIGN=0;
__root const CFG_CBOX_ITEM cfgcbox138[3]={"Слева","Справа","По центру"};
__root const CFG_HDR cfghdr139={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr140={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr141={CFG_LEVEL,"Прогрессбар",1,0};
__root const CFG_HDR cfghdr142={CFG_CHECKBOX,"Включить",0,0};
__root const int PROGRESSBAR_EN=1;
__root const CFG_HDR cfghdr143={CFG_RECT,"Координаты",0,0};
__root const RECT PROGRESSBAR_RC={80,120,50,100};
__root const CFG_HDR cfghdr144={CFG_CHECKBOX,"Картинка на прогрессбаре",0,0};
__root const int PROGRESSBAR_IMG_EN=1;
__root const CFG_HDR cfghdr145={CFG_CBOX,"Тип",0,3};
__root const int PROGRESSBAR_TYPE=0;
__root const CFG_CBOX_ITEM cfgcbox146[3]={"Обычный","Закруглённый","Обратный"};
__root const CFG_HDR cfghdr147={CFG_COLOR_INT,"Цвет фона",0,0};
__root const unsigned int PROGRESSBAR_BACKGROUND_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr148={CFG_COLOR_INT,"Цвет обводки",0,0};
__root const unsigned int PROGRESSBAR_BORDER_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr149={CFG_COLOR_INT,"Цвет прогрессбара",0,0};
__root const unsigned int PROGRESSBAR_NORMAL_COLOR=0xFFFFFFFF;
__root const CFG_HDR cfghdr150={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr151={CFG_LEVEL,"Обложка",1,0};
__root const CFG_HDR cfghdr152={CFG_CHECKBOX,"Включить",0,0};
__root const int COVER_EN=1;
__root const CFG_HDR cfghdr153={CFG_RECT,"Координаты",0,0};
__root const RECT COVER_RC={80,120,50,100};
__root const CFG_HDR cfghdr154={CFG_LEVEL,"",0,0};
