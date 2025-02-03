/** fMSX: portable MSX emulator ******************************/
/**                                                         **/
/**                      pcats.c                            **/
/**                                                         **/
/** This file contains PC-AT dependent sound drivers.       **/
/**                                                         **/
/** fMSX      Copyright (C) Marat Fayzullin 1994,1995       **/
/** fMSX98-AT Copyright (C) MURAKAMI Reki 1995,1996         **/
/**      You are not allowed to distribute this software    **/
/**      commercially. Please, notify me, if you make any   **/
/**      changes to this file.                              **/
/*************************************************************/

//#include <stdio.h>
//#include <stdlib.h>
//#include <dos.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
extern int Verbose;

byte UseSB16=3;                   /* 1 if use Sound Blaster 16 */

#define CH_OPLL 0
#define CH_SCC 9
#define CH_PSG 14
#define CH_NOISE 17
#define CH_RHYTHM 6

#define VOICE_OPLL 0
#define VOICE_SCC 16
#define VOICE_PSG 17
#define VOICE_NOISE 18
#define VOICE_RHYTHM 19

word ch2reg[18] = {
  0x000, 0x001, 0x002, 0x008, 0x009, 0x00a, 0x010, 0x011, 0x012,
  0x100, 0x101, 0x102, 0x108, 0x109, 0x10a, 0x110, 0x111, 0x112
};

byte OPL3TL[16] = { 0, 1, 2, 3, 4, 6, 8, 10, 12, 16, 20, 28, 36, 44, 52, 63 };

int InitSound(int mode);
void TrashSound(void);

int OPL3Init();                   /*** Initialize OPL3          ***/
void OPL3Out(dword R,byte V);     /*** Write value into an OPL3 ***/
void PSGOutOPL3(byte R,byte V);   /*** OPL3 driver for PSG      ***/
byte PSGInOPL3(void);             /*** GAME port driver         ***/
void OPLLOutOPL3(byte R,byte V);  /*** OPL3 driver for OPLL     ***/
void SCCOutOPL3(byte R,byte V);   /*** OPL3 driver for SCC      ***/

dword OPL3Reg  = 0x220;           /* OPL3 I/O port             */
dword GameReg  = 0x201;           /* Game I/O port             */
dword JoyCal1,JoyCal2;
char JoySwap1 = 0, JoySwap2 = 0;


