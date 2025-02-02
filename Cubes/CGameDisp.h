#include "..\\classlib\cl.h"
#include "CGame.h"
#ifndef CGAMEDISP_H_
#define CGAMEDISP_H_
#define GAMEDISPNAME "Cubes_DispObj"
class CGameDisp:public CDispObjT<CGameDisp>
{
	bool isondraw;
	CGame*game;
public:
	CGame*GetGame();
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);
	void onRefresh();
	void onDestroy();
	int onCreate();
};
#endif
