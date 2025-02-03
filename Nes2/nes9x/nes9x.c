/*
     NES 9x   1.0 Beta Release
     Programmed by NES 9X Team
     nes9x@cjb.net

*/

#include "m6502.h"
#include "nes9x.h"

#include "../gui.h"

#include "palette.h"

#define CD_HEADER_MODE
#include "../config_data.c"
#undef CD_HEADER_MODE


#define VBlankON        (PPU[0]&0x80)
#define SpHitON         (PPU[0]&0x40)
#define Sprite16        (PPU[0]&0x20)
#define BGAdrHI         (PPU[0]&0x10)
#define SpAdrHI         (PPU[0]&0x08)
#define INC32           (PPU[0]&0x04)
#define NameTable       (PPU[0]&0x3)

#define BGcolor         (PPU[1]&0xE0)
#define SpriteON        (PPU[1]&0x10)
#define ScreenON        (PPU[1]&0x08)
#define SpClip          (PPU[1]&0x04)
#define BGClip          (PPU[1]&0x02)
#define DispMono        (PPU[1]&0x01)
#define	PPU_status	PPU[2]

#define	PPU_InVBlank	0x80
#define	PPU_SpHit	0x40
#define	PPU_SpMAX	0x20
#define	PPU_WriteIgnore	0x10

#define	PSG_BASE	111860

#define	NODATA	0xff

#define	MinLine	20/*8*/
#define	MaxLine	260/*231*/

#define	FC_W	256
#define	FC_H	240


byte PPU[4],PSG[0x18],ScrollX,ScrollY;
byte ScrollX2,ScrollY2,sndlen[4];
byte *Page[8],*VPage[8];
byte RAM[0x800],SPRAM[0x100];
byte *ROM,*VROM,*WRAM,*VRAM;

word VAddr;
word VAddr_temp;
byte VAddr_write;

byte joy_1,joy_2,joy_readbit;

int scanline, palchange, scrollchange;

int Debug, vmode;
//int sndcount,sndPeriod = 244;

int ROM_size;
int VROM_size;
int ROM_type;
int MMC_type;

int Mirroring,Mirrorxor;

const int IPeriod = 115;
const int scanlines_per_frame = 262;//244;

void IO_write(word A,byte V);
byte IO_read(word A);
void RefreshLine(byte Y1,byte Y2);
void RefreshSprite(byte Y1,byte Y2);
void RefreshScreen(void);

#define	VRAMADR(V)	&VPage[(V)>>10][(V)]

byte vram_read_buffer = 0;

int spal[32];

//void KeyOff(int ch){SetVolume(ch,0);}

byte _Rd6502(word A)
{
  switch(A>>13)
  {
  case 0:
    return RAM[A&0x7FF];
  case 3:
    return WRAM[A&0x1FFF];
  case 1:
  case 2:
    //	if (A!=0x2002) printf("r%x ",A);
    return IO_read(A);
  default:
    return Page[A>>13][A];
  }
}

void _Wr6502(word A,byte V)
{
  switch(A>>13)
  {
  case 0:
#if 1
    RAM[A&0x7FF]=V;
#else
    {byte *p=&RAM[A&0x7FF];
    p[0x0000]=p[0x0800]=p[0x1000]=p[0x1800]=V;
    }
#endif
    break;
  case 3:
    WRAM[A&0x1FFF]=V; break;
  case 1:
  case 2:
    //	printf("w%x,%x\n",A,V);
    IO_write(A,V);
    break;
  default:
    //	printf("mapper write $%04x,$%02x\n",A,V);
    MMC_write(A,V);
    break;
  }
}