byte OPL3Voice[][9] = {
  { 0x32,0x32, 0x3f,0x3f, 0xF0,0xF0, 0xFF,0xFF, 0xf1 }, /* @63 */
  { 0x61,0x61, 0x12,0x3f, 0xB4,0x56, 0x14,0x17, 0xfE }, /* @02 */
  { 0x02,0x41, 0x15,0x3f, 0xA3,0xA3, 0x75,0x05, 0xfA }, /* @10 */
  { 0x31,0x11, 0x0E,0x3f, 0xD9,0xB2, 0x11,0xF4, 0xfA }, /* @00 */
  { 0x61,0x31, 0x20,0x3f, 0x6C,0x43, 0x18,0x26, 0xfE }, /* @03 */
  { 0xA2,0x30, 0xA0,0x3f, 0x88,0x54, 0x14,0x06, 0xfE }, /* @04 */
  { 0x31,0x34, 0x20,0x3f, 0x72,0x56, 0x0A,0x1C, 0xfA }, /* @05 */
  { 0x31,0x71, 0x16,0x3f, 0x51,0x52, 0x26,0x24, 0xfE }, /* @06 */
  { 0xE1,0x63, 0x0A,0x3f, 0xFC,0xF8, 0x28,0x29, 0xfD }, /* @09 */
  { 0x61,0x71, 0x0D,0x3f, 0x75,0xF2, 0x18,0x03, 0xfA }, /* @48 */
  { 0x42,0x44, 0x0B,0x3f, 0x94,0xB0, 0x33,0xF6, 0xfC }, /* @24 */
  { 0x01,0x00, 0x06,0x3f, 0xA3,0xE2, 0xF4,0xF4, 0xfD }, /* @14 */
  { 0xF9,0xF1, 0x24,0x3f, 0x95,0xD1, 0xE5,0xF2, 0xfC }, /* @16 */
  { 0x40,0x31, 0x89,0x3f, 0xC7,0xF9, 0x14,0x04, 0xfC }, /* @23 */
  { 0x11,0x11, 0x11,0x3f, 0xC0,0xB2, 0x01,0xF4, 0xf0 }, /* @33 */
  { 0x23,0x43, 0x0F,0x3f, 0xDD,0xBF, 0x4A,0x05, 0xfE }, /* @12 */
  { 0x32,0x34, 0x0E,0x3f, 0xF0,0xF0, 0x0F,0x0F, 0xf8 }, /* SCC tone */
  { 0x32,0x36, 0x1e,0x3f, 0xF0,0xF0, 0x0F,0x0F, 0xfa }, /* PSG tone */
  { 0x22,0x21, 0x00,0x3f, 0xF0,0xF0, 0x0F,0x0F, 0xfe }, /* PSG noise */
  { 0x00,0x00, 0x00,0x3f, 0xf9,0xf7, 0xf9,0xf7, 0xf8 }, /* bass drum */
  { 0x00,0x00, 0x3f,0x3f, 0xf8,0xf6, 0xf8,0xf6, 0xf0 }, /* snare & hi-hat */
  { 0x04,0x01, 0x3f,0x3f, 0xf7,0xf6, 0xf7,0xf6, 0xf0 }  /* tom & cymbal */
};




void OPL3Out(dword R,byte V)
{
  dword reg;
  int i;

  if(!OPL3Reg) return;
  reg=(R>0xff)? OPL3Reg+2:OPL3Reg;
  outportb(reg,R&0xff);
  for(i=0;i<6;i++) inportb(OPL3Reg);
  outportb(reg+1,V);
  for(i=0;i<35;i++) inportb(OPL3Reg);
}



void OPL3SetVoice(byte ch, byte no)
{
  byte i, j;
  dword reg;

  reg = ch2reg[ch] + 0x20;
  for (i = 0, j = 0; i < 4; i++) {
    OPL3Out(reg, OPL3Voice[no][j++]);
    OPL3Out(reg + 3, OPL3Voice[no][j++]);
    reg += 0x20;
  }
  OPL3Out((ch > 8) ? ch - 9 + 0x1c0 : ch + 0xc0, OPL3Voice[no][8]);
}

