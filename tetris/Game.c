//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Menu.h"
#include "MyFUNC.h"
#include "rand.h"
#include "NewRecord.h"
#include "Pages.h"
#include "Save.h"
int left, right;
int giperniz=0;
DATETIME dt;
GC* hMGC=0;//тут будем хранить изображение заднего фона и кубиков в стакане (статическая часть изображения)
GC* ggc=0;
GVI_PEN  p=0;
GVI_GC gvi;
bool redraw=true;//флаг, для перерисовки статической части

typedef GUI GUI_CUSTOM;
DISP_DESC *dd;
GUI_CUSTOM*np;

FIGURA*f;
GAME*g;

FIGURA*returnf()
{
  return f;
}

GAME*returng()
{
  return g;
}


static const char custom_guiname[]="gui_custom";

int custom_onCreate(DISP_OBJ *DO)
{
  return 1;
};

void custom_onClose(DISP_OBJ *DO)
{
  return;
};

void custom_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,custom_guiname);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)custom_onCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)custom_onClose);
};

void custom_destr(GUI *desc)
{
  return;
};

GUI_CUSTOM *custom_create(BOOK *bk)
{
  GUI_CUSTOM *gui_cs=new GUI_CUSTOM;
  if (!CreateObject((GUI*)gui_cs,custom_destr, custom_constr,bk,0,0,0))
  {
    delete gui_cs;
    return 0;
  }
  if (bk) addGui2book(bk,(GUI*)gui_cs);
  GUI_SetStyle((GUI*)gui_cs, 4);
  GuiObject_SetTitleType((GUI*)gui_cs, 1);
  GUIObject_HideSoftkeys((GUI*)gui_cs);
  return gui_cs;
};

void DrawCub(int cx, int cy, int imageID, int col, GC*gc,GVI_GC gvi)
{
  if(col==0) col=0xff000000;
  if(cy<=20)
  {
    int cub=MyBK()->dis[GetDis()]->cub;
    int x=MyBK()->dis[GetDis()]->x;
    int y=MyBK()->dis[GetDis()]->y;
    int x1,y1,x2,y2;
    x1=cx*cub;
    y1=y-(cy*cub);
    x2=x1+cub;
    y2=y1+cub;
    if(MyBK()->image->fi[imageID] && MyBK()->nast->im->on)
    {
      putchar(gc , x1, y1, 0, 0, MyBK()->image->im[imageID]->ImageID);
    }
    else
    {
      GC_SetPenColor(hMGC, 0xff000000);
      GC_DrawFRect(gc,0xffffffff,x1, y1,cub,cub);
      GC_DrawFRect(gc,col,x1+2, y1+2,cub-2,cub-2);
      GC_DrawLine(gc,x1,y1,x1,y2);
      GC_DrawLine(gc,x1,y1,x2,y1);
      GC_DrawLine(gc,x2,y2,x1,y2);
      GC_DrawLine(gc,x2,y2,x2,y1);
    }
  }
}

void DrawNewFigura(char xm[5][5],GC*gc,GVI_GC gvi)
{
  int x, y;
  for(x=0; x<5; x++)
  {
    for(y=0; y<5; y++)
    {
      if(xm[x][y]) DrawCub(x+12, 8-y, 6,MyBK()->nast->colors[4],gc,gvi);
    }
  }
}

void DrawFigura(int sx,int sy,char xm[5][5],GC*gc,GVI_GC gvi)
{
  int x, y;
  for(x=0; x<5; x++)
  {
    for(y=0; y<5; y++)
    {
      if(xm[x][y]) DrawCub(x+sx, sy-y, f->CubID,f->col*MyBK()->nast->coloron,gc,gvi);
    }
  }
}

void DrawPole(char x[12][24],GC*gc,GVI_GC gvi)
{
  int i1, i2;
  for(i2=1;i2<24;i2++)
  {
    bool f;
    f=false;
    for(i1=1;i1<11;i1++)
    {
      if(x[i1][i2])
      {
        DrawCub(i1,i2,g->CubID[i1][i2],g->color[i1][i2]*MyBK()->nast->coloron,gc,gvi);
        f=true;
      }
    }
    if(!f) break;
  }
  for(i2=1;i2<21;i2++)
  {
    DrawCub(0,i2,g->CubID[i1][i2],g->color[i1][i2]*MyBK()->nast->coloron,gc,gvi);
    DrawCub(11,i2,g->CubID[i1][i2],g->color[i1][i2]*MyBK()->nast->coloron,gc,gvi);
  }
}

