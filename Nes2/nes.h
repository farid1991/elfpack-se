#ifndef __NES_H__
#define __NES_H__

extern int timing_fps;
extern int timing_hertz;

extern int timing_half_speed;

extern int redraw_flag;

extern int frame_count;

extern int executed_frames;
extern int rendered_frames;

extern int actual_fps_count;
extern int virtual_fps_count;

extern volatile int throttle_counter;

extern volatile int frame_interrupt;

extern volatile int active;

extern volatile int needstop;

void resume_timing (void);
void suspend_timing (void);

int start_emu();
void stop_emu();

#endif
