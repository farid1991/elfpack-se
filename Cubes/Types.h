#include "..\\include\lib_clara.h"
#ifndef TYPES_H_
#define TYPES_H_
#define CONFIGPATH L"Games/Cubes"
#define RECORDSFILENAME L"Records.dat"
#define ELFINFO L"Cubes\nv.1.9\n(c)mmcorp\naka\nDoCent"
struct Cursor
{
	int x,y;
};//Курсор

enum COLOR
{
	COLOR_RED=0xFFFF0000,
	COLOR_GREEN=0xFF00FF00,
	COLOR_BLUE=0xFF0000FF,
	COLOR_YELLOW=0xFFFFFF00,
	COLOR_PURPLE=0xFFFF00FF,
	COLOR_WHITE=0xFFFFFFFF
};//Цвета

enum DIRECTION
{
	DIRECTION_LEFT=0,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN
};//Направления

enum STATE
{
	STATE_OFF=0,
	STATE_ON
};//Статус кубика

enum SELECT
{
	UNSELECTED=0,
	SELECTED=1
};//Статус выделения кубика

struct CUBE_PARAM
{
	COLOR color;
	STATE state;
	SELECT select;
};//Параметры кубика

struct RECORD
{
	COLOR colors[15][20];
	int point;
};

struct BITRECORD
{
	char byte[113];
	int point;
};

typedef void (*ONGAMEOVERPROC)(void* lparam);
#endif
