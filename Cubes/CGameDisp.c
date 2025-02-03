#include "CGameDisp.h"
const int TOTAL_X=20;
const int TOTAL_Y=15;

char* CGameDisp::getName()
{
	return GAMEDISPNAME;
};

void CGameDisp::onDraw(int a,int b,int c)
{
	isondraw=true;
	game->Out(get_DisplayGC());
	isondraw=false;
};

void CGameDisp::onKey(int key,int,int repeat,int type)
{
	if(key==KEY_DIGITAL_0+1 && type==KBD_SHORT_PRESS)
	{
		game->ChangedInfoState();
		if(!isondraw) InvalidateRect(NULL);
	};
	if(key==KEY_DIGITAL_0+5 && type==KBD_SHORT_PRESS)
	{
		game->EnterAction();
		if(!isondraw) InvalidateRect(NULL);
	};
	if(key==KEY_DEL && type==KBD_SHORT_PRESS)
	{
		game->BackAction();
		if(!isondraw) InvalidateRect(NULL);
	};
	if(key==KEY_DIGITAL_0 && type==KBD_SHORT_PRESS)
	{
		game->InitNewGame();
		if(!isondraw) InvalidateRect(NULL);
	};
	if((key==KEY_LEFT || key==KEY_DIGITAL_0+2) && (type==KBD_SHORT_PRESS || type==KBD_REPEAT && repeat>4))
	{
		game->MoveCursor(DIRECTION_LEFT);
		if(!isondraw) InvalidateRect(NULL);
	};
	if((key==KEY_UP || key==KEY_DIGITAL_0+6) && (type==KBD_SHORT_PRESS || type==KBD_REPEAT && repeat>4))
	{
		game->MoveCursor(DIRECTION_UP);
		if(!isondraw) InvalidateRect(NULL);
	};
	if((key==KEY_RIGHT || key==KEY_DIGITAL_0+8) && (type==KBD_SHORT_PRESS || type==KBD_REPEAT && repeat>4))
	{
		game->MoveCursor(DIRECTION_RIGHT);
		if(!isondraw) InvalidateRect(NULL);
	};
	if((key==KEY_DOWN || key==KEY_DIGITAL_0+4) && (type==KBD_SHORT_PRESS || type==KBD_REPEAT && repeat>4))
	{
		game->MoveCursor(DIRECTION_DOWN);
		if(!isondraw) InvalidateRect(NULL);
	};
};

CGame*CGameDisp::GetGame()
{
	return game;
};

void CGameDisp::onRefresh()
{
	SetRefreshTimer(100);
	game->onRefresh();
	if(!isondraw) InvalidateRect(NULL);
};

void CGameDisp::onDestroy()
{
	delete game;
};
int CGameDisp::onCreate()
{
	isondraw=false;
	int ymax=Display_GetHeight(0);
	int cub_size;
	if(ymax==240 || ymax==320) cub_size=16;
	else
		if(ymax==176 || ymax==220) cub_size=11;
		else
			if(ymax==128 || ymax==160) cub_size=8;
			else cub_size=16;
	game=new CGame(TOTAL_X,TOTAL_Y,cub_size);
	InvalidateRect(NULL);
	SetRefreshTimer(100);
	return 1;
};
