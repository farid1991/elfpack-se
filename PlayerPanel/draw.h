#ifndef _MYPAINTGC_H_
#define _MYPAINTGC_H_

#include "..\include\Lib_Clara.h"

  void DrawPicture(RECT rect,int OffsetX,int OffsetY,wchar_t id);
  void DrawProgressBar(int type,RECT rect,wchar_t imageID,int BackgroundColor, int ProgressBarColor,int BorderColor,int OffsetX,int OffsetY,int percent);
  void DrawHighlightString(TEXTID StrID,int Font,int Align,RECT rect,int NormalColor,int BorderColor,int OffsetX,int OffsetY);


#endif
