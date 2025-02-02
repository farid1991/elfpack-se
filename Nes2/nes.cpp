#include "../include/Lib_Clara.h"

#include "nes9x/M6502.h"

#include "gui.h"
#include "book.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

int timing_fps = 0;

int frame_count = 1;

volatile int throttle_counter = 0;
//volatile int frame_interrupt = FALSE;

volatile int needstop = 0;

PROCESS eproc = 0;

u16 fps_interrupt_timer;
u16 throttle_interrupt_timer;
u16 emu_timer;

void emu_proc (u16 timerID, LPARAM lparam);
//OS_PROCESS(emu_proc);

void fps_interrupt (u16 timerID, LPARAM lparam)
{
  //frame_interrupt = TRUE;
  Timer_ReSet( &fps_interrupt_timer, 1000, fps_interrupt, NULL);
}




void suspend_timing (void)
{/*
  Timer_Kill(&fps_interrupt_timer);
  //Timer_Kill(&throttle_interrupt_timer);

  Timer_Kill(&emu_timer);

  redraw_flag = TRUE;

  actual_fps_count = 0;
  virtual_fps_count = 0;

  frame_count = 1;

  throttle_counter = 0;

  frame_interrupt = FALSE;*/
}


void resume_timing (void)
{/*
  int speed;

  speed = ((machine_type == MACHINE_TYPE_NTSC) ? 60 : 50);

  if (timing_half_speed)
    speed /= 2;

  fps_interrupt_timer = Timer_Set(1000, fps_interrupt, NULL);
  //throttle_interrupt_timer = Timer_Set(1000/speed, throttle_interrupt, NULL);*/
}


extern int InitFCE(wchar_t *fdir, wchar_t *fname);
extern void TrashFCE(void);
extern void ResetFCE(M6502 *M);
M6502 M;

void stop_emu()
{
  //kill_proc(eproc);
  TrashFCE();

  return;
}

int start_emu()
{
  if( InitFCE(MyBook->fdir, MyBook->fname) < 0 ) return 0;
  ResetFCE(&M);

  //resume_timing();

  emu_timer = Timer_Set(1, emu_proc, NULL);

  //eproc = create_process(OS_PRI_PROC,"NESEmu_proc",emu_proc,32768,31,0,0,0,0,0);
  //start(eproc);

  return 1;
}

void emu_proc(u16 timerID, LPARAM lparam)
//OS_PROCESS(emu_proc)
{
  Run6502(&M);

  //InvalidateRect(GUIObj_GetDISPObj(MyGui), 0);

  if(!needstop)
    Timer_ReSet(&emu_timer, 0, emu_proc, NULL);
  else
  {
    Timer_Kill(&emu_timer);
    stop_emu();
    FreeBook(MyBook);
  }
}
