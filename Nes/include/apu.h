/*
** Nofrendo (c) 1998-2000 Matthew Conte (matt@conte.com)
**
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of version 2 of the GNU Library General 
** Public License as published by the Free Software Foundation.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
**
**
** nes_apu.h
**
** NES APU emulation header file
** $Id: apu.h,v 1.6 2002/07/17 10:46:40 stainless Exp $
*/

#ifndef _NES_APU_H_
#define _NES_APU_H_

#include "misc.h"

#define boolean UINT8

/* HACK to get Win32 SDL to compile */
#if 0
#if defined(WIN32) && !defined(_MSC_VER) && !defined(boolean)
#define INLINE static inline
#define INT8 char
#define INT16 short
#define INT32 int
#define UINT8 unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned int
#define boolean UINT8
#endif /* a whole bunch of crap */
#endif

/*
#ifdef __GNUC__
#define  INLINE      static inline
#elif defined(WIN32)
#define  INLINE      static __inline
#else
#define  INLINE      static
#endif
*/

/* define this for realtime generated noise */
#define  REALTIME_NOISE

/* define this for enabling sound code by T.Yano */
#define  APU_YANO

#define  APU_WRA0       0x4000
#define  APU_WRA1       0x4001
#define  APU_WRA2       0x4002
#define  APU_WRA3       0x4003
#define  APU_WRB0       0x4004
#define  APU_WRB1       0x4005
#define  APU_WRB2       0x4006
#define  APU_WRB3       0x4007
#define  APU_WRC0       0x4008
#define  APU_WRC2       0x400A
#define  APU_WRC3       0x400B
#define  APU_WRD0       0x400C
#define  APU_WRD2       0x400E
#define  APU_WRD3       0x400F
#define  APU_WRE0       0x4010
#define  APU_WRE1       0x4011
#define  APU_WRE2       0x4012
#define  APU_WRE3       0x4013

#define  APU_SMASK      0x4015

/* length of generated noise */
#define  APU_NOISE_32K  0x7FFF
#define  APU_NOISE_93   93

//#define  APU_BASEFREQ   1789772.7272727272727272
#define  APU_BASEFREQ   1789772.5

/* to/from 16.16 fixed point */
#define  APU_TO_FIXED(x)    ((x) << 16)
#define  APU_FROM_FIXED(x)  ((x) >> 16)

// ----------------------------------------------------------------------------
// APU Sound struct

/* channel structures */
/* As much data as possible is precalculated,
** to keep the sample processing as lean as possible
*/
 
typedef struct rectangle_s
{
   UINT8 regs[4];

   boolean enabled;
   
   INT32 phaseacc;
   INT32 freq;
   INT32 output_vol;
   boolean fixed_envelope;
   boolean holdnote;
   UINT8 volume;

   INT32 sweep_phase;
   INT32 sweep_delay;
   boolean sweep_on;
   UINT8 sweep_shifts;
   UINT8 sweep_length;
   boolean sweep_inc;

   /* this may not be necessary in the future */
   INT32 freq_limit;

   /* rectangle 0 uses a complement addition for sweep
   ** increases, while rectangle 1 uses subtraction
   */
   boolean sweep_complement;

   INT32 env_phase;
   INT32 env_delay;
   UINT8 env_vol;

   int vbl_length;
   UINT8 adder;
   int duty_flip;

   /* for sync read $4105 */
   boolean enabled_cur;
   boolean holdnote_cur;
   int vbl_length_cur;

#ifdef APU_YANO
   boolean smooth_envelope;
   boolean smooth_sweep;
#endif
} rectangle_t;

typedef struct triangle_s
{
   UINT8 regs[3];

   boolean enabled;

   INT32 freq;
   INT32 phaseacc;
   INT32 output_vol;

   UINT8 adder;

   boolean holdnote;
   boolean counter_started;
   /* quasi-hack */
   int write_latency;

   int vbl_length;
   int linear_length;

   /* for sync read $4105 */
   boolean enabled_cur;
   boolean holdnote_cur;
   boolean counter_started_cur;
   int vbl_length_cur;

#ifdef APU_YANO
   /* less compatibility, clearer sound if enabled */
   boolean ideal_triangle;
#endif
} triangle_t;

typedef struct noise_s
{
   UINT8 regs[3];

   boolean enabled;

   INT32 freq;
   INT32 phaseacc;
   INT32 output_vol;

   INT32 env_phase;
   INT32 env_delay;
   UINT8 env_vol;
   boolean fixed_envelope;
   boolean holdnote;

   UINT8 volume;

   int vbl_length;

#ifdef REALTIME_NOISE
   UINT8 xor_tap;
#else
   boolean short_sample;
   int cur_pos;
#endif /* REALTIME_NOISE */

   /* for sync read $4105 */
   boolean enabled_cur;
   boolean holdnote_cur;
   int vbl_length_cur;
} noise_t;

typedef struct dmc_s
{
   UINT8 regs[4];

   /* bodge for timestamp queue */
   boolean enabled;
   
   INT32 freq;
   INT32 phaseacc;
   INT32 output_vol;

   UINT32 address;
   UINT32 cached_addr;
   int dma_length;
   int cached_dmalength;
   UINT8 cur_byte;

   boolean looping;
   boolean irq_gen;
   boolean irq_occurred;

   /* for sync read $4105 and DPCM IRQ */
   INT32 freq_cur;
   INT32 phaseacc_cur;
   int dma_length_cur;
   int cached_dmalength_cur;
   boolean enabled_cur;
   boolean looping_cur;
   boolean irq_gen_cur;
   boolean irq_occurred_cur;
} dmc_t;

typedef struct apusound_s
{
   rectangle_t rectangle[2];
   triangle_t triangle;
   noise_t noise;
   dmc_t dmc;
} APUSOUND;

// ----------------------------------------------------------------------------
// VRC6 Sound struct

typedef struct {
    UINT32 cps;
    INT32 cycles;
    UINT32 spd;
    UINT8 regs[3];
    UINT8 update;
    UINT8 adr;
    UINT8 mute;
} VRC6_SQUARE;

typedef struct {
    UINT32 cps;
    INT32 cycles;
    UINT32 spd;
    UINT32 output;
    UINT8 regs[3];
    UINT8 update;
    UINT8 adr;
    UINT8 mute;
} VRC6_SAW;

typedef struct {
	VRC6_SQUARE square[2];
	VRC6_SAW saw;
    UINT32 mastervolume;
} VRC6SOUND;

// ----------------------------------------------------------------------------
// APU Sound struct

typedef struct {
    UINT32  pg_phase;
    UINT32  pg_spd;
    INT32   vib_cycles;
    UINT32  input;
    UINT32  eg_phase;
    UINT32  eg_sl;
    UINT32  eg_arr;
    UINT32  eg_drr;
    UINT32  eg_rrr;
    UINT8   pg_vib;
    UINT32  *sintblp;
    UINT8   tl;
    UINT8   eg_mode;
    UINT8   eg_type;
    UINT8   su_type;
    UINT8   eg_ar;
    UINT8   eg_dr;
    UINT8   eg_rr;
    UINT8   eg_ks;
    UINT8   eg_am;
} OPLL_OP;

typedef struct {
    UINT32  cps;
    UINT32  spd;
    INT32   cycles;
    UINT32  adr;
    UINT32  adrmask;
    UINT32  *table;
    UINT32  output;
} OPLL_LFO;

typedef struct {
    UINT32 cps;
    INT32 cycles;
    UINT32 fbbuf[2];
    UINT32 output;
	OPLL_OP op[2];
    UINT32 mastervolume;
    UINT8 tone[8];
    UINT8 key;
    UINT8 toneno;
    UINT8 freql;
    UINT8 freqh;
    UINT8 fb;
    UINT8 update;
} OPLL_CH;

typedef struct {
	OPLL_CH ch[6];
	OPLL_LFO lfo[2];
    UINT32 mastervolume;
    UINT8 usertone[8];
    UINT8 adr;
    UINT8 rhythmc;
    UINT8 toneupdate;
} OPLLSOUND;

// ----------------------------------------------------------------------------
// FDS Sound struct

typedef struct {
    UINT32 wave[0x40];
    UINT32 envspd;
    INT32 envphase;
    UINT32 envout;
    UINT32 outlvl;

    UINT32 phase;
    UINT32 spd;
    UINT32 volume;
    INT32 sweep;

    UINT8 enable;
    UINT8 envmode;
    UINT8 xxxxx;
    UINT8 xxxxx2;

    INT32 timer;
    UINT32 last_spd;
} FDS_FMOP;

typedef struct FDSSOUND {
    UINT32 cps;
    INT32 cycles;
    UINT32 mastervolume;
    INT32 output;
    INT32 fade;

	FDS_FMOP op[2];

    UINT32 waveaddr;
    UINT8 mute;
    UINT8 key;
    UINT8 reg[0x10];
    UINT8 reg_cur[0x10];
} FDSSOUND;

// ----------------------------------------------------------------------------
// MMC5 Sound struct

typedef struct {
    UINT32 cps;
    INT32 cycles;
    INT32 sweepphase;
    INT32 envphase;

    UINT32 spd;
    UINT32 envspd;
    UINT32 sweepspd;

    UINT32 length;
    UINT32 freq;
    UINT32 mastervolume;
    UINT32 release;

    UINT8 regs[4];
    UINT8 update;
    UINT8 key;
    UINT8 adr;
    UINT8 envadr;
    UINT8 duty;
    UINT8 mute;
} MMC5_SQUARE;

typedef struct {
    INT32 output;
    UINT8 key;
    UINT8 mute;
} MMC5_DA;

typedef struct {
	MMC5_SQUARE square[2];
	MMC5_DA da;
} MMC5SOUND;

// ----------------------------------------------------------------------------
// N106 Sound struct

typedef struct {
    UINT32 logvol;
    INT32 cycles;
    UINT32 spd;
    UINT32 phase;
    UINT32 tlen;
    UINT8 update;
    UINT8 freql;
    UINT8 freqm;
    UINT8 freqh;
    UINT8 vreg;
    UINT8 tadr;
    UINT8 nazo;
    UINT8 mute;
} N106_WM;

typedef struct {
    UINT32 cps;
    UINT32 mastervolume;

	N106_WM ch[8];

    UINT8 addressauto;
    UINT8 address;
    UINT8 chinuse;

    UINT32 tone[0x100]; /* TONE DATA */
    UINT8 data[0x80];
} N106SOUND;

// ----------------------------------------------------------------------------
// FME7 Sound struct

typedef struct {
    UINT32 cps;
    INT32 cycles;
    UINT32 spd;
    UINT8 regs[3];
    UINT8 update;
    UINT8 adr;
    UINT8 mute;
    UINT8 key;
} PSG_SQUARE;

typedef struct {
    UINT32 cps;
    INT32 cycles;
    UINT32 spd;
    UINT32 noiserng;
    UINT8 regs[1];
    UINT8 update;
    UINT8 noiseout;
} PSG_NOISE;

typedef struct {
    UINT32 cps;
    INT32 cycles;
    UINT32 spd;
    UINT32 envout;
    INT8 *adr;
    UINT8 regs[3];
    UINT8 update;
} PSG_ENVELOPE;

typedef struct {
	PSG_SQUARE square[3];
	PSG_ENVELOPE envelope;
	PSG_NOISE noise;
    UINT32 mastervolume;
    UINT32 adr;
} PSGSOUND;

// ----------------------------------------------------------------------------
// APU Sound struct

enum
{
   APU_FILTER_NONE,
   APU_FILTER_LOWPASS,
   APU_FILTER_WEIGHTED,
   APU_FILTER_DYNAMIC,
   APU_FILTER_HIGHPASS
};

