#include "..\\classlib\cl.h"
#ifndef CGUIMAINMENU_H_
#define CGUIMAINMENU_H_
#define CGUIMAINMENU_NEWGAME_TXT L"New Game"
#define CGUIMAINMENU_NEWGAME_ICN L"ENTERTAINMENT_MENU_GAMES_ICN"
#define CGUIMAINMENU_RECORDS_TXT L"Records"
#define CGUIMAINMENU_RECORDS_ICN L"CALE_TASK_MISC_ICN"
#define CGUIMAINMENU_EXIT_TXT L"Exit"
#define CGUIMAINMENU_SELECT_TXT L"Select"
#define CGUIMAINMENU_EXIT_ICN L"WV_YAHOO_BUDDYLIST_CONTACT_DISCREET_ICN"
#define CGUIMAINMENU_TITLE_TXT L"Main Menu"
class CGuiMainMenu:public CGuiListMenu
{
	virtual int OnMessage(CGUIMessage*msg);
public:
	CGuiMainMenu(CBook*bk);
};

#endif
