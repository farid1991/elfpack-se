#ifndef _MIGRATE_H_
	#define _MIGRATE_H_

#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"

typedef struct
{
 CFG_HDR cfghdr0;
 CFG_HDR cfghdr1;
 int KEY_START;
 int KEY_START_MODE;

 CFG_HDR cfghdr2;
 int ONTOP;

 CFG_HDR cfghdr3;
 RECT BACKGROUND_RC;

 CFG_HDR cfghdr4;
 unsigned int BACKGROUND_COLOR;

 CFG_HDR cfghdr5;
 int ON_TRACK_CHANGE;

 CFG_HDR cfghdr6;
 unsigned int AUTO_HIDE_TIME;

 CFG_HDR cfghdr7;
 unsigned int STEP;

 CFG_HDR cfghdr8;
 int ANIMATION_SHOW;
 CFG_CBOX_ITEM cfgcbox9[9];

 CFG_HDR cfghdr10;
 int ANIMATION_HIDE;
 CFG_CBOX_ITEM cfgcbox11[9];
 CFG_HDR cfghdr12;


 CFG_HDR cfghdr13;
 CFG_HDR cfghdr14;
 int WIN1251_SUPPORT;
 CFG_HDR cfghdr15;
 CFG_HDR cfghdr16;
 int ARTIST_EN;
 CFG_HDR cfghdr17;
 RECT ARTIST_RC;
 CFG_HDR cfghdr18;
 unsigned int ARTIST_NORAMAL_COLOR;
 CFG_HDR cfghdr19;
 unsigned int ARTIST_BORDER_COLOR;
 CFG_HDR cfghdr20;
 int ARTIST_FONT;
 CFG_HDR cfghdr21;
 int ARTIST_ALIGN;
 CFG_CBOX_ITEM cfgcbox22[3];
 CFG_HDR cfghdr23;

 CFG_HDR cfghdr24;
 CFG_HDR cfghdr25;
 int ALBUM_EN;
 CFG_HDR cfghdr26;
 RECT ALBUM_RC;
 CFG_HDR cfghdr27;
 unsigned int ALBUM_NORAMAL_COLOR;
 CFG_HDR cfghdr28;
 unsigned int ALBUM_BORDER_COLOR;
 CFG_HDR cfghdr29;
 int ALBUM_FONT;
 CFG_HDR cfghdr30;
 int ALBUM_ALIGN;
 CFG_CBOX_ITEM cfgcbox31[3];
 CFG_HDR cfghdr32;

 CFG_HDR cfghdr33;
 CFG_HDR cfghdr34;
 int TITLE_EN;
 CFG_HDR cfghdr35;
 RECT TITLE_RC;
 CFG_HDR cfghdr36;
 unsigned int TITLE_NORAMAL_COLOR;
 CFG_HDR cfghdr37;
 unsigned int TITLE_BORDER_COLOR;
 CFG_HDR cfghdr38;
 int TITLE_FONT;
 CFG_HDR cfghdr39;
 int TITLE_ALIGN;
 CFG_CBOX_ITEM cfgcbox40[3];
 CFG_HDR cfghdr41;

 CFG_HDR cfghdr42;
 CFG_HDR cfghdr43;
 int YEAR_EN;
 CFG_HDR cfghdr44;
 RECT YEAR_RC;
 CFG_HDR cfghdr45;
 unsigned int YEAR_NORAMAL_COLOR;
 CFG_HDR cfghdr46;
 unsigned int YEAR_BORDER_COLOR;
 CFG_HDR cfghdr47;
 int YEAR_FONT;
 CFG_HDR cfghdr48;
 int YEAR_ALIGN;
 CFG_CBOX_ITEM cfgcbox49[3];
 CFG_HDR cfghdr50;

 CFG_HDR cfghdr51;
 CFG_HDR cfghdr52;
 int GENRE_EN;
 CFG_HDR cfghdr53;
 RECT GENRE_RC;
 CFG_HDR cfghdr54;
 unsigned int GENRE_NORAMAL_COLOR;
 CFG_HDR cfghdr55;
 unsigned int GENRE_BORDER_COLOR;
 CFG_HDR cfghdr56;
 int GENRE_FONT;
 CFG_HDR cfghdr57;
 int GENRE_ALIGN;
 CFG_CBOX_ITEM cfgcbox58[3];
 CFG_HDR cfghdr59;

 CFG_HDR cfghdr60;
 CFG_HDR cfghdr61;
 int CHANNEL_EN;
 CFG_HDR cfghdr62;
 RECT CHANNEL_RC;
 CFG_HDR cfghdr63;
 unsigned int CHANNEL_NORAMAL_COLOR;
 CFG_HDR cfghdr64;
 unsigned int CHANNEL_BORDER_COLOR;
 CFG_HDR cfghdr65;
 int CHANNEL_FONT;
 CFG_HDR cfghdr66;
 int CHANNEL_ALIGN;
 CFG_CBOX_ITEM cfgcbox67[3];
 CFG_HDR cfghdr68;

 CFG_HDR cfghdr69;
 CFG_HDR cfghdr70;
 int BITRATE_EN;
 CFG_HDR cfghdr71;
 RECT BITRATE_RC;
 CFG_HDR cfghdr72;
 wchar_t  BITRATE_MASK[64];
 CFG_HDR cfghdr73;
 unsigned int BITRATE_NORAMAL_COLOR;
 CFG_HDR cfghdr74;
 unsigned int BITRATE_BORDER_COLOR;
 CFG_HDR cfghdr75;
 int BITRATE_FONT;
 CFG_HDR cfghdr76;
 int BITRATE_ALIGN;
 CFG_CBOX_ITEM cfgcbox77[3];
 CFG_HDR cfghdr78;

 CFG_HDR cfghdr79;
 CFG_HDR cfghdr80;
 int FREQUENCY_EN;
 CFG_HDR cfghdr81;
 RECT FREQUENCY_RC;
 CFG_HDR cfghdr82;
 wchar_t  FREQUENCY_MASK[64];
 CFG_HDR cfghdr83;
 unsigned int FREQUENCY_NORAMAL_COLOR;
 CFG_HDR cfghdr84;
 unsigned int FREQUENCY_BORDER_COLOR;
 CFG_HDR cfghdr85;
 int FREQUENCY_FONT;
 CFG_HDR cfghdr86;
 int FREQUENCY_ALIGN;
 CFG_CBOX_ITEM cfgcbox87[3];
 CFG_HDR cfghdr88;

 CFG_HDR cfghdr89;
 CFG_HDR cfghdr90;
 int CURRENT_TIME_EN;
 CFG_HDR cfghdr91;
 RECT CURRENT_TIME_RC;
 CFG_HDR cfghdr92;
 wchar_t  CURRENT_TIME_MASK[64];
 CFG_HDR cfghdr93;
 unsigned int CURRENT_TIME_NORAMAL_COLOR;
 CFG_HDR cfghdr94;
 unsigned int CURRENT_TIME_BORDER_COLOR;
 CFG_HDR cfghdr95;
 int CURRENT_TIME_FONT;
 CFG_HDR cfghdr96;
 int CURRENT_TIME_ALIGN;
 CFG_CBOX_ITEM cfgcbox97[3];
 CFG_HDR cfghdr98;

 CFG_HDR cfghdr99;
 CFG_HDR cfghdr100;
 int FULL_TIME_EN;
 CFG_HDR cfghdr101;
 RECT FULL_TIME_RC;
 CFG_HDR cfghdr102;
 wchar_t  FULL_TIME_MASK[64];
 CFG_HDR cfghdr103;
 unsigned int FULL_TIME_NORAMAL_COLOR;
 CFG_HDR cfghdr104;
 unsigned int FULL_TIME_BORDER_COLOR;
 CFG_HDR cfghdr105;
 int FULL_TIME_FONT;
 CFG_HDR cfghdr106;
 int FULL_TIME_ALIGN;
 CFG_CBOX_ITEM cfgcbox107[3];
 CFG_HDR cfghdr108;

 CFG_HDR cfghdr109;
 CFG_HDR cfghdr110;
 int LEFT_TIME_EN;
 CFG_HDR cfghdr111;
 RECT LEFT_TIME_RC;
 CFG_HDR cfghdr112;
 wchar_t  LEFT_TIME_MASK[64];
 CFG_HDR cfghdr113;
 unsigned int LEFT_TIME_NORAMAL_COLOR;
 CFG_HDR cfghdr114;
 unsigned int LEFT_TIME_BORDER_COLOR;
 CFG_HDR cfghdr115;
 int LEFT_TIME_FONT;
 CFG_HDR cfghdr116;
 int LEFT_TIME_ALIGN;
 CFG_CBOX_ITEM cfgcbox117[3];
 CFG_HDR cfghdr118;

 CFG_HDR cfghdr119;
 CFG_HDR cfghdr120;
 int TRACK_COUNT_EN;
 CFG_HDR cfghdr121;
 RECT TRACK_COUNT_RC;
 CFG_HDR cfghdr122;
 wchar_t  TRACK_COUNT_MASK[64];
 CFG_HDR cfghdr123;
 unsigned int TRACK_COUNT_NORAMAL_COLOR;
 CFG_HDR cfghdr124;
 unsigned int TRACK_COUNT_BORDER_COLOR;
 CFG_HDR cfghdr125;
 int TRACK_COUNT_FONT;
 CFG_HDR cfghdr126;
 int TRACK_COUNT_ALIGN;
 CFG_CBOX_ITEM cfgcbox127[3];
 CFG_HDR cfghdr128;

 CFG_HDR cfghdr129;
 CFG_HDR cfghdr130;
 int CURRENT_TRACK_EN;
 CFG_HDR cfghdr131;
 RECT CURRENT_TRACK_RC;
 CFG_HDR cfghdr132;
 wchar_t  CURRENT_TRACK_MASK[64];
 CFG_HDR cfghdr133;
 unsigned int CURRENT_TRACK_NORAMAL_COLOR;
 CFG_HDR cfghdr134;
 unsigned int CURRENT_TRACK_BORDER_COLOR;
 CFG_HDR cfghdr135;
 int CURRENT_TRACK_FONT;
 CFG_HDR cfghdr136;
 int CURRENT_TRACK_ALIGN;
 CFG_CBOX_ITEM cfgcbox137[3];
 CFG_HDR cfghdr138;
 CFG_HDR cfghdr139;

 CFG_HDR cfghdr140;
 CFG_HDR cfghdr141;
 int PROGRESSBAR_EN;
 CFG_HDR cfghdr142;
 RECT PROGRESSBAR_RC;
 CFG_HDR cfghdr143;
 int PROGRESSBAR_IMG_EN;
 CFG_HDR cfghdr144;
 int PROGRESSBAR_TYPE;
 CFG_CBOX_ITEM cfgcbox145[2];
 CFG_HDR cfghdr146;
 unsigned int PROGRESSBAR_BACKGROUND_COLOR;
 CFG_HDR cfghdr147;
 unsigned int PROGRESSBAR_BORDER_COLOR;
 CFG_HDR cfghdr148;
 unsigned int PROGRESSBAR_NORMAL_COLOR;
 CFG_HDR cfghdr149;

 CFG_HDR cfghdr150;
 CFG_HDR cfghdr151;
 int COVER_EN;
 CFG_HDR cfghdr152;
 RECT COVER_RC;
 CFG_HDR cfghdr153;
}CFG_test5;