static byte IO_read(word A)
{
  byte ret;
  switch(A)
  {
  case 0x2000:
    return PPU[0];
  case 0x2001:
    return PPU[1];
  case 0x2002:
    ret = PPU_status;
    PPU_status &= ~(PPU_InVBlank | PPU_SpHit );
    VAddr_write = 0;
    return ret;
  case 0x2004:
    return SPRAM[PPU[3]++];
  case 0x2007:
    ret = vram_read_buffer;

    VAddr&=0x3FFF;
    if (VAddr<0x2000) vram_read_buffer = *VRAMADR(VAddr);
    else vram_read_buffer = VRAM[VAddr];

    if (INC32) VAddr+=32;
    else VAddr++;

    return ret;
    /* PSG */
  case 0x4000:
  case 0x4001:
  case 0x4002:
  case 0x4003:
  case 0x4004:
  case 0x4005:
  case 0x4006:
  case 0x4007:
  case 0x4008:
  case 0x4009:
  case 0x400A:
  case 0x400B:
  case 0x400C:
  case 0x400D:
  case 0x400E:
  case 0x400F:
  case 0x4010:
  case 0x4011:
  case 0x4012:
  case 0x4013:
  case 0x4015:
    return PSG[A-0x4000];
  case 0x4016:
    {
      if( joy_readbit == 19 )
        ret = 1;
      else if( (joy_readbit > 7) && (joy_readbit < 16) ) //Joy 3
        ret = 0;
      else if( (joy_readbit > 15) && (joy_readbit < 23) ) //Ignore
        ret = 0;
      else if( joy_readbit == 23 )
      {
        joy_readbit = 0;
        return 0;
      }
      else //Joy 1
        ret = ( (joy_1>>joy_readbit)&1 ) | 0x40;

      joy_readbit++;
      return ret;
    }
  case 0x4017:
    //ret =(joy_2>>joy_readbit)&1;
    //joy_readbit=(joy_readbit+1)&7;
    return 0x40;//ret;
  default:
    //printf("ignore read $%04x\n",A);
    return NODATA;
    //return RAM[A];
  }
}

static void IO_write(word A,byte V)
{
  switch(A)
  {
  case 0x2000:
    PPU[0]=V;
    VAddr_temp = (VAddr_temp & ~(3 << 10)) | ((V & 3) << 10);
    break;
  case 0x2001:PPU[1]=V;break;
  case 0x2003:PPU[3]=V;break;
  case 0x2004:SPRAM[PPU[3]++]=V; break;
  case 0x2005:
    if (!VAddr_write)
    {
      VAddr_write = 1;
      VAddr_temp = (VAddr_temp & ~0x1F) | ((V >> 3) & 0x1F);
      ScrollX2 = V;
    }
    else
    {
      VAddr_write = 0;
      VAddr_temp = (VAddr_temp & ~(0x1F << 5)) | (((V >> 3) & 0x1F) << 5);
      VAddr_temp = (VAddr_temp & ~(7 << 12)) | ((V & 7) << 12);
      ScrollY2 = V;
    }

    scrollchange = TRUE;
    break;
  case 0x2006:
    if( !VAddr_write )
    {
      VAddr_temp = (VAddr_temp & 0xFF) | ((V & 0x3F) << 8);
      VAddr_write = 1;
    }
    else
    {
      VAddr_temp = (VAddr_temp & ~0xFF) | V;
      VAddr = VAddr_temp;
      VAddr_write = 0;
    }
    break;
  case 0x2007:
    if (PPU_status&PPU_WriteIgnore) break;

    VAddr&=0x3FFF;
    VRAM[VAddr]=V;

    if ( (VAddr&0x3000) == 0x2000 )
      VRAM[VAddr^Mirrorxor]=V;
    else if (VAddr>=0x3F00)
    {
      if (VAddr==0x3F00)
        VRAM[0x3F04]=VRAM[0x3F08]=VRAM[0x3F0C]=V;
      else if (VAddr==0x3F10)
        VRAM[0x3F14]=VRAM[0x3F18]=VRAM[0x3F1C]=V;

      palchange = TRUE;
    }
    else if( VAddr >= 0x3000 )
    {
      VRAM[VAddr^0x1000]=V;
    }

    if (INC32) VAddr+=32;
    else VAddr++;

    break;
  case 0x4000:
  case 0x4004:
  case 0x4008:
  case 0x400C:
    //	printf("%d ",V&0x20);
    PSG[A-0x4000]=V;
    break;
  case 0x4001:
  case 0x4005:
  case 0x4009:
  case 0x400D:
    A-=0x4000;
    PSG[A]=V;
    //SetVolume(A/4,(V&7)<<5);
    break;
  case 0x4002:
  case 0x4006:
  case 0x400A:
  case 0x400E:
    PSG[A-0x4000]=V;
    break;
  case 0x4003:
  case 0x4007:
  case 0x400B:
  case 0x400F:
    {
      static char lentbl[]= {
        5,10,20,40,80,30,7,14,
        8,12,24,48,96,36,8,16,
      };
      int freq,ch,len;
      A-=0x4000;
      PSG[A]=V;
      ch = A/4;
      if (PSG[A-3]&0x20)
      {
        PSG[0x15]|=(1<<ch);
        len = V>>4;
        if (V&8)
        {
          if (len==0) len=128;
        }
        else len=lentbl[len];
      }
      else len = 0;
      sndlen[ch]=len;
      //freq=((V&7)<<8)|PSG[A-1];
      //freq=PSG_BASE/(freq+1);
      //SetFreq(ch,freq);
      //SetVolume(ch,(PSG[A-2]&7)<<5);
    }
    break;
  case 0x4010:
  case 0x4011:
  case 0x4012:
  case 0x4013:
    PSG[A-0x4000]=V;
    break;
  case 0x4014:
    memcpy(SPRAM, V < 0x80 ? &RAM[(V&7)<<8] : &Page[V>>(13-8)][V<<8], 256);
    break;
  case 0x4015:
    {
      int ch;
      for(ch=0;ch<4;ch++) {
        if ((V>>ch)&1) {
        } else {
          sndlen[ch]=0;
          //KeyOff(ch);
        }
      }
      PSG[0x15]=V;
    }
    break;
  case 0x4016:
    if ((PSG[0x16]&1) && !(V&1))
      joy_readbit = 0;
    PSG[0x16]=V;
    break;
  case 0x4017: //???
    //PSG[0x17]=V;
    break;
  default:
    //RAM[A]=V;
    //printf("ignore write $%04x,$%02x\n",A,V);
    break;
  }
}