int Check(int x, int y, char p[5][5])
{
  int yc;
  int xc;
  for(xc=0; xc<5; xc++)
  {
    for(yc=0; yc<5; yc++)
    {
      if(p[xc][yc])
      {
        if((x+xc)<1||(x+xc)>10) return 0;
        if((y-yc)==0) return 0;
        if(g->pole[xc+x][y-yc]) return 0;
      }
    }
  }
  return 1;
}

void DrawText(MyBook*bk, int FSize, int x, int y, int cub)
{
  SetFont(FSize);
  int o[2];
#ifdef ENG_LANG
  o[0]=STR("Очки:");
#else
  o[0]=STR("Point:");
#endif
  o[1]=int2strID(g->point);
  DrawString(o[0],0, cub*12+5, y-20*cub, cub*12+5+GetFSize(FSize)*5, y-20*cub+GetFSize(FSize), 1, 0x01,MyBK()->nast->colors[7],MyBK()->nast->colors[7]);
  DrawString(o[1],0, cub*12+5, y-20*cub+GetFSize(FSize)+1, cub*12+5+GetFSize(FSize)*4, y-20*cub+2*GetFSize(FSize)+1, 1, 0x01,MyBK()->nast->colors[7],MyBK()->nast->colors[7]);
  TextFree(o[0]);
  TextFree(o[1]);
  int met[3];
  int ID;
  REQUEST_DATEANDTIME_GET(SYNC, &dt);
#ifdef ENG_LANG
  ID=STR("Время:");
#else
  ID=STR("Time:");
#endif
  DrawString(ID,0, cub*12+5, y-20*cub+2*GetFSize(FSize)+6, cub*12+5+GetFSize(FSize)*6, y-20*cub+3*GetFSize(FSize)+6, 1, 0x01,MyBK()->nast->colors[7],MyBK()->nast->colors[7]);
  TextFree(ID);
  met[0]=int2strID(dt.time.hour);
  if(dt.time.min<10) met[1]=STR(":0");
  else met[1]=STR(":");
  met[2]=int2strID(dt.time.min);
  ID=Str2ID(met, ENC_TEXTID, 3);
  DrawString(ID,0, cub*12+5, y-20*cub+3*GetFSize(FSize)+7, cub*12+5+GetFSize(FSize)*5, y-20*cub+4*GetFSize(FSize)+6, 1, 0x01,MyBK()->nast->colors[7],MyBK()->nast->colors[7]);
  TextFree(ID);
  TextFree(met[0]);
  TextFree(met[1]);
  TextFree(met[2]);
#ifdef ENG_LANG
  ID=STR("Уровень:");
#else
  ID=STR("Level:");
#endif
  DrawString(ID, 0, cub*12+5, y-20*cub+4*GetFSize(FSize)+9, cub*12+5+GetFSize(FSize)*8, y-20*cub+5*GetFSize(FSize)+9, 1, 0x01, MyBK()->nast->colors[7],MyBK()->nast->colors[7]);
  TextFree(ID);
  ID=int2strID(g->point/10);
  DrawString(ID, 0, cub*12+5, y-20*cub+5*GetFSize(FSize)+10, cub*12+5+GetFSize(FSize)*3, y-20*cub+6*GetFSize(FSize)+10, 1, 0x01, MyBK()->nast->colors[7],MyBK()->nast->colors[7]);
  TextFree(ID);
}

void DrawPause(int FSize, int x, int y)
{
  DrawRect(0,0,x,y, 0xa0000000, 0xa0000000);
  SetFont(FSize);
#ifdef ENG_LANG
  int ID=STR("Пауза");
#else
  int ID=STR("Pause");
#endif
  DrawString(ID, 0, (x-GetFSize(FSize)*5)/2, (y-GetFSize(FSize))/2, (x-GetFSize(FSize)*5)/2+5*GetFSize(FSize), (y-GetFSize(FSize))/2+GetFSize(FSize), 1, 0x01,0xFFFF0000,0xFFFFFFFF);
  TextFree(ID);
}