typedef struct
{

 CFG_HDR cfghdr0;
 CFG_HDR cfghdr1;
 int KEY_START;
 int KEY_START_MODE;

 CFG_HDR cfghdr2;
 int ONTOP;

 CFG_HDR cfghdr222;
 int ONPLAYERCONTROL;

 CFG_HDR cfghdr3;
 RECT BACKGROUND_RC;

 CFG_HDR cfghdr4;
 unsigned int BACKGROUND_COLOR;

 CFG_HDR cfghdr5;
 int ON_TRACK_CHANGE;

 CFG_HDR cfghdr6;
 unsigned int AUTO_HIDE_TIME;

 CFG_HDR cfghdr7;
 unsigned int STEP;

 CFG_HDR cfghdr8;
 int ANIMATION_SHOW;
 CFG_CBOX_ITEM cfgcbox9[9];

 CFG_HDR cfghdr10;
 int ANIMATION_HIDE;
 CFG_CBOX_ITEM cfgcbox11[9];
 CFG_HDR cfghdr12;


 CFG_HDR cfghdr13;
 CFG_HDR cfghdr14;
 int WIN1251_SUPPORT;
 CFG_HDR cfghdr15;
 CFG_HDR cfghdr16;
 int ARTIST_EN;
 CFG_HDR cfghdr17;
 RECT ARTIST_RC;
 CFG_HDR cfghdr18;
 unsigned int ARTIST_NORAMAL_COLOR;
 CFG_HDR cfghdr19;
 unsigned int ARTIST_BORDER_COLOR;
 CFG_HDR cfghdr20;
 int ARTIST_FONT;
 CFG_HDR cfghdr21;
 int ARTIST_ALIGN;
 CFG_CBOX_ITEM cfgcbox22[3];
 CFG_HDR cfghdr23;

 CFG_HDR cfghdr24;
 CFG_HDR cfghdr25;
 int ALBUM_EN;
 CFG_HDR cfghdr26;
 RECT ALBUM_RC;
 CFG_HDR cfghdr27;
 unsigned int ALBUM_NORAMAL_COLOR;
 CFG_HDR cfghdr28;
 unsigned int ALBUM_BORDER_COLOR;
 CFG_HDR cfghdr29;
 int ALBUM_FONT;
 CFG_HDR cfghdr30;
 int ALBUM_ALIGN;
 CFG_CBOX_ITEM cfgcbox31[3];
 CFG_HDR cfghdr32;

 CFG_HDR cfghdr33;
 CFG_HDR cfghdr34;
 int TITLE_EN;
 CFG_HDR cfghdr35;
 RECT TITLE_RC;
 CFG_HDR cfghdr36;
 unsigned int TITLE_NORAMAL_COLOR;
 CFG_HDR cfghdr37;
 unsigned int TITLE_BORDER_COLOR;
 CFG_HDR cfghdr38;
 int TITLE_FONT;
 CFG_HDR cfghdr39;
 int TITLE_ALIGN;
 CFG_CBOX_ITEM cfgcbox40[3];
 CFG_HDR cfghdr41;

 CFG_HDR cfghdr42;
 CFG_HDR cfghdr43;
 int YEAR_EN;
 CFG_HDR cfghdr44;
 RECT YEAR_RC;
 CFG_HDR cfghdr45;
 unsigned int YEAR_NORAMAL_COLOR;
 CFG_HDR cfghdr46;
 unsigned int YEAR_BORDER_COLOR;
 CFG_HDR cfghdr47;
 int YEAR_FONT;
 CFG_HDR cfghdr48;
 int YEAR_ALIGN;
 CFG_CBOX_ITEM cfgcbox49[3];
 CFG_HDR cfghdr50;

 CFG_HDR cfghdr51;
 CFG_HDR cfghdr52;
 int GENRE_EN;
 CFG_HDR cfghdr53;
 RECT GENRE_RC;
 CFG_HDR cfghdr54;
 unsigned int GENRE_NORAMAL_COLOR;
 CFG_HDR cfghdr55;
 unsigned int GENRE_BORDER_COLOR;
 CFG_HDR cfghdr56;
 int GENRE_FONT;
 CFG_HDR cfghdr57;
 int GENRE_ALIGN;
 CFG_CBOX_ITEM cfgcbox58[3];
 CFG_HDR cfghdr59;

 CFG_HDR cfghdr60;
 CFG_HDR cfghdr61;
 int CHANNEL_EN;
 CFG_HDR cfghdr62;
 RECT CHANNEL_RC;
 CFG_HDR cfghdr63;
 unsigned int CHANNEL_NORAMAL_COLOR;
 CFG_HDR cfghdr64;
 unsigned int CHANNEL_BORDER_COLOR;
 CFG_HDR cfghdr65;
 int CHANNEL_FONT;
 CFG_HDR cfghdr66;
 int CHANNEL_ALIGN;
 CFG_CBOX_ITEM cfgcbox67[3];
 CFG_HDR cfghdr68;

 CFG_HDR cfghdr69;
 CFG_HDR cfghdr70;
 int BITRATE_EN;
 CFG_HDR cfghdr71;
 RECT BITRATE_RC;
 CFG_HDR cfghdr72;
 wchar_t  BITRATE_MASK[64];
 CFG_HDR cfghdr73;
 unsigned int BITRATE_NORAMAL_COLOR;
 CFG_HDR cfghdr74;
 unsigned int BITRATE_BORDER_COLOR;
 CFG_HDR cfghdr75;
 int BITRATE_FONT;
 CFG_HDR cfghdr76;
 int BITRATE_ALIGN;
 CFG_CBOX_ITEM cfgcbox77[3];
 CFG_HDR cfghdr78;

 CFG_HDR cfghdr79;
 CFG_HDR cfghdr80;
 int FREQUENCY_EN;
 CFG_HDR cfghdr81;
 RECT FREQUENCY_RC;
 CFG_HDR cfghdr82;
 wchar_t  FREQUENCY_MASK[64];
 CFG_HDR cfghdr83;
 unsigned int FREQUENCY_NORAMAL_COLOR;
 CFG_HDR cfghdr84;
 unsigned int FREQUENCY_BORDER_COLOR;
 CFG_HDR cfghdr85;
 int FREQUENCY_FONT;
 CFG_HDR cfghdr86;
 int FREQUENCY_ALIGN;
 CFG_CBOX_ITEM cfgcbox87[3];
 CFG_HDR cfghdr88;

 CFG_HDR cfghdr89;
 CFG_HDR cfghdr90;
 int CURRENT_TIME_EN;
 CFG_HDR cfghdr91;
 RECT CURRENT_TIME_RC;
 CFG_HDR cfghdr92;
 wchar_t  CURRENT_TIME_MASK[64];
 CFG_HDR cfghdr93;
 unsigned int CURRENT_TIME_NORAMAL_COLOR;
 CFG_HDR cfghdr94;
 unsigned int CURRENT_TIME_BORDER_COLOR;
 CFG_HDR cfghdr95;
 int CURRENT_TIME_FONT;
 CFG_HDR cfghdr96;
 int CURRENT_TIME_ALIGN;
 CFG_CBOX_ITEM cfgcbox97[3];
 CFG_HDR cfghdr98;

 CFG_HDR cfghdr99;
 CFG_HDR cfghdr100;
 int FULL_TIME_EN;
 CFG_HDR cfghdr101;
 RECT FULL_TIME_RC;
 CFG_HDR cfghdr102;
 wchar_t  FULL_TIME_MASK[64];
 CFG_HDR cfghdr103;
 unsigned int FULL_TIME_NORAMAL_COLOR;
 CFG_HDR cfghdr104;
 unsigned int FULL_TIME_BORDER_COLOR;
 CFG_HDR cfghdr105;
 int FULL_TIME_FONT;
 CFG_HDR cfghdr106;
 int FULL_TIME_ALIGN;
 CFG_CBOX_ITEM cfgcbox107[3];
 CFG_HDR cfghdr108;

 CFG_HDR cfghdr109;
 CFG_HDR cfghdr110;
 int LEFT_TIME_EN;
 CFG_HDR cfghdr111;
 RECT LEFT_TIME_RC;
 CFG_HDR cfghdr112;
 wchar_t  LEFT_TIME_MASK[64];
 CFG_HDR cfghdr113;
 unsigned int LEFT_TIME_NORAMAL_COLOR;
 CFG_HDR cfghdr114;
 unsigned int LEFT_TIME_BORDER_COLOR;
 CFG_HDR cfghdr115;
 int LEFT_TIME_FONT;
 CFG_HDR cfghdr116;
 int LEFT_TIME_ALIGN;
 CFG_CBOX_ITEM cfgcbox117[3];
 CFG_HDR cfghdr118;

 CFG_HDR cfghdr119;
 CFG_HDR cfghdr120;
 int TRACK_COUNT_EN;
 CFG_HDR cfghdr121;
 RECT TRACK_COUNT_RC;
 CFG_HDR cfghdr122;
 wchar_t  TRACK_COUNT_MASK[64];
 CFG_HDR cfghdr123;
 unsigned int TRACK_COUNT_NORAMAL_COLOR;
 CFG_HDR cfghdr124;
 unsigned int TRACK_COUNT_BORDER_COLOR;
 CFG_HDR cfghdr125;
 int TRACK_COUNT_FONT;
 CFG_HDR cfghdr126;
 int TRACK_COUNT_ALIGN;
 CFG_CBOX_ITEM cfgcbox127[3];
 CFG_HDR cfghdr128;

 CFG_HDR cfghdr129;
 CFG_HDR cfghdr130;
 int CURRENT_TRACK_EN;
 CFG_HDR cfghdr131;
 RECT CURRENT_TRACK_RC;
 CFG_HDR cfghdr132;
 wchar_t  CURRENT_TRACK_MASK[64];
 CFG_HDR cfghdr133;
 unsigned int CURRENT_TRACK_NORAMAL_COLOR;
 CFG_HDR cfghdr134;
 unsigned int CURRENT_TRACK_BORDER_COLOR;
 CFG_HDR cfghdr135;
 int CURRENT_TRACK_FONT;
 CFG_HDR cfghdr136;
 int CURRENT_TRACK_ALIGN;
 CFG_CBOX_ITEM cfgcbox137[3];
 CFG_HDR cfghdr138;
 CFG_HDR cfghdr139;

 CFG_HDR cfghdr140;
 CFG_HDR cfghdr141;
 int PROGRESSBAR_EN;
 CFG_HDR cfghdr142;
 RECT PROGRESSBAR_RC;
 CFG_HDR cfghdr143;
 int PROGRESSBAR_IMG_EN;
 CFG_HDR cfghdr144;
 int PROGRESSBAR_TYPE;
 CFG_CBOX_ITEM cfgcbox145[3];
 CFG_HDR cfghdr146;
 unsigned int PROGRESSBAR_BACKGROUND_COLOR;
 CFG_HDR cfghdr147;
 unsigned int PROGRESSBAR_BORDER_COLOR;
 CFG_HDR cfghdr148;
 unsigned int PROGRESSBAR_NORMAL_COLOR;
 CFG_HDR cfghdr149;

 CFG_HDR cfghdr150;
 CFG_HDR cfghdr151;
 int COVER_EN;
 CFG_HDR cfghdr152;
 RECT COVER_RC;
 CFG_HDR cfghdr153;
}CFG_Release;

int MigrateTest5ToRelease(CFG_test5 *oldcfg, CFG_Release *cfg);

#endif
