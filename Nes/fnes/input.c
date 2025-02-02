/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

input.c: Implementation of the input abstraction.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/



//#include "audio.h"
//#include "gui.h"

#include "input.h"
#include "ppu.h"
#include "rom.h"
//#include "video.h"

#include "misc.h"
#include "timing.h"


int input_enable_zapper = FALSE;

int input_mode = 0;


PACKFILE replay_file = NIL;
PACKFILE replay_file_chunk = NIL;

int buttons [4][8];

int last_write = 0;

int current_read_p1 = 0;
int current_read_p2 = 0;

int input_zapper_x_offset = 0;
int input_zapper_y_offset = 0;
int input_zapper_trigger = 0;
int input_zapper_on_screen = FALSE;

int zapper_mask = 0;
#if 0
void input_update_zapper_offsets (void)
{
    int mouse_needs_range_fixup = FALSE;

    input_zapper_x_offset = mouse_x;
    input_zapper_y_offset = mouse_y;

    input_zapper_trigger = (mouse_b & 0x01);

    /* Perform bounds checking */
    if (input_zapper_x_offset >= 256)
    {
        mouse_needs_range_fixup = TRUE;
        input_zapper_x_offset = 255;
    }

    if (input_zapper_y_offset >= 240)
    {
        mouse_needs_range_fixup = TRUE;
        input_zapper_y_offset = 239;
    }

    if (mouse_needs_range_fixup)
    {
        position_mouse (input_zapper_x_offset, input_zapper_y_offset);
    }


    if ((input_zapper_x_offset < 256) && (input_zapper_y_offset < 240))
    {
        input_zapper_on_screen = TRUE;
    }


    if (! input_zapper_on_screen)
    {
        input_update_zapper ();
    }
}


void input_update_zapper (void)
{
    int pixel;

    zapper_mask = 0x08;

    if (input_zapper_trigger)
    {
        /* Left button. */

        zapper_mask |= 0x10;
    }


    if (input_zapper_on_screen)
    {
        pixel = (_getpixel (video_buffer, input_zapper_x_offset, input_zapper_y_offset) - 1);


        if ((pixel == 32) || (pixel == 48))
        {
            zapper_mask &= ~0x08;
        }
    }
}


static INLINE void load_keyboard_layouts (void)
{
    memset (key1_buffer, NIL, sizeof (key1_buffer));
    memset (key2_buffer, NIL, sizeof (key2_buffer));


    sprintf (key1_buffer, "%s", get_config_string ("input", "key1_scancodes", key1_defaults));
    sprintf (key2_buffer, "%s", get_config_string ("input", "key2_scancodes", key2_defaults));


    if (sscanf (key1_buffer, "%d%d%d%d%d%d%d%d",
        &key1_scancodes [0], &key1_scancodes [1], &key1_scancodes [2],
        &key1_scancodes [3], &key1_scancodes [4], &key1_scancodes [5],
        &key1_scancodes [6], &key1_scancodes [7]) < 8)
    {
        sscanf (key1_defaults, "%d%d%d%d%d%d%d%d",
        &key1_scancodes [0], &key1_scancodes [1], &key1_scancodes [2],
        &key1_scancodes [3], &key1_scancodes [4], &key1_scancodes [5],
        &key1_scancodes [6], &key1_scancodes [7]);
    }


    if (sscanf (key2_buffer, "%d%d%d%d%d%d%d%d",
        &key2_scancodes [0], &key2_scancodes [1], &key2_scancodes [2],
        &key2_scancodes [3], &key2_scancodes [4], &key2_scancodes [5],
        &key2_scancodes [6], &key2_scancodes [7]) < 8)
    {
        sscanf (key2_defaults, "%d%d%d%d%d%d%d%d",
        &key2_scancodes [0], &key2_scancodes [1], &key2_scancodes [2],
        &key2_scancodes [3], &key2_scancodes [4], &key2_scancodes [5],
        &key2_scancodes [6], &key2_scancodes [7]);
    }
}


static INLINE void load_joystick_layouts (void)
{
    memset (joy1_buffer, NIL, sizeof (joy1_buffer));

    memset (joy2_buffer, NIL, sizeof (joy2_buffer));


    sprintf (joy1_buffer, "%s", get_config_string ("input", "joy1_buttons", joy1_defaults));

    sprintf (joy2_buffer, "%s", get_config_string ("input", "joy2_buttons", joy2_defaults));


    if (sscanf (joy1_buffer, "%d%d%d%d", &joy1_buttons [0],
        &joy1_buttons [1], &joy1_buttons [2], &joy1_buttons [3]) < 4)
    {
        sscanf (joy1_defaults, "%d%d%d%d", &joy1_buttons [0],
        &joy1_buttons [1], &joy1_buttons [2], &joy1_buttons [3]);
    }


    if (sscanf (joy2_buffer, "%d%d%d%d", &joy2_buttons [0],
        &joy2_buttons [1], &joy2_buttons [2], &joy2_buttons [3]) < 4)
    {
        sscanf (joy2_defaults, "%d%d%d%d", &joy2_buttons [0],
        &joy2_buttons [1], &joy2_buttons [2], &joy2_buttons [3]);
    }
}
#endif

