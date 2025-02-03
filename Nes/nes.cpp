#include "cpu.h"
#include "input.h"
#include "mmc.h"
#include "ppu.h"
#include "rom.h"
#include "misc.h"
#include "timing.h"

#include "gui.h"
#include "book.h"

#include "nes.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

int timing_fps = 0;
int timing_hertz = 0;

int redraw_flag = TRUE;

int frame_count = 1;

int executed_frames = 0;
int rendered_frames = 0;

int actual_fps_count = 0;
int virtual_fps_count = 0;

volatile int throttle_counter = 0;
volatile int frame_interrupt = FALSE;

volatile int needstop = 0;

PROCESS eproc = 0;

u16 fps_interrupt_timer;
u16 throttle_interrupt_timer;
u16 emu_timer;

void emu_proc (u16 timerID, LPARAM lparam);

void fps_interrupt (u16 timerID, LPARAM lparam)
{
  frame_interrupt = TRUE;
  Timer_ReSet( &fps_interrupt_timer, 1000, fps_interrupt, NULL);
}

void throttle_interrupt (u16 timerID, LPARAM lparam)
{
  throttle_counter ++;
  int speed;

  speed = ((machine_type == MACHINE_TYPE_NTSC) ? 60 : 50);

  if (timing_half_speed)
    speed /= 2;

  Timer_ReSet(&throttle_interrupt_timer, 1000/speed, throttle_interrupt, NULL);
}

INLINE int fix (int value, int base, int limit)
{
    if (value < base)
        value = base;

    if (value > limit)
        value = limit;

    return (value);
}


void suspend_timing (void)
{
  Timer_Kill(&fps_interrupt_timer);
  //Timer_Kill(&throttle_interrupt_timer);

  Timer_Kill(&emu_timer);

  redraw_flag = TRUE;

  actual_fps_count = 0;
  virtual_fps_count = 0;

  frame_count = 1;

  throttle_counter = 0;

  frame_interrupt = FALSE;
}


void resume_timing (void)
{
  int speed;

  speed = ((machine_type == MACHINE_TYPE_NTSC) ? 60 : 50);

  if (timing_half_speed)
    speed /= 2;

  fps_interrupt_timer = Timer_Set(1000, fps_interrupt, NULL);
  //throttle_interrupt_timer = Timer_Set(1000/speed, throttle_interrupt, NULL);
}


int machine_init (void)
{
  if (rom_is_loaded)
  {
    cpu_memmap_init ();

    if (mmc_init () != 0)
    {
      free_rom (&global_rom);
      return (1);
    }

    if (cpu_init () != 0)
    {
      free_rom (&global_rom);
      return (1);
    }

    if (ppu_init () != 0)
    {
      free_rom (&global_rom);
      return (1);
    }

    input_reset ();
    cpu_reset ();
  }

  return (0);
}


void machine_reset (void)
{
  //audio_exit ();

  //audio_init ();

  mmc_reset ();
  //papu_reset ();

  ppu_reset ();
  input_reset ();
  cpu_reset ();
}


void stop_emu()
{
  //needstop = 1;
  //while( needstop != 2 )
  //  delay(1);

  suspend_timing();

  //papu_exit ();
  //audio_exit ();
  //video_exit ();
  ppu_exit ();
  //netplay_exit ();

  if (rom_is_loaded)
    cpu_exit ();

  input_exit ();

  if (rom_is_loaded)
    free_rom (&global_rom);

  return;
}

int start_emu()
{
  if (input_init () != 0)
  {
    return 0;
  }

  input_reset ();
  wchar_t buf[0x100];

  wstrcpy(buf, MyBook->fdir);
  wstrcat(buf, L"/");
  wstrcat(buf, MyBook->fname);

  if (load_rom(buf, &global_rom) != 0)
  {
    return 0;
  }
  rom_is_loaded = TRUE;

  if ( machine_init() != 0 )
  {
    return 0;
  }

  resume_timing();

  emu_timer = Timer_Set(1, emu_proc, NULL);

  return 1;
}