int Sprite0y(void)
{//MAXELEMS
  int y = SPRAM[0];
  return y+1;//;
}

byte Loop6502(M6502 *R)
{
  static int UCount = 0;
  //static int ACount = 0;
  static int prevline;
  static int hity;
  //int ch;

  int ret = INT_NONE;

  scanline=(scanline+1)%scanlines_per_frame;
  //printf("scan %d\n",scanline);
#if 0
  if (--sndcount<=0) {
    sndcount=sndPeriod;
    for(ch=0;ch<4;ch++) {
      if (sndlen[ch]) {
        if (--sndlen[ch]==0) {
          //KeyOff(ch);
          PSG[0x15]&=~(1<<ch);
        }
      }
    }
  }
#endif
  /*
  if( SpriteON || ScreenON )
  {
    if( scanline == 0 )
      VAddr = VAddr_temp;
  VAddr = (VAddr & (~0x1F & ~(1 << 10))) | (VAddr_temp & (0x1F | (1 << 10));
  }
  */

  if (scanline == MinLine)
  {
    PPU_status &= ~( PPU_InVBlank | PPU_SpMAX );
    prevline = MinLine;
    hity = Sprite0y();
  }
  else if (scanline == MaxLine)
  {
    if (UCount) UCount--;
    else
    {
      RefreshLine(prevline-MinLine,scanline-MinLine);
      prevline = scanline+1;
      UCount = frameskip;
      RefreshScreen();
    }
  }

  if (scanline >= MinLine && scanline <= MaxLine)
  {
    if (scrollchange)
    {
      RefreshLine(prevline-MinLine,scanline-1-MinLine);
      prevline = scanline;
      ScrollX = ScrollX2;
      ScrollY = ScrollY2;
      scrollchange = FALSE;
    }
    if (SpriteON && scanline == hity)
    {
      PPU_status |= PPU_SpHit;
      if (SpHitON) ret = INT_NMI;
    }
  }
  else if (scanline == MaxLine+1 /*240*/)
  {
      PPU_status = (PPU_status&~PPU_SpHit)|PPU_InVBlank;
      PSG[0x16] &= 0xFD;
      if (VBlankON) ret = INT_NMI;
  }
  /*
  if (SpriteON || ScreenON)
  {
    VAddr += 0x1000;
    if((VAddr & (7 << 12)) == 0)
    {
      VAddr += 32;
      switch (VAddr & (0x1F << 5))
      {
      case 0:
        VAddr -= (1 << 10);
        break;
      case (30 << 5):
        VAddr = (VAddr - (30 << 5)) ^ (1 << 11);
        break;
      }
    }
  }*/

  return ret;
}

#define	PAL(c)	((c)+cc)
//#define	PAL(c)	R[c]
extern byte *XBuf;
#define	WIDTH	(256+16)
#define	HEIGHT	240
byte Black = 0;
#define	Pal	(VRAM+0x3F00)
#define	SPal	(VRAM+0x3F10)

