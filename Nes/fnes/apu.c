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
** nes_apu.c
**
** NES APU emulation
** $Id: apu.c,v 1.12 2003/03/04 05:32:52 stainless Exp $
*/

/*
** Changes for nester:
** changes are marked with DCR
**
** removed bad #includes
** nullified ASSERT and log_printf
** added custom log_printf()
** #included stlib.h for malloc()
**
** - Darren Ranalli
*/

/*
** Changes for nester:
** support ExSounds
**
** - TAKEDA, toshiya
*/

#include <allegro.h>
#include <string.h>
#include <stdlib.h> //DCR
#include <time.h>
#include "apu.h"
#include "cpu.h"
//DCR#include "log.h"
#include "misc.h"

/* included for nes_irq() and nes_clearframeirq() */
#ifndef NSF_PLAYER
//DCR#include "nes.h"
#endif /* !NSF_PLAYER */

// DCR
// #define ASSERT(CONDITION)
#define log_printf(STR)

#ifndef  APU_YANO
#define  APU_OVERSAMPLE
#define  APU_VOLUME_DECAY(x)  ((x) -= ((x) >> 7))
#endif /* !APU_YANO */


/* pointer to active APU */
static apu_t *apu;

/* look up table madness */
static INT32 decay_lut[16];
static int vbl_lut[32];
static int trilength_lut[128];

/* noise lookups for both modes */
#ifndef REALTIME_NOISE
static INT8 noise_long_lut[APU_NOISE_32K];
static INT8 noise_short_lut[APU_NOISE_93];
#endif /* !REALTIME_NOISE */


/* vblank length table used for rectangles, triangle, noise */
static AL_CONST UINT8 vbl_length[32] =
{
    5, 127,
   10,   1,
   19,   2,
   40,   3,
   80,   4,
   30,   5,
    7,   6,
   13,   7,
    6,   8,
   12,   9,
   24,  10,
   48,  11,
   96,  12,
   36,  13,
    8,  14,
   16,  15
};

/* frequency limit of rectangle channels */
static AL_CONST int freq_limit[8] =
{
   0x3FF, 0x555, 0x666, 0x71C, 0x787, 0x7C1, 0x7E0, 0x7F0
};

/* noise frequency lookup table */
static AL_CONST int noise_freq[16] =
{
     4,    8,   16,   32,   64,   96,  128,  160,
   202,  254,  380,  508,  762, 1016, 2034, 4068
};

/* DMC transfer freqs */
AL_CONST int dmc_clocks[16] =
{
   428, 380, 340, 320, 286, 254, 226, 214,
   190, 160, 142, 128, 106,  85,  72,  54
};

/* ratios of pos/neg pulse for rectangle waves */
static AL_CONST int duty_lut[4] = { 2, 4, 8, 12 };

// for $4017:bit7 by T.Yano
static int apu_cnt_rate = 5;


void apu_setcontext(apu_t *src_apu)
{
   apu = src_apu;
}

apu_t *apu_getcontext(void)
{
   return apu;
}

/*
** Simple queue routines
*/
#define  APU_QEMPTY()   (apu->q_head == apu->q_tail)
#define  EX_QEMPTY()   (apu->ex_q_head == apu->ex_q_tail)

#include "apu_ex.h"

static void apu_enqueue(apudata_t *d)
{
   ASSERT(apu);
   apu->queue[apu->q_head] = *d;

   apu->q_head = (apu->q_head + 1) & APUQUEUE_MASK;

   if (APU_QEMPTY())
      log_printf("apu: queue overflow\n");      
}

static apudata_t *apu_dequeue(void)
{
   int loc;

   ASSERT(apu);

   if (APU_QEMPTY())
      log_printf("apu: queue empty\n");

   loc = apu->q_tail;
   apu->q_tail = (apu->q_tail + 1) & APUQUEUE_MASK;

   return &apu->queue[loc];
}

static void ex_enqueue(apudata_t *d)
{
   ASSERT(apu);
   apu->ex_queue[apu->ex_q_head] = *d;

   apu->ex_q_head = (apu->ex_q_head + 1) & APUQUEUE_MASK;

   if (EX_QEMPTY())
      log_printf("ex_apu: queue overflow\n");      
}

static apudata_t *ex_dequeue(void)
{
   int loc;

   ASSERT(apu);

   if (EX_QEMPTY())
      log_printf("ex_apu: queue empty\n");

   loc = apu->ex_q_tail;
   apu->ex_q_tail = (apu->ex_q_tail + 1) & APUQUEUE_MASK;

   return &apu->ex_queue[loc];
}

void apu_setchan(int chan, boolean enabled)
{
   ASSERT(apu);
   apu->mix_enable[chan] = enabled;
}

/* emulation of the 15-bit shift register the
** NES uses to generate pseudo-random series
** for the white noise channel
*/
#ifdef REALTIME_NOISE
INLINE INT8 shift_register15(UINT8 xor_tap)
{
   static int sreg = 0x4000;
   int bit0, tap, bit14;

   bit0 = sreg & 1;
   tap = (sreg & xor_tap) ? 1 : 0;
   bit14 = (bit0 ^ tap);
   sreg >>= 1;
   sreg |= (bit14 << 14);
   return (bit0 ^ 1);
}
#else /* !REALTIME_NOISE */
static void shift_register15(INT8 *buf, int count)
{
   static int sreg = 0x4000;
   int bit0, bit1, bit6, bit14;

   if (count == APU_NOISE_93)
   {
      while (count--)
      {
         bit0 = sreg & 1;
         bit6 = (sreg & 0x40) >> 6;
         bit14 = (bit0 ^ bit6);
         sreg >>= 1;
         sreg |= (bit14 << 14);
         *buf++ = bit0 ^ 1;
      }
   }
   else /* 32K noise */
   {
      while (count--)
      {
         bit0 = sreg & 1;
         bit1 = (sreg & 2) >> 1;
         bit14 = (bit0 ^ bit1);
         sreg >>= 1;
         sreg |= (bit14 << 14);
         *buf++ = bit0 ^ 1;
      }
   }
}
#endif /* !REALTIME_NOISE */

