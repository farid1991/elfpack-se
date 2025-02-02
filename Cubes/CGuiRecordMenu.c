#include "CGuiRecordMenu.h"
int CGuiRecordMenu::OnMessage(CGUIMessage*msg)
{
	int item;
	switch(msg->GetMsg())
	{
	case LISTMSG_GetItem:
		item=msg->GetCreatedItem();
		if(_recs[item].point) msg->SetItemText(TextID_CreateIntegerID(_recs[item].point));
		else msg->SetItemText(TextID_Create(CGUIRECORDMENU_EMPTY_TXT,ENC_UCS2,TEXTID_ANY_LEN));
	};
	return 0;
};

CGuiRecordMenu::CGuiRecordMenu(CBook*bk,RECORD*recs)
:CGuiListMenu(bk)
{
	_recs=recs;
	SetItemCount(10);
	SetItemStyle(0);
	SetFocused(0);
	SetTitleText(TextID_Create(CGUIRECORDMENU_TITLE_TXT,ENC_UCS2,TEXTID_ANY_LEN));
};