void RefreshLine(byte Y1,byte Y2)
{
  byte *nametbl/*,*ptntbl*/;
  byte Line;
  int ntbl,vofs,h,x,y,X1/*,X2*/;
  byte *P;

  byte /*ZBuf[35],*/*PP;
  //	printf("%d-%d ",Y1,Y2);
  Y2++;
  PP = XBuf + WIDTH*(HEIGHT-FC_H)/2 + (WIDTH-FC_W)/2 + WIDTH*Y1;

  if(!ScreenON)
    memset(PP,Black,(Y2-Y1)*WIDTH);
  else
  {
    vofs = 0;
    if (BGAdrHI) vofs = 0x1000;

    y = Y1+ScrollY;

    h = 8-(y&7);
    vofs += y&7;

    y/=8;

    PP -= ScrollX&7;
    ntbl = 0x2000 + NameTable*0x400;

    //  printf("\n");
    for( Line = Y1; Line < Y2; y++)
    {
      x = ScrollX/8;

      if (y >= 30)
      {
        y -= 30;
        ntbl ^= 0x800;
      }
      //printf("%d ",y);

      nametbl = VRAM + ntbl;
      for( X1 = 0; X1 < 33; X1++, PP += 8 )
      {
        byte *C, /**R,*/ c1, c2, cc;
        byte no, atr;
        int vadr, i;

        no  = nametbl[x + y*32];
        atr = nametbl[0x3C0 + (x/4) + (y/4)*8];

        cc = ( ( atr >> ( (x&2) + (y&2)*2 ) )&3 )*4;
        vadr = no*16 + vofs;
        C = VRAMADR(vadr);
	//	R = &Pal[cc];
        P = PP;
        for( i=0; i < h; i++, P+=WIDTH, C++)
        {
          c1 = ( ( C[0]>>1 )&0x55 ) | ( C[8]&0xAA );
          c2 = ( C[0]&0x55 ) | ( ( C[8]<<1 )&0xAA );

          P[0] = PAL( (c1>>6)&3 );
          P[1] = PAL( (c2>>6)&3 );
          P[2] = PAL( (c1>>4)&3 );
          P[3] = PAL( (c2>>4)&3 );
          P[4] = PAL( (c1>>2)&3 );
          P[5] = PAL( (c2>>2)&3 );
          P[6] = PAL( (c1)&3);
          P[7] = PAL( (c2)&3);
        }

        if( ++x == 32 )
        {
          x = 0;
          nametbl = VRAM + (ntbl^0x400);
        }
      }

      Line += h;
      PP += WIDTH*h - 33*8;
      vofs &= ~7;
      h = Y2 - Line;
      if(h>8) h=8;
    }
  }
  if (SpriteON) RefreshSprite(Y1,Y2);
}

#define	V_FLIP	0x80
#define	H_FLIP	0x40
#define	SP_BACK	0x20

typedef struct {
	byte y,no,atr,x;
} SPR;

void RefreshSprite(byte Y1,byte Y2)
{
  Y1++;
  Y2++;
  int n,vofs;
  byte H;
  SPR *spr;

  if (Sprite16) H=16;
  else H=8;

  vofs = 0;
  if (SpAdrHI && !Sprite16) vofs = 0x1000;
  spr = (SPR*)SPRAM + 63;

  int spnum = 0;

  for(n=63; n >= 0; n--, spr--)
  {
    byte x,y,no,atr,cc;
    byte /**R,*/*C,*P;
    int h,t,inc,vadr;

    y = spr->y;
    if (y>Y2 || y<Y1-H) continue;

    if(++spnum > 8)
    {
      PPU_status |= PPU_SpMAX;
      //break;
    }

    x = spr->x;
    no= spr->no;
    atr=spr->atr;
    cc = (atr&3)*4+16;
    //	R = &Pal[cc];
    if (Sprite16)
      vadr = ((no&1)<<12) + (no>>1)*32;
    else
      vadr = no*16+vofs;

    C = VRAMADR(vadr);
    P = XBuf + WIDTH*(HEIGHT-FC_H)/2 + (WIDTH-FC_W)/2 + WIDTH*y+x;

    inc = 1;
    if (atr&V_FLIP)
    {
      inc=-1; C+=7;
      if (Sprite16) C+=16;
    }

    t = Y1-y;
    if (t>0)
    {
      C += t*inc;
      P += WIDTH*t;
      if (t>8) C+=inc*8;
    }

    else t=0;
    h=Y2-y+1;
    if (h>H) h=H;

    for(; t < h; t++, C += inc, P += WIDTH )
    {
      byte J,c1,c2;
      if (t==8) C += inc*8;

      c1=((C[0]>>1)&0x55)|(C[8]&0xAA);
      c2=(C[0]&0x55)|((C[8]<<1)&0xAA);
      J=C[0]|C[8];

      if (J)
      {
        if (atr&H_FLIP)
        {
          if (J&0x80) P[7]=PAL((c1>>6)&3);
          if (J&0x40) P[6]=PAL((c2>>6)&3);
          if (J&0x20) P[5]=PAL((c1>>4)&3);
          if (J&0x10) P[4]=PAL((c2>>4)&3);
          if (J&0x08) P[3]=PAL((c1>>2)&3);
          if (J&0x04) P[2]=PAL((c2>>2)&3);
          if (J&0x02) P[1]=PAL((c1)&3);
          if (J&0x01) P[0]=PAL((c2)&3);
        }
        else
        {
          if (J&0x80) P[0]=PAL((c1>>6)&3);
          if (J&0x40) P[1]=PAL((c2>>6)&3);
          if (J&0x20) P[2]=PAL((c1>>4)&3);
          if (J&0x10) P[3]=PAL((c2>>4)&3);
          if (J&0x08) P[4]=PAL((c1>>2)&3);
          if (J&0x04) P[5]=PAL((c2>>2)&3);
          if (J&0x02) P[6]=PAL((c1)&3);
          if (J&0x01) P[7]=PAL((c2)&3);
        }
      }

    }

  }
}