void myOnRedraw(DISP_OBJ *DO, int a, int b, int c)
{
  int cub=MyBK()->dis[GetDis()]->cub;
  int x=MyBK()->dis[GetDis()]->x;
  int y=MyBK()->dis[GetDis()]->y;
  if(redraw)//если флаг, то перерисовываем статическое изображение
  {
    if(hMGC) GC_FreeGC(hMGC);
    hMGC = GC_CreateMemoryGC(x, y, 16,0, 0 , 0);
    GC_DrawFRect (hMGC, 0xFF000000, 0,0,x,y);
    CANVAS_Get_GviGC(hMGC->pcanvas, &gvi);
    if(MyBK()->image->fi[0] && MyBK()->nast->im->on)
    {
      putchar(hMGC, 0, 0, 0, 0, MyBK()->image->im[0]->ImageID);//выводим иконку заднего фона в память
    }
    else
    {
      GC_DrawFRect (hMGC,MyBK()->nast->colors[5],0,0,x,y);//или рисуем его в ручную (зависит от настроек пользователя)
    }
    if(MyBK()->image->fi[1] && MyBK()->nast->im->on)
    {
      putchar(hMGC , 0, y-(20*cub), 0, 0, MyBK()->image->im[1]->ImageID);//выводим иконку игрового поля в память
    }
    else
    {
      GC_DrawFRect (hMGC,MyBK()->nast->colors[6],0,y-(20*cub),12*cub,y);//или же рисуем вручную
      //рисуем сеточку
      for(int ix=0; ix<21; ix++)
      {
        GC_SetPenColor(hMGC, 0x60000000);
        if(ix<=11) GC_DrawLine(hMGC,ix*cub,y,ix*cub,y-cub*20);
        GC_DrawLine(hMGC,0,y-ix*cub,11*cub,y-ix*cub);
      }
    }
    int id;
    //иконка звука
    if(MyBK()->nast->sound->sound)
    {
      iconidname2id(L"TR_ALARM_ON_ICN", -1, &id);
    }
    else
    {
      iconidname2id(L"TR_ALARM_OFF_ICN", -1, &id);
    }
    putchar(hMGC, MyBK()->dis[GetDis()]->x-2*cub, MyBK()->dis[GetDis()]->y-2*cub, 0, 0, id);
    DrawPole(g->pole,hMGC,gvi);        //рисуем в память кубики в стакане
    DrawNewFigura(f->newpole,hMGC,gvi);//и следующую фигурку
    redraw=false;
  }
  ggc=(GC*)get_DisplayGC();
  GVI_GC gvigcdisp;
  CANVAS_Get_GviGC( ggc->pcanvas,&gvigcdisp);
  GVI_BitBlt(gvigcdisp, 0, 0, x, y, gvi, 0, 0, 204, 0, 0, 0);  //выводим статическое изображение из памяти на экран
  DrawFigura(f->x, f->y, f->pole,ggc,gvi);//далее рисуем фигурку на экране
  DrawText(MyBK(), MyBK()->dis[GetDis()]->FS,x,y,cub);//и текст
  if(MyBK()->pause) DrawPause(MyBK()->dis[GetDis()]->FP, x, y);
}

void obn()
{
  MyBK()->r->x1=(f->x-1)*MyBK()->dis[GetDis()]->cub;
  MyBK()->r->x2=MyBK()->r->x1+7*MyBK()->dis[GetDis()]->cub;
  if(MyBK()->r->x2>MyBK()->dis[GetDis()]->cub*12) MyBK()->r->x2=MyBK()->dis[GetDis()]->cub*12;
  MyBK()->r->y1=MyBK()->dis[GetDis()]->y-f->y*MyBK()->dis[GetDis()]->cub;
  MyBK()->r->y2=MyBK()->r->y1+6*MyBK()->dis[GetDis()]->cub;
  if(MyBK()->r->y2>MyBK()->dis[GetDis()]->y) MyBK()->r->y2=MyBK()->dis[GetDis()]->y;
  DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
}


void onpTimer (u16 tmr , LPARAM)
{
  bool flag=false;
  if(left && Check(f->x-1, f->y, f->pole))
  {
    f->x+=-1;
    flag=true;
  }
  if(right && Check(f->x+1, f->y, f->pole))
  {
    f->x++;
    flag=true;
  }
  if(flag)
  {
    RECT*r=new RECT;
    r->x1=(f->x-1)*MyBK()->dis[GetDis()]->cub;
    r->x2=r->x1+7*MyBK()->dis[GetDis()]->cub;
    if(MyBK()->r->x2>MyBK()->dis[GetDis()]->cub*12) MyBK()->r->x2=MyBK()->dis[GetDis()]->cub*12;
    r->y1=MyBK()->dis[GetDis()]->y-(f->y+1)*MyBK()->dis[GetDis()]->cub;
    r->y2=r->y1+6*MyBK()->dis[GetDis()]->cub;
    if(MyBK()->r->y2>MyBK()->dis[GetDis()]->y) MyBK()->r->y2=MyBK()->dis[GetDis()]->y;
    DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
  }
  Timer_ReSet(&(MyBK()->ptimer),50,onpTimer,0);
  MyBK()->iptimer=1;
}

