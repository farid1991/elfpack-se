#include "CGuiMainMenu.h"

int CGuiMainMenu::OnMessage(CGUIMessage*msg)
{
	int item;
	int IconID;
	TEXTID TextID;
	switch(msg->GetMsg())
	{
	case LISTMSG_GetItem:
		item=msg->GetCreatedItem();
		switch(item)
		{
		case 0:
			TextID=TextID_Create(CGUIMAINMENU_NEWGAME_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			iconidname2id(CGUIMAINMENU_NEWGAME_ICN,-1,&IconID);
			break;
		case 1:
			TextID=TextID_Create(CGUIMAINMENU_RECORDS_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			iconidname2id(CGUIMAINMENU_RECORDS_ICN,-1,&IconID);
			break;
		case 2:
			TextID=TextID_Create(CGUIMAINMENU_EXIT_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			iconidname2id(CGUIMAINMENU_EXIT_ICN,-1,&IconID);
			break;
		}
		msg->SetItemText(TextID);
		msg->SetItemIcon(0,IconID);
	};
	return 0;
};

CGuiMainMenu::CGuiMainMenu(CBook*bk)
:CGuiListMenu(bk)
{
	SetItemCount(3);
	SetItemStyle(0);
	SetFocused(0);
	SetTitleText(TextID_Create(CGUIMAINMENU_TITLE_TXT,ENC_UCS2,TEXTID_ANY_LEN));
};
