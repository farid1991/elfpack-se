/*
** Konami VRC7 ExSound by TAKEDA, toshiya
**
** original: s_vrc7.c in nezp0922
*/

#define OPLL_CH_MAX 6/* 9 */

/* PG_BITS + PG_SHIFT < 32 */
#define PG_BITS 10
#define AM_BITS 8
#define PM_BITS 8

#define AM_DEPTH 4.8  /* 1.0 */		/* dB */
#define PM_DEPTH 14.0 /* 7.0 */		/* cent */
#define PM_SHIFT 16

#define PG_SHIFT (20 - PG_BITS)
#define EG_SHIFT 7/*8*/
#define LOG_KEYOFF (32 << LOG_BITS)
#define EG_KEYOFF (LOG_KEYOFF << EG_SHIFT)

typedef enum {
	EGM_OFF,
	EGM_A,
	EGM_D,
	EGM_S,
	EGM_SS,
	EGM_R
} OPLL_EGMODE;

#define OPLL_UPDATE_FREQH		(1 << 0)
#define OPLL_UPDATE_FREQL		(1 << 1)
#define OPLL_UPDATE_TONE		(1 << 2)

static UINT32 amtbl[1 << AM_BITS];
static UINT32 pmtbl[1 << PM_BITS];
static UINT32 sintbl[1 << PG_BITS];
static UINT32 sintbld[1 << PG_BITS];

