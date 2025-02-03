/*
** Konami VRC6 ExSound by TAKEDA, toshiya
**
** original: s_vrc6.c in nezp0922
*/

static INT32 VRC6SoundSquareRender(VRC6_SQUARE *ch)
{
    UINT32 output;
	if (ch->update)
	{
		if (ch->update & (2 | 4))
		{
			ch->spd = (((ch->regs[2] & 0x0F) << 8) + ch->regs[1] + 1) << 18;
		}
		ch->update = 0;
	}

	if (!ch->spd) return 0;

	ch->cycles -= ch->cps;
	while (ch->cycles < 0)
	{
		ch->cycles += ch->spd;
		ch->adr++;
	}
	ch->adr &= 0xF;

	if (ch->mute || !(ch->regs[2] & 0x80)) return 0;

	output = LinearToLog(ch->regs[0] & 0x0F) + apu->vrc6s.mastervolume;
	if (!(ch->regs[0] & 0x80) && (ch->adr < ((ch->regs[0] >> 4) + 1)))
	{
#if 1
		return 0;	/* and array gate */
#else
		output++;	/* negative gate */
#endif
	}
	return LogToLinear(output, LOG_LIN_BITS - LIN_BITS - 16 - 1);
}

static INT32 VRC6SoundSawRender(VRC6_SAW *ch)
{
    UINT32 output;

	if (ch->update)
	{
		if (ch->update & (2 | 4))
		{
			ch->spd = (((ch->regs[2] & 0x0F) << 8) + ch->regs[1] + 1) << 18;
		}
		ch->update = 0;
	}

	if (!ch->spd) return 0;

	ch->cycles -= ch->cps;
	while (ch->cycles < 0)
	{
		ch->cycles += ch->spd;
		ch->output += (ch->regs[0] & 0x3F);
		if (7 == ++ch->adr)
		{
			ch->adr = 0;
			ch->output = 0;
		}
	}

	if (ch->mute || !(ch->regs[2] & 0x80)) return 0;

	output = LinearToLog((ch->output >> 3) & 0x1F) + apu->vrc6s.mastervolume;
	return LogToLinear(output, LOG_LIN_BITS - LIN_BITS - 16 - 1);
}

static INT32 VRC6SoundRender(void)
{
    INT32 accum = 0;
	accum += VRC6SoundSquareRender(&apu->vrc6s.square[0]);
	accum += VRC6SoundSquareRender(&apu->vrc6s.square[1]);
	accum += VRC6SoundSawRender(&apu->vrc6s.saw);
	return accum;
}

static void VRC6SoundVolume(UINT32 volume)
{
	apu->vrc6s.mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

static void VRC6SoundWrite9000(UINT32 address, UINT8 value)
{
	apu->vrc6s.square[0].regs[address & 3] = value;
	apu->vrc6s.square[0].update |= 1 << (address & 3); 
}
static void VRC6SoundWriteA000(UINT32 address, UINT8 value)
{
	apu->vrc6s.square[1].regs[address & 3] = value;
	apu->vrc6s.square[1].update |= 1 << (address & 3); 
}
static void VRC6SoundWriteB000(UINT32 address, UINT8 value)
{
	apu->vrc6s.saw.regs[address & 3] = value;
	apu->vrc6s.saw.update |= 1 << (address & 3); 
}

static void VRC6SoundWrite(UINT32 address, UINT8 value)
{
	switch(address & 0xF000) {
		case 0x9000:
			VRC6SoundWrite9000(address, value);
			break;
		case 0xA000:
			VRC6SoundWriteA000(address, value);
			break;
		case 0xB000:
			VRC6SoundWriteB000(address, value);
			break;
	}
}

static void VRC6SoundSquareReset(VRC6_SQUARE *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 12 * SAMPLE_RATE, 18);
}

static void VRC6SoundSawReset(VRC6_SAW *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 24 * SAMPLE_RATE, 18);
}

static void VRC6SoundReset(void)
{
	memset(&apu->vrc6s, 0, sizeof(VRC6SOUND));
	VRC6SoundSquareReset(&apu->vrc6s.square[0]);
	VRC6SoundSquareReset(&apu->vrc6s.square[1]);
	VRC6SoundSawReset(&apu->vrc6s.saw);
}