/* RECTANGLE WAVE
** ==============
** reg0: 0-3=volume, 4=envelope, 5=hold, 6-7=duty cycle
** reg1: 0-2=sweep shifts, 3=sweep inc/dec, 4-6=sweep length, 7=sweep on
** reg2: 8 bits of freq
** reg3: 0-2=high freq, 7-4=vbl length counter
*/
#define  APU_RECTANGLE_OUTPUT (chan->output_vol)
static INT32 apu_rectangle(rectangle_t *chan)
{
   INT32 output;

#ifdef APU_YANO
   double total;
   double sample_weight;
#else /* !APU_YANO */
#ifdef APU_OVERSAMPLE
   int num_times;
   INT32 total;
#endif /* APU_OVERSAMPLE */

   APU_VOLUME_DECAY(chan->output_vol);
#endif /* !APU_YANO */

   if (FALSE == chan->enabled || chan->vbl_length <= 0)
      return APU_RECTANGLE_OUTPUT;

   /* vbl length counter */
   if (FALSE == chan->holdnote)
      chan->vbl_length -= apu_cnt_rate;

   /* envelope decay at a rate of (env_delay + 1) / 240 secs */
   chan->env_phase -= 4 * apu_cnt_rate; /* 240/60 */
   while (chan->env_phase < 0)
   {
      chan->env_phase += chan->env_delay;

      if (chan->holdnote)
         chan->env_vol = (chan->env_vol + 1) & 0x0F;
      else if (chan->env_vol < 0x0F)
         chan->env_vol++;
   }

   /* TODO: using a table of max frequencies is not technically
   ** clean, but it is fast and (or should be) accurate 
   */
   if (chan->freq < 8 || (FALSE == chan->sweep_inc && chan->freq > chan->freq_limit))
      return APU_RECTANGLE_OUTPUT;

   /* frequency sweeping at a rate of (sweep_delay + 1) / 120 secs */
   if (chan->sweep_on && chan->sweep_shifts)
   {
      chan->sweep_phase -= 2 * apu_cnt_rate; /* 120/60 */
      while (chan->sweep_phase < 0)
      {
         chan->sweep_phase += chan->sweep_delay;

         if (chan->sweep_inc) /* ramp up */
         {
            if (TRUE == chan->sweep_complement)
               chan->freq += ~(chan->freq >> chan->sweep_shifts);
            else
               chan->freq -= (chan->freq >> chan->sweep_shifts);
         }
         else /* ramp down */
         {
            chan->freq += (chan->freq >> chan->sweep_shifts);
         }
      }
   }

#ifdef APU_YANO
   if (chan->fixed_envelope)
      output = chan->volume << 8; /* fixed volume */
   else
      output = (chan->env_vol ^ 0x0F) << 8;

   sample_weight = chan->phaseacc;
   if ( sample_weight > apu->cycle_rate) {
      sample_weight = apu->cycle_rate;
   }
   total = (chan->adder < chan->duty_flip)?sample_weight:-sample_weight;

   chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
   while ( chan->phaseacc < 0) {
      chan->phaseacc += APU_TO_FIXED(chan->freq + 1);
      chan->adder = (chan->adder + 1) & 0x0F;

      sample_weight = APU_TO_FIXED(chan->freq + 1);
      if (chan->phaseacc > 0) {
         sample_weight -= chan->phaseacc;
      }
      total += (chan->adder < chan->duty_flip)?sample_weight:-sample_weight;
   }
   chan->output_vol = (int)floor( output*total/apu->cycle_rate + 0.5);
#else /* !APU_YANO */
   chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
   if (chan->phaseacc >= 0)
      return APU_RECTANGLE_OUTPUT;

#ifdef APU_OVERSAMPLE
   num_times = total = 0;

   if (chan->fixed_envelope)
      output = chan->volume << 8; /* fixed volume */
   else
      output = (chan->env_vol ^ 0x0F) << 8;
#endif /* APU_OVERSAMPLE */

   while (chan->phaseacc < 0)
   {
      chan->phaseacc += APU_TO_FIXED(chan->freq + 1);
      chan->adder = (chan->adder + 1) & 0x0F;

#ifdef APU_OVERSAMPLE
      if (chan->adder < chan->duty_flip)
         total += output;
      else
         total -= output;

      num_times++;
#endif /* APU_OVERSAMPLE */
   }

#ifdef APU_OVERSAMPLE
   chan->output_vol = total / num_times;
#else /* !APU_OVERSAMPLE */
   if (chan->fixed_envelope)
      output = chan->volume << 8; /* fixed volume */
   else
      output = (chan->env_vol ^ 0x0F) << 8;

   if (0 == chan->adder)
      chan->output_vol = output;
   else if (chan->adder == chan->duty_flip)
      chan->output_vol = -output;
#endif /* !APU_OVERSAMPLE */
#endif /* !APU_YANO */

   return APU_RECTANGLE_OUTPUT;
}

/* TRIANGLE WAVE
** =============
** reg0: 7=holdnote, 6-0=linear length counter
** reg2: low 8 bits of frequency
** reg3: 7-3=length counter, 2-0=high 3 bits of frequency
*/
//#define  APU_TRIANGLE_OUTPUT  (chan->output_vol + (chan->output_vol >> 1))
#define  APU_TRIANGLE_OUTPUT  ((chan->output_vol*21)>>4)
static INT32 apu_triangle(triangle_t *chan)
{
#ifdef APU_YANO
   static double val;
   double sample_weight, total, prev_val;
#else /* !APU_YANO */
   APU_VOLUME_DECAY(chan->output_vol);
#endif /* !APU_YANO */

   if (FALSE == chan->enabled || chan->vbl_length <= 0)
      return APU_TRIANGLE_OUTPUT;

   if (chan->counter_started)
   {
      if (chan->linear_length > 0)
         chan->linear_length -= 4 * apu_cnt_rate;  /* 240/60 */
      if (chan->vbl_length > 0 && FALSE == chan->holdnote)
         chan->vbl_length -= apu_cnt_rate;
   }
   else if (FALSE == chan->holdnote && chan->write_latency)
   {
      if (--chan->write_latency == 0)
         chan->counter_started = TRUE;
   }

   if (chan->linear_length <= 0 || chan->freq < APU_TO_FIXED(4)) /* inaudible */
      return APU_TRIANGLE_OUTPUT;

#ifdef APU_YANO
   if ( chan->ideal_triangle) {
      total = 0;
      sample_weight = 0;
      prev_val = val;
      if ( chan->adder) {
         val -= (double)apu->cycle_rate/chan->freq;
      } else {
         val += (double)apu->cycle_rate/chan->freq;
      }
      while ( (val < -8) || (val >= 8)) {
         if (val <-8) {
            total += (prev_val + (-8))*(prev_val-(-8));
            sample_weight += prev_val-(-8);
            prev_val = -8;
            val = -16-val;
            chan->adder = 0;
         }
         if (val >= 8) {
            total += (prev_val + 8)*(8-prev_val);
            sample_weight += 8-prev_val;
            prev_val = 8;
            val = 16-val;
            chan->adder = 1;
         }
      }
      if ( chan->adder) {
         total += (prev_val + val)*(prev_val-val);
         sample_weight += prev_val-val;
      } else {
         total += (prev_val + val)*(val-prev_val);
         sample_weight += val-prev_val;
      }
      total /= sample_weight;
      chan->output_vol = (int)floor(total*256+0.5);
   } else { /* !ideal_triangle */
      sample_weight = chan->phaseacc;
      if ( sample_weight > apu->cycle_rate) {
         sample_weight = apu->cycle_rate;
      }
      total = (((chan->adder & 0x10)?0x1f:0)^chan->adder)*sample_weight;

      chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
      while ( chan->phaseacc < 0) {
         chan->phaseacc += chan->freq;
         chan->adder = (chan->adder + 1) & 0x1F;

         sample_weight = chan->freq;
         if (chan->phaseacc > 0) {
            sample_weight -= chan->phaseacc;
         }
         total += (((chan->adder & 0x10)?0x1f:0)^chan->adder)*sample_weight;
      }
      chan->output_vol = (int)floor( total*512/apu->cycle_rate + 0.5);
   }
#else /* !APU_YANO */
   chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
   while (chan->phaseacc < 0)
   {
      chan->phaseacc += chan->freq;
      chan->adder = (chan->adder + 1) & 0x1F;

      if (chan->adder & 0x10)
         chan->output_vol -= (2 << 8);
      else
         chan->output_vol += (2 << 8);
   }
#endif /* !APU_YANO */

   return APU_TRIANGLE_OUTPUT;
}


/* WHITE NOISE CHANNEL
** ===================
** reg0: 0-3=volume, 4=envelope, 5=hold
** reg2: 7=small(93 byte) sample,3-0=freq lookup
** reg3: 7-4=vbl length counter
*/
//#define  APU_NOISE_OUTPUT  (chan->output_vol - (chan->output_vol >> 2))
#define  APU_NOISE_OUTPUT  ((chan->output_vol*13)>>4)