#ifdef APU_YANO
enum
{
   APUMODE_IDEAL_TRIANGLE,
   APUMODE_SMOOTH_ENVELOPE,
   APUMODE_SMOOTH_SWEEP
};
#endif /* APU_YANO */

typedef struct
{
   UINT32 min_range, max_range;
   UINT8 (*read_func)(UINT32 address);
} apu_memread;

typedef struct
{
   UINT32 min_range, max_range;
   void (*write_func)(UINT32 address, UINT8 value);
} apu_memwrite;

/* external sound chip stuff */
typedef struct apuext_s
{
   void  (*init)(void);
   void  (*shutdown)(void);
   void  (*reset)(void);
   INT32 (*process)(void);
   apu_memread *mem_read;
   apu_memwrite *mem_write;
} apuext_t;


/* APU queue structure */
#define  APUQUEUE_SIZE  4096
#define  APUQUEUE_MASK  (APUQUEUE_SIZE - 1)

/* apu ring buffer member */
typedef struct apudata_s
{
   UINT32 timestamp, address;
   UINT8 value;
} apudata_t;

typedef struct apu_s
{
   APUSOUND apus;
   VRC6SOUND vrc6s;
   OPLLSOUND ym2413s;
   FDSSOUND fdssound;
   MMC5SOUND mmc5;
   N106SOUND n106s;
   PSGSOUND psg;

   UINT8 enable_reg;
   UINT8 enable_reg_cur;

   apudata_t queue[APUQUEUE_SIZE];
   int q_head, q_tail;

   // for ExSound
   apudata_t ex_queue[APUQUEUE_SIZE];
   int ex_q_head, ex_q_tail;
   UINT8 ex_chip;

   UINT32 elapsed_cycles;

   void *buffer; /* pointer to output buffer */
   int num_samples;

   boolean mix_enable[6];
   int filter_type;

   INT32 cycle_rate;

   int sample_rate;
   int sample_bits;
   int refresh_rate;

   void (*process)(void *buffer, int num_samples, int dither);

   /* external sound chip */
   apuext_t *ext;
} apu_t;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Function prototypes */
extern apu_t *apu_getcontext(void);
extern void apu_setcontext(apu_t *src_apu);

extern apu_t *apu_create(int sample_rate, int refresh_rate, int frag_size,
                         int sample_bits);
extern void apu_destroy(apu_t **apu);
extern void apu_setparams(int sample_rate, int refresh_rate, int frag_size,
                          int sample_bits);

extern void apu_process(void *buffer, int num_samples, int dither);
extern void apu_process_stereo(void *buffer, int num_samples, int dither, int style, int flip, int surround);
extern void apu_reset(void);

extern void apu_setext(apu_t *apu, apuext_t *ext);
extern void apu_setfilter(int filter_type);
extern void apu_setchan(int chan, boolean enabled);
#ifdef APU_YANO
extern void apu_setmode(int item, int mode);
#endif /* APU_YANO */

extern UINT8 apu_read(UINT32 address);
extern void apu_write(UINT32 address, UINT8 value);

extern UINT8 ex_read(UINT32 address);
extern void ex_write(UINT32 address, UINT8 value);

extern void apu_write_cur(UINT32 address, UINT8 value);
extern void sync_apu_register();
extern boolean sync_dmc_register(UINT32 cpu_cycles);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _NES_APU_H_ */

/*
** $Log: apu.h,v $
** Revision 1.6  2002/07/17 10:46:40  stainless
** Added random noise dithering for 8-bit audio.
**
** Revision 1.5  2002/05/02 16:48:43  stainless
** Added 'enhanced' & 'accurate' variations of pseudo stereo.
**
** Revision 1.4  2002/04/05 04:39:56  stainless
** Overhauled audio base, new features added.
**
** Revision 2.16  2001/02/18 21:31:00  YANO, takashi
** added $4017:bit7 control
** fixed set chan->enabled
** fixed bug of DPCM last sample in APU_YANO
** trim channel balance again
** fixed APU_BASEFREQ
**
** Revision 2.15  2001/01/04 22:09:10  YANO, takashi
** fixed dmc bug of delta_bit
**
** Revision 2.14  2001/01/03 17:46:00  YANO, takashi
** fixed code for DC balance
** trim noise and dmc volume
**
** Revision 2.13  2000/12/23 02:00:40  YANO, takashi
** added apu_setmode()
**
** Revision 2.12  2000/12/22 23:23:40  YANO, takashi
** trim output volume balance
** re-disabled APU_IDEAL_TRIANGLE
** modify weighted filter
**
** Revision 2.11  2000/12/17 00:08:10  YANO, takashi
** cancel DC offset
** enable APU_IDEAL_TRIANGLE
**
** Revision 2.10  2000/12/12 02:07:46  YANO, takashi
** improve sound purity
**
** Revision 2.09  2000/12/09 11:41:00  TAKEDA, toshiya
** sync DPCM registers
** support DMCP IRQ
**
** Revision 2.08  2000/12/07 00:10:00  TAKEDA, toshiya
** sync DPCM registers
**
** Revision 2.07  2000/11/15 16:32:00  TAKEDA, toshiya
** fixed memory reak of ExtraSound
**
** Revision 2.06  2000/11/02 21:40:00  TAKEDA, toshiya
** fixed read $4015 (triangle.write_latency)
**
** Revision 2.05  2000/11/01 21:44:00  TAKEDA, toshiya
** fixed read $4015
**
** Revision 2.04  2000/10/26 00:05:00  TAKEDA, toshiya
** changed VRC6 volume
** changed chip number
**
** Revision 2.03  2000/10/23 16:06:00  TAKEDA, toshiya
** added ExtraSound Support of MMC5
** sync All ExtraSound
**
** Revision 2.02  2000/10/23 00:07:00  TAKEDA, toshiya
** fixed VRC6 write reg
**
** Revision 2.01  2000/10/22 21:12:00  TAKEDA, toshiya
** added ExtraSound Support of FME7
**
** Revision 2.00  2000/10/22 00:12:15  TAKEDA, toshiya
** added ExtraSound Support of N106, FDS, VRC6, VRC7
**
** ---------------------------------------------------
**
** Revision 1.21  2000/08/11 02:27:21  matt
** general cleanups, plus apu_setparams routine
**
** Revision 1.20  2000/07/30 04:32:59  matt
** no more apu_getcyclerate hack
**
** Revision 1.19  2000/07/27 02:49:50  matt
** eccentricity in sweeping hardware now emulated correctly
**
** Revision 1.18  2000/07/25 02:25:15  matt
** safer apu_destroy
**
** Revision 1.17  2000/07/23 15:10:54  matt
** hacks for win32
**
** Revision 1.16  2000/07/23 00:48:15  neil
** Win32 SDL
**
** Revision 1.15  2000/07/17 01:52:31  matt
** made sure last line of all source files is a newline
**
** Revision 1.14  2000/07/11 02:39:26  matt
** added setcontext() routine
**
** Revision 1.13  2000/07/10 05:29:34  matt
** moved joypad/oam dma from apu to ppu
**
** Revision 1.12  2000/07/04 04:54:48  matt
** minor changes that helped with MAME
**
** Revision 1.11  2000/07/03 02:18:53  matt
** much better external module exporting
**
** Revision 1.10  2000/06/26 05:00:37  matt
** cleanups
**
** Revision 1.9  2000/06/23 03:29:28  matt
** cleaned up external sound inteface
**
** Revision 1.8  2000/06/20 04:06:16  matt
** migrated external sound definition to apu module
**
** Revision 1.7  2000/06/20 00:07:35  matt
** added convenience members to apu_t struct
**
** Revision 1.6  2000/06/09 16:49:02  matt
** removed all floating point from sound generation
**
** Revision 1.5  2000/06/09 15:12:28  matt
** initial revision
**
*/