int OPL3Init()
{
  byte ch,C,i,vn;
  dword I,reg,count;
  byte buf0,buf1;
  char *s;

/*
  i=0;
  while(Env[i]!=NULL) {
    if(sscanf(Env[i++],"BLASTER=A%x ",&OPL3Reg)==1) {
      break;
    }
  }
*/
  if((s=getenv("BLASTER"))!=0) {
    while(*s){
      if(toupper(*s++)=='A') OPL3Reg=strtol(s,0,16);
    }
  }

  OPL3Out(4,0x60);
  OPL3Out(4,0x80);
  buf0 = inportb(OPL3Reg);
  OPL3Out(2,0xff);
  OPL3Out(4,0x21);
  for(i=0;i<120;i++) buf1 = inportb(OPL3Reg);
  OPL3Out(4,0x60);
  OPL3Out(4,0x80);
  if(((buf0 & 0xe0) == 0x00) && ((buf1 & 0xe0) == 0xc0)) {
    OPL3Out(0x105,0x01);
    OPL3Out(0x104,0x00);
    OPL3Out(0x01,0x00);
    OPL3Out(0x08,0x00);
    for(I=0x40;I<0x56;I++) {
      OPL3Out(I,0x3f);
      OPL3Out(I+0x100,0x3f);
    }
    /*** set SCC/PSG tone ***/
    for (ch = CH_SCC; ch < CH_SCC + 5; ch++) {
      OPL3SetVoice(ch, VOICE_SCC);
    }
    for (ch = CH_PSG; ch < CH_PSG + 3; ch++) {
      OPL3SetVoice(ch, VOICE_PSG);
    }
    OPL3SetVoice(CH_NOISE, VOICE_NOISE);

    for (i = 0; i < 18; i++) {
      OPL3Out(ch2reg[i] + 0xe0,0x00); /* waveform */
    }
    OPL3Out(0xbd, 0x00); /* rhythm off */

    /* silent */
    for(I=0xB0;I<0xB9;I++) {
      OPL3Out(I,0);
      OPL3Out(I+0x100,0);
    }
    if(Verbose) printf(" Found YMF-262 at %04Xh",OPL3Reg);
  } else {
    OPL3Reg = 0;
    if(Verbose) printf(" Not Found FM-Synthesizer");
  }

  /* joystick support */
  JoyCal1=JoyCal2=0;
  count=0;
  outportb(GameReg,0x0f);
  while(count++<65536) {
    if(!(inportb(GameReg)&0x01)) break;
  }
  if(count<65536) {
     JoyCal1=count;
     if(Verbose) {
       printf(", Joy-A");
       if(JoySwap1) printf("(sw)");
     }
  }
  count=0;
  outportb(GameReg,0x0f);
  while(count++<65536) {
    if(!(inportb(GameReg)&0x04)) break;
  }
  if(count<65536) {
     JoyCal2=count;
     if(Verbose) {
       printf(", Joy-B");
       if(JoySwap2) printf("(sw)");
     }
  }

  if(Verbose) printf(". ");
  return 1;
}


/****************************************************************/
/*** Write number into given PSG register.                    ***/
/****************************************************************/

  static byte keyon[4] = {0, 0, 0, 0};

void SetSound(int Channel,int Type)      /* Set sound type   */
{
}
void SetChannels(int Volume,int Toggle)
    /* Set master volume (0..255) and turn channels on/off.  */
    /* Each bit in Toggle corresponds to a channel (1=on).   */
{
}
void Sound(int Channel,int Freq,int Volume)
    /* Generate sound of given frequency (Hz) and volume     */
    /* (0..255) via given channel.                           */
{
}

void SetFreq(int ch,int freq)
{
	byte B;
	long f;

      B = (freq > 6208) ? 8 : (freq > 3104) ? 7 : (freq > 1552) ? 6
        : (freq >  776) ? 5 : (freq >  388) ? 4 : (freq >  194) ? 3
        : (freq >   97) ? 2 : 1;
      f = (freq << (20 - B) ) / 49716;
      OPL3Out(ch + 0x1a5, f & 0xff);
      keyon[ch] = (keyon[ch] & 0x20) | ((f >> 8) & 0x03) | ((B - 1) << 2);
      OPL3Out(ch + 0x1b5, keyon[ch]);
}

void SetVolume(int ch,int V)
{
	if (V==0) {
        keyon[ch] &= 0xdf;
        OPL3Out(ch + 0x1b5, keyon[ch]);
	} else {
        if ((keyon[ch] & 0x20) == 0) {
          keyon[ch] |= 0x20;
          OPL3Out(ch + 0x1b5, keyon[ch]);
        }
        OPL3Out(ch2reg[ch + CH_PSG] + 0x43, OPL3TL[15 - (V>>4)]);
	}
}

int InitSound(int mode)
{
  int status=1;

  if(UseSB16) status=OPL3Init();
  return status;
}


void TrashSound(void)
{
  byte i;

  if(UseSB16) {
    for(i=0x40;i<0x56;i++) {
      OPL3Out(i,0x3f);
      OPL3Out(i+0x100,0x3f);
    }
    for(i=0xB0;i<0xB9;i++) {
      OPL3Out(i,0);
      OPL3Out(i+0x100,0);
    }
  }
}

