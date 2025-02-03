#include "CGame.h"

const wchar_t*ICONNAMES[2][5]=
{
	{L"R.png", L"G.png", L"B.png", L"Y.png", L"P.png"},
	{L"RS.png",L"GS.png",L"BS.png",L"YS.png",L"PS.png"}
};

void CGame::onSetGameOver(void* lparam,ONGAMEOVERPROC func)
{
	_func=func;
	_lparam=lparam;
};

void CGame::ChangedInfoState()
{
	infostate=!infostate;
};

CCube*CGame::CreateCube()
{
	int random=rand()%5;
	CUBE_PARAM* param=new CUBE_PARAM;
	switch(random)
	{
	case 0:
		param->color=COLOR_RED;
		break;
	case 1:
		param->color=COLOR_GREEN;
		break;
	case 2:
		param->color=COLOR_BLUE;
		break;
	case 3:
		param->color=COLOR_YELLOW;
		break;
	case 4:
		param->color=COLOR_PURPLE;
		break;
	};
	param->state=STATE_ON;
	param->select=UNSELECTED;
	CCube*cube=new CCube(param);
	return cube;
};

void CGame::Cube2Map()
{
	int x=0;
	int y=0;
	for(int i=0;i<_ysize;i++)
		for(int j=0;j<_xsize;j++)
			map[i][j]=0;
	for(int j=0;j<_xsize;j++)
	{
		y=_ysize;
		for(int i=_ysize-1;i>=0;i--)
			if(cubes[i][j]->GetState()==STATE_ON)
			{
				y--;
				map[y][x]=cubes[i][j];
			};
		if(y!=_ysize) x++;
	};
};

void CGame::GetAllSelect(LIST*list,int x,int y,COLOR color)
{
	CCube*cube=map[y][x];
	if(cube)
	{
		if(cube->GetColor()==color)
		{
			if(List_IndexOf(list,cube)==LIST_ERROR)
			{
				cube->Select();
				List_InsertLast(list,cube);
				if(y>0) GetAllSelect(list,x,y-1,color);
				if(y<_ysize-1) GetAllSelect(list,x,y+1,color);
				if(x>0) GetAllSelect(list,x-1,y,color);
				if(x<_xsize-1) GetAllSelect(list,x+1,y,color);
			};
		};
	};
};

void CGame::ReSelect()
{
	CCube*cube1=map[cursor->y][cursor->x];
	if(List_IndexOf(selectlist,cube1)==LIST_ERROR)
	{
		CCube*cube;
		int i=selectlist->FirstFree;
		while(i)
		{
			i--;
			cube=(CCube*)List_RemoveAt(selectlist,i);
			cube->UnSelect();
		};
		GetAllSelect(selectlist,cursor->x,cursor->y,cube1->GetColor());
		ReDrawGlobal();
	};
};

GC*CGame::CreateCubImage(const wchar_t*name)
{
	GC*gc=GC_CreateMemoryGC(_csize,_csize,16,0,0,0);
	IMAGEID img;
	GC_DrawFRect(gc,0xFF000000,0,0,_csize,_csize);
	ImageID_Get(_cpath,name,&img);
	GC_PutChar(gc,0,0,_csize,_csize,img);
	ImageID_Free(img);
	return gc;
};

void CGame::CheckGameEnd()
{
	bool isgameover=true;
	for(int i=0;i<_ysize;i++)
	{
		for(int j=0;j<_xsize;j++)
			if(GetNearby(i,j))
			{
				isgameover=false;
				break;
			}
		if(!isgameover) break;
	};
	if(isgameover)
	{
		_func(_lparam);
		PAudioControl pAC = AudioControl_Init();
		if( !pAC ) pAC = *GetAudioControlPtr();
		AudioControl_Vibrate(pAC, 200, 200, 400);
	};
};

void CGame::GetRecord(RECORD*rec)
{
	rec->point=_point;
	for(int i=0;i<_ysize;i++)
		for(int j=0;j<_xsize;j++)
			rec->colors[i][j]=cubes[i][j]->GetColor();
};

