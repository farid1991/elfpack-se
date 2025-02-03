#include "draw.h"

void DrawPicture(RECT rect,int OffsetX,int OffsetY,wchar_t id)
{
	GC_PutChar(get_DisplayGC(),rect.x1+OffsetX,rect.y1+OffsetY,rect.x2-rect.x1,rect.y2-rect.y1,id);
}

void DrawHighlightString(TEXTID StrID,int Font,int Align,RECT rect,int NormalColor,int BorderColor,int OffsetX,int OffsetY)
{
	SetFont(Font);
	DrawString(StrID,Align,rect.x1-1+OffsetX,rect.y1-1+OffsetY,rect.x2-1+OffsetX,rect.y2-1+OffsetY,60,1,BorderColor,0);
	DrawString(StrID,Align,rect.x1+1+OffsetX,rect.y1+1+OffsetY,rect.x2+1+OffsetX,rect.y2+1+OffsetY,60,1,BorderColor,0);
	DrawString(StrID,Align,rect.x1+1+OffsetX,rect.y1-1+OffsetY,rect.x2+1+OffsetX,rect.y2-1+OffsetY,60,1,BorderColor,0);
	DrawString(StrID,Align,rect.x1-1+OffsetX,rect.y1+1+OffsetY,rect.x2-1+OffsetX,rect.y2+1+OffsetY,60,1,BorderColor,0);
	DrawString(StrID,Align,rect.x1+OffsetX	,rect.y1+OffsetY   ,rect.x2+OffsetX  ,rect.y2+OffsetY,60,1,NormalColor,0);
}

void DrawProgressBar(int type,RECT rect,wchar_t imageID,int BackgroundColor, int ProgressBarColor,int BorderColor,int OffsetX,int OffsetY,int percent)
{
	if(!type) DrawRect(rect.x1-1+OffsetX,rect.y1-1+OffsetY,rect.x2+1+OffsetX,rect.y2+1+OffsetY,BorderColor,BorderColor);
	else
	{
		DrawRect(rect.x1-1+OffsetX,rect.y1+OffsetY,rect.x2+1+OffsetX,rect.y2+OffsetY,BorderColor,BorderColor);
		DrawRect(rect.x1+OffsetX,rect.y1-1+OffsetY,rect.x2+OffsetX,rect.y2+1+OffsetY,BorderColor,BorderColor);
	}
	DrawRect(rect.x1+OffsetX,rect.y1+OffsetY,rect.x2+OffsetX,rect.y2+OffsetY,BackgroundColor,BackgroundColor);
	float Length=(rect.x2-rect.x1)*percent/100.0;
	if(type<2) DrawRect(rect.x1+OffsetX,rect.y1+OffsetY,rect.x1+Length+OffsetX,rect.y2+OffsetY,ProgressBarColor,ProgressBarColor);
	else DrawRect(rect.x1+Length+OffsetX,rect.y1+OffsetY,rect.x2+OffsetX,rect.y2+OffsetY,ProgressBarColor,ProgressBarColor);
	if (imageID!=NOIMAGE)
	{
		int Width=GetImageWidth(imageID);
		int Height=GetImageHeight(imageID);
		GC_PutChar(get_DisplayGC(), rect.x1+Length-Width/2+OffsetX ,rect.y1+(rect.y2-rect.y1)/2-Height/2+OffsetY,0,0,imageID);
	}
}