void myOnKey(DISP_OBJ *db,int key,int a,int b,int type)
{
  if((key==MyBK()->nast->buttons[0][3] || key==MyBK()->nast->buttons[1][3]) && (type==KBD_LONG_PRESS || type==KBD_SHORT_PRESS) && !MyBK()->pause) giperniz=1;
  if((key==MyBK()->nast->buttons[0][2] || key==MyBK()->nast->buttons[1][2]) && (type==KBD_LONG_PRESS || type==KBD_SHORT_PRESS) && !MyBK()->pause)
  {
    if(!g->megaspeed) g->megaspeed=1;
  }
  if((key==MyBK()->nast->buttons[0][2] || key==MyBK()->nast->buttons[1][2]) && (type==KBD_LONG_RELEASE || type==KBD_SHORT_RELEASE))
  {
    if(g->megaspeed) g->megaspeed=0;
  }
  if((key==MyBK()->nast->buttons[0][5] || key==MyBK()->nast->buttons[1][5])&& (type==KBD_LONG_RELEASE || type==KBD_SHORT_RELEASE)&&!MyBK()->pause)
  {
    char newpole[5][5];
    for(int i=0; i<5; i++)
    {
      for(int j=0; j<5; j++)
      {
        newpole[j][4-i]=f->pole[i][j];
      }
    }
    if(f->ID!=6)
    {
      if(Check(f->x, f->y, newpole))
      {
      }
      else
      {
        if(Check(f->x+1, f->y, newpole)) f->x++;
        else
        {
          if(Check(f->x+2, f->y, newpole ) && f->ID==0) f->x+=2;
          else
          {
            if(Check(f->x-1, f->y, newpole)) f->x+=-1;
            else
            {
              if(Check(f->x-2, f->y, newpole) && f->ID==0) f->x+=-2;
              else return;
            }
          }
        }
      }
      for(int i=0; i<5; i++)
      {
        for(int j=0; j<5; j++)
        {
          f->pole[i][j]=newpole[i][j];
        }
      }
    }
    obn();
  }
  if((key==MyBK()->nast->buttons[0][4] || key==MyBK()->nast->buttons[1][4])&& (type==KBD_LONG_RELEASE || type==KBD_SHORT_RELEASE)&&!MyBK()->pause)
  {
    char newpole[5][5];
    for(int i=0; i<5; i++)
    {
      for(int j=0; j<5; j++)
      {
        newpole[4-j][i]=f->pole[i][j];
      }
    }
    if(f->ID!=6)
    {
      if(Check(f->x, f->y, newpole));
      else
      {
        if(Check(f->x+1, f->y, newpole)) f->x++;
        else
        {
          if(Check(f->x+2, f->y, newpole ) && f->ID==0) f->x+=2;
          else
          {
            if(Check(f->x-1, f->y, newpole)) f->x+=-1;
            else
            {
              if(Check(f->x-2, f->y, newpole) && f->ID==0) f->x+=-2;
              else return;
            }
          }
        }
      }
      for(int i=0; i<5; i++)
      {
        for(int j=0; j<5; j++)
        {
          f->pole[i][j]=newpole[i][j];
        }
      }
    }
    obn();
  }
  if((key==MyBK()->nast->buttons[0][8] || key==MyBK()->nast->buttons[1][8]) && type==KBD_LONG_PRESS)
  {
    GUI_Free((GUI*)np);
    SaveTempGame(g,f);
    Timer_Kill(&(MyBK()->ftimer));
    Timer_Kill(&(MyBK()->ptimer));
    MyBK()->iptimer=0;
    MyBK()->gtimer=0;
    return;
  }
  if((key==MyBK()->nast->buttons[0][8] || key==MyBK()->nast->buttons[1][8]) && type==KBD_SHORT_RELEASE)
  {
    if(MyBK()->pause==0) MyBK()->pause=1;
    else MyBK()->pause=0;
    PlayMyMusic(4);
    g->megaspeed=0;
    DispObject_InvalidateRect(db,0);
    return;
  }
  if((key==MyBK()->nast->buttons[0][6] || key==MyBK()->nast->buttons[1][6]) && type==KBD_SHORT_RELEASE)
  {
    BookObj_Hide((BOOK*)MyBK(), 0);
    PlayMyMusic(5);
    if(MyBK()->pause==0) MyBK()->pause=1;
    redraw=true;
    DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
    return;
  }
  if((key==MyBK()->nast->buttons[0][7] || key==MyBK()->nast->buttons[1][7]) && type==KBD_SHORT_RELEASE)
  {
    if(MyBK()->nast->sound->sound) MyBK()->nast->sound->sound=0;
    else MyBK()->nast->sound->sound=1;
    RECT*rs=new RECT;
    rs->x1=MyBK()->dis[GetDis()]->x-MyBK()->dis[GetDis()]->cub*2;
    rs->x2=MyBK()->dis[GetDis()]->x;
    rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->dis[GetDis()]->cub*2;
    rs->y2=MyBK()->dis[GetDis()]->y;
    redraw=true;
    DispObject_InvalidateRect(db,0);
    return;
  }
  if((key==MyBK()->nast->buttons[0][0] || key==MyBK()->nast->buttons[1][0] || key==MyBK()->nast->buttons[0][1] || key==MyBK()->nast->buttons[1][1]) && (type==KBD_LONG_RELEASE || type==KBD_SHORT_RELEASE))
  {
    left=0;
    right=0;
  }
  if((key==MyBK()->nast->buttons[0][0] || key==MyBK()->nast->buttons[1][0]) && type==KBD_SHORT_PRESS && !MyBK()->pause)
  {
    if(Check(f->x-1, f->y, f->pole)) f->x=f->x-1;
    left=1;
    right=0;
    obn();
    Timer_ReSet(&(MyBK()->ptimer),500,onpTimer,0);
  }
  if((key==MyBK()->nast->buttons[0][1] || key==MyBK()->nast->buttons[1][1]) && type==KBD_SHORT_PRESS && !MyBK()->pause)
  {
    if(Check(f->x+1, f->y, f->pole)) f->x+=+1;
    left=0;
    right=1;
    obn();
    Timer_ReSet(&(MyBK()->ptimer),500,onpTimer,0);
  }
  //r->x1=11*MyBK()->dis[GetDis()]->cub;
  //r->x2=MyBK()->dis[GetDis()]->x;
  //r->y1=0;
  //r->y2=MyBK()->dis[GetDis()]->y;
  //DispObject_InvalidateRect(((GUI*)np)->DISP_OBJ,0);
}

