#include "Types.h"
#ifndef CCUBE_H_
#define CCUBE_H_
class CCube
{
	CUBE_PARAM*_param;
public:
	CCube(CUBE_PARAM *param);
	CCube(COLOR color,STATE state,SELECT select);
	COLOR GetColor();
	STATE GetState();
	void SetState(STATE state);
	void SetColor(COLOR color);
	void Select();
	void UnSelect();
	SELECT GetSelect();
	~CCube();
};

#endif
