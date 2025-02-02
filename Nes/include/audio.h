

/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

audio.h: Declarations for the audio interface.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#ifndef AUDIO_H_INCLUDED

#define AUDIO_H_INCLUDED


enum
{
    AUDIO_CLASSIC_STEREO_MIXING = 1,

    AUDIO_ENHANCED_STEREO_MIXING,

    AUDIO_ACCURATE_STEREO_MIXING
};


int audio_enable_output;


int audio_sample_rate;

int audio_sample_size;


int audio_buffer_length;


int audio_pseudo_stereo;


volatile int audio_fps;


int audio_init (void);

void audio_exit (void);


void audio_start (void);

void audio_stop (void);


void audio_update (void);


void audio_suspend (void);

void audio_resume (void);


void * audio_buffer;


#endif /* ! AUDIO_H_INCLUDED */