static UINT8 multbl[16] =
{
	1,1*2,2*2,3*2,4*2,5*2,6*2,7*2,8*2,9*2,10*2,10*2,12*2,12*2,15*2,15*2,
};
static UINT8 fbtbl[8] =
{
	31,6,5,4,3,2,1,0,
};
static UINT8 oplltone[][8] =
{
	{  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
#if 1
	/* VRC7ED2(00/06/26) vrc7tone.bin */
	/*  AM1  AM2  KS1  KS2  AD1  AD2  SR1  SR2 */
	{  0x53,0x02,0x10,0x07,0xF3,0xF0,0x48,0x1E },
	{  0x03,0x51,0x5E,0x07,0xF1,0xF1,0x0E,0x0C },
	{  0x01,0x51,0x4F,0x04,0xD8,0xE0,0x0F,0x0F },
	{  0x01,0x41,0x1A,0x01,0xA0,0xC0,0xFF,0xFF },
	{  0x02,0x01,0x18,0x07,0xD2,0xA1,0xA7,0xE8 },
	{  0x34,0x21,0xD0,0x07,0x44,0x84,0xF4,0xEE },
	{  0x21,0x61,0x17,0x07,0x71,0x81,0xCE,0x9B },
	{  0x33,0x31,0x58,0x06,0xF0,0xF0,0x0E,0x0E },
	{  0x07,0x01,0x2F,0x00,0xE0,0xE0,0xFF,0xFF },
	{  0x37,0xA1,0x1B,0x07,0xF1,0xF1,0xF4,0xEE },
	{  0x07,0x81,0x24,0x07,0xFF,0xFF,0xF4,0xFF },
	{  0x61,0x81,0x08,0x72,0xF0,0xF0,0xBD,0xAC },
	{  0x01,0x02,0xD6,0x07,0xF8,0xE4,0xFF,0xCF },
	{  0x21,0x61,0x00,0x00,0xF3,0xE2,0xB6,0xD9 },
	{  0x11,0x11,0xD4,0x07,0xFF,0xFF,0xFF,0xFF },
#endif
	{  0x13,0x11,0x25,0x00,0xd7,0xb7,0xf4,0xf4 }, /* 16 Rhythm 1: */
	{  0x13,0x11,0x25,0x00,0xd7,0xb7,0xf4,0xf4 }, /* 17 Rhythm 2: */
	{  0x13,0x11,0x25,0x00,0xd7,0xb7,0xf4,0xf4 }, /* 18 Rhythm 3: */
};

INLINE static UINT32 PGCALC(OPLL_OP *pop)
{
	if (pop->pg_vib)
	{
		pop->vib_cycles -= 1 << PM_SHIFT;
		while (pop->vib_cycles < 0)
		{
			pop->vib_cycles += apu->ym2413s.lfo[1].output;
			pop->pg_phase += pop->pg_spd;
		}
	}
	else
	{
		pop->pg_phase += pop->pg_spd;
	}
	return (pop->input + (pop->pg_phase >> PG_SHIFT)) & ((1 << PG_BITS) - 1);
}

INLINE static void OPKEY(OPLL_OP *pop, UINT8 key)
{
	if (key)
	{
		pop->eg_mode = EGM_A;
		pop->eg_phase = 63 << (EG_SHIFT + LOG_BITS - 3);
	}
	else
	{
		if (pop->eg_mode == EGM_OFF) return;
		pop->eg_mode = EGM_R;
	}
}
INLINE static UINT32 EGCALC(OPLL_OP *pop)
{
    UINT32 egout = pop->tl << (LOG_BITS - 3);
	if (pop->eg_am) egout += apu->ym2413s.lfo[0].output;
	switch (pop->eg_mode)
	{
		case EGM_A:
			{
                UINT32 attack;
				attack = 1 + (pop->eg_phase >> pop->eg_arr);
				if (pop->eg_phase > attack)
				{
					pop->eg_phase -= attack;
					break;
				}
			}
			pop->eg_phase = 0;
			pop->eg_mode = EGM_D;
		case EGM_D:
			if (pop->eg_phase < pop->eg_sl)
			{
				pop->eg_phase += pop->eg_drr;
				break;
			}
			if (pop->eg_type)
			{
				pop->eg_mode = EGM_SS;
				break;
			}
			pop->eg_mode = EGM_S;
			break;
		case EGM_S:
			if (pop->eg_phase < EG_KEYOFF)
			{
				pop->eg_phase += pop->eg_rrr;
			}
			else
			{
				pop->eg_mode = EGM_OFF;
			}
			break;
		case EGM_SS:
			break;
		case EGM_R:
			if (pop->eg_phase < EG_KEYOFF)
			{
				if (pop->su_type)
				{
					pop->eg_phase += (5 << 2);
				}
				else if (pop->eg_type)
					pop->eg_phase += pop->eg_rrr;
				else
					pop->eg_phase += (13 << 2);
				break;
			}
			pop->eg_mode = EGM_OFF;
			break;
		case EGM_OFF:
			return EG_KEYOFF;
	}
	return ((pop->eg_phase >> EG_SHIFT) + egout) << 1;
}

static UINT32 OPCALC(OPLL_OP *pop)
{
	return pop->sintblp[PGCALC(pop)] + EGCALC(pop);
}

INLINE static void EGSET(OPLL_CH *pch)
{
	pch->op[0].eg_am = pch->tone[0] & 0x80;
	pch->op[1].eg_am = pch->tone[1] & 0x80;
	pch->op[0].tl = pch->tone[2] & 0x3F;
	pch->op[0].sintblp = (pch->tone[3] & 0x08) ? sintbld : sintbl;
	pch->op[1].sintblp = (pch->tone[3] & 0x10) ? sintbld : sintbl;
	pch->op[0].eg_ks = pch->tone[2] >> 6;
	pch->op[1].eg_ks = pch->tone[3] >> 6;
	pch->op[0].eg_type = pch->tone[0] & 0x20;
	pch->op[1].eg_type = pch->tone[1] & 0x20;
	pch->op[0].eg_ar = (pch->tone[4] & 0xF0) >> 2;
	pch->op[1].eg_ar = (pch->tone[5] & 0xF0) >> 2;
	pch->op[0].eg_dr = (pch->tone[4] & 0x0F) << 2;
	pch->op[1].eg_dr = (pch->tone[5] & 0x0F) << 2;
	pch->op[0].eg_rr = (pch->tone[6] & 0x0F) << 2;
	pch->op[1].eg_rr = (pch->tone[7] & 0x0F) << 2;
	pch->op[0].eg_sl = (pch->tone[6] & 0xF0) << (EG_SHIFT + LOG_BITS - 3 - 2);
	pch->op[1].eg_sl = (pch->tone[7] & 0xF0) << (EG_SHIFT + LOG_BITS - 3 - 2);
}
INLINE static void EGSET2(OPLL_OP *pop)
{
	pop->eg_arr = (15 - (pop->eg_ar >> 2));
	pop->eg_drr = pop->eg_dr;
	pop->eg_rrr = pop->eg_rr;
}

INLINE static void EGSET2KSR(OPLL_OP *pop, UINT8 freqh)
{
    UINT8 ksr = (freqh & 0x0F) >> (3 - pop->eg_ks);
	if (pop->eg_ar)
	{
        UINT8 arr = pop->eg_ar + ksr;
		if (arr > 63) arr = 63;
		pop->eg_arr = (15 - (arr >> 2));
	}
	else
	{
		pop->eg_arr = 15;
	}
	if (pop->eg_dr)
	{
		pop->eg_drr = pop->eg_dr + ksr;
		if (pop->eg_drr > 63) pop->eg_drr = 63;
	}
	else
	{
		pop->eg_drr = 0;
	}
	if (pop->eg_rr)
	{
		pop->eg_rrr = pop->eg_rr + ksr;
		if (pop->eg_rrr > 63) pop->eg_rrr = 63;
	}
	else
	{
		pop->eg_rrr = 0;
	}
}

INLINE static void PGSET(OPLL_CH *pch)
{
    UINT32 fnum, block;
	fnum = ((pch->freqh << 8) + pch->freql) & 0x1FF;
	block = (pch->freqh >> 1) & 7;
	pch->op[0].pg_spd = (multbl[pch->tone[0] & 0xF] * fnum) << block;
	pch->op[1].pg_spd = (multbl[pch->tone[1] & 0xF] * fnum) << block;
}

INLINE static INT32 CHCALC(OPLL_CH *pch)
{
	if (pch->update)
	{
		if (pch->update & (OPLL_UPDATE_FREQL | OPLL_UPDATE_FREQH | OPLL_UPDATE_TONE))
		{
			PGSET(pch);
		}
		if (pch->update & OPLL_UPDATE_TONE)
		{
			pch->fb = fbtbl[pch->tone[3] & 0x7] + (LOG_LIN_BITS - PG_BITS);
			pch->fbbuf[0] = pch->fbbuf[1] = 0;
			if (pch->fb > 31) pch->fb = 31;
			pch->op[0].pg_vib = pch->tone[0] & 0x40;
			pch->op[1].pg_vib = pch->tone[1] & 0x40;
			EGSET(pch);
		}
		if (pch->update & OPLL_UPDATE_FREQH)
		{
			if ((pch->key ^ pch->freqh) & 0x10)
			{
				pch->key ^= 0x10;
                OPKEY(&pch->op[0], (UINT8)(pch->key & 0x10));
                OPKEY(&pch->op[1], (UINT8)(pch->key & 0x10));
			}
			pch->op[0].su_type = pch->freqh & 0x20;
			pch->op[1].su_type = pch->freqh & 0x20;
		}
		if (pch->update & (OPLL_UPDATE_FREQH | OPLL_UPDATE_TONE))
		{
			int op;
			for (op = 0; op < 2; op++)
			{
				if (pch->tone[op] & 0x10)
					EGSET2KSR(&pch->op[op], pch->freqh);
				else
					EGSET2(&pch->op[op]);
			}
		}
		pch->update = 0;
	}
	pch->cycles -= pch->cps;
	while (pch->cycles < 0)
	{
        UINT32 opout;
		pch->cycles += 1 << 18;
#if 0
		pch->op[0].input = pch->fbbuf[0] + pch->fbbuf[1];
		opout = OPCALC(&pch->op[0]);
		pch->fbbuf[1] = pch->fbbuf[0];
		pch->fbbuf[0] = LogToLinear(opout, pch->fb + 1);
		pch->op[1].input = LogToLinear(opout, (LOG_LIN_BITS - PG_BITS - 1));
#else
		pch->op[0].input = pch->fbbuf[0];
		opout = OPCALC(&pch->op[0]);
		pch->fbbuf[0] = LogToLinear(opout, pch->fb);
		pch->op[1].input = LogToLinear(opout, (LOG_LIN_BITS - PG_BITS - 1));
#endif
		pch->output = OPCALC(&pch->op[1]);
	}
	return LogToLinear(pch->output + apu->ym2413s.mastervolume, (LOG_LIN_BITS - 21));
}

INLINE static INT32 LFOCALC(OPLL_LFO *pch)
{
	pch->cycles -= pch->cps;
	while (pch->cycles < 0)
	{
		pch->cycles += pch->spd;
		pch->adr++;
	}
	pch->adr &= pch->adrmask;
	pch->output = pch->table[pch->adr];
	return pch->output;
}

static INT32 OPLLSoundRender(void)
{
    INT32 accum = 0;
    UINT8 ch;
#if OPLL_CH_MAX == 9
    UINT8 chmax = (apu->ym2413s.rhythmc & 0x20) ? 6 : 9;
#else
    AL_CONST UINT8 chmax = 6;  /* VRCVII */
#endif
	for (ch = 0; ch < 2; ch++) LFOCALC(&apu->ym2413s.lfo[ch]);
	for (ch = 0; ch < chmax; ch++)
	{
		accum += CHCALC(&apu->ym2413s.ch[ch]);
	}
	return accum;
}

static void OPLLSoundVolume(UINT32 volume)
{
	apu->ym2413s.mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

static void OPLLSoundWriteAddr(UINT32 address, UINT8 value)
{
	apu->ym2413s.adr = value;
}

static void OPLLSoundWriteData(UINT32 address, UINT8 value)
{
	int ch = apu->ym2413s.adr & 0xF;
	switch (apu->ym2413s.adr & 0xF8)
	{
		case 0x00:
			apu->ym2413s.usertone[apu->ym2413s.adr] = value;
			apu->ym2413s.toneupdate = ~0;
			break;
#if OPLL_CH_MAX == 9
		case 0x08:
			if (ch != 0xE) break;
			apu->ym2413s.rhythmc = value;
			/* rhythmmode */
			break;
#endif
		case 0x10:
		case 0x18:
			if (ch >= OPLL_CH_MAX) break;
			apu->ym2413s.ch[ch].freql = value;
			apu->ym2413s.ch[ch].update |= OPLL_UPDATE_FREQL;
			break;
		case 0x20:
		case 0x28:
			if (ch >= OPLL_CH_MAX) break;
			apu->ym2413s.ch[ch].freqh = value;
			apu->ym2413s.ch[ch].update |= OPLL_UPDATE_FREQH;
			break;
		case 0x30:
		case 0x38:
			if (ch >= OPLL_CH_MAX) break;
#if OPLL_CH_MAX == 9
			if ((apu->ym2413s.rhythmc & 0x20) && (ch > 5))
			{
				if (ch != 6)
					apu->ym2413s.ch[ch].op[0].tl = ((value >> 4) & 0xF) << 2;
				apu->ym2413s.ch[ch].op[1].tl = (value & 0xF) << 2;
			}
			else
#endif
			{
				if(apu->ym2413s.ch[ch].toneno != (value >> 4))
				{
					if (value >> 4)
					{
						apu->ym2413s.ch[ch].toneno = value >> 4;
						memcpy(apu->ym2413s.ch[ch].tone, oplltone[value >> 4], 8);
						apu->ym2413s.ch[ch].update |= OPLL_UPDATE_TONE;
					}
					else if ((apu->ym2413s.ch[ch].toneno != 0x10) || (apu->ym2413s.toneupdate & (1 << ch)))
					{
						apu->ym2413s.toneupdate &= ~(1 << ch);
						apu->ym2413s.ch[ch].toneno = 0x10;
						memcpy(apu->ym2413s.ch[ch].tone, apu->ym2413s.usertone, 8);
						apu->ym2413s.ch[ch].update |= OPLL_UPDATE_TONE;
					}
				}
				apu->ym2413s.ch[ch].op[1].tl = (value & 0xF) << 2;
			}
			break;
	}
}

static void OPLLSoundWrite(UINT32 address, UINT8 value)
{
	if(address == 0x9010)
	{
		OPLLSoundWriteAddr(address, value);
	}
	else if(address == 0x9030)
	{
		OPLLSoundWriteData(address, value);
	}
}

static void OPLLSoundReset(void)
{
    UINT32 i, w;
	double a;
    UINT32 cps;
	memset(&apu->ym2413s, 0, sizeof(OPLLSOUND));
	cps = DivFix(NES_BASECYCLES, 6 * 72 * SAMPLE_RATE, 18);
	for (i = 0; i < OPLL_CH_MAX; i++)
	{
		apu->ym2413s.ch[i].cps = cps;
		apu->ym2413s.ch[i].toneno = 0x10;
		apu->ym2413s.ch[i].fb = 31;
		apu->ym2413s.ch[i].output = 0 + (LOG_KEYOFF << 1);
		apu->ym2413s.ch[i].op[0].sintblp = sintbl;
		apu->ym2413s.ch[i].op[1].sintblp = sintbl;
	}

	w = 1 << (PG_BITS - 2);
	sintbl[w * 0] = 0 + (LOG_KEYOFF << 1);
	sintbl[w * 1] = 0;
	sintbl[w * 2] = 1 + (LOG_KEYOFF << 1);
	sintbl[w * 3] = 1;
	sintbld[w * 0] = 0 + (LOG_KEYOFF << 1);
	sintbld[w * 1] = 0;
	sintbld[w * 2] = 0 + (LOG_KEYOFF << 1);
	sintbld[w * 3] = 0 + (LOG_KEYOFF << 1);
	for (i = 1; i < w; i++)
	{
        UINT32 ua;
		a = sin((2 * M_PI * i) / (double)(w << 2)) * (1 << LOG_LIN_BITS);
        ua = (UINT32)((LOG_LIN_BITS - (log(a) / log(2))) * (1 << LOG_BITS));
		sintbl[w * 0 + i] = sintbl[w * 2 - i] = 0 + (ua << 1);
		sintbl[w * 2 + i] = sintbl[w * 4 - i] = 1 + (ua << 1);
		sintbld[w * 0 + i] = sintbld[w * 2 - i] = 0 + (ua << 1);
		sintbld[w * 2 + i] = sintbld[w * 4 - i] = 0 + (LOG_KEYOFF << 1);
	}
	/* AM(tremoro) */
	w = 1 << AM_BITS;
	for (i = 0; i < w; i++)
	{
		a = (1 + sin((2 * M_PI * i) / w)) * ((1 << LOG_BITS) * AM_DEPTH / 12);
        amtbl[i] = (UINT32)a;
	}
	apu->ym2413s.lfo[0].cps = cps;
    apu->ym2413s.lfo[0].spd = DivFix(NES_BASECYCLES, (UINT32)((6 * 72 * (1 << AM_BITS)) * 3.7), 18);
	apu->ym2413s.lfo[0].adrmask = w - 1;
	apu->ym2413s.lfo[0].table = amtbl;
	apu->ym2413s.lfo[0].output = apu->ym2413s.lfo[0].table[0];
	/* VIB */
	w = 1 << PM_BITS;
	for (i = 0; i < w; i++)
	{
		a = pow(2, PM_DEPTH * sin((2 * M_PI * i) / w) / 1200.0) * (1 << PM_SHIFT);
        pmtbl[i] = (UINT32)a;
	}
	apu->ym2413s.lfo[1].cps = cps;
    apu->ym2413s.lfo[1].spd = DivFix(NES_BASECYCLES, (UINT32)((6 * 72 * (1 << PM_BITS)) * 6.4), 18);
	apu->ym2413s.lfo[1].adrmask = w - 1;
	apu->ym2413s.lfo[1].table = pmtbl;
	apu->ym2413s.lfo[1].output = apu->ym2413s.lfo[1].table[0];
}

void VRC7SetTone(UINT8 *p)
{
	int i, j;
	for (i = 1; i < 16; i++)
	{
		for (j = 0; j < 8; j++)
		{
			oplltone[i][j] = *p++;
		}
	}
}

