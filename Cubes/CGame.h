#include "..\\include\lib_clara.h"
#include "..\\include\dir.h"
#include "CCube.h"
#include "CPoint.h"
#ifndef CGAME_H_
#define CGAME_H_
class CGame
{
	GC*globalgc;//Изображение игры
	CCube***cubes;//Массив всех кубиков
	CCube***map;//Массив оставшихся кубиков
	LIST*selectlist;//Список выделенных кубиков
	Cursor*cursor;//Курсор
	LIST*actionlist;//Список действий
	LIST*pointlist;//Список очков
	int _point;//Очки
	int _place;//Место
	int count[5];//Количество кубиков разных цветов
	int _xsize,_ysize,_csize;//Размер поля в кубиках
	GC*imagescub[2][5];//Изображение кубиков
	GC*imageempty;//Изображение курсора
	int img[10];//ID изображений цифр
	int x_max,//Макс. x экрана
	y_max,//Макс. y экрана
	x_point,//x вывода очков
	y_point,//y вывода очков
	x_spoint,//x вывода очков выделенных кубиков
	y_spoint;//y вывода очков выделенных кубиков
	wchar_t*_cpath;//Имя папки с ресурсами
	bool infostate;//Статус показа количества кубиков
	ONGAMEOVERPROC _func;//callback конца игры
	void* _lparam;//Параметр callback'a
	CCube*CreateCube();//Создание кубика
	void ReDrawGlobal();
	GC*CreateCubImage(COLOR color,SELECT select);//Создание изображения кубика средствами графики
	GC*CreateCubImage(const wchar_t*name);//Создание изображения кубика из ресурсов
	int GetLenPixels(int number);
	void DrawNumeral(GC*gc,int &x,int &y,int numeral);//Отрисовка цифры
	void DrawNumber(GC*gc,int x,int y,int number);//Отрисовка числа
	void DrawCursor(GC*gc);//Рисуем курсор
	void DrawCubes(GC*gc);//Рисуем кубики
	void DrawInfo(GC*gc);//Рисуем количество кубиков каждого цвета
	void DrawPoints(GC*gc);//Рисуем очки
	void DrawGlobal(GC*gc);//Рисуем кубики в область
	void DrawRecord(GC*gc);//Рисуем занятое место
	void DrawWindow(GC*gc,int x,int y,int xsize,int ysize);//Рисуем окошко
	void Cube2Map();//Создание поля из полного набора
	void GetAllSelect(LIST*list,int x,int y,COLOR color);//Занести в список все кубики, принадлежащих одной группе
	void ReSelect();//Выделение группы
	bool GetNearby(int i,int j);//Проверка кубика на наличи кубиков того же цыет по соседству
	void CheckGameEnd();//Проверка конца игры
public:
	CGame(int xsize,int ysize,int csize);//Конструктор, инициализация игры
	void InitNewGame();//Инициализация новый игры
	void onRefresh();//
	void SetPlace(int place);
	void onSetGameOver(void* lparam,ONGAMEOVERPROC func);//Установка callback'а при конце игры
	void ChangedInfoState();//Вкл/откл окошко с количеством кубиков
	void MoveCursor(DIRECTION direction);//Перемещение курсора
	void BackAction();//Отмена хода
	void EnterAction();//Убийство кубиков
	void GetRecord(RECORD*);//Получить расклад и очки игры
	void SetRecord(RECORD*);//Установить расклад игры
	void Out(GC*gc);//Вывод в ГК
	wchar_t* GetConfigPath(const wchar_t*Dir);//move
	~CGame();//Деструктор, чистка мусора
};
#endif
