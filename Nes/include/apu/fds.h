/*
** Nintendo FDS ExSound by TAKEDA, toshiya
**
** original: s_fds.c in nezp0922
*/

/* modifyed by stun 2000.10.24 */
/* bug fix  by stun 2000.11.27 */

static INT32 FDSSoundRender(void)
{
	FDS_FMOP *pop;
    UINT32 vol;

	pop = &apu->fdssound.op[0];
	if(pop->timer>=0) pop->timer--;
	if(pop->timer==0)
	{
		apu->fdssound.op[1].sweep=pop->last_spd;
		pop->envmode=1;
	}
	if(pop->envmode==1)
	{
		apu->fdssound.op[1].spd = apu->fdssound.op[1].sweep;
	}
	else if(pop->envmode & 0x80)
	{
        UINT32 sweeps=(UINT32)apu->fdssound.op[1].sweep;
		pop->envphase++;
		if (!(pop->envmode & 0x40))
		{
            if((UINT32)pop->envphase==pop->envspd)
			{
				pop->envphase=0;
				if(apu->fdssound.op[1].spd <sweeps)
				{
					apu->fdssound.op[1].spd += pop->volume;
					if(apu->fdssound.op[1].spd >sweeps)
						apu->fdssound.op[1].spd=sweeps;
				}
			}
		}
		else
		{
            if((UINT32)pop->envphase==pop->envspd)
			{
				pop->envphase=0;
				if(apu->fdssound.op[1].spd > sweeps/2)
				{
					apu->fdssound.op[1].spd -= pop->volume; 
					if(apu->fdssound.op[1].spd < sweeps)
						apu->fdssound.op[1].spd=sweeps;
				}
			}
		}
	}
	vol = pop->volume;
	if (pop->sweep)
	{
		vol += pop->sweep;
		if (vol < 0)
			vol = 0;
		else if (vol > 0x3f)
			vol = 0x3f;
	}
	//pop->envout = LinearToLog(vol);
	pop->envout = LinearToLog(0);
	pop = &apu->fdssound.op[1];
	{
        UINT32 vol;
		if (pop->envmode && apu->fdssound.fade)
		{
			pop->envphase -= apu->fdssound.cps >> (11 - 1);
			if (pop->envmode & 0x40)
				while (pop->envphase < 0)
				{
					pop->envphase += pop->envspd;
					pop->volume += (pop->volume < 0x1f);
				}
			else
				while (pop->envphase < 0)
				{
					pop->envphase += pop->envspd;
					pop->volume -= (pop->volume > 0x00);
				}
		}
		vol = pop->volume;
		pop->envout = LinearToLog(vol);
	}

	apu->fdssound.op[1].envout += apu->fdssound.mastervolume;

	apu->fdssound.cycles -= apu->fdssound.cps;
	while (apu->fdssound.cycles < 0)
	{
		apu->fdssound.cycles += 1 << 23;
		apu->fdssound.output = 0;
		for (pop = &apu->fdssound.op[0]; pop < &apu->fdssound.op[2]; pop++)
		{
			if (!pop->spd || !pop->enable)
			{
				apu->fdssound.output = 0;
				continue;
			}
			pop->phase += pop->spd + apu->fdssound.output;
			apu->fdssound.output = LogToLinear(pop->envout + pop->wave[(pop->phase >> (23 - 1)) & 0x3f], pop->outlvl);
		}
	}
	if (apu->fdssound.mute) return 0;
	return apu->fdssound.output;
}

