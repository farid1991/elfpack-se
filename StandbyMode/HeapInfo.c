#include "..\include\Lib_Clara.h"
#include "HeapInfo.h"
#include "config_data.h"
#include "main.h"
#include "MediaInfo.h"

u16 timer_heap = 0;
int heap_show = 0;
TEXTID heapID = EMPTY_TEXTID;

void UpdateHeapInfo(void)
{
     wchar_t uHeap[10];
     if(cfg_heap_format == 0)
     {
         int intHeap = GetFreeBytesOnHeap();
         snwprintf(uHeap,MAXELEMS(uHeap),L"%d%KB",intHeap/1024);
     }
     else
     {
         float floatHeap = (float)GetFreeBytesOnHeap();
         snwprintf(uHeap,MAXELEMS(uHeap),L"%4.2f%MB",floatHeap/1024.0/1024.0);
     }
     if(heapID != EMPTY_TEXTID) TextID_Destroy(heapID);
     heapID = TextID_Create(uHeap,ENC_UCS2,MAXELEMS(uHeap));
}

void ShowHeapInfo(int y, int location)
{
   if (heap_show)
   {
     if(cfg_heap_align == RIGHT)
        myDrawString(cfg_heap_font, heapID, cfg_heap_align, 1, cfg_heap_y - y, cfg_heap_x, DisplayHeight, cfg_heap_border, cfg_heap_color, cfg_heap_highlight);
     else
        myDrawString(cfg_heap_font, heapID, cfg_heap_align, cfg_heap_x, cfg_heap_y - y, DisplayWidth, DisplayHeight, cfg_heap_border, cfg_heap_color, cfg_heap_highlight);
   }
}

void onTimer_heap (u16 timerID, LPARAM lparam)
{
    UpdateHeapInfo();
    Timer_ReSet(&timer_heap,cfg_refresh_heap,onTimer_heap,0);
}

void UpdateHeapInfoParams(void)
{
    if(cfg_heap_show)
    {
      UpdateHeapInfo();
      Timer_ReSet(&timer_heap,cfg_refresh_heap,onTimer_heap,0);
    }
    else if(timer_heap)
    {
      Timer_Kill(&timer_heap);
      timer_heap = 0;
    }
}

void UpdateHeapShowParams(void)
{
    heap_show = cfg_heap_show && (!(PlayerIsStarted && cfg_heap_not_show_if_player));
}

void FreeHeapInfoParams(void)
{
    if(timer_heap)
    {
      Timer_Kill(&timer_heap);
      timer_heap = 0;
    }
    if(heapID != EMPTY_TEXTID)
    {
      TextID_Destroy(heapID);
      heapID = EMPTY_TEXTID;
    }
}