GC*CGame::CreateCubImage(COLOR color,SELECT select)
{
	GC*gc=GC_CreateMemoryGC(_csize,_csize,16,0,0,0);
	GC_DrawFRect(gc,color,0,0,_csize,_csize);
	GVI_GC gvi_gc;
	CANVAS_Get_GviGC(gc->pcanvas ,&gvi_gc);
	GVI_PEN pen;
	if(select==UNSELECTED) pen=GVI_CreatePen(1,0xFFB0B0B0);
	else pen=GVI_CreatePen(1,0xFF404040);
	GVI_DrawLine(gvi_gc,0,0,_csize-1,0,pen);
	GVI_DrawLine(gvi_gc,0,0,0,_csize-1,pen);
	GVI_Delete_GVI_Object(&pen);
	if(select==UNSELECTED) pen=GVI_CreatePen(1,0xFF404040);
	else pen=GVI_CreatePen(1,0xFFB0B0B0);
	GVI_DrawLine(gvi_gc,1,_csize-1,_csize-1,_csize-1,pen);
	GVI_DrawLine(gvi_gc,_csize-1,1,_csize-1,_csize-1,pen);
	GVI_Delete_GVI_Object(&pen);
	if(select==UNSELECTED) pen=GVI_CreatePen(1,0x80FFFFFF);
	else pen=GVI_CreatePen(1,0x80000000);
	GVI_DrawLine(gvi_gc,1,1,_csize-2,1,pen);
	GVI_DrawLine(gvi_gc,1,1,1,_csize-2,pen);
	GVI_Delete_GVI_Object(&pen);
	if(select==UNSELECTED) pen=GVI_CreatePen(1,0x80000000);
	else pen=GVI_CreatePen(1,0x80FFFFFF);
	GVI_DrawLine(gvi_gc,2,_csize-2,_csize-2,_csize-2,pen);
	GVI_DrawLine(gvi_gc,_csize-2,2,_csize-2,_csize-2,pen);
	GVI_Delete_GVI_Object(&pen);
	return gc;
};

void CGame::DrawCubes(GC*gc)
{
	GVI_GC gvi_gc_cub;
	GVI_GC gvi_gc;
	CANVAS_Get_GviGC(gc->pcanvas ,&gvi_gc);
	for(int i=0;i<5;i++) count[i]=0;
	for(int i=0;i<_ysize;i++)
		for(int j=0;j<_xsize;j++)
		{
			CCube*cube=map[i][j];
			if(!cube) CANVAS_Get_GviGC(imageempty->pcanvas ,&gvi_gc_cub);
			else
			{
				switch(cube->GetColor())
				{
				case COLOR_RED:
					if(cube->GetSelect()==UNSELECTED) CANVAS_Get_GviGC(imagescub[0][0]->pcanvas ,&gvi_gc_cub);
					else CANVAS_Get_GviGC(imagescub[1][0]->pcanvas ,&gvi_gc_cub);
					count[0]++;
					break;
				case COLOR_GREEN:
					if(cube->GetSelect()==UNSELECTED) CANVAS_Get_GviGC(imagescub[0][1]->pcanvas ,&gvi_gc_cub);
					else CANVAS_Get_GviGC(imagescub[1][1]->pcanvas ,&gvi_gc_cub);
					count[1]++;
					break;
				case COLOR_BLUE:
					if(cube->GetSelect()==UNSELECTED) CANVAS_Get_GviGC(imagescub[0][2]->pcanvas ,&gvi_gc_cub);
					else CANVAS_Get_GviGC(imagescub[1][2]->pcanvas ,&gvi_gc_cub);
					count[2]++;
					break;
				case COLOR_YELLOW:
					if(cube->GetSelect()==UNSELECTED) CANVAS_Get_GviGC(imagescub[0][3]->pcanvas ,&gvi_gc_cub);
					else CANVAS_Get_GviGC(imagescub[1][3]->pcanvas ,&gvi_gc_cub);
					count[3]++;
					break;
				case COLOR_PURPLE:
					if(cube->GetSelect()==UNSELECTED) CANVAS_Get_GviGC(imagescub[0][4]->pcanvas ,&gvi_gc_cub);
					else CANVAS_Get_GviGC(imagescub[1][4]->pcanvas ,&gvi_gc_cub);
					count[4]++;
					break;
				};
			};
			GVI_BitBlt(gvi_gc,j*_csize,i*_csize,_csize,_csize,gvi_gc_cub,0,0,204,0,0,0);
		};
};