void emu_proc (u16 timerID, LPARAM lparam)
{
  if (frame_interrupt)
  {
    timing_fps = actual_fps_count;
    //timing_hertz = virtual_fps_count;

    //timing_audio_fps = audio_fps;

    actual_fps_count = 0;
    //virtual_fps_count = 0;

    //audio_fps = 0;

    frame_interrupt = FALSE;
  }

  executed_frames++;

  /* decrement frame skip counter */
  /* when # of frame periods between start of first skipped frame  */
  /*  before last drawn frame and end of last drawn frame have     */
  /*  passed, draw another */
  if (-- frame_count > 0)
    redraw_flag = FALSE;
  else
  {
    redraw_flag = TRUE;

    //rendered_frames ++;
    actual_fps_count ++;

    frame_count = frame_skip_max;
  }

  //virtual_fps_count++;

  switch (machine_type)
  {
  case MACHINE_TYPE_PAL:
    ppu_frame_last_line = (TOTAL_LINES_PAL - 1);

    break;

  case MACHINE_TYPE_NTSC:
  default:
    ppu_frame_last_line = (TOTAL_LINES_NTSC - 1);

    break;
  }

  if (redraw_flag)
  {
    /* Perform a full render. */

    ppu_start_render ();

    for (ppu_scanline = 0; ppu_scanline <= ppu_frame_last_line; ppu_scanline ++)
    {
      cpu_start_new_scanline ();

      if ((ppu_scanline >= FIRST_DISPLAYED_LINE) && (ppu_scanline <= LAST_DISPLAYED_LINE))
      {
        ppu_start_line ();
        ppu_render_line (ppu_scanline);
        cpu_execute (RENDER_CLOCKS);
      }
      else if (ppu_scanline == FIRST_VBLANK_LINE)
      {
        ppu_end_render ();
        cpu_execute (RENDER_CLOCKS);
      }
      else if (ppu_scanline == FIRST_VBLANK_LINE + 1)
      {
        ppu_vblank_nmi ();
        cpu_execute (RENDER_CLOCKS);
      }
      else if (ppu_scanline == ppu_frame_last_line)
      {
        ppu_clear ();
        cpu_execute (RENDER_CLOCKS);
      }
      else
        cpu_execute (RENDER_CLOCKS);

      if (mmc_hblank_start)
      {
        int type = mmc_hblank_start (ppu_scanline);
        if (type != CPU_INTERRUPT_NONE)
          cpu_interrupt (type);
      }

      if ((ppu_scanline >= FIRST_DISPLAYED_LINE) && (ppu_scanline <= LAST_DISPLAYED_LINE))
      {
        cpu_execute (HBLANK_CLOCKS_BEFORE_VRAM_ADDRESS_FIXUP);
        ppu_end_line ();
        cpu_execute ((HBLANK_CLOCKS - HBLANK_CLOCKS_BEFORE_VRAM_ADDRESS_FIXUP));
      }
      else
        cpu_execute (HBLANK_CLOCKS);

      if (mmc_scanline_end)
      {
        int type = mmc_scanline_end (ppu_scanline);
        if (type != CPU_INTERRUPT_NONE)
          cpu_interrupt (type);
      }
    }

    //papu_process ();
  }
  else
  {
    /* Perform a partial render. */

    ppu_start_frame ();

    for (ppu_scanline = 0; ppu_scanline <= ppu_frame_last_line; ppu_scanline ++)
    {
      cpu_start_new_scanline ();

      if ((ppu_scanline >= FIRST_DISPLAYED_LINE) && (ppu_scanline <= LAST_DISPLAYED_LINE))
      {
        ppu_start_line ();
        ppu_stub_render_line (ppu_scanline);
        cpu_execute (RENDER_CLOCKS);
      }
      else if (ppu_scanline == FIRST_VBLANK_LINE)
      {
        ppu_vblank ();
        cpu_execute (RENDER_CLOCKS);
      }
      else if (ppu_scanline == FIRST_VBLANK_LINE + 1)
      {
        ppu_vblank_nmi ();
        cpu_execute (RENDER_CLOCKS);
      }
      else if (ppu_scanline == ppu_frame_last_line)
      {
        ppu_clear ();
        cpu_execute (RENDER_CLOCKS);
      }
      else
        cpu_execute (RENDER_CLOCKS);

      if (mmc_hblank_start)
      {
        int type = mmc_hblank_start (ppu_scanline);
        if (type != CPU_INTERRUPT_NONE)
          cpu_interrupt (type);
      }

      if ((ppu_scanline >= FIRST_DISPLAYED_LINE) && (ppu_scanline <= LAST_DISPLAYED_LINE))
      {
        cpu_execute (HBLANK_CLOCKS_BEFORE_VRAM_ADDRESS_FIXUP);
        ppu_end_line ();
        cpu_execute ((HBLANK_CLOCKS - HBLANK_CLOCKS_BEFORE_VRAM_ADDRESS_FIXUP));
      }
      else
        cpu_execute (HBLANK_CLOCKS);

      if (mmc_scanline_end)
      {
        int type = mmc_scanline_end (ppu_scanline);
        if (type != CPU_INTERRUPT_NONE)
          cpu_interrupt (type);
      }
    }


    //papu_process ();
  }

  if(!needstop)
    Timer_ReSet(&emu_timer, 0, emu_proc, NULL);
  else
  {
    stop_emu();
    FreeBook(MyBook);
  }
}
