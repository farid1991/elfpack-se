#include "rand.h"
#ifndef CPOINT_H_
#define CPOINT_H_
class CPoint
{
	int _point;
	int _ox;
	int _x;
	int _y;
	int _v;
	int _a;
public:
	CPoint(int x,int y,int point);
	void onMove();
	int GetX();
	int GetY();
	int GetPoint();
	~CPoint();
};
#endif