void CGame::DrawCursor(GC*gc)
{
	GVI_GC gvi_gc;
	CANVAS_Get_GviGC(gc->pcanvas ,&gvi_gc);
	GVI_FillSolidEllipse(gvi_gc,
						 cursor->x*_csize+_csize/4,
						 cursor->y*_csize+_csize/4,
						 _csize/2,
						 _csize/2,
						 0xFF000000
							 );
};

void CGame::DrawInfo(GC*gc)
{
	DrawWindow(gc,8*_csize,4*_csize,4*_csize,7*_csize);
	GVI_GC gvi_gc_cub;
	GVI_GC gvi_gc;
	CANVAS_Get_GviGC(gc->pcanvas ,&gvi_gc);
	int y=5*_csize;
	int x=9*_csize;
	for(int i=0;i<5;i++)
	{
		CANVAS_Get_GviGC(imagescub[0][i]->pcanvas ,&gvi_gc_cub);
		GVI_BitBlt(gvi_gc,x,y,_csize,_csize,gvi_gc_cub,0,0,204,0,0,0);
		DrawNumber(gc,x+_csize,y,count[i]);
		y+=_csize;
	};
};

void CGame::ReDrawGlobal()
{
	GC_DrawFRect(globalgc,0xFFFFFFFF,0,0,x_max,y_max);
	DrawCubes(globalgc);
};

void CGame::DrawGlobal(GC*gc)
{
	GVI_GC gvi_globalgc;
	GVI_GC gvi_gc;
	CANVAS_Get_GviGC(gc->pcanvas ,&gvi_gc);
	CANVAS_Get_GviGC(globalgc->pcanvas ,&gvi_globalgc);
	GVI_BitBlt(gvi_gc,0,0,x_max,y_max,gvi_globalgc,0,0,204,0,0,0);
};

void CGame::DrawWindow(GC*gc,int x,int y,int xsize,int ysize)
{
	GC_DrawFRect(gc,0xA0FFFFFF,x,y,xsize,ysize);
	GC_SetPenColor(gc,0xFF000000);
	GC_DrawLine(gc,x,y,x+xsize-1,y);
	GC_DrawLine(gc,x,y,x,y+ysize-1);
	GC_DrawLine(gc,x+xsize-1,y,x+xsize-1,y+ysize-1);
	GC_DrawLine(gc,x,y+ysize-1,x+xsize-1,y+ysize-1);
};

void CGame::DrawRecord(GC*gc)
{
	DrawWindow(gc,8*_csize,5*_csize,4*_csize,5*_csize);
	int xsize=GetLenPixels(_place+1);
	DrawNumber(gc,10*_csize-xsize/2,6*_csize,_place+1);
	xsize=GetLenPixels(_point);
	DrawNumber(gc,10*_csize-xsize/2,8*_csize,_point);
};

void CGame::Out(GC*gc)
{
	DrawGlobal(gc);
	DrawCursor(gc);
	DrawPoints(gc);
	DrawNumber(gc,x_point,y_point,_point);
	DrawNumber(gc,x_spoint,y_spoint,selectlist->FirstFree*(selectlist->FirstFree-1));
	if(_place<10) DrawRecord(gc);
	if(infostate) DrawInfo(gc);
};

