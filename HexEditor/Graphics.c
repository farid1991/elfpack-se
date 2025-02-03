#include "Graphics.h"

int FillColor; //, ForegroundColor; //, BackgroundColor;

int DrawLine(int x1, int y1, int x2, int y2)
{
    GC_DrawLine(get_DisplayGC(), x1, y1, x2, y2);
    return RESULT_OK;
}

int FillRect(int x, int y, int w, int h) // прямоугольник с заливкой
{
    GC_DrawFRect(get_DisplayGC(), FillColor, x, y, w, h);
    //DrawRect(x, y, x+w, y+h, FillColor, FillColor);
    return RESULT_OK;
}

int SetFillColor(int color) // заливка для прямоугольника	
{
    FillColor = color;
    return RESULT_OK;
}

int SetForegroundColor(int color) // устанавливает цвет для линии, текста...
{
    //ForegroundColor = color;
    GC_SetPenColor(get_DisplayGC(), color);
    return RESULT_OK;
}

/*
int SetBackgroundColor(int color) // устанавливает цвет фона 
{
    BackgroundColor = color;
    return RESULT_OK;
}
*/

int DrawT(TEXTID id, int x, int y, int align)
{
    int ForegroundColor = GC_GetPenColor(get_DisplayGC());
    DrawString(id, align, x, y, Display_GetWidth(0), Display_GetHeight(0), 0, 0, ForegroundColor, ForegroundColor);
    TextID_Destroy(id);
    return(RESULT_OK);
}

int DrawText(wchar_t *wstr, int x, int y, int align)
{
  TEXTID id = TextID_Create(wstr, ENC_UCS2, TEXTID_ANY_LEN);
  return(DrawT(id, x, y, align));
}

int DrawTextS(char *str, int x, int y, int align)
{
  TEXTID id = TextID_Create(str, ENC_LAT1, strlen(str));
  return(DrawT(id ,x, y, align));
}



