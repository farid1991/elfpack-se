//библиотека функций дл€ рисовани€ и анимации
//(c)arban, UltraShot
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "draw_lib.h"
/*
v.1.0 перва€ верси€
v.1.1 убраны возвращени€ значений дл€ ускорени€ работы
v.1.2 добавлены стили текста + убраны некоторые лишние функции
*/

//модуль
int mod(int i)
{
  int res;
  if(i>=0)res=i;
  else if(i<0)res=-i;
  return(res);
}
//-модуль
int lmod(int i)
{
  int res;
  if(i>=0)res=-i;
  else if(i<0)res=i;
  return(res);
}

void AnimSpeedMove(int*src,int dest, int speed)
{
  int i;
  if(*src<dest)i=mod(speed);
  else if(*src>dest)i=lmod(speed);
  else i=0;
  if(*src!=dest)*src+=i;
  if(((i>0)&&(*src>dest-speed))||((i<0)&&(*src<dest+speed)))*src=dest;
}

void AnimCicleMove(int*src,int from, int to, int speed, int*st)
{
  if(from<to)
  {
    if(*src<=from)*st=1;
    else if(*src>=to)*st=0;
  }
  else if(from>to)
  {
    if(*src<=to)*st=1;
    else if(*src>=from)*st=0;
  }
  if(*st==1)AnimSpeedMove(src,to,speed);
  else AnimSpeedMove(src,from,speed);
}

void DrawGradient(int x, int y, int x2, int y2, int upc, int loc)
{
  int r1=GET_RED(upc),g1=GET_GREEN(upc),b1=GET_BLUE(upc);
  int r2=GET_RED(loc),g2=GET_GREEN(loc),b2=GET_BLUE(loc);
  int a1=GET_ALPHA(upc),a2=GET_ALPHA(loc);
  int i;
  for(i=0;i<x2-x;i++)
  {
    int r,g,b,a;
    r = r1 + (i * (r2-r1) / (x2-x));
    g = g1 + (i * (g2-g1) / (x2-x));
    b = b1 + (i * (b2-b1) / (x2-x));
    a = a1 + (i * (a2-a1) / (x2-x));
    int cl=a*0x1000000 + r * 0x10000 + g *0x100 + b;
    DrawRect(x+i,y,x+i+1,y2,cl,cl);
  }
};

void DrawGradientVert(int x, int y, int x2, int y2, int upc, int loc)
{
  int r1=GET_RED(upc),g1=GET_GREEN(upc),b1=GET_BLUE(upc);
  int r2=GET_RED(loc),g2=GET_GREEN(loc),b2=GET_BLUE(loc);
  int a1=GET_ALPHA(upc),a2=GET_ALPHA(loc);
  int i;
  for(i=0;i<y2-y;i++)
  {
    int r,g,b,a;
    r = r1 + (i * (r2-r1) / (y2-y));
    g = g1 + (i * (g2-g1) / (y2-y));
    b = b1 + (i * (b2-b1) / (y2-y));
    a = a1 + (i * (a2-a1) / (y2-y));
    int cl=a*0x1000000 + r * 0x10000 + g *0x100 + b;
    DrawRect(x, y+i, x2, y+i+1, cl, cl);
  }
}
void DrawSTR(int Font,int text,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int NormalColor)
{
SetFont(Font);
DrawString(text,CEN,XPos,YPos,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,NormalColor,NormalColor);
}

void DrawHighlightSTR(int Font,int text,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int BordColor,int NormalColor)
{
SetFont(Font);
DrawString(text,CEN,XPos+1,YPos,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,BordColor,BordColor);
DrawString(text,CEN,XPos,YPos+1,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,BordColor,BordColor);
DrawString(text,CEN,XPos-1,YPos,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,BordColor,BordColor);
DrawString(text,CEN,XPos,YPos-1,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,BordColor,BordColor);
DrawString(text,CEN,XPos,YPos,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,NormalColor,NormalColor);
}

void DrawShadSTR(int Font,int text,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int BordColor,int NormalColor)
{
SetFont(Font);
DrawString(text,CEN,XPos+1,YPos+1,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,BordColor,BordColor);
DrawString(text,CEN,XPos,YPos,MaxXPos,MaxYPos,GetImageHeight(' ')+1,0,NormalColor,NormalColor);
}

void DrawS(int Font,int text,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int BordColor,int NormalColor, int type)
{
  if(type==0)
    DrawSTR(Font,text,CEN,XPos,YPos,MaxXPos,MaxYPos,NormalColor);
  if(type==2)
    DrawShadSTR(Font,text,CEN,XPos,YPos,MaxXPos,MaxYPos,BordColor,NormalColor);
  if(type==1)
    DrawHighlightSTR(Font,text,CEN,XPos,YPos,MaxXPos,MaxYPos,BordColor,NormalColor);
}