static void FDSSoundVolume(UINT32 volume)
{
	apu->fdssound.mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

static void FDSSoundWrite(UINT32 address, UINT8 value)
{
	if (0x4040 <= address && address <= 0x407F)
	{
        apu->fdssound.op[1].wave[address - 0x4040] = LinearToLog(((INT32)value & 0x3f) - 0x20);
	}
	else if (0x4080 <= address && address <= 0x408F)
	{
		int ch = (address < 0x4084);
		FDS_FMOP *pop = &apu->fdssound.op[ch];
		apu->fdssound.reg[address - 0x4080] = value;
		switch (address & 15)
		{
			case 0:
				if (value & 0x80)
				{
					pop->volume = (value & 0x3f);
					pop->envmode = 0;
				}
				else
				{
					pop->envspd = ((value & 0x3f) + 1) << 23;
					pop->envmode = 0x80 | value;
				}
				break;
			case 4:
				if(value & 0x80)
				{
                    INT32 a=apu->fdssound.op[1].spd;
                    INT32 b=apu->fdssound.op[1].sweep;
					pop->timer=(0x3f-(value & 0x3f)) << 10;
					if(pop->timer==0) pop->timer=1;
					pop->last_spd=a*(0x3f-(value & 0x3f))/0x3f+
								  b*(value & 0x3f)/0x3f;
				}
				else if(apu->fdssound.op[1].sweep)
				{
					pop->envspd = (value & 0x3f) << 5;
					if((value & 0x3f)==0) pop->envspd=1;
					pop->envphase = 0;
					pop->envmode = 0x80 | (value & 0x40) ;
					pop->volume=abs(apu->fdssound.op[1].sweep - apu->fdssound.op[1].spd);
					pop->volume/=pop->envspd;
					if((value & 0x3f)==0) pop->envmode=1;
				}
				apu->fdssound.waveaddr = 0;
				break;
			case 1:
				if ((value & 0x7f) < 0x60)
					apu->fdssound.op[0].sweep = value & 0x7f;
				else
                    apu->fdssound.op[0].sweep = ((INT32)value & 0x7f) - 0x80;
				break;
			case 5:
				if (!value) break;
				if ((value & 0x7f) < 0x60)
				{
                    apu->fdssound.op[1].sweep = (INT32)apu->fdssound.op[1].spd+
						((apu->fdssound.op[1].spd * (value & 0x7f))>>5);
				}
				else
				{
                    apu->fdssound.op[1].sweep = (INT32)apu->fdssound.op[1].spd-
                        (((apu->fdssound.op[1].spd) * (((INT32)value & 0x7f) - 0x80)) >> 5);
				}
				break;
			case 2:
				pop->spd &= 0x00000F00 << 7;
				pop->spd |= (value & 0xFF) << 7;
				apu->fdssound.op[0].envmode = 0;
				apu->fdssound.op[0].timer=0;
				break;
			case 6:
				pop->spd &= 0x00000F00 << 7;
				pop->spd |= (value & 0xFF) << 7;
				pop->envmode = 0;
				break;
			case 3:
				pop->spd &= 0x000000FF << 7;
				pop->spd |= (value & 0x0F) << (7 + 8);
				pop->enable = !(value & 0x80);
				apu->fdssound.op[0].envmode = 0;
				apu->fdssound.op[0].timer=0;
				break;
			case 7:
				pop->spd &= 0x000000FF << 7;
				pop->spd |= (value & 0x0F) << (7 + 8);
				pop->enable = !(value & 0x80);
				apu->fdssound.waveaddr = 0;
				break;
			case 8:
				{
                    static INT8 lfotbl[8] = { 0,1,2,3,-4,-3,-2,-1 };
                    UINT32 v = LinearToLog(lfotbl[value & 7]);
					apu->fdssound.op[0].wave[apu->fdssound.waveaddr++] = v;
					apu->fdssound.op[0].wave[apu->fdssound.waveaddr++] = v;
					if (apu->fdssound.waveaddr == 0x40)
					{
						apu->fdssound.waveaddr = 0;
					}
				}
				break;
			case 9:
				apu->fdssound.op[0].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10 - (value & 3);
				if(value & 0x80) apu->fdssound.mute=1;
				else             apu->fdssound.mute=0;
				break;
			case 10:
				apu->fdssound.op[1].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10 - (value & 3);
				if(value & 0x80) apu->fdssound.fade=1;
				else             apu->fdssound.fade=0;
				break;
		}
	}
}

static void FDSSoundWriteCurrent(UINT32 address, UINT8 value)
{
	if (0x4080 <= address && address <= 0x408F)
	{
		apu->fdssound.reg_cur[address - 0x4080] = value;
	}
}

static UINT8 FDSSoundRead(UINT32 address)
{
	if (0x4090 <= address && address <= 0x409F)
	{
		return apu->fdssound.reg_cur[address - 0x4090];
	}
	return 0;
}

static void FDSSoundReset(void)
{
    INT8 i;
	FDS_FMOP *pop;
	memset(&apu->fdssound, 0, sizeof(FDSSOUND));
	apu->fdssound.cps = DivFix(NES_BASECYCLES, 12 * (1 << 1) * SAMPLE_RATE, 23);
	for (pop = &apu->fdssound.op[0]; pop < &apu->fdssound.op[2]; pop++)
	{
		pop->enable = 1;
	}
	apu->fdssound.op[0].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10;
	apu->fdssound.op[1].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10;
	for (i = 0; i < 0x40; i++)
	{
		apu->fdssound.op[1].wave[i] = LinearToLog((i < 0x20)?0x1f:-0x20);
	}
}