void newFig()
{
  int x, y;
  for(x=0; x<5; x++)
  {
    for(y=0; y<5; y++)
    {
      f->pole[x][y]=f->newpole[x][y];
      f->newpole[x][y]=0;
    }
  }
  f->ID=f->newID;
  f->CubID=rand(4)+2;
  f->col=MyBK()->nast->colors[f->CubID-2];
  int rand_x;
  if(g->speed==3) rand_x=rand(7)+rand(7);
  else rand_x=rand(7);
  if(rand_x==13) rand_x=0;
  switch(rand_x)
  {
  case 0:
    f->newpole[0][2]=1;
    f->newpole[1][2]=1;
    f->newpole[2][2]=1;
    f->newpole[3][2]=1;
    break;
  case 1:
    f->newpole[1][2]=1;
    f->newpole[2][1]=1;
    f->newpole[2][2]=1;
    f->newpole[3][2]=1;
    break;
  case 2:
    f->newpole[1][1]=1;
    f->newpole[1][2]=1;
    f->newpole[2][2]=1;
    f->newpole[3][2]=1;
    break;
  case 3:
    f->newpole[1][2]=1;
    f->newpole[2][2]=1;
    f->newpole[3][1]=1;
    f->newpole[3][2]=1;
    break;
  case 4:
    f->newpole[1][1]=1;
    f->newpole[1][2]=1;
    f->newpole[2][2]=1;
    f->newpole[2][3]=1;
    break;
  case 5:
    f->newpole[2][2]=1;
    f->newpole[2][3]=1;
    f->newpole[3][1]=1;
    f->newpole[3][2]=1;
    break;
  case 6:
    f->newpole[1][1]=1;
    f->newpole[1][2]=1;
    f->newpole[2][1]=1;
    f->newpole[2][2]=1;
    break;
    //-------------
  case 7:
    f->newpole[1][2]=1;
    f->newpole[2][1]=1;
    f->newpole[2][2]=1;
    f->newpole[2][3]=1;
    f->newpole[3][2]=1;
    break;
  case 8:
    f->newpole[1][1]=1;
    f->newpole[1][2]=1;
    f->newpole[2][2]=1;
    f->newpole[3][2]=1;
    f->newpole[3][3]=1;
    break;
  case 9:
    f->newpole[1][2]=1;
    f->newpole[1][3]=1;
    f->newpole[2][2]=1;
    f->newpole[3][1]=1;
    f->newpole[3][2]=1;
    break;
  case 10:
    f->newpole[1][2]=1;
    f->newpole[1][3]=1;
    f->newpole[2][2]=1;
    f->newpole[3][2]=1;
    f->newpole[3][3]=1;
    break;
  case 11:
    f->newpole[1][1]=1;
    f->newpole[2][1]=1;
    f->newpole[2][2]=1;
    f->newpole[2][3]=1;
    f->newpole[3][1]=1;
    break;
  case 12:
    f->newpole[2][2]=1;
    f->newpole[2][3]=1;
    f->newpole[3][2]=1;
    break;
  }
  f->newID=rand_x;
  int pov=rand(4);
  while(pov>0)
  {
    char newpole[5][5];
    for(int i=0; i<5; i++)
    {
      for(int j=0; j<5; j++)
      {
        newpole[4-j][i]=f->newpole[i][j];
      }
    }
    pov+=-1;
    for(int i=0; i<5; i++)
    {
      for(int j=0; j<5; j++)
      {
        f->newpole[i][j]=newpole[i][j];
      }
    }
  }
  f->x=3;
  f->y=24;
}

