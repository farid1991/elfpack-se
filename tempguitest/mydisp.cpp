#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "mydisp.h"


char* CFirstDisp::getName()
{
	return "FirstDisp";
}

void CFirstDisp::onKey(int key, int, int repeat, int type)
{

}

void CFirstDisp::onDestroy()
{
  delete paint;
}

int CFirstDisp::onCreate()
{
   paint = new CPaintGC();
   return 1;
}

void CFirstDisp::onRefresh()
{

}

void CFirstDisp::onLayout( void* layoutstruct )
{

}


void CFirstDisp::onDraw(int a, int b, int c)
{
  paint->DrawFRect(clWhite,0,0,176,220);
}

char* CSecondDisp::getName()
{
   return "SecondDisp";
}

void CSecondDisp::onKey(int key, int, int repeat, int type)
{

}

void CSecondDisp::onDestroy()
{
   delete paint;
}

int CSecondDisp::onCreate()
{
  paint = new CPaintGC();
  return 1;
}

void CSecondDisp::onRefresh()
{

}

void CSecondDisp::onLayout( void* layoutstruct )
{
  SetLayerColor(0);
}

void CSecondDisp::onDraw(int a, int b, int c)
{
  paint->DrawFRect(clGreen,0,0,176/2,220/2);
}

char* CThirdDisp::getName()
{
   return "ThirdDisp";
}

void CThirdDisp::onKey(int key, int, int repeat, int type)
{

}

void CThirdDisp::onDestroy()
{
   delete paint;
}

int CThirdDisp::onCreate()
{
  paint = new CPaintGC();
  return 1;
}

void CThirdDisp::onRefresh()
{

}

void CThirdDisp::onLayout( void* layoutstruct )
{
  SetLayerColor(0);
}

void CThirdDisp::onDraw(int a, int b, int c)
{
  paint->DrawFRect(clGreen,176/2,220/2,176,220);
}
