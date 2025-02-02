/** EMULib Emulation Library *********************************/
/**                                                         **/
/**                            SB.h                         **/
/**                                                         **/
/** This file contains definitions and declarations for     **/
/** routines in SB.c.                                       **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1996                      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/
#ifndef SB_H
#define SB_H
#ifdef MSDOS

#define SND_MELODIC  0x01      /* Melodic sound (default)    */
#define SND_NOISE    0x02      /* White noise                */
#define SND_PERIODIC 0x04      /* Periodic noise (not imp-d) */

#define SND_CHANNELS 7         /* Number of sound channels   */
#define SND_BUFSIZE  128       /* Buffer size for DMA        */

int InitSound(int Mode);                 /* Initialize sound */
    /* Returns 1 on success, 0 otherwise. Mode=0 to skip     */
    /* initialization (will be silent), Mode=1 to use Adlib, */
    /* Mode=2..7 to use SoundBlaster wave synthesis.         */
void TrashSound(void);                   /* Shut down sound  */
void SetSound(int Channel,int Type);     /* Set sound type   */
void SetFreq(int Channel,int Freq);      /* Set frequency    */
void SetVolume(int Channel,int Volume);  /* Set volume       */
void SetChannels(int Volume,int Toggle);
    /* Set master volume (0..255) and turn channels on/off.  */
    /* Each bit in Toggle corresponds to a channel (1=on).   */
void Sound(int Channel,int Freq,int Volume);
    /* Generate sound of given frequency (Hz) and volume     */
    /* (0..255) via given channel.                           */

#endif /* MSDOS */
#endif /* SB_H */