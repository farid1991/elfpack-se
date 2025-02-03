#include "softkeys.h"

wchar_t Softkeys_GetFreeAction(GUI *gui)
{
	int i;
	wchar_t ret=0xFFFF;
	LIST *lst = DispObject_SoftKeys_GetList( GUIObject_GetDispObject(gui),GUIObject_GetBook(gui), 0);
	if(lst)
	{
		wchar_t * actionlist = new wchar_t[lst->FirstFree];
		for(int k=0;k<lst->FirstFree;k++)
			actionlist[k]=((SOFTKEY_DESC*)List_Get(lst,k))->action;
		QSort(actionlist,lst->FirstFree);
		for(i=0;i<lst->FirstFree;i++)
		{
			if(actionlist[i]>=ACTION_MORE) break;
		}
		i--;
		for(int k=0;k<=i-1;k++)
		{
			if(actionlist[k]+1 != actionlist[k+1])
			{
				ret=actionlist[k]+1;
				break;
			}
		}
		if(!ret) ret=actionlist[i]+1;
		delete(actionlist);
	}
	return(ret);
}

wchar_t Softkeys_FindAction(GUI *gui, int strid)
{
	LIST *lst = DispObject_SoftKeys_GetList( GUIObject_GetDispObject(gui),GUIObject_GetBook(gui), 0);
	for(int k=0;k<lst->FirstFree;k++)
	{
		SOFTKEY_DESC * sd = (SOFTKEY_DESC*) List_Get(lst,k);
		if(sd->ButtonText == strid) return sd->action;
		else
		{
			wchar_t s1[32],s2[32];
			TextID_GetWString(strid,s1,MAXELEMS(s1));
			TextID_GetWString(sd->ButtonText,s2,MAXELEMS(s2));
			if(!wstrcmp(s1,s2)) return sd->action;
		}
	}
	return(0xFFFF);
}
