#include "CPoint.h"
int CPoint::GetY()
{
	return _y;
};

int CPoint::GetX()
{
	return _x;
};

int CPoint::GetPoint()
{
	return _point;
};

CPoint::CPoint(int x,int y,int point)
{
	_x=x;
	_ox=x;
	_y=y;
	_v=0;
	_a=-1;
	_point=point;
};

void CPoint::onMove()
{
	_v+=_a;
	_y+=_v;
	if((_x-_ox)>-5 && (_x-_ox)<5) _x+=1-rand()%3;
	if((_x-_ox)>5) _x--;
	if((_x-_ox)<-5) _x++;
};

CPoint::~CPoint()
{
};
