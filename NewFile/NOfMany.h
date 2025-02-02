#ifndef _NOFMANY_H_
#define _NOFMANY_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"


class CMyGuiNOfMany:public CGuiNOfMany
{
public:
	TEXTID* strids;
	u16* checked_table;
	LIST *itemlst;
	unsigned long lparam;
protected:
	static void OnBack( CBookBase* book, CGuiBase* gui );
	static void OnLongBack( CBookBase* book, CGuiBase* gui );
	static void OnSelect( CBookBase* book, CGuiBase* gui );
	void Destroy()
	{
		delete strids;
		delete checked_table;
		Free();
	};
public:
	void(*CallBack)(LIST *lst,u16 *checked,int count,unsigned long ClientData);
	CMyGuiNOfMany( CBook* book, int display,LIST *lst,void(*OnAccept)(LIST *lst,u16 *checked,int count,unsigned long ClientData),unsigned long ClientData);
	~CMyGuiNOfMany();
};

void CreateNameInput( BOOK *book, GUI *);
void CreateSizeInput( BOOK *book, GUI *);


extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);

#endif