static INT32 apu_noise(noise_t *chan)
{
   INT32 outvol;

#ifdef APU_YANO
   static INT32 noise_bit;
   double total;
   double sample_weight;
#else /* !APU_YANO */
#if defined(APU_OVERSAMPLE) && defined(REALTIME_NOISE)
#else /* !(APU_OVERSAMPLE && REALTIME_NOISE) */
   INT32 noise_bit;
#endif /* !(APU_OVERSAMPLE && REALTIME_NOISE) */
#ifdef APU_OVERSAMPLE
   int num_times;
   INT32 total;
#endif /* APU_OVERSAMPLE */

   APU_VOLUME_DECAY(chan->output_vol);
#endif /* !APU_YANO */

   if (FALSE == chan->enabled || chan->vbl_length <= 0)
      return APU_NOISE_OUTPUT;

   /* vbl length counter */
   if (FALSE == chan->holdnote)
      chan->vbl_length -= apu_cnt_rate;

   /* envelope decay at a rate of (env_delay + 1) / 240 secs */
   chan->env_phase -= 4 * apu_cnt_rate; /* 240/60 */
   while (chan->env_phase < 0)
   {
      chan->env_phase += chan->env_delay;

      if (chan->holdnote)
         chan->env_vol = (chan->env_vol + 1) & 0x0F;
      else if (chan->env_vol < 0x0F)
         chan->env_vol++;
   }

#ifdef APU_YANO
   if (chan->fixed_envelope)
      outvol = chan->volume << 8; /* fixed volume */
   else
      outvol = (chan->env_vol ^ 0x0F) << 8;

   sample_weight = chan->phaseacc;
   if ( sample_weight > apu->cycle_rate) {
      sample_weight = apu->cycle_rate;
   }
   total = noise_bit ? sample_weight:-sample_weight;

   chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
   while ( chan->phaseacc < 0) {
      chan->phaseacc += chan->freq;

#ifdef REALTIME_NOISE
      noise_bit = shift_register15(chan->xor_tap);
#else /* !REALTIME_NOISE */
      chan->cur_pos++;

      if (chan->short_sample)
      {
         if (APU_NOISE_93 == chan->cur_pos)
            chan->cur_pos = 0;
      }
      else
      {
         if (APU_NOISE_32K == chan->cur_pos)
            chan->cur_pos = 0;
      }

      if (chan->short_sample)
         noise_bit = noise_short_lut[chan->cur_pos];
      else
         noise_bit = noise_long_lut[chan->cur_pos];
#endif /* !REALTIME_NOISE */

      sample_weight = chan->freq;
      if (chan->phaseacc > 0) {
         sample_weight -= chan->phaseacc;
      }
      total += noise_bit ? sample_weight:-sample_weight;
   }
   chan->output_vol = (int)floor( outvol*total/apu->cycle_rate + 0.5);
#else /* !APU_YANO */
   chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
   if (chan->phaseacc >= 0)
      return APU_NOISE_OUTPUT;
   
#ifdef APU_OVERSAMPLE
   num_times = total = 0;

   if (chan->fixed_envelope)
      outvol = chan->volume << 8; /* fixed volume */
   else
      outvol = (chan->env_vol ^ 0x0F) << 8;
#endif /* APU_OVERSAMPLE */

   while (chan->phaseacc < 0)
   {
      chan->phaseacc += chan->freq;

#ifdef REALTIME_NOISE

#ifdef APU_OVERSAMPLE
      if (shift_register15(chan->xor_tap))
         total += outvol;
      else
         total -= outvol;

      num_times++;
#else /* !APU_OVERSAMPLE */
      noise_bit = shift_register15(chan->xor_tap);
#endif /* !APU_OVERSAMPLE */

#else /* !REALTIME_NOISE */
      chan->cur_pos++;

      if (chan->short_sample)
      {
         if (APU_NOISE_93 == chan->cur_pos)
            chan->cur_pos = 0;
      }
      else
      {
         if (APU_NOISE_32K == chan->cur_pos)
            chan->cur_pos = 0;
      }

#ifdef APU_OVERSAMPLE
      if (chan->short_sample)
         noise_bit = noise_short_lut[chan->cur_pos];
      else
         noise_bit = noise_long_lut[chan->cur_pos];

      if (noise_bit)
         total += outvol;
      else
         total -= outvol;

      num_times++;
#endif /* APU_OVERSAMPLE */
#endif /* !REALTIME_NOISE */
   }

#ifdef APU_OVERSAMPLE
   chan->output_vol = total / num_times;
#else /* !APU_OVERSAMPLE */
   if (chan->fixed_envelope)
      outvol = chan->volume << 8; /* fixed volume */
   else
      outvol = (chan->env_vol ^ 0x0F) << 8;

#ifndef REALTIME_NOISE
   if (chan->short_sample)
      noise_bit = noise_short_lut[chan->cur_pos];
   else
      noise_bit = noise_long_lut[chan->cur_pos];
#endif /* !REALTIME_NOISE */

   if (noise_bit)
      chan->output_vol = outvol;
   else
      chan->output_vol = -outvol;
#endif /* !APU_OVERSAMPLE */
#endif /* !APU_YANO */

   return APU_NOISE_OUTPUT;
}


INLINE void apu_dmcreload(dmc_t *chan)
{
   chan->address = chan->cached_addr;
   chan->dma_length = chan->cached_dmalength;
   chan->irq_occurred = FALSE;
}

/* DELTA MODULATION CHANNEL
** =========================
** reg0: 7=irq gen, 6=looping, 3-0=pointer to clock table
** reg1: output dc level, 6 bits unsigned
** reg2: 8 bits of 64-byte aligned address offset : $C000 + (value * 64)
** reg3: length, (value * 16) + 1
*/
//#define  APU_DMC_OUTPUT (chan->output_vol - (chan->output_vol >> 3))
#define  APU_DMC_OUTPUT ((chan->output_vol*13)>>4)
static INT32 apu_dmc(dmc_t *chan)
{
#ifdef APU_YANO
   double total;
   double sample_weight;
#endif /* APU_YANO */
   int delta_bit;

#ifndef APU_YANO
   APU_VOLUME_DECAY(chan->output_vol);
#endif /* !APU_YANO */

   /* only process when channel is alive */
   if (chan->dma_length)
   {
#ifdef APU_YANO
      sample_weight = chan->phaseacc;
      if ( sample_weight > apu->cycle_rate) {
         sample_weight = apu->cycle_rate;
      }
      total = (chan->regs[1]<<8)*sample_weight;
#endif /* APU_YANO */
      chan->phaseacc -= apu->cycle_rate; /* # of cycles per sample */
      
      while (chan->phaseacc < 0)
      {
         chan->phaseacc += chan->freq;
         
         if (0 == (chan->dma_length & 7))
         {
            chan->cur_byte = cpu_read(chan->address);
            
            /* steal a cycle from CPU*/
            cpu_consume_cycles(1); // for New nes6502.c v1.23 by matt
//            nes6502_setdma(1); // for nes6502.c v1.4 by matt

            if (0xFFFF == chan->address)
               chan->address = 0x8000;
            else
               chan->address++;
         }

         if (--chan->dma_length == 0)
         {
            /* if loop bit set, we're cool to retrigger sample */
            if (chan->looping)
               apu_dmcreload(chan);
            else
            {
               /* check to see if we should generate an irq */
               if (chan->irq_gen)
               {
                  chan->irq_occurred = TRUE;
//#ifndef NSF_PLAYER
//DCR                  nes_irq();
//#endif /* !NSF_PLAYER */
                  cpu_interrupt (CPU_INTERRUPT_IRQ);
               }

               /* bodge for timestamp queue */
#ifdef APU_YANO
               sample_weight = chan->freq - chan->phaseacc;
               total += (chan->regs[1]<<8)*sample_weight;
               while ( chan->phaseacc < 0) chan->phaseacc += chan->freq;
#endif /* !APU_YANO */
               chan->enabled = FALSE;
               break;
            }
         }
         delta_bit = (chan->dma_length & 7) ^ 7;

         /* positive delta */
         if (chan->cur_byte & (1 << delta_bit))
         {
            if (chan->regs[1] < 0x7D)
            {
               chan->regs[1] += 2;
#ifndef APU_YANO
               chan->output_vol += (2 << 8);
#endif /* !APU_YANO */
            }
         }
         /* negative delta */
         else            
         {
            if (chan->regs[1] > 1)
            {
               chan->regs[1] -= 2;
#ifndef APU_YANO
               chan->output_vol -= (2 << 8);
#endif /* !APU_YANO */
            }
         }
#ifdef APU_YANO
         sample_weight = chan->freq;
         if (chan->phaseacc > 0) {
            sample_weight -= chan->phaseacc;
         }
         total += (chan->regs[1]<<8)*sample_weight;
#endif /* !APU_YANO */
      }
#ifdef APU_YANO
      chan->output_vol = (int)floor( total/apu->cycle_rate + 0.5);
#endif /* !APU_YANO */
   }
#ifdef APU_YANO
   else
   {
      chan->output_vol = chan->regs[1] << 8;
   }
#endif /* !APU_YANO */

   return APU_DMC_OUTPUT;
}


static void apu_regwrite(UINT32 address, UINT8 value)
{  
   int chan;

   ASSERT(apu);

   switch (address)
   {
   /* rectangles */
   case APU_WRA0:
   case APU_WRB0:
      chan = (address & 4) ? 1 : 0;
      apu->apus.rectangle[chan].regs[0] = value;

      apu->apus.rectangle[chan].volume = value & 0x0F;
      apu->apus.rectangle[chan].env_delay = decay_lut[value & 0x0F];
      apu->apus.rectangle[chan].holdnote = (value & 0x20) ? TRUE : FALSE;
      apu->apus.rectangle[chan].fixed_envelope = (value & 0x10) ? TRUE : FALSE;
      apu->apus.rectangle[chan].duty_flip = duty_lut[value >> 6];
      break;

   case APU_WRA1:
   case APU_WRB1:
      chan = (address & 4) ? 1 : 0;
      apu->apus.rectangle[chan].regs[1] = value;
      apu->apus.rectangle[chan].sweep_on = (value & 0x80) ? TRUE : FALSE;
      apu->apus.rectangle[chan].sweep_shifts = value & 7;
      apu->apus.rectangle[chan].sweep_delay = decay_lut[(value >> 4) & 7];
      apu->apus.rectangle[chan].sweep_inc = (value & 0x08) ? TRUE : FALSE;
      apu->apus.rectangle[chan].freq_limit = freq_limit[value & 7];
      break;

   case APU_WRA2:
   case APU_WRB2:
      chan = (address & 4) ? 1 : 0;
      apu->apus.rectangle[chan].regs[2] = value;
      apu->apus.rectangle[chan].freq = (apu->apus.rectangle[chan].freq & ~0xFF) | value;
      break;

   case APU_WRA3:
   case APU_WRB3:
      chan = (address & 4) ? 1 : 0;
      apu->apus.rectangle[chan].regs[3] = value;

      apu->apus.rectangle[chan].vbl_length = vbl_lut[value >> 3];
      apu->apus.rectangle[chan].env_vol = 0;
      apu->apus.rectangle[chan].freq = ((value & 7) << 8) | (apu->apus.rectangle[chan].freq & 0xFF);
      apu->apus.rectangle[chan].adder = 0;
      if ( apu->enable_reg & (1<<chan))
         apu->apus.rectangle[chan].enabled = TRUE;
      break;

   /* triangle */
   case APU_WRC0:
      apu->apus.triangle.regs[0] = value;
      apu->apus.triangle.holdnote = (value & 0x80) ? TRUE : FALSE;

      if (FALSE == apu->apus.triangle.counter_started && apu->apus.triangle.vbl_length > 0)
         apu->apus.triangle.linear_length = trilength_lut[value & 0x7F];
      break;

   case APU_WRC2:

      apu->apus.triangle.regs[1] = value;
      apu->apus.triangle.freq = APU_TO_FIXED((((apu->apus.triangle.regs[2] & 7) << 8) + value) + 1);
      break;

   case APU_WRC3:

      apu->apus.triangle.regs[2] = value;
  
      /* this is somewhat of a hack.  there appears to be some latency on 
      ** the Real Thing between when trireg0 is written to and when the 
      ** linear length counter actually begins its countdown.  we want to 
      ** prevent the case where the program writes to the freq regs first, 
      ** then to reg 0, and the counter accidentally starts running because 
      ** of the sound queue's timestamp processing.
      **
      ** set latency to a couple hundred cycles -- should be plenty of time 
      ** for the 6502 code to do a couple of table dereferences and load up 
      ** the other triregs
      */

      /* 06/13/00 MPC -- seems to work OK */
      apu->apus.triangle.write_latency = (int) (228 / APU_FROM_FIXED(apu->cycle_rate));

      apu->apus.triangle.freq = APU_TO_FIXED((((value & 7) << 8) + apu->apus.triangle.regs[1]) + 1);
      apu->apus.triangle.vbl_length = vbl_lut[value >> 3];
      apu->apus.triangle.counter_started = FALSE;
      apu->apus.triangle.linear_length = trilength_lut[apu->apus.triangle.regs[0] & 0x7F];
      if ( apu->enable_reg & 0x04) apu->apus.triangle.enabled = TRUE;
      break;

   /* noise */
   case APU_WRD0:
      apu->apus.noise.regs[0] = value;
      apu->apus.noise.env_delay = decay_lut[value & 0x0F];
      apu->apus.noise.holdnote = (value & 0x20) ? TRUE : FALSE;
      apu->apus.noise.fixed_envelope = (value & 0x10) ? TRUE : FALSE;
      apu->apus.noise.volume = value & 0x0F;
      break;

   case APU_WRD2:
      apu->apus.noise.regs[1] = value;
      apu->apus.noise.freq = APU_TO_FIXED(noise_freq[value & 0x0F]);

#ifdef REALTIME_NOISE
      apu->apus.noise.xor_tap = (value & 0x80) ? 0x40: 0x02;
#else /* !REALTIME_NOISE */
      /* detect transition from long->short sample */
      if ((value & 0x80) && FALSE == apu->apus.noise.short_sample)
      {
         /* recalculate short noise buffer */
         shift_register15(noise_short_lut, APU_NOISE_93);
         apu->apus.noise.cur_pos = 0;
      }
      apu->apus.noise.short_sample = (value & 0x80) ? TRUE : FALSE;
#endif /* !REALTIME_NOISE */
      break;

   case APU_WRD3:
      apu->apus.noise.regs[2] = value;

      apu->apus.noise.vbl_length = vbl_lut[value >> 3];
      apu->apus.noise.env_vol = 0; /* reset envelope */
      if ( apu->enable_reg & 0x08) apu->apus.noise.enabled = TRUE;
      break;

   /* DMC */
   case APU_WRE0:
      apu->apus.dmc.regs[0] = value;

      apu->apus.dmc.freq = APU_TO_FIXED(dmc_clocks[value & 0x0F]);
      apu->apus.dmc.looping = (value & 0x40) ? TRUE : FALSE;

      if (value & 0x80)
         apu->apus.dmc.irq_gen = TRUE;
      else
      {
         apu->apus.dmc.irq_gen = FALSE;
         apu->apus.dmc.irq_occurred = FALSE;
      }
      break;

   case APU_WRE1: /* 7-bit DAC */
      /* add the _delta_ between written value and
      ** current output level of the volume reg
      */
      value &= 0x7F; /* bit 7 ignored */
#ifndef APU_YANO
      apu->apus.dmc.output_vol += ((value - apu->apus.dmc.regs[1]) << 8);
#endif /* !APU_YANO */
      apu->apus.dmc.regs[1] = value;
      break;

   case APU_WRE2:
      apu->apus.dmc.regs[2] = value;
      apu->apus.dmc.cached_addr = 0xC000 + (UINT16) (value << 6);
      break;

   case APU_WRE3:
      apu->apus.dmc.regs[3] = value;
      apu->apus.dmc.cached_dmalength = ((value << 4) + 1) << 3;
      break;

   case APU_SMASK:
      /* bodge for timestamp queue */
      apu->apus.dmc.enabled = (value & 0x10) ? TRUE : FALSE;

      apu->enable_reg = value;

      for (chan = 0; chan < 2; chan++)
      {
         if (0 == (value & (1 << chan)) )
         {
            apu->apus.rectangle[chan].enabled = FALSE;
            apu->apus.rectangle[chan].vbl_length = 0;
         }
      }

      if (0 == (value & 0x04))
      {
         apu->apus.triangle.enabled = FALSE;
         apu->apus.triangle.vbl_length = 0;
         apu->apus.triangle.linear_length = 0;
         apu->apus.triangle.counter_started = FALSE;
         apu->apus.triangle.write_latency = 0;
      }

      if (0 == (value & 0x08))
      {
         apu->apus.noise.enabled = FALSE;
         apu->apus.noise.vbl_length = 0;
      }

      if (value & 0x10)
      {
         if (0 == apu->apus.dmc.dma_length)
            apu_dmcreload(&apu->apus.dmc);
      }
      else
      {
         apu->apus.dmc.dma_length = 0;
         apu->apus.dmc.irq_occurred = FALSE;
      }
      break;

      /* unused, but they get hit in some mem-clear loops */
   case 0x4009:
   case 0x400D:
      break;

   case 0x4017:
      if (value & 0x80)
         apu_cnt_rate = 4;
      else
         apu_cnt_rate = 5;
      break;

   default:
      break;
   }
}

/* Read from $4000-$4017 */
UINT8 apu_read(UINT32 address)
{
   UINT8 value;

   ASSERT(apu);

   switch (address)
   {
   case APU_SMASK:
      value = 0;
      /* Return 1 in 0-5 bit pos if a channel is playing */
      if (apu->apus.rectangle[0].enabled_cur && apu->apus.rectangle[0].vbl_length_cur>0)
         value |= 0x01;
      if (apu->apus.rectangle[1].enabled_cur && apu->apus.rectangle[1].vbl_length_cur>0)
         value |= 0x02;
      if (apu->apus.triangle.enabled_cur && apu->apus.triangle.vbl_length_cur>0)
         value |= 0x04;
      if (apu->apus.noise.enabled_cur && apu->apus.noise.vbl_length_cur>0)
         value |= 0x08;
      /* bodge for timestamp queue */
      if (apu->apus.dmc.enabled_cur)
         value |= 0x10;
      if (apu->apus.dmc.irq_occurred_cur)
         value |= 0x80;
      break;

   default:
      value = (address >> 8); /* heavy capacitance on data bus */
      break;
   }

   return value;
}

UINT8 ex_read(UINT32 address)
{
   if(apu->ex_chip & 4)
   {
      return FDSSoundRead (address);
   }
   else if(apu->ex_chip & 16)
   {
      //return N106SoundReadData (address);
      apudata_t d;
      d.timestamp = cpu_get_cycles(FALSE);
      d.address = address + 0x10000;
      ex_enqueue(&d);
      return 0x00;
   }
   else
   {
      return 0x00;
   }
}

void apu_write(UINT32 address, UINT8 value)
{
   apudata_t d;

   switch (address)
   {
   case 0x4015:
      /* bodge for timestamp queue */
      apu->apus.dmc.enabled = (value & 0x10) ? TRUE : FALSE;

   case 0x4000: case 0x4001: case 0x4002: case 0x4003:
   case 0x4004: case 0x4005: case 0x4006: case 0x4007:
   case 0x4008: case 0x4009: case 0x400A: case 0x400B:
   case 0x400C: case 0x400D: case 0x400E: case 0x400F:
   case 0x4010: case 0x4011: case 0x4012: case 0x4013:
   case 0x4017:
      d.timestamp = cpu_get_cycles(FALSE);
      d.address = address;
      d.value = value;
      apu_enqueue(&d);
      break;

   default:
      break;
   }
}

void apu_write_cur(UINT32 address, UINT8 value)
{
   /* for sync read $4015 */
   int chan;
   switch (address)
   {
   case APU_WRA0:
   case APU_WRB0:
      chan = (address & 4) ? 1 : 0;
      apu->apus.rectangle[chan].holdnote_cur = (value & 0x20) ? TRUE : FALSE;
      break;
   case APU_WRA3:
   case APU_WRB3:
      chan = (address & 4) ? 1 : 0;
      apu->apus.rectangle[chan].vbl_length_cur = vbl_length[value >> 3] * 5;
      if (apu->enable_reg_cur & (1<<chan))
         apu->apus.rectangle[chan].enabled_cur = TRUE;
      break;
   case APU_WRC0:
      apu->apus.triangle.holdnote_cur = (value & 0x80) ? TRUE : FALSE;
      break;
   case APU_WRC3:
      apu->apus.triangle.vbl_length_cur = vbl_length[value >> 3] * 5;
      if (apu->enable_reg_cur & 0x04)
         apu->apus.triangle.enabled_cur = TRUE;
      apu->apus.triangle.counter_started_cur = TRUE;
      break;
   case APU_WRD0:
      apu->apus.noise.holdnote_cur = (value & 0x20) ? TRUE : FALSE;
      break;
   case APU_WRD3:
      apu->apus.noise.vbl_length_cur = vbl_length[value >> 3] * 5;
      if (apu->enable_reg_cur & 0x08)
         apu->apus.noise.enabled_cur = TRUE;
      break;
   case APU_WRE0:
      apu->apus.dmc.freq_cur = dmc_clocks[value & 0x0F];
      apu->apus.dmc.phaseacc_cur = 0;
      apu->apus.dmc.looping_cur = (value & 0x40) ? TRUE : FALSE;
      if (value & 0x80)
         apu->apus.dmc.irq_gen_cur = TRUE;
      else
      {
         apu->apus.dmc.irq_gen_cur = FALSE;
         apu->apus.dmc.irq_occurred_cur = FALSE;
      }
      break;
   case APU_WRE3:
      apu->apus.dmc.cached_dmalength_cur = (value << 4) + 1;
      break;
   case APU_SMASK:
      apu->enable_reg_cur = value;
      for (chan = 0; chan < 2; chan++)
      {
         if (0 == (value & (1 << chan)))
         {
            apu->apus.rectangle[chan].enabled_cur = FALSE;
            apu->apus.rectangle[chan].vbl_length_cur = 0;
         }
      }
      if (0 == (value & 0x04))
      {
         apu->apus.triangle.enabled_cur = FALSE;
         apu->apus.triangle.vbl_length_cur = 0;
         apu->apus.triangle.counter_started_cur = FALSE;
      }
      if (0 == (value & 0x08))
      {
         apu->apus.noise.enabled_cur = FALSE;
         apu->apus.noise.vbl_length_cur = 0;
      }
      if (value & 0x10)
      {
         if(0 == apu->apus.dmc.dma_length_cur)
         {
           apu->apus.dmc.dma_length_cur = apu->apus.dmc.cached_dmalength_cur;
         }
         apu->apus.dmc.enabled_cur = TRUE;
      }
      else
      {
         apu->apus.dmc.dma_length_cur = 0;
         apu->apus.dmc.enabled_cur = FALSE;
         apu->apus.dmc.irq_occurred_cur = FALSE;
      }
      break;
   }
}

void ex_write(UINT32 address, UINT8 value)
{
   apudata_t d;
   d.timestamp = cpu_get_cycles(FALSE);
   d.address = address;
   d.value = value;
   ex_enqueue(&d);

   if(apu->ex_chip & 4)
   {
      FDSSoundWriteCurrent(address, value);
   }
}

void apu_getpcmdata(void **data, int *num_samples, int *sample_bits)
{
   ASSERT(apu);
   *data = apu->buffer;
   *num_samples = apu->num_samples;
   *sample_bits = apu->sample_bits;
}


void apu_process(void *buffer, int num_samples, int dither)
{
   apudata_t *d;
   UINT32 elapsed_cycles;
   static INT32 prev_sample = 0;
   INT32 next_sample, accum;

   ASSERT(apu);

   /* grab it, keep it local for speed */
   elapsed_cycles = (UINT32) apu->elapsed_cycles;

   if (NIL == buffer)
   {
      /* just go through the motions... */
      while (num_samples--)
      {
         while ((FALSE == APU_QEMPTY()) && (apu->queue[apu->q_tail].timestamp <= elapsed_cycles))
         {
            d = apu_dequeue();
            apu_regwrite(d->address, d->value);
         }

         while((FALSE == EX_QEMPTY()) && apu->ex_queue[apu->ex_q_tail].timestamp <= elapsed_cycles)
         {
            d = ex_dequeue();
            if(apu->ex_chip & 1)
            {
               VRC6SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 2)
            {
               OPLLSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 4)
            {
               FDSSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 8)
            {
               MMC5SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 16)
            {
               if(d->address & 0x10000)
               {
                  UINT8 dummy = N106SoundRead(d->address & 0xFFFF);
               }
               else
               {
                  N106SoundWrite(d->address, d->value);
               }
            }
            else if(apu->ex_chip & 32)
            {
               PSGSoundWrite(d->address, d->value);
            }
         }

         elapsed_cycles += APU_FROM_FIXED(apu->cycle_rate);
      }
   }
   else
   {
      /* bleh */
      apu->buffer = buffer; 

      while (num_samples--)
      {
         while ((FALSE == APU_QEMPTY()) && (apu->queue[apu->q_tail].timestamp <= elapsed_cycles))
         {
            d = apu_dequeue();
            apu_regwrite(d->address, d->value);
         }

         while((FALSE == EX_QEMPTY()) && apu->ex_queue[apu->ex_q_tail].timestamp <= elapsed_cycles)
         {
            d = ex_dequeue();
            if(apu->ex_chip & 1)
            {
               VRC6SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 2)
            {
               OPLLSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 4)
            {
               FDSSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 8)
            {
               MMC5SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 16)
            {
               if(d->address & 0x10000)
               {
                  UINT8 dummy = N106SoundRead(d->address & 0xFFFF);
               }
               else
               {
                  N106SoundWrite(d->address, d->value);
               }
            }
            else if(apu->ex_chip & 32)
            {
               PSGSoundWrite(d->address, d->value);
            }
         }

         elapsed_cycles += APU_FROM_FIXED(apu->cycle_rate);

         accum = 0;
         if (apu->mix_enable[0]) accum += apu_rectangle(&apu->apus.rectangle[0]);
         if (apu->mix_enable[1]) accum += apu_rectangle(&apu->apus.rectangle[1]);
         if (apu->mix_enable[2]) accum += apu_triangle(&apu->apus.triangle);
         if (apu->mix_enable[3]) accum += apu_noise(&apu->apus.noise);
         if (apu->mix_enable[4]) accum += apu_dmc(&apu->apus.dmc);

         //if (apu->ext && apu->mix_enable[5]) accum += apu->ext->process();

         if (apu->mix_enable[5])
         {
            if (apu->ex_chip & 1)
            {
               accum += VRC6SoundRender() >> 8;
            }
            else if (apu->ex_chip & 2)
            {
               accum += OPLLSoundRender() >> 8;
            }
            else if (apu->ex_chip & 4)
            {
               accum += FDSSoundRender() >> 8;
            }
            else if (apu->ex_chip & 8)
            {
               accum += MMC5SoundRender() >> 8;
            }
            else if (apu->ex_chip & 16)
            {
               accum += N106SoundRender() >> 8;
            }
            else if (apu->ex_chip & 32)
            {
               accum += PSGSoundRender() >> 7;
            }
         }

         /* do any filtering */
         if (APU_FILTER_NONE != apu->filter_type)
         {
            next_sample = accum;

            if (APU_FILTER_LOWPASS == apu->filter_type)
            {
               accum += prev_sample;
               accum >>= 1;
            }
            else if (APU_FILTER_WEIGHTED == apu->filter_type)
            {
               next_sample =
               accum = (accum + accum + accum + prev_sample) >> 2;
            }
            else if (APU_FILTER_DYNAMIC == apu->filter_type)
            {
                accum += prev_sample;
                accum >>= 1;

                next_sample = accum;
            }
            else
            {
                /* high pass */
                accum -= (prev_sample / 2);
                accum <<= 1;
            }

            prev_sample = next_sample;
         }

         /* little extra kick for the kids */
//         accum <<= 1;
/* Overflow fixed by T.Yano Dec.12.2000 */
#ifdef APU_YANO
         // Cancel DC offset Dec.17.2000
         {
            static double ave, max, min;
            double delta;
            delta = (max-min)/32768.0;
            max -= delta;
            min += delta;
            if (accum>max) max=accum;
            if (accum<min) min=accum;
            ave -= ave/1024.0;
            ave += (max+min)/2048.0;
            accum -= (INT32)ave;
         }
#ifdef APU_HPF_ENABLE
         { // Just test (1/167 for 44.1k)
            static double lpf_out;
            lpf_out *=166.0/167.0;
            lpf_out += accum/167.0;
            accum -= (INT32) lpf_out;
         }
#endif /* APU_HPF_ENABLE */
#endif /* APU_YANO */

         /* prevent clipping */
         if (accum > 0x7FFF)
            accum = 0x7FFF;
         else if (accum < -0x8000)
            accum = -0x8000;

         /* unsigned 16-bit output, unsigned 8-bit */
         if (16 == apu->sample_bits)
            //*((INT16 *) buffer)++ = (INT16) accum;
            *((UINT16 *) buffer)++ = accum ^ 0x8000;
         else
         {
            /* Important: Don't use + 1 on this or it will actually
            be 512, we want it to be 0-511, so that there is a 50/50
            chance of the value being > 255 and causing a bit
            transition in the quantized result. */
            if (dither)
                accum += (rand () % 512);

            *((UINT8 *) buffer)++ = (accum >> 8) ^ 0x80;
         }
      }
   }

   /* resync cycle counter */
   apu->elapsed_cycles = cpu_get_cycles(FALSE);
}

static boolean cycle_noise = TRUE;

void apu_process_stereo(void *buffer, int num_samples, int dither, int style, int flip, int surround)
{
   apudata_t *d;
   UINT32 elapsed_cycles;
   INT32 scrap1, scrap2;

   INT32 accum_centre, accum_left, accum_right;
   INT32 old_accum_left, old_accum_right;

   INT32 next_sample_left, next_sample_right;
   INT32 prev_sample_left = 0, prev_sample_right = 0;

   ASSERT(apu);

   /* grab it, keep it local for speed */
   elapsed_cycles = (UINT32) apu->elapsed_cycles;

   if (NIL == buffer)
   {
      /* just go through the motions... */
      while (num_samples--)
      {
         while ((FALSE == APU_QEMPTY()) && (apu->queue[apu->q_tail].timestamp <= elapsed_cycles))
         {
            d = apu_dequeue();
            apu_regwrite(d->address, d->value);
         }

         while((FALSE == EX_QEMPTY()) && apu->ex_queue[apu->ex_q_tail].timestamp <= elapsed_cycles)
         {
            d = ex_dequeue();
            if(apu->ex_chip & 1)
            {
               VRC6SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 2)
            {
               OPLLSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 4)
            {
               FDSSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 8)
            {
               MMC5SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 16)
            {
               if(d->address & 0x10000)
               {
                  UINT8 dummy = N106SoundRead(d->address & 0xFFFF);
               }
               else
               {
                  N106SoundWrite(d->address, d->value);
               }
            }
            else if(apu->ex_chip & 32)
            {
               PSGSoundWrite(d->address, d->value);
            }
         }

         elapsed_cycles += APU_FROM_FIXED(apu->cycle_rate);
      }
   }
   else
   {
      /* bleh */
      apu->buffer = buffer; 

      while (num_samples--)
      {
         while ((FALSE == APU_QEMPTY()) && (apu->queue[apu->q_tail].timestamp <= elapsed_cycles))
         {
            d = apu_dequeue();
            apu_regwrite(d->address, d->value);
         }

         while((FALSE == EX_QEMPTY()) && apu->ex_queue[apu->ex_q_tail].timestamp <= elapsed_cycles)
         {
            d = ex_dequeue();
            if(apu->ex_chip & 1)
            {
               VRC6SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 2)
            {
               OPLLSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 4)
            {
               FDSSoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 8)
            {
               MMC5SoundWrite(d->address, d->value);
            }
            else if(apu->ex_chip & 16)
            {
               if(d->address & 0x10000)
               {
                  UINT8 dummy = N106SoundRead(d->address & 0xFFFF);
               }
               else
               {
                  N106SoundWrite(d->address, d->value);
               }
            }
            else if(apu->ex_chip & 32)
            {
               PSGSoundWrite(d->address, d->value);
            }
         }

         elapsed_cycles += APU_FROM_FIXED(apu->cycle_rate);

         accum_centre = accum_left = accum_right = 0;

         if (style == 1)
         {
             /* FakeNES classic. */
             if (apu->mix_enable[0]) accum_left += apu_rectangle(&apu->apus.rectangle[0]);
             if (apu->mix_enable[1]) accum_right += apu_rectangle(&apu->apus.rectangle[1]);
             if (apu->mix_enable[2]) accum_left += apu_triangle(&apu->apus.triangle);
             if (apu->mix_enable[3]) accum_right += apu_noise(&apu->apus.noise);
             if (apu->mix_enable[4]) accum_centre += apu_dmc(&apu->apus.dmc);
         }
         else if (style == 2)
         {
             /* FakeNES enhanced. (may/2002) */
             /* Cycles the noise and centers the triangle. */

             if (apu->mix_enable[0]) accum_left += apu_rectangle(&apu->apus.rectangle[0]);
             if (apu->mix_enable[1]) accum_right += apu_rectangle(&apu->apus.rectangle[1]);

             if (apu->mix_enable[2]) accum_centre += apu_triangle(&apu->apus.triangle);
             if (apu->mix_enable[4]) accum_centre += apu_dmc(&apu->apus.dmc);

             if (! cycle_noise) {
                if (apu->mix_enable[3]) accum_left += apu_noise(&apu->apus.noise);
             } else {
                if (apu->mix_enable[3]) accum_right += apu_noise(&apu->apus.noise);
             }
         }
         else
         {
            /* Real NES. */
             if (apu->mix_enable[0]) accum_left += apu_rectangle(&apu->apus.rectangle[0]);
             if (apu->mix_enable[1]) accum_left += apu_rectangle(&apu->apus.rectangle[1]);
             if (apu->mix_enable[2]) accum_right += apu_triangle(&apu->apus.triangle);
             if (apu->mix_enable[3]) accum_right += apu_noise(&apu->apus.noise);
             if (apu->mix_enable[4]) accum_right += apu_dmc(&apu->apus.dmc);
         }

         //if (apu->ext && apu->mix_enable[5]) accum += apu->ext->process();

         if (apu->mix_enable[5])
         {
            if (apu->ex_chip & 1)
            {
               accum_centre += VRC6SoundRender() >> 8;
            }
            else if (apu->ex_chip & 2)
            {
               accum_centre += OPLLSoundRender() >> 8;
            }
            else if (apu->ex_chip & 4)
            {
               accum_centre += FDSSoundRender() >> 8;
            }
            else if (apu->ex_chip & 8)
            {
               accum_centre += MMC5SoundRender() >> 8;
            }
            else if (apu->ex_chip & 16)
            {
               accum_centre += N106SoundRender() >> 8;
            }
            else if (apu->ex_chip & 32)
            {
               accum_centre += PSGSoundRender() >> 7;
            }
         }

         accum_left += (accum_centre / 2);
         accum_right += (accum_centre / 2);

         /* do any filtering */
         if (APU_FILTER_NONE != apu->filter_type)
         {
            next_sample_left = accum_left;
            next_sample_right = accum_right;

            if (APU_FILTER_LOWPASS == apu->filter_type)
            {
               accum_left += prev_sample_left;
               accum_left >>= 1;

               accum_right += prev_sample_right;
               accum_right >>= 1;
            }
            else if (APU_FILTER_WEIGHTED == apu->filter_type)
            {
               accum_left = (accum_left +
                accum_left + accum_left + prev_sample_left) >> 2;

               accum_right = (accum_right +
                accum_right + accum_right + prev_sample_right) >> 2;
            }
            else if (APU_FILTER_DYNAMIC == apu->filter_type)
            {
                /* dynamic */
               accum_left += prev_sample_left;
               accum_left >>= 1;

               next_sample_left = accum_left;

               accum_right += prev_sample_right;
               accum_right >>= 1;

               next_sample_right = accum_right;
            }
            else
            {
                /* high pass */
                accum_left -= (prev_sample_left / 2);
                accum_left <<= 1;

                accum_right -= (prev_sample_right / 2);
                accum_right <<= 1;
            }

            prev_sample_left = next_sample_left;
            prev_sample_right = next_sample_right;
         }

         /* stereo blending */
         old_accum_left = accum_left;
         old_accum_right = accum_right;

         accum_left += (old_accum_right / 2);
         accum_right += (old_accum_left / 2);

         /* little extra kick for the kids */
//         accum <<= 1;
/* Overflow fixed by T.Yano Dec.12.2000 */
#ifdef APU_YANO
         // Cancel DC offset Dec.17.2000
         {
            static double ave_left, ave_right, max_left, max_right, min_left, min_right;
            double delta_left, delta_right;
            delta_left = (max_left-min_left)/32768.0;
            delta_right = (max_right-min_right)/32768.0;
            max_left -= delta_left;
            max_right -= delta_right;
            min_left += delta_left;
            min_right += delta_right;
            if (accum_left>max_left) max_left=accum_left;
            if (accum_right>max_right) max_right=accum_right;
            if (accum_left<min_left) min_left=accum_left;
            if (accum_right<min_right) min_right=accum_right;
            ave_left -= ave_left/1024.0;
            ave_right -= ave_right/1024.0;
            ave_left += (max_left+min_left)/2048.0;
            ave_right += (max_right+min_right)/2048.0;
            accum_left -= (INT32)ave_left;
            accum_right -= (INT32)ave_right;
         }
#ifdef APU_HPF_ENABLE
         { // Just test (1/167 for 44.1k)
            static double lpf_out_left, lpf_out_right;
            lpf_out_left *=166.0/167.0;
            lpf_out_left += accum_left/167.0;
            lpf_out_right *=166.0/167.0;
            lpf_out_right += accum_right/167.0;
            accum_left -= (INT32) lpf_out_left;
            accum_right -= (INT32) lpf_out_right;
         }
#endif /* APU_HPF_ENABLE */
#endif /* APU_YANO */

         /* surround sound */
         if (surround)
         {
            /* thanks to kode54 */
            old_accum_left = accum_left;

            scrap1 = ((accum_left + accum_right) >> 1);

            scrap2 = (accum_right - scrap1);
            accum_left -= scrap2;

            accum_right -= (old_accum_left - scrap1);
         }

         /* prevent clipping */
         if (accum_left > 0x7FFF)
            accum_left = 0x7FFF;
         else if (accum_left < -0x8000)
            accum_left = -0x8000;

         if (accum_right > 0x7FFF)
            accum_right = 0x7FFF;
         else if (accum_right < -0x8000)
            accum_right = -0x8000;

         /* reverse stereo */
         if (flip)
         {
            old_accum_left = accum_left;

            accum_left = accum_right;
            accum_right = old_accum_left;
         }

         /* signed 16-bit output, unsigned 8-bit */
         if (16 == apu->sample_bits) {
            //*((INT16 *) buffer)++ = (INT16) accum;
            *((UINT16 *) buffer)++ = accum_left ^ 0x8000;
            *((UINT16 *) buffer)++ = accum_right ^ 0x8000;
         } else {
            if (dither) {
                accum_left += (rand () % 512);
                accum_right += (rand () % 512);
            }

            *((UINT8 *) buffer)++ = (accum_left >> 8) ^ 0x80;
            *((UINT8 *) buffer)++ = (accum_right >> 8) ^ 0x80;
         }
      }
   }

   /* resync cycle counter */
   apu->elapsed_cycles = cpu_get_cycles(FALSE);

   if (style == 2)
   {
      cycle_noise = (! cycle_noise);
   }
}

/* set the filter type */
void apu_setfilter(int filter_type)
{
   ASSERT(apu);
   apu->filter_type = filter_type;
}

// apu_reset_apus() added by T.Yano
void apu_reset_apus(APUSOUND *apus)
{
   int i;
   boolean mode;

   // Reset rectangles
   for ( i=0; i<2; i++) {
      memset(&apus->rectangle[i], 0, sizeof(apus->rectangle[i]));
   }
   apus->rectangle[0].sweep_complement = TRUE;
   apus->rectangle[1].sweep_complement = FALSE;
   mode = apus->triangle.ideal_triangle;
   memset(&apus->triangle, 0, sizeof(apus->triangle));
   apus->triangle.ideal_triangle = mode;
   memset(&apus->noise, 0, sizeof(apus->noise));
   memset(&apus->dmc, 0, sizeof(apus->dmc));

   srand (time (0));    // for 8-bit dither
}

void apu_reset(void)
{
   UINT32 address;

   ASSERT(apu);

   apu->elapsed_cycles = 0;
   memset(&apu->queue, 0, APUQUEUE_SIZE * sizeof(apudata_t));
   apu->q_head = apu->q_tail = 0;

   memset(&apu->ex_queue, 0, APUQUEUE_SIZE * sizeof(apudata_t));
   apu->ex_q_head = apu->ex_q_tail = 0;
   apu->ex_chip = 0;

   // added by T.Yano
   apu_reset_apus( &apu->apus);
   apu->enable_reg = 0;
   apu->enable_reg_cur = 0;

   /* use to avoid bugs =) */
   for (address = 0x4000; address <= 0x4013; address++)
   {
      apu_regwrite(address, 0);
      apu_write_cur(address, 0);
   }

#ifdef NSF_PLAYER
   apu_regwrite(0x400C, 0x10); /* silence noise channel on NSF start */
   apu_regwrite(0x4015, 0x0F);
   apu_write_cur(0x400c, 0x10);
   apu_write_cur(0x4015, 0x0F);
#else /* !NSF_PLAYER */
   apu_regwrite(0x4015, 0x00);
   apu_write_cur(0x4015, 0x00);
#endif /* !NSF_PLAYER */

   if (apu->ext)
      apu->ext->reset();

   // for ExSound
   LogTableInitialize ();
   FDSSoundReset();
   FDSSoundVolume(1);
   PSGSoundReset();
   PSGSoundVolume(1);
   N106SoundReset();
   N106SoundVolume(1);
   VRC6SoundReset();
   VRC6SoundVolume(1);
   OPLLSoundReset();
   OPLLSoundVolume(1);
   MMC5SoundReset();
   MMC5SoundVolume(1);

   // for $4017:bit7 by T.Yano
   apu_cnt_rate = 5;
}

void apu_build_luts(int num_samples)
{
   int i;

   // decay_lut[], vbl_lut[], trilength_lut[] modified (x5) for $4017:bit7 by T.Yano
   /* lut used for enveloping and frequency sweeps */
   for (i = 0; i < 16; i++)
      decay_lut[i] = num_samples * (i + 1) * 5;

   /* used for note length, based on vblanks and size of audio buffer */
   for (i = 0; i < 32; i++)
      vbl_lut[i] = vbl_length[i] * num_samples * 5;

   /* triangle wave channel's linear length table */
   for (i = 0; i < 128; i++)
      trilength_lut[i] = num_samples * i * 5;

#ifndef REALTIME_NOISE
   /* generate noise samples */
   shift_register15(noise_long_lut, APU_NOISE_32K);
   shift_register15(noise_short_lut, APU_NOISE_93);
#endif /* !REALTIME_NOISE */
}

static void apu_setactive(apu_t *active)
{
   ASSERT(active);
   apu = active;
}

void apu_setparams(int sample_rate, int refresh_rate, int frag_size, int sample_bits)
{
   apu->sample_rate = sample_rate;
   apu->refresh_rate = refresh_rate;
   apu->sample_bits = sample_bits;

   apu->num_samples = sample_rate / refresh_rate;
   //apu->num_samples = frag_size;
   frag_size = frag_size; /* quell warnings */

   /* turn into fixed point! */
   apu->cycle_rate = (INT32) (APU_BASEFREQ * 65536.0 / (float) sample_rate);

   /* build various lookup tables for apu */
   apu_build_luts(apu->num_samples);

//DCR   apu_reset();
}

#ifdef APU_YANO
void apu_setmode(int item, int mode)
{
   switch (item) {
   case APUMODE_IDEAL_TRIANGLE:
      if ( apu != NIL) apu->apus.triangle.ideal_triangle = mode;
      break;
   case APUMODE_SMOOTH_ENVELOPE:
      if ( apu != NIL) apu->apus.rectangle[0].smooth_envelope = mode;
      if ( apu != NIL) apu->apus.rectangle[1].smooth_envelope = mode;
      break;
   case APUMODE_SMOOTH_SWEEP:
      if ( apu != NIL) apu->apus.rectangle[0].smooth_sweep = mode;
      if ( apu != NIL) apu->apus.rectangle[1].smooth_sweep = mode;
      break;
   default:
      break;
   }
}
#endif /* APU_YANO */

/* Initializes emulated sound hardware, creates waveforms/voices */
apu_t *apu_create(int sample_rate, int refresh_rate, int frag_size, int sample_bits)
{
   apu_t *temp_apu;
   int channel;

   temp_apu = malloc(sizeof(apu_t));
   if (NIL == temp_apu)
      return NIL;

   /* set the stupid flag to tell difference between two rectangles */
   temp_apu->apus.rectangle[0].sweep_complement = TRUE;
   temp_apu->apus.rectangle[1].sweep_complement = FALSE;

   /* set the update routine */
   temp_apu->process = apu_process;
   temp_apu->ext = NIL;

   apu_setactive(temp_apu);

   apu_setparams(sample_rate, refresh_rate, frag_size, sample_bits);
   apu_reset(); //DCR

   for (channel = 0; channel < 6; channel++)
      apu_setchan(channel, TRUE);

   apu_setfilter(APU_FILTER_LOWPASS);
#ifdef APU_YANO
   apu_setmode(APUMODE_IDEAL_TRIANGLE, FALSE);
#endif /* APU_YANO */

   return temp_apu;
}

void apu_destroy(apu_t **src_apu)
{
   if (*src_apu)
   {
      if ((*src_apu)->ext)
         (*src_apu)->ext->shutdown();
      free(*src_apu);
   }
}

void apu_setext(apu_t *src_apu, apuext_t *ext)
{
   ASSERT(src_apu);

   src_apu->ext = ext;

   /* initialize it */
   if (src_apu->ext)
      src_apu->ext->init();
}

void sync_apu_register()
{
   if (!apu->apus.rectangle[0].holdnote_cur && apu->apus.rectangle[0].vbl_length_cur > 0)
   {
      apu->apus.rectangle[0].vbl_length_cur -= apu_cnt_rate;
   }
   if (!apu->apus.rectangle[1].holdnote_cur && apu->apus.rectangle[1].vbl_length_cur > 0)
   {
      apu->apus.rectangle[1].vbl_length_cur -= apu_cnt_rate;
   }
   if (apu->apus.triangle.counter_started_cur)
   {
      if (apu->apus.triangle.vbl_length_cur > 0 && !apu->apus.triangle.holdnote_cur)
      {
         apu->apus.triangle.vbl_length_cur -= apu_cnt_rate;
      }
   }
   if (!apu->apus.noise.holdnote_cur && apu->apus.noise.vbl_length_cur > 0)
   {
      apu->apus.noise.vbl_length_cur -= apu_cnt_rate;
   }
}

boolean sync_dmc_register(UINT32 cpu_cycles)
{
   boolean irq_occurred = FALSE;

   apu->apus.dmc.phaseacc_cur -= (int)cpu_cycles;
   while(apu->apus.dmc.phaseacc_cur < 0)
   {
      apu->apus.dmc.phaseacc_cur += apu->apus.dmc.freq_cur * 8;
      if (apu->apus.dmc.dma_length_cur)
      {
        if (--apu->apus.dmc.dma_length_cur == 0)
        {
           if (apu->apus.dmc.looping_cur)
           {
              apu->apus.dmc.dma_length_cur = apu->apus.dmc.cached_dmalength_cur;
              apu->apus.dmc.irq_occurred_cur = FALSE;
           }
           else
           {
              apu->apus.dmc.dma_length_cur = 0;
              if (apu->apus.dmc.irq_gen_cur)
              {
                 apu->apus.dmc.irq_occurred_cur = TRUE;
                 irq_occurred = TRUE;
              }
              apu->apus.dmc.enabled_cur = FALSE;
           }
        }
      }
   }
   return irq_occurred;
}

/*
** $Log: apu.c,v $
** Revision 1.12  2003/03/04 05:32:52  stainless
** Replaced all references to `NULL' with `NIL' to avoid compiler conflicts.
**
** Revision 1.11  2002/07/18 20:34:19  stainless
** Mingw32 compile fix.
**
** Revision 1.10  2002/07/17 10:46:39  stainless
** Added random noise dithering for 8-bit audio.
**
** Revision 1.9  2002/05/03 01:22:12  stainless
** Enhanced stereo fix & improvements.
**
** Revision 1.7  2002/04/27 03:26:47  stainless
** Enabled generation of DMC IRQ.
**
** Revision 1.5  2002/04/05 04:39:56  stainless
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
** Revision 1.33  2000/08/15 12:38:04  matt
** removed debug output
**
** Revision 1.32  2000/08/15 12:36:51  matt
** calling apu_process with buffer=NULL causes silent emulation of APU
**
** Revision 1.31  2000/08/11 02:27:21  matt
** general cleanups, plus apu_setparams routine
**
** Revision 1.30  2000/07/31 04:32:52  matt
** fragsize problem fixed, perhaps
**
** Revision 1.29  2000/07/30 04:32:59  matt
** no more apu_getcyclerate hack
**
** Revision 1.28  2000/07/28 03:15:46  matt
** accuracy changes for rectangle frequency sweeps
**
** Revision 1.27  2000/07/27 02:49:50  matt
** eccentricity in sweeping hardware now emulated correctly
**
** Revision 1.26  2000/07/25 02:25:14  matt
** safer apu_destroy
**
** Revision 1.25  2000/07/23 15:10:54  matt
** hacks for win32
**
** Revision 1.24  2000/07/17 01:52:31  matt
** made sure last line of all source files is a newline
**
** Revision 1.23  2000/07/10 19:24:55  matt
** irqs are not supported in NSF playing mode
**
** Revision 1.22  2000/07/10 13:54:32  matt
** using generic nes_irq() now
**
** Revision 1.21  2000/07/10 05:29:34  matt
** moved joypad/oam dma from apu to ppu
**
** Revision 1.20  2000/07/09 03:49:31  matt
** apu irqs now draw an irq line (bleh)
**
** Revision 1.19  2000/07/04 04:53:26  matt
** minor changes, sound amplification
**
** Revision 1.18  2000/07/03 02:18:53  matt
** much better external module exporting
**
** Revision 1.17  2000/06/26 11:01:55  matt
** made triangle a tad quieter
**
** Revision 1.16  2000/06/26 05:10:33  matt
** fixed cycle rate generation accuracy
**
** Revision 1.15  2000/06/26 05:00:37  matt
** cleanups
**
** Revision 1.14  2000/06/23 11:06:24  matt
** more faithful mixing of channels
**
** Revision 1.13  2000/06/23 03:29:27  matt
** cleaned up external sound inteface
**
** Revision 1.12  2000/06/20 00:08:39  matt
** bugfix to rectangle wave
**
** Revision 1.11  2000/06/13 13:48:58  matt
** fixed triangle write latency for fixed point apu cycle rate
**
** Revision 1.10  2000/06/12 01:14:36  matt
** minor change to clipping extents
**
** Revision 1.9  2000/06/09 20:00:56  matt
** fixed noise hiccup in NSF player mode
**
** Revision 1.8  2000/06/09 16:49:02  matt
** removed all floating point from sound generation
**
** Revision 1.7  2000/06/09 15:12:28  matt
** initial revision
**
*/

