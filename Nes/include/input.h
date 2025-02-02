/*

FakeNES - A portable, Open Source NES emulator.

Distributed under Clarified Artistic License.

input.h: Declarations for the input abstraction.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "misc.h"

extern int input_enable_zapper;
extern int input_zapper_x_offset;
extern int input_zapper_y_offset;


extern int input_zapper_trigger;
extern int input_zapper_on_screen;

extern int input_autosave_interval;
extern int input_autosave_triggered;

extern int input_mode;

extern int buttons [4] [8];

extern int input_chat_offset;
int input_init (void);
void input_exit (void);

void input_reset (void);

UINT8 input_read (UINT16);
void input_write (UINT16, UINT8);

inline void input_update_zapper (void){};
inline void input_update_zapper_offsets (void){};

enum
{
    INPUT_DEVICE_BUTTON_A = 0,
    INPUT_DEVICE_BUTTON_B = 1,

    INPUT_DEVICE_BUTTON_SELECT = 2,
    INPUT_DEVICE_BUTTON_START = 3,

    INPUT_DEVICE_BUTTON_UP = 4,
    INPUT_DEVICE_BUTTON_DOWN = 5,
    INPUT_DEVICE_BUTTON_LEFT = 6,
    INPUT_DEVICE_BUTTON_RIGHT = 7
};

enum
{
    INPUT_PLAYER_1 = 0, INPUT_PLAYER_2 = 1,
    INPUT_PLAYER_3 = 2, INPUT_PLAYER_4 = 3
};

extern PACKFILE replay_file;
extern PACKFILE replay_file_chunk;

void input_save_state (PACKFILE, int);
void input_load_state (PACKFILE, int);


#endif /* ! INPUT_H_INCLUDED */
