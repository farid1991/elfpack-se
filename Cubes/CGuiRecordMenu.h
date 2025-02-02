#include "..\\classlib\cl.h"
#include "Types.h"
#ifndef CGUIRECORDMENU_H_
#define CGUIRECORDMENU_H_
#define CGUIRECORDMENU_TITLE_TXT L"Record Menu"
#define CGUIRECORDMENU_EMPTY_TXT L"Empty"
#define CGUIRECORDMENU_BACK_TXT L"Back"
#define CGUIRECORDMENU_BEAT_TXT L"Beat!"
class CGuiRecordMenu:public CGuiListMenu
{
	RECORD*_recs;
	virtual int OnMessage(CGUIMessage*msg);
public:
	CGuiRecordMenu(CBook*bk,RECORD*recs);
};

#endif
