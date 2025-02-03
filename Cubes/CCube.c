#include "CCube.h"

CCube::CCube(CUBE_PARAM *param)
{
	_param=param;
};

CCube::CCube(COLOR color,STATE state,SELECT select)
{
	_param=new CUBE_PARAM;
	_param->color=color;
	_param->state=state;
	_param->select=select;
};

COLOR CCube::GetColor()
{
	return _param->color;
};

STATE CCube::GetState()
{
	return _param->state;
};

void CCube::SetState(STATE state)
{
	_param->state=state;
};

void CCube::SetColor(COLOR color)
{
	_param->color=color;
};

void CCube::Select()
{
	_param->select=SELECTED;
};

void CCube::UnSelect()
{
	_param->select=UNSELECTED;
};

SELECT CCube::GetSelect()
{
	return _param->select;
};

CCube::~CCube()
{
	delete _param;
};