extern int isframeend;
void RefreshScreen(void)
{
  int i;
  if (palchange)
  {
    for(i=0;i<32;i++) spal[i] = *(int*)palette[Pal[i]];//(palette[Pal[i]].r << 16) | (palette[Pal[i]].g << 8) | (palette[Pal[i]].b) | 0xFF << 24;
    palchange = FALSE;
  }

  video2screen();
  DispObject_InvalidateRect(GUIObject_GetDispObject(MyGui), 0);

  isframeend = 1;
}

#define	ROM_MIRROR	1
#define	ROM_WRAM	2
#define	ROM_TRAINER	4
#define	ROM_FOUR	8

int CartLoad(wchar_t *fdir, wchar_t *fname)
{
  typedef struct {
    char ID[4];

    byte ROM_size;
    byte VROM_size;

    byte ROM_type;
    byte ROM_type2;

    char reserve[8];
  } iNES_HEADER;

  int fp;
  iNES_HEADER head;
  fp = _fopen(fdir, fname, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 );
  if (fp < 0)
  {
    return -1;
  }
  fread(fp, &head, sizeof(head));
  if (memcmp(head.ID,"NES\x1a",4))
  {
    fclose(fp);
    return -1;
  }

  ROM_type = head.ROM_type;
  ROM_size = head.ROM_size;
  VROM_size = head.VROM_size;

  MMC_type = ((head.ROM_type2 & 0xf0) | ((head.ROM_type & 0xf0) >> 4));

  Mirroring = (head.ROM_type&ROM_MIRROR);
  Mirrorxor = 0x400<<Mirroring; /* Horizonal:0x400 Vertival:0x800 */

  ROM = (byte *)malloc(ROM_size*0x4000);
  if (VROM_size) VROM = (byte *)malloc(VROM_size*0x2000);

  fread(fp, ROM, 0x4000*ROM_size);
  fread(fp, VROM, 0x2000*VROM_size);

  fclose(fp);
  return 0;
}

void ResetFCE(M6502 *M)
{
  memset(M,0,sizeof(*M));
  M->IPeriod = IPeriod;

  if (Debug)
  {
    M->Trace = 1;
    M->TrapBadOps = 1;
  }
  vram_read_buffer = 0;

  PPU[0]=PPU[1]=PPU[2]=PPU[3]=0;
  ScrollX = ScrollY = ScrollX2 = ScrollY2 = 0;

  VAddr = 0;
  VAddr_temp = 0;
  VAddr_write = 0;

  joy_1 = joy_2 = 0;
  joy_readbit = 0;

  scanline = 0;
  Reset6502(M);
}

void TrashFCE(void)
{
  if (VRAM) free(VRAM);
  if (ROM)  free(ROM);
  if (VROM) free(VROM);
  if (WRAM) free(WRAM);
}

int InitFCE(wchar_t *fdir, wchar_t *fname)
{
  Debug = 0;
  vmode = 1;

  if (CartLoad(fdir, fname)) return -1;


  VRAM=(byte*)malloc(0x4000);
  if (ROM_type&ROM_WRAM)
  {
    WRAM=(byte*)malloc(0x2000);
    Page[3] = WRAM-0x6000;
  }
  Page[0] = RAM;
#if 1
  if ( MMC_init(MMC_type) < 0 )
  {
    TrashFCE();
    return -1;
  }
#else
  if (MMC_type!=0) return -1;
  if (ROM_size==1)
  {
    Page[6]=Page[7]=ROM-0xC000;
  }
  else
  {
    Page[4] = Page[5]=ROM-0x8000; /* first 16K */
    Page[6] = Page[7]=ROM+(ROM_size*0x4000)-0x10000; /* last 16K */
  }
  if (VROM_size>0)
  {
    VPage[0]=VROM;
    VPage[1]=VROM+0x1000;
  }
  else
  {
    VPage[0]=VRAM;
    VPage[1]=VRAM+0x1000;
  }
#endif
  return 0;
}


