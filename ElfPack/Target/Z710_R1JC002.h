#ifndef _Z710_R1JC002_
#define _Z710_R1JC002_

#define EXT_TABLE 0x4590BA04
#define HELPER_SIG 0x6FF0

#define PATH_USER_INT         L"/tpa/user"
#define PATH_USER_EXT         L"/card"
#define PATH_AUDIO_INT        L"/tpa/user/music"
#define PATH_AUDIO_EXT        L"/card/music"
#define PATH_IMAGE_INT        L"/tpa/user/picture"
#define PATH_IMAGE_EXT        L"/card/picture"
#define PATH_VIDEO_INT        L"/tpa/user/video"
#define PATH_VIDEO_EXT        L"/card/video"
#define PATH_THEME_INT        L"/tpa/user/theme"
#define PATH_THEME_EXT        L"/card/theme"
#define PATH_OTHER_INT        L"/tpa/user/other"
#define PATH_OTHER_EXT        L"/card/other"

#define PATH_ELF_ROOT_INT     L"/tpa/user/other/ZBin"
#define PATH_ELF_ROOT_EXT     L"/card/other/ZBin"
#define PATH_ELF_INT          L"/tpa/user/other/ZBin"
#define PATH_ELF_EXT          L"/card/other/ZBin"
#define PATH_ELF_DAEMONS_INT  L"/tpa/user/other/ZBin/Daemons"
#define PATH_ELF_DAEMONS_EXT  L"/card/other/ZBin/Daemons"
#define PATH_ELF_CONFIG_INT   L"/tpa/user/other/ZBin/Config"
#define PATH_ELF_CONFIG_EXT   L"/card/other/ZBin/Config"
#define PATH_INI              L"/tpa/user/other/ini"
#define PATH_DLL              L"/tpa/user/other/ZBin/DLL"
#define PATH_DEFAULT          L"/tpa/user/other"

#define DB_CMD_SETSMALLICON 0x3A
#define DB_CMD_SETTHUMBNAILICON 0x3B

#define PAGE_ENTER_EVENT 7
#define PAGE_EXIT_EVENT 8
#define ACCEPT_EVENT 2
#define PREVIOUS_EVENT 3
#define CANCEL_EVENT 4

#define KEY_LAST 0x154

#define DB2020

#ifndef NOICONS
#define EP_EXTICON16x20
#define EP_EXTICON48x36
#endif

#endif
