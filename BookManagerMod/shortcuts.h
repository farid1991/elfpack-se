#ifndef _SHORTCUTS_H_
#define _SHORTCUTS_H_

#define MAX_BOOKS_ELFS_SH_NUM 10
#define MAX_OTHER_SH_NUM      9

#define IDN_DIGIT_0_ICON _T( "FM_RADIO_MHZ_DIGIT_0_ICN" )
#define IDN_DIGIT_1_ICON _T( "FM_RADIO_MHZ_DIGIT_1_ICN" )
#define IDN_DIGIT_2_ICON _T( "FM_RADIO_MHZ_DIGIT_2_ICN" )
#define IDN_DIGIT_3_ICON _T( "FM_RADIO_MHZ_DIGIT_3_ICN" )
#define IDN_DIGIT_4_ICON _T( "FM_RADIO_MHZ_DIGIT_4_ICN" )
#define IDN_DIGIT_5_ICON _T( "FM_RADIO_MHZ_DIGIT_5_ICN" )
#define IDN_DIGIT_6_ICON _T( "FM_RADIO_MHZ_DIGIT_6_ICN" )
#define IDN_DIGIT_7_ICON _T( "FM_RADIO_MHZ_DIGIT_7_ICN" )
#define IDN_DIGIT_8_ICON _T( "FM_RADIO_MHZ_DIGIT_8_ICN" )
#define IDN_DIGIT_9_ICON _T( "FM_RADIO_MHZ_DIGIT_9_ICN" )

enum {
 ShortPress = 0,
 LongPress,
 MaxModeNum
};

typedef struct
{
	wchar_t* name;
	wchar_t* fullpath;
	int appID;
	IMAGEID imageID;
}java_list_elem;

extern const wchar_t *books_tab_actions[MaxModeNum][MAX_BOOKS_ELFS_SH_NUM];
extern const wchar_t *elfs_tab_actions[MaxModeNum][MAX_BOOKS_ELFS_SH_NUM];
extern const wchar_t *shortcuts_tab_actions[MAX_OTHER_SH_NUM];

int getShortcutsNum(int tab);
void DelShortcut(BOOK * book, GUI* );
void AddShortcut(BOOK* book, GUI* );
void Shortcuts( BOOK* book, GUI* );
int elem_filter(void* elem);
void elem_free(void* elem);

int CreateButtonList( void* data, BOOK* book );

void * SHORTCUT_DESC_Init(char * param);
void * SHORTCUT_DESC_A2_Init(char * param);

#endif