bool CGame::GetNearby(int i,int j)
{
	CCube*cub=map[i][j];
	if(!cub) return false;
	CCube*cub2;
	if(i>0)
	{
		cub2=map[i-1][j];
		if(cub2)
			if(cub2->GetColor()==cub->GetColor()) return true;
	};
	if(j>0)
	{
		cub2=map[i][j-1];
		if(cub2)
			if(cub2->GetColor()==cub->GetColor()) return true;
	};
	if(i<_ysize-1)
	{
		cub2=map[i+1][j];
		if(cub2)
			if(cub2->GetColor()==cub->GetColor()) return true;
	};
	if(j<_xsize-1)
	{
		cub2=map[i][j+1];
		if(cub2)
			if(cub2->GetColor()==cub->GetColor()) return true;
	};
	return false;
};

void CGame::SetPlace(int place)
{
	_place=place;
};

int CGame::GetLenPixels(int number)
{
	int len=1;
	int i=10;
	while(number>i)
	{
		len++;
		i*=10;
	};
	return len*GetImageWidth(img[0]);
};

void CGame::DrawNumeral(GC*gc,int &x,int &y,int numeral)
{
	GC_PutChar(gc,x,y,0,0,img[numeral]);
	x+=GetImageWidth(img[numeral]);
};

void CGame::DrawNumber(GC*gc,int x,int y,int number)
{
	char buf[16];
	sprintf(buf,"%i",number);
	int len=strlen(buf);
	for(int i=0;i<len;i++) DrawNumeral(gc,x,y,buf[i]-0x30);
}

void CGame::BackAction()
{
	_place=10;
	int i=actionlist->FirstFree;
	if(i)
	{
		CCube*ocube=map[cursor->y][cursor->x];
		LIST*list=(LIST*)List_RemoveAt(actionlist,i-1);
		i=list->FirstFree;
		_point-=i*(i-1);
		CCube*cube;
		while(i)
		{
			i--;
			cube=(CCube*)List_RemoveAt(list,i);
			cube->SetState(STATE_ON);
		};
		List_Destroy(list);
		Cube2Map();
		ReSelect();
		cube=map[cursor->y][cursor->x];
		if(ocube==cube) ReDrawGlobal();
	}
};

void CGame::DrawPoints(GC*gc)
{
	CPoint*pointdraw;
	int i=pointlist->FirstFree;
	while(i)
	{
		i--;
		pointdraw=(CPoint*)List_Get(pointlist,i);
		DrawNumber(gc,pointdraw->GetX(),pointdraw->GetY(),pointdraw->GetPoint());
	};
};

void CGame::onRefresh()
{
	CPoint*pointdraw;
	int i=pointlist->FirstFree;
	while(i)
	{
		i--;
		pointdraw=(CPoint*)List_Get(pointlist,i);
		pointdraw->onMove();
		if(pointdraw->GetY()<0)
		{
			pointdraw=(CPoint*)List_RemoveAt(pointlist,i);
			delete pointdraw;
		};
	};
};

void CGame::EnterAction()
{
	int i=selectlist->FirstFree;
	if(i>1)
	{
		int dpoint=i*(i-1);
		_point+=dpoint;
		CPoint*pointdraw=new CPoint(cursor->x*_csize,cursor->y*_csize,dpoint);
		List_InsertLast(pointlist,pointdraw);
		CCube*cube;
		LIST*destroylist=List_Create();
		while(i)
		{
			i--;
			cube=(CCube*)List_RemoveAt(selectlist,i);
			cube->UnSelect();
			cube->SetState(STATE_OFF);
			List_InsertLast(destroylist,cube);
		};
		List_InsertLast(actionlist,destroylist);
		Cube2Map();
		cube=map[cursor->y][cursor->x];
		if(!cube)
		{
			int minlen=_ysize*_ysize+_xsize*_xsize;
			int len;
			int posx=0,
			posy=_ysize-1;
			for(int y=cursor->y;y<_ysize;y++)
				for(int x=0;x<_xsize;x++)
				{
					cube=map[y][x];
					if(cube)
					{
						len=(y-cursor->y)*(y-cursor->y)+(x-cursor->x)*(x-cursor->x);
						if(len<=minlen)
						{
							minlen=len;
							posx=x;
							posy=y;
						};
					};
				};
			cursor->x=posx;
			cursor->y=posy;
		};
		ReSelect();
		CheckGameEnd();
	};
};

