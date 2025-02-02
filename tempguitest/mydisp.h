#ifndef _MYDISP_H_
#define _MYDISP_H_

#include "..\classlib\cl.h"


class CFirstDisp:public CDispObjT<CFirstDisp>
{
       CPaintGC* paint;
public:
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);
	void onRefresh();
	void onLayout( void* layoutstruct );
	void onDestroy();
	int onCreate();
};


class CSecondDisp:public CDispObjT<CSecondDisp>
{
       CPaintGC* paint;
public:
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);
	void onRefresh();
	void onLayout( void* layoutstruct );
	void onDestroy();
	int onCreate();
};

class CThirdDisp:public CDispObjT<CThirdDisp>
{
       CPaintGC* paint;
public:
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);
	void onRefresh();
	void onLayout( void* layoutstruct );
	void onDestroy();
	int onCreate();
};

#endif