void StopFigura()
{
  int x=0;
  int y=0;
  int j;
  int len=0;
  for(x=0; x<5; x++)
  {
    for(y=4; y>=0; y+=-1)
    {
      if(f->pole[x][y])
      {
        g->pole[x+f->x][f->y-y]=f->pole[x][y];
        g->color[x+f->x][f->y-y]=f->col;
        g->CubID[x+f->x][f->y-y]=f->CubID;
      }
    }
  }
  for(y=1; y<24; y++)
  {
    int sum=1;
    for(int ii=1;ii<=10;ii++)
      sum*=g->pole[ii][y];
    if(sum==1)
    {
      int i;
      len++;
      for(i=y; i<20; i++)
      {
        for(j=1; j<11; j++)
        {
          g->pole[j][i]=g->pole[j][i+1];
          g->color[j][i]=g->color[j][i+1];
        }
      }
      for(j=1; j<11; j++)
      {
        g->pole[j][20]=0;
        g->color[j][20]=0;
      }
      y=y-1;
    }
  }
  if(len)
  {
    switch(len)
    {
    case 1:
      g->point++;
      break;
    case 2:
      g->point+=2*(rand(4)/2+1);
      break;
    case 3:
      g->point+=3*(rand(4)/2+1);
      break;
    case 4:
      g->point+=4*(rand(4)/2+1);
      break;
    }
  }
  redraw=true;
  DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
  newFig();
}