wchar_t* CGame::GetConfigPath(const wchar_t*Dir)
{
	wchar_t*res=new wchar_t[256];
	wstrcpy(res,GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
	wstrcat(res,L"/");
	wstrcat(res,Dir);
	return res;
};

void CGame::MoveCursor(DIRECTION direction)
{
	int ox=cursor->x;
	int oy=cursor->y;
	int count=0;
	switch(direction)
	{
	case DIRECTION_LEFT:
		do
		{
			cursor->x--;
			if(cursor->x<0) cursor->x=_xsize-1;
			count++;
		}
		while(count<_xsize && !map[cursor->y][cursor->x]);
		break;
	case DIRECTION_UP:
		do
		{
			cursor->y--;
			if(cursor->y<0) cursor->y=_ysize-1;
			count++;
		}
		while(count<_ysize && !map[cursor->y][cursor->x]);
		break;
	case DIRECTION_RIGHT:
		do
		{
			cursor->x++;
			if(cursor->x==_xsize) cursor->x=0;
			count++;
		}
		while(count<_xsize && !map[cursor->y][cursor->x]);
		break;
	case DIRECTION_DOWN:
		do
		{
			cursor->y++;
			if(cursor->y==_ysize) cursor->y=0;
			count++;
		}
		while(count<_ysize && !map[cursor->y][cursor->x]);
		break;
	};
	if(!map[cursor->y][cursor->x])
	{
		cursor->y=oy;
		cursor->x=ox;
	}
	else ReSelect();
};

void CGame::InitNewGame()
{
	_place=10;
	infostate=false;
	DATETIME dt;
	REQUEST_DATEANDTIME_GET(0,&dt);
	srand( (dt.time.sec<<16) | (dt.time.min<<8) | (dt.time.hour) );
	_point=0;
	cursor->x=0;
	cursor->y=0;
	for(int i=0;i<_ysize;i++)
		for(int j=0;j<_xsize;j++)
		{
			if(cubes[i][j]) delete cubes[i][j];
			cubes[i][j]=CreateCube();
			map[i][j]=cubes[i][j];
		};
	int i=actionlist->FirstFree;
	LIST*list;
	while(i)
	{
		i--;
		list=(LIST*)List_RemoveAt(actionlist,i);
		int j=actionlist->FirstFree;
		while(j)
		{
			j--;
			List_RemoveAt(list,j);
		};
		List_Destroy(list);
	};
	i=selectlist->FirstFree;
	while(i)
	{
		i--;
		List_RemoveAt(selectlist,i);
	};
	i=pointlist->FirstFree;
	CPoint*pointdraw;
	while(i)
	{
		i--;
		pointdraw=(CPoint*)List_RemoveAt(pointlist,i);
		delete pointdraw;
	};
	ReSelect();
};

void CGame::SetRecord(RECORD*rec)
{
	for(int i=0;i<_ysize;i++)
		for(int j=0;j<_xsize;j++)
		{
			cubes[i][j]->SetColor(rec->colors[i][j]);
		};
	Cube2Map();
	ReSelect();
	ReDrawGlobal();
};

CGame::CGame(int xsize,int ysize,int csize)
{
	_cpath=GetConfigPath(CONFIGPATH);
	y_max=ysize*csize;
	x_max=xsize*csize;
	globalgc=GC_CreateMemoryGC(x_max,y_max,16,0,0,0);
	x_point=0;
	y_point=0;
	x_spoint=x_max/2;
	y_spoint=0;
	_xsize=xsize;
	_ysize=ysize;
	_csize=csize;
	bool check=true;
	FSTAT _fstat;
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<5;j++)
			if(fstat(_cpath,ICONNAMES[i][j],&_fstat)!=0)
			{
				check=false;
				break;
			};
		if(!check) break;
	};
	if(fstat(_cpath,L"BG.png",&_fstat)!=0) check=false;
	if(check)
	{
		for(int i=0;i<2;i++)
			for(int j=0;j<5;j++)
				imagescub[i][j]=CreateCubImage(ICONNAMES[i][j]);
		imageempty=CreateCubImage(L"BG.png");
	}
	else
	{
		imagescub[0][0]=CreateCubImage(COLOR_RED,UNSELECTED);
		imagescub[0][1]=CreateCubImage(COLOR_GREEN,UNSELECTED);
		imagescub[0][2]=CreateCubImage(COLOR_BLUE,UNSELECTED);
		imagescub[0][3]=CreateCubImage(COLOR_YELLOW,UNSELECTED);
		imagescub[0][4]=CreateCubImage(COLOR_PURPLE,UNSELECTED);
		imagescub[1][0]=CreateCubImage(COLOR_RED,SELECTED);
		imagescub[1][1]=CreateCubImage(COLOR_GREEN,SELECTED);
		imagescub[1][2]=CreateCubImage(COLOR_BLUE,SELECTED);
		imagescub[1][3]=CreateCubImage(COLOR_YELLOW,SELECTED);
		imagescub[1][4]=CreateCubImage(COLOR_PURPLE,SELECTED);
		imageempty=CreateCubImage(COLOR_WHITE,SELECTED);
	};
	iconidname2id(L"MIDI_COMP_BL_NBR_0_ICN",-1,&img[0]);
	iconidname2id(L"MIDI_COMP_BL_NBR_1_ICN",-1,&img[1]);
	iconidname2id(L"MIDI_COMP_BL_NBR_2_ICN",-1,&img[2]);
	iconidname2id(L"MIDI_COMP_BL_NBR_3_ICN",-1,&img[3]);
	iconidname2id(L"MIDI_COMP_BL_NBR_4_ICN",-1,&img[4]);
	iconidname2id(L"MIDI_COMP_BL_NBR_5_ICN",-1,&img[5]);
	iconidname2id(L"MIDI_COMP_BL_NBR_6_ICN",-1,&img[6]);
	iconidname2id(L"MIDI_COMP_BL_NBR_7_ICN",-1,&img[7]);
	iconidname2id(L"MIDI_COMP_BL_NBR_8_ICN",-1,&img[8]);
	iconidname2id(L"MIDI_COMP_BL_NBR_9_ICN",-1,&img[9]);
	cursor=new Cursor;
	actionlist=List_Create();
	selectlist=List_Create();
	pointlist=List_Create();
	cubes=new CCube**[_ysize];
	map=new CCube**[_ysize];
	for(int i=0;i<ysize;i++)
	{
		cubes[i]=new CCube*[_xsize];
		map[i]=new CCube*[_xsize];
		for(int j=0;j<_xsize;j++)
			cubes[i][j]=0;
	};
	InitNewGame();
};

CGame::~CGame()
{
	delete[] _cpath;
	GC_FreeGC(imageempty);
	for(int i=0;i<2;i++)
		for(int j=0;j<5;j++)
			GC_FreeGC(imagescub[i][j]);
	GC_FreeGC(globalgc);
	delete cursor;
	int i=actionlist->FirstFree;
	LIST*list;
	while(i)
	{
		i--;
		list=(LIST*)List_RemoveAt(actionlist,i);
		int j=actionlist->FirstFree;
		while(j)
		{
			j--;
			List_RemoveAt(list,j);
		};
		List_Destroy(list);
	};
	List_Destroy(actionlist);
	i=selectlist->FirstFree;
	while(i)
	{
		i--;
		List_RemoveAt(selectlist,i);
	};
	List_Destroy(selectlist);
	CPoint*pointdraw;
	i=pointlist->FirstFree;
	while(i)
	{
		i--;
		pointdraw=(CPoint*)List_RemoveAt(pointlist,i);
		delete pointdraw;
	};
	List_Destroy(pointlist);
	for(i=0;i<_ysize;i++)
		for(int j=0;j<_xsize;j++)
			delete cubes[i][j];
	for(i=0;i<_ysize;i++)
	{
		delete[] map[i];
		delete[] cubes[i];
	};
	delete[] map;
	delete[] cubes;
};
