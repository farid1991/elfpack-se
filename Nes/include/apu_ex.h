
/*
** ExSound by TAKEDA, toshiya
**
** original: nezp0922
*/

#include <math.h>

// ----------------------------------------------------------------------------
// common

// ex_chip	=  0; none
//		=  1; VRC6
//		=  2; VRC7
//		=  4; FDS
//		=  8; MMC5
//		= 16; N106
//		= 32; FME7
//		= 64; J106 (reserved)

//#define SAMPLE_RATE 44100
#define SAMPLE_RATE apu->sample_rate
#define NES_BASECYCLES (21477270)
#define M_PI 3.14159265358979323846

#define LOG_BITS 12
#define LIN_BITS 6
#define LOG_LIN_BITS 30

static UINT32 lineartbl[(1 << LIN_BITS) + 1];
static UINT32 logtbl[1 << LOG_BITS];

UINT32 LinearToLog(INT32 l)
{
	return (l < 0) ? (lineartbl[-l] + 1) : lineartbl[l];
}

INT32 LogToLinear(UINT32 l, UINT32 sft)
{
    INT32 ret;
    UINT32 ofs;
	sft += (l >> 1) >> LOG_BITS;
	if (sft >= LOG_LIN_BITS) return 0;
	ofs = (l >> 1) & ((1 << LOG_BITS) - 1);
	ret = logtbl[ofs] >> sft;
	return (l & 1) ? -ret : ret;
}

void LogTableInitialize(void)
{
    static INT32 initialized = 0;
    INT32 i;
	double a;
	if (initialized) return;
	initialized = 1;
	for (i = 0; i < (1 << LOG_BITS); i++)
	{
		a = (1 << LOG_LIN_BITS) / pow(2, i / (double)(1 << LOG_BITS));
        logtbl[i] = (UINT32)a;
	}
	lineartbl[0] = LOG_LIN_BITS << LOG_BITS;
	for (i = 1; i < (1 << LIN_BITS) + 1; i++)
	{
        UINT32 ua;
		a = i << (LOG_LIN_BITS - LIN_BITS);
        ua = (UINT32)((LOG_LIN_BITS - (log(a) / log(2))) * (1 << LOG_BITS));
		lineartbl[i] = ua << 1;
	}
}

static UINT32 DivFix(UINT32 p1, UINT32 p2, UINT32 fix)
{
    UINT32 ret;
	ret = p1 / p2;
	p1  = p1 % p2;/* p1 = p1 - p2 * ret; */
	while (fix--)
	{
		p1 += p1;
		ret += ret;
		if (p1 >= p2)
		{
			p1 -= p2;
			ret++;
		}
	}
	return ret;
}

#include "apu/vrc6.h"
#include "apu/vrc7.h"
//#include "nes_fds_new.cpp"
#include "apu/fds.h"
#include "apu/mmc5.h"
#include "apu/n106.h"
#include "apu/fme7.h"

