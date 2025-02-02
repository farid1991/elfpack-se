

/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

audio.c: Implementation of the audio interface.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#include <allegro.h>


#include <stdio.h>


#include "audio.h"

#include "gui.h"


#include "misc.h"


#include "timing.h"


AUDIOSTREAM * audio_stream;


int audio_enable_output = FALSE;


int audio_sample_rate = 0;

int audio_sample_size = 0;


int audio_buffer_length = 0;


int audio_pseudo_stereo = FALSE;


volatile int audio_fps = 0;


int audio_init (void)
{
    int speed;


    audio_enable_output = get_config_int ("audio", "enable_output", TRUE);


    audio_sample_rate = get_config_int ("audio", "sample_rate", 96000);

    audio_sample_size = get_config_int ("audio", "sample_size", 16);


    audio_buffer_length = get_config_int ("audio", "buffer_length", 6);


    audio_pseudo_stereo = get_config_int ("audio", "pseudo_stereo", AUDIO_ENHANCED_STEREO_MIXING);


    if (audio_enable_output)
    {
        set_volume_per_voice (0);

    
        if (install_sound (DIGI_AUTODETECT, MIDI_NONE, NIL) != 0)
        {
            return (1);
        }


        if (digi_driver -> id == DIGI_NONE)
        {
            return (1);
        }


        if (! gui_is_active)
        {
            printf ("Audio initialized: %s (%d kHz).\n\n", digi_driver -> name, (audio_sample_rate / 1000));
        }
    }


    if (audio_enable_output)
    {
        speed = ((machine_type == MACHINE_TYPE_NTSC) ? 60 : 50);


        if (timing_half_speed)
        {
            speed /= 2;
        }


        audio_stream = play_audio_stream (((audio_sample_rate / speed) * audio_buffer_length),
            audio_sample_size, audio_pseudo_stereo, audio_sample_rate, 255, 128);
    

        if (! audio_stream)
        {
            return (2);
        }
    }


    return (0);
}


void audio_exit (void)
{
    if (audio_enable_output)
    {
        remove_sound ();


        if (audio_stream)
        {
            stop_audio_stream (audio_stream);
        }
    }


    set_config_int ("audio", "enable_output", audio_enable_output);


    set_config_int ("audio", "sample_rate", audio_sample_rate);

    set_config_int ("audio", "sample_size", audio_sample_size);


    set_config_int ("audio", "buffer_length", audio_buffer_length);


    set_config_int ("audio", "pseudo_stereo", audio_pseudo_stereo);
}


void audio_update (void)
{
    audio_buffer = get_audio_stream_buffer (audio_stream);

    if (audio_buffer)
    {
        voice_stop (audio_stream -> voice);
    }
}


void audio_suspend (void)
{
    if (audio_enable_output)
    {
        voice_stop (audio_stream -> voice);
    }
}


void audio_resume (void)
{
    if (audio_enable_output)
    {
        voice_start (audio_stream -> voice);
    }
}


void audio_start (void)
{
    audio_buffer = get_audio_stream_buffer (audio_stream);
}


void audio_stop (void)
{
    voice_start (audio_stream -> voice);

    free_audio_stream_buffer (audio_stream);


    audio_fps += audio_buffer_length;
}