void onfTimer (u16 tmr , LPARAM)
{
  int gg=0;
  MyBK()->gtimer=1;
  for(int i=1; i<11; i++)
  {
    gg=gg+g->pole[i][20];
  }
  if(gg)
  {
    GUI_Free((GUI*)np);
    MyBK()->point=g->point;
    MyBK()->speed=g->speed;
    PlayMyMusic(2);
    BookObj_GotoPage(isBookX(NameMyElf, 0),&bk_go);
    Timer_Kill(&(MyBK()->ftimer));
    MyBK()->gtimer=0;
    Timer_Kill(&(MyBK()->ptimer));
    MyBK()->iptimer=0;
    return;
  }
  if(!MyBK()->pause)
  {
    if(!Check(f->x, f->y-1, f->pole))
    {
      StopFigura();
      PlayMyMusic(1);
      Timer_ReSet(&(MyBK()->ftimer),(1000/g->speed-g->point*g->speed),onfTimer,0);
      return;
    }
    f->y+=-1;
    if(giperniz)
    {
      while(Check(f->x, f->y-2, f->pole)) f->y+=-1;
      giperniz=0;
      DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
    }
    MyBK()->r->x1=(f->x-1)*MyBK()->dis[GetDis()]->cub;
    MyBK()->r->x2=MyBK()->r->x1+7*MyBK()->dis[GetDis()]->cub;
    if(MyBK()->r->x2>MyBK()->dis[GetDis()]->cub*11) MyBK()->r->x2=MyBK()->dis[GetDis()]->cub*11;
    MyBK()->r->y1=MyBK()->dis[GetDis()]->y-(f->y+1)*MyBK()->dis[GetDis()]->cub;
    MyBK()->r->y2=MyBK()->r->y1+6*MyBK()->dis[GetDis()]->cub;
    DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
    //r->x1=11*MyBK()->dis[GetDis()]->cub;
    //r->x2=MyBK()->dis[GetDis()]->x;
    //r->y1=0;
    //r->y2=MyBK()->dis[GetDis()]->y;
    //DispObject_InvalidateRect(((GUI*)np)->DISP_OBJ,r);
    if(!isBookX(MyElf, 1))
    {
      MyBK()->pause=1;
      PlayMyMusic(4);
      g->megaspeed=0;
      DispObject_InvalidateRect(GUIObject_GetDispObject(np),0);
    }
  }
  int time;
  if(g->megaspeed) time=20;
  else time=1000/g->speed-g->point*g->speed;
  if(time<=20) time=20;
  Timer_ReSet(&(MyBK()->ftimer),time,onfTimer,0);
}

void CreatGameGUI(BOOK*bk)
{
  redraw=true;
  MyBK()->pause=0;
  PlayMyMusic(0);
  left=0;
  right=0;
  giperniz=0;
  np=custom_create(bk);
  dd=DISP_OBJ_GetDESC (GUIObject_GetDispObject(np));
  DISP_DESC_SetOnKey(dd, myOnKey);
  DISP_DESC_SetOnRedraw(dd, myOnRedraw);
  ShowWindow((GUI*)np);
  BookObj_Show(bk, 0);
  BookObj_SetFocus(bk, 0);
}

int OpenGame(void*,BOOK*bk)
{
  if(FileSize(MyBK()->path, MyBK()->name)!=sizeof(FIGURA)+sizeof(GAME))
  {
    BookObj_GotoPage((BOOK *)(MyBK()),&bk_gm);
    MessageBox(0x6fffffff,STR("Сохранение от другой версии или некорректно!"),0,0,0,0);
    return 0;
  }
  f=new FIGURA;
  g=new GAME;
  int ifile=_fopen(MyBK()->path, MyBK()->name, 0x1, 0x180, 0);
  fread(ifile, g, sizeof(GAME));
  fread(ifile, f, sizeof(FIGURA));
  fclose(ifile);
  g->megaspeed=0;
  int err;
  FileDelete(MyBK()->path, MyBK()->name, &err);
  randInit();
  CreatGameGUI(bk);
  int time=1000/g->speed-g->point*g->speed;
  if(time<=20) time=20;
  (MyBK()->ftimer)=Timer_Set(time,onfTimer,0);
  MyBK()->ptimer=Timer_Set(150,onpTimer,0);
  MyBK()->gtimer=1;
  MyBK()->iptimer=1;
  return 0;
}

int CreateGame(void*, BOOK*bk)
{
  randInit();
  f=new FIGURA;
  g=new GAME;
  for(int i=0; i<5; i++)
  {
    for(int j=0; j<5; j++)
    {
      f->pole[i][j]=0;
      f->newpole[j][i]=0;
    }
  }
  f->ID=0;
  f->newID=0;
  newFig();
  newFig();
  g->point=0;
  g->speed=MyBK()->speed;
  g->megaspeed=0;
  g->lvl=0;
  for(int i=0; i<24; i++)
  {
    for(int j=0; j<12; j++)
    {
      if(j==0 || j==11) g->pole[j][i]=1;
      else g->pole[j][i]=0;
      g->color[j][i]=0;
      g->CubID[j][i]=2;
    }
  }
  CreatGameGUI(bk);
  PlayMyMusic(0);
  (MyBK()->ftimer)=Timer_Set(1000/((MyBook*)bk)->speed,onfTimer,0);
  MyBK()->ptimer=Timer_Set(150,onpTimer,0);
  MyBK()->gtimer=1;
  MyBK()->iptimer=1;
  return 0;
}
