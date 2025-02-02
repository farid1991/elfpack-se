#include "..\\include\lib_clara.h"
#include "..\\include\dir.h"
#include "CCube.h"
#include "CPoint.h"
#ifndef CGAME_H_
#define CGAME_H_
class CGame
{
	GC*globalgc;//����������� ����
	CCube***cubes;//������ ���� �������
	CCube***map;//������ ���������� �������
	LIST*selectlist;//������ ���������� �������
	Cursor*cursor;//������
	LIST*actionlist;//������ ��������
	LIST*pointlist;//������ �����
	int _point;//����
	int _place;//�����
	int count[5];//���������� ������� ������ ������
	int _xsize,_ysize,_csize;//������ ���� � �������
	GC*imagescub[2][5];//����������� �������
	GC*imageempty;//����������� �������
	int img[10];//ID ����������� ����
	int x_max,//����. x ������
	y_max,//����. y ������
	x_point,//x ������ �����
	y_point,//y ������ �����
	x_spoint,//x ������ ����� ���������� �������
	y_spoint;//y ������ ����� ���������� �������
	wchar_t*_cpath;//��� ����� � ���������
	bool infostate;//������ ������ ���������� �������
	ONGAMEOVERPROC _func;//callback ����� ����
	void* _lparam;//�������� callback'a
	CCube*CreateCube();//�������� ������
	void ReDrawGlobal();
	GC*CreateCubImage(COLOR color,SELECT select);//�������� ����������� ������ ���������� �������
	GC*CreateCubImage(const wchar_t*name);//�������� ����������� ������ �� ��������
	int GetLenPixels(int number);
	void DrawNumeral(GC*gc,int &x,int &y,int numeral);//��������� �����
	void DrawNumber(GC*gc,int x,int y,int number);//��������� �����
	void DrawCursor(GC*gc);//������ ������
	void DrawCubes(GC*gc);//������ ������
	void DrawInfo(GC*gc);//������ ���������� ������� ������� �����
	void DrawPoints(GC*gc);//������ ����
	void DrawGlobal(GC*gc);//������ ������ � �������
	void DrawRecord(GC*gc);//������ ������� �����
	void DrawWindow(GC*gc,int x,int y,int xsize,int ysize);//������ ������
	void Cube2Map();//�������� ���� �� ������� ������
	void GetAllSelect(LIST*list,int x,int y,COLOR color);//������� � ������ ��� ������, ������������� ����� ������
	void ReSelect();//��������� ������
	bool GetNearby(int i,int j);//�������� ������ �� ������ ������� ���� �� ���� �� ���������
	void CheckGameEnd();//�������� ����� ����
public:
	CGame(int xsize,int ysize,int csize);//�����������, ������������� ����
	void InitNewGame();//������������� ����� ����
	void onRefresh();//
	void SetPlace(int place);
	void onSetGameOver(void* lparam,ONGAMEOVERPROC func);//��������� callback'� ��� ����� ����
	void ChangedInfoState();//���/���� ������ � ����������� �������
	void MoveCursor(DIRECTION direction);//����������� �������
	void BackAction();//������ ����
	void EnterAction();//�������� �������
	void GetRecord(RECORD*);//�������� ������� � ���� ����
	void SetRecord(RECORD*);//���������� ������� ����
	void Out(GC*gc);//����� � ��
	wchar_t* GetConfigPath(const wchar_t*Dir);//move
	~CGame();//����������, ������ ������
};
#endif