int input_autosave_interval = 0;
int input_autosave_triggered = FALSE;


int input_init (void)
{
  return (0);
}


void input_exit (void)
{

}


void input_reset (void)
{
    int index, player;

    for (player = 0; player < 4; player ++)
    {
        for (index = 0; index < 8; index ++)
        {
            buttons [player] [index] = 0;
        }
    }

    last_write = 0;

    current_read_p1 = 0;
    current_read_p2 = 0;
}


UINT8 input_read (UINT16 address)
{
   int index;

   if (!input_enable_zapper)
   {
      /* Zapper disabled; clear mask. */
      zapper_mask = 0;
   }

   switch (address)
   {
      case 0x4016:
      {
         /* 1st and 3rd players. */

         if (current_read_p1 == 19)
         {
            /* Signature. */

            /* Increment read counter. */
            current_read_p1++;

            /* Return ?? */
            return (0x01);
         }
         else if ((current_read_p1 > 7) &&
                  (current_read_p1 < 16))
         {
            /* Player 3 button status. */

             /* Get button index. */
             index = (current_read_p1 - 8);

             /* Increment read counter. */
             current_read_p1++;

             /* Return button status. */
             return (buttons[INPUT_PLAYER_3][index] | 0x40);
         }
         else if ((current_read_p1 > 15) &&
                  (current_read_p1 < 23))
         {
            /* Ignored. */

            /* Increment read counter. */
            current_read_p1++;

            /* Return nothing. */
            return (0);
         }
         else if (current_read_p1 == 23)
         {
            /* Strobe flip-flop. */

            /* Clear read counter. */
            current_read_p1 = 0;

            /* Return nothing. */
            return (0);
         }
         else
         {
            /* Player 1 button status. */

            return (buttons[INPUT_PLAYER_1][current_read_p1++] |
               0x40);
         }

         break;
      }

      case 0x4017:
      {
         /* 2nd and 4th players. */

         if (current_read_p2 == 18)
         {
            /* Signature. */

            /* Increment read counter. */
            current_read_p2++;

            /* Return ?? */
            return ((0x01 | zapper_mask));
         }
         else if ((current_read_p2 > 7) &&
                  (current_read_p2 < 16))
         {
            /* Player 4 button status. */

            /* Get button index. */
            index = (current_read_p2 - 8);

            /* Increment read counter. */
            current_read_p2++;

            /* Return button status. */
            return ((buttons[INPUT_PLAYER_4][index] | 0x40) |
               zapper_mask);
         }
         else if ((current_read_p2 > 15) &&
                  (current_read_p2 < 23))
         {
            /* Ignored. */

            /* Increment read counter. */
            current_read_p2++;

            /* Return nothing. */
            return (zapper_mask);
         }
         else if (current_read_p2 == 23)
         {
            /* Strobe flip-flop. */

            /* Clear read counter. */
            current_read_p2 = 0;

            /* Return nothing. */
            return (zapper_mask);
         }
         else
         {
            /* Player 2 button status. */

            return ((buttons[INPUT_PLAYER_2][current_read_p2++] |
               0x40) | zapper_mask);
         }

         break;
      }

      default:
      {
         /* Untrapped port. */
         return (0);
      }

      break;
   }
}

void input_write (UINT16 address, UINT8 value)
{
   switch (address)
   {
      case 0x4016:
      {
         /* 1st and 3rd players. */

         if ((!(value & 0x01)) && (last_write & 0x01))
         {
            /* Full strobe. */

            /* Clear read counters. */
            current_read_p1 = 0;
            current_read_p2 = 0;
         }

         last_write = value;

         break;
      }

      case 0x4017:
      {
         /* 2nd and 4th players. */

         /* Do nothing. */
         break;
      }

      default:
      {
         /* Untrapped port. */
         break;
      }
   }
}



void input_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;

    file_chunk = pack_fopen_chunk (file, FALSE);

    pack_putc (last_write, file_chunk);

    pack_putc (current_read_p1, file_chunk);
    pack_putc (current_read_p2, file_chunk);

    pack_fclose_chunk (file_chunk);
}


void input_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;

    file_chunk = pack_fopen_chunk (file, FALSE);

    last_write = pack_getc (file_chunk);

    current_read_p1 = pack_getc (file_chunk);
    current_read_p2 = pack_getc (file_chunk);

    pack_fclose_chunk (file_chunk);
}
