/*
	Mapper routine
*/
typedef unsigned char byte;
typedef unsigned short word;
#include	"nes9x.h"
//#indlude "mapper.h" //from FCEU mappers 18-

#define	DISABLE	0
#define	ENABLE	1
#define	LAST_BANK	ROM_size-1
#define	ROM16_INIT()
#define	VROM8_INIT()
#define	VROM_BANK1(A,V)	VPage[(A)>>10]=&VROM[(V)*0x400]-(A)
#define	VROM_BANK4(A,V)	VPage[(A)>>10]=VPage[((A)>>10)+1]= \
	VPage[((A)>>10)+2]=VPage[((A)>>10)+3]=&VROM[(V)*0x1000]-(A)
#define	VROM_BANK8(A,V)	VPage[0]=VPage[1]=VPage[2]=VPage[3]= \
	VPage[4]=VPage[5]=VPage[6]=VPage[7]=&VROM[(V)*0x2000]
#define	ROM_BANK8(A,V)	Page[(A)>>13]=&ROM[V*0x2000]-(A)
#define	ROM_BANK16(A,V)	Page[(A)>>13]=Page[((A)>>13)+1]=&ROM[(V)*0x4000]-(A)
#define	ROM_BANK32(A,V)	Page[4]=Page[5]=Page[6]=Page[7]=&ROM[(V)*0x8000]-(A)
#define	MIRROR_SET(V)	Mirrorxor = 0x400<<(Mirroring=(V))

void (*MMC_write)(word A,byte V);

static word IRQ_counter;
static byte IRQ_flg,IRQ_ratch;
static int vmask;
////////////from FCEU


static void MMC107_write(word A,byte V)
{
 ROM_BANK32(0x8000,(V>>1)&0x3);
 VROM_BANK8(0,V&0x7);
}

static void MMC156_write(word A,byte V)
{
 if(A>=0xc000 && A<=0xC003)
  VROM_BANK1((A&3)*1024,V);
 else if(A>=0xc008 &&  A<=0xc00b)
  VROM_BANK1(0x1000+(A&3)*1024,V);
 if(A==0xc010) ROM_BANK16(0x8000,V);
// printf("$%04x:$%02x\n",A,V);
}

static void MMC140_write(word A,byte V)
{
 VROM_BANK8(0,V&0xF);
 ROM_BANK32(0x6000,(V>>4)&0xF);
}

static void MMC94_write(word A,byte V)
{
 ROM_BANK16(0x8000,V>>2);
}

static void MMC93_write(word A,byte V)
{
  ROM_BANK16(0x8000,V>>4);
  MIRROR_SET(V&1);
 // latche=V;
}

static void MMC87_write(word A,byte V)
{
 VROM_BANK8(0x6000,V>>1);
}

static void MMC78_write(word A,byte V)
{
 ROM_BANK16(0x8000,V&0x7);
 VROM_BANK8(0,V>>4);
}

static void MMC70_write(word A,byte V)
{
 ROM_BANK16(0x8000,V>>4);
 VROM_BANK8(0,V&0xF);//0x6000
}

static void MMC152_write(word A,byte V)
{
 ROM_BANK16(0x8000,(V>>4)&0x7);
 VROM_BANK8(0,V&0xF);//0x6000
}

static void MMC66_write(word A,byte V)
{
 VROM_BANK8(0,V&0xF);//0x6000
 ROM_BANK32(0x6000,(V>>4));
}

static void MMC34_write(word A,byte V)
{
 switch(A)
 {
  case 0x7FFD:ROM_BANK32(0x8000,V);break;
  case 0x7FFE:VROM_BANK4(0x0000,V);break;
  case 0x7fff:VROM_BANK4(0x1000,V);break;
 }
 if(A>=0x8000)
  ROM_BANK32(0x8000,V);
}


int mapbyte1=0;
static void MMC112_write(word A,byte V)
{
switch(A)
{
 case 0xe000:MIRROR_SET(V&1);break;
 case 0x8000:mapbyte1=V;break;
 case 0xa000:switch(mapbyte1)
            {
            case 0:ROM_BANK8(0x8000,V);break;
            case 1:ROM_BANK8(0xA000,V);break;
                case 2: V&=0xFE;VROM_BANK1(0,V);
                        VROM_BANK1(0x400,(V+1));break;
                case 3: V&=0xFE;VROM_BANK1(0x800,V);
                        VROM_BANK1(0xC00,(V+1));break;
            case 4:VROM_BANK1(0x1000,V);break;
            case 5:VROM_BANK1(0x1400,V);break;
            case 6:VROM_BANK1(0x1800,V);break;
            case 7:VROM_BANK1(0x1c00,V);break;
            }
            break;
 }
}

static void MMC113_init(){  ROM_BANK32(0,0);}
static void MMC113_write(word A,byte V)
{
  if(A<0x8000){
        ROM_BANK32(0x4020,(V>>3)&7);
        VROM_BANK8(0,V&7);
  }else{
    ROM_BANK32(0x8000,V&0x7);
  }
}

static void MMC200_write(word A,byte V)
{
  ROM_BANK16(0x8000,A&0x07);
  ROM_BANK16(0xC000,A&0x07);
  VROM_BANK8(0,A&0x07);//0x8000
  MIRROR_SET((A&0x08)>>3);
}

static void MMC180_write(word A,byte V)
{
ROM_BANK16(0xC000,V);
}

static void MMC182_write(word A,byte V)
{
  switch(A&0xf003)
  {
//   case 0xe003:IRQCount=V;IRQa=1;X6502_IRQEnd(FCEU_IQEXT);break;
   case 0x8001:MIRROR_SET(V&1);break;
   case 0xA000:mapbyte1=V;break;
   case 0xC000:
               switch(mapbyte1&7)
               {
                case 0:VROM_BANK1(0x0000,V>>1);break;//BANK2!!!
                case 1:VROM_BANK1(0x1400,V);break;
                case 2:VROM_BANK1(0x0800,V>>1);break;//BANK2!!
                case 3:VROM_BANK1(0x1c00,V);break;
                case 4:ROM_BANK8(0x8000,V);break;
                case 5:ROM_BANK8(0xA000,V);break;
                case 6:VROM_BANK1(0x1000,V);break;
                case 7:VROM_BANK1(0x1800,V);break;
               }
               break;
  }
}


static void MMC184_write(word A,byte V)
{
VROM_BANK4(0x0000,V);
VROM_BANK4(0x1000,(V>>4));
}


static void MMC151_write(word A,byte V)
{
 switch(A&0xF000)
 {
  case 0x8000:ROM_BANK8(0x8000,V);break;
  case 0xA000:ROM_BANK8(0xA000,V);break;
  case 0xC000:ROM_BANK8(0xC000,V);break;
  case 0xe000:VROM_BANK4(0x0000,V);break;
  case 0xf000:VROM_BANK4(0x1000,V);break;
 }
}

static void MMC202_init(){
 ROM_BANK16(0x8000,0);
 ROM_BANK16(0xc000,0);
 VROM_BANK8(0,0);
}
static void MMC202_write(word A,byte V)
{
 int tmp=(A>>1)&0x7;
 MIRROR_SET(A&1);
 ROM_BANK16(0x8000,tmp);
 ROM_BANK16(0xc000,tmp+(((tmp&0x6)==0x6)?1:0));
 VROM_BANK8(0,tmp);
}

static void MMC203_init(){
 ROM_BANK16(0x8000,0);
 ROM_BANK16(0xc000,0);
 VROM_BANK8(0,0);
}
static void MMC203_write(word A,byte V)
{
 ROM_BANK16(0x8000,(V>>2)&3);
 ROM_BANK16(0xc000,(V>>2)&3);
 VROM_BANK8(0,V&3);
}


static void MMC228_write(word A,byte V)
{
 unsigned int page,pagel,pageh;

 MIRROR_SET((A>>13)&1);

 page=(A>>7)&0x3F;
 //printf("%04x\n",A);
 if((page&0x30)==0x30)
  page-=0x10;

 pagel=pageh=(page<<1) + (((A>>6)&1)&((A>>5)&1));
 pageh+=((A>>5)&1)^1;

 ROM_BANK16(0x8000,pagel);
 ROM_BANK16(0xC000,pageh);
 VROM_BANK8( 0,(V&0x3) | ((A&0xF)<<2) );//0x8000
}


static void MMC229_write(word A,byte V)
{
if(A>=0x8000)
{
MIRROR_SET((A>>5)&1);
if(!(A&0x1e))
 {
 ROM_BANK32(0x8000,0);
 }
else
 {
 ROM_BANK16(0x8000,A&0x1f);
 ROM_BANK16(0xC000,A&0x1f);
 }
 VROM_BANK8(0,A);//0x8000
}
}

static void MMC231_init(){
   ROM_BANK16(0x8000,0);
   ROM_BANK16(0xc000,0);
}
static void MMC231_write(word A,byte V)
{
 if(A&0x20)
  ROM_BANK32(0x8000,(A>>1)&0xF);
 else
 {
  ROM_BANK16(0x8000,(A&0x1E));
  ROM_BANK16(0xc000,(A&0x1E));
 }
 MIRROR_SET((A>>7)&1);
}

static void MMC201_init(){
  ROM_BANK32(0x8000,0);
  VROM_BANK8(0,0);//0x8000
}
static void MMC201_write(word A,byte V)
{
  if (A&0x08)
  {
     ROM_BANK32(0x8000,A&0x03);
     VROM_BANK8(0,A&0x03);//?
  }
  else
  {
     ROM_BANK32(0x8000,0);
     VROM_BANK8(0,0);//?
  }
}

static void MMC240_write(word A,byte V)
{
 if(A<0x8000)
 {
  ROM_BANK32(0x4020,V>>4);
  VROM_BANK8(0,V&0xF);
 }
}

static void MMC241_write(word A,byte V)
{
 if(A<0x8000)
 {
 // printf("$%04x:$%02x, $%04x\n",A,V,X.PC);
 }
 else
  ROM_BANK32(0x8000,V);
}

static void MMC244_write(word A,byte V)
{
  if(A<0x80a5){
    ROM_BANK32(0x8065,(A-0x8065)&0x03);
  }else{
     VROM_BANK8(0,(A-0x80A5)&0x07);
  }
}

static void MMC246_write(word A,byte V)
{
 switch(A&0xF007)
 {
 case 0x6000:ROM_BANK8(0x8000,V);break;
 case 0x6001:ROM_BANK8(0xA000,V);break;
 case 0x6002:ROM_BANK8(0xC000,V);break;
 case 0x6003:ROM_BANK8(0xE000,V);break;
 case 0x6004:VROM_BANK1(0x0000,V);break;//BANK2
 case 0x6005:VROM_BANK1(0x0800,V);break;//...
 case 0x6006:VROM_BANK1(0x1000,V);break;
 case 0x6007:VROM_BANK1(0x1800,V);break;
 }
}

static void MMC72_write(word A,byte V)
{
 mapbyte1=V;
 if(V&0x80)
  ROM_BANK16(0x8000,V&0xF);
 if(V&0x40)
  VROM_BANK8(0,V&0xF);//0x6000
}

static void MMC57_write(word A,byte V)
{
 A&=0x8800;
 if(A==0x8800)
 {
  mapbyte1=V;
  if(V&0x80)
   ROM_BANK32(0x8000,2|(V>>6));
  else
  {
   ROM_BANK16(0x8000,(V>>5)&3);
   ROM_BANK16(0xc000,(V>>5)&3);
  }
  MIRROR_SET((V&0x8)>>3);
 }
 else
  mapbyte1=V;
 VROM_BANK8(0,(mapbyte1&3)|(mapbyte1&7)|((mapbyte1&0x10)>>1));//0x8000
//static void MMC240_write(word A,byte V)
}

static void MMC58_init(){
 ROM_BANK32(0x8000,0);
 VROM_BANK8(0x8000,0);//?
}
static void MMC58_write(word A,byte V)
{
 //printf("$%04x:$%02x\n",A,V);
 if(A&0x40)
 {
  ROM_BANK16(0x8000,(A&0x07));
  ROM_BANK16(0xc000,(A&0x07));
 }
 else
  ROM_BANK32(0x8000,(A&0x06)>>1);

 VROM_BANK8(0,(A&0x38)>>3);
 MIRROR_SET((V&2)>>1);//2
}

#define mCount mapbyte1
static void MMC60_write(word A,byte V)
{
        VROM_BANK8(0,mCount);//0x8000
        ROM_BANK16(0x8000,mCount);
        ROM_BANK16(0xc000,mCount);
        mCount++;
        mCount&=0x03;
}

static void MMC61_write(word A,byte V)
{
 switch(A&0x30)
 {
  case 0x00:
  case 0x30:
	  ROM_BANK32(0x8000,A&0xF);
	  break;
  case 0x20:
  case 0x10:
	  ROM_BANK16(0x8000,((A&0xF)<<1)| (((A&0x20)>>4)) );
	  ROM_BANK16(0xC000,((A&0xF)<<1)| (((A&0x20)>>4)) );
	  break;
 }
 #ifdef moo
 if(!(A&0x10))
  ROM_BANK32(A&0xF);
 else
 {
  ROM_BANK16(0x8000,((A&0xF)<<1)| (((A&0x10)>>4)^1) );
  ROM_BANK16(0xC000,((A&0xF)<<1)| (((A&0x10)>>4)^1) );
 }
 #endif
 MIRROR_SET((A&0x80)>>7);
}

static void MMC62_write(word A,byte V)
{
  VROM_BANK8(0,((A&0x1F)<<2)|(V&0x03));//0x8000
  if ((A&0x20)>>5) {
     ROM_BANK16(0x8000,(A&0x40)|((A>>8)&0x3F));
     ROM_BANK16(0xc000,(A&0x40)|((A>>8)&0x3F));
  }
  else
     ROM_BANK32(0x8000,((A&0x40)|((A>>8)&0x3F))>>1);
  MIRROR_SET((A&0x80)>>7);
}
//static void MMC72_write(word A,byte V)
//{
// mapbyte1=V;
// if(V&0x80)
//  ROM_BANK16(0x8000,V&0xF);
// if(V&0x40)
//  VROM_BANK8(V&0xF);
//}
static void MMC71_write(word A,byte V)
{
switch(A&0xF000)
 {
 case 0xF000:
 case 0xE000:
 case 0xD000:
 case 0xC000:ROM_BANK16(0x8000,V);break;
 //case 0x9000:onemir((V>>3)&2);break;
 }
}

static void MMC79_init(){
  ROM_BANK32(0x8000,~0);//0x4020
}
static void MMC79_write(word A,byte V)
{
 if(A<0x8000 && ((A^0x4100)==0))
 {
  ROM_BANK32(0x4020,(V>>3)&1);
 }
 VROM_BANK8(0,V);
}

static void MMC8_init(){
  ROM_BANK32(0x8000,0);
}
static void MMC8_write(word A,byte V)
{
        ROM_BANK16(0x8000,V>>3);
        VROM_BANK8(0,V&7);
}

static void MMC86_write(word A,byte V)
{
 if(A>=0x6000 && A<=0x6fFF)
 {
  VROM_BANK8(0,(V&3)|((V>>4)&4));
  ROM_BANK32(0x6000,(V>>4)&3);
 }
 else;
 //if(A!=0x6000)
//  printf("$%04x:$%02x\n",A,V);
}
static void MMC89_write(word A,byte V)
{
 VROM_BANK8(0,(V&7)|((V>>4)&8));
 ROM_BANK16(0x8000,(V>>4)&7);
 //onemir((V>>3)&1);
}


//////////////
static void NONE_init(void)
{
	ROM_BANK16(0x8000,0); /* first 16K */
	ROM_BANK16(0xC000,LAST_BANK); /* last 16K */
	VPage[0]=VPage[1]=VPage[2]=VPage[3]=
	VPage[4]=VPage[5]=VPage[6]=VPage[7]=(VROM_size>0)?VROM:VRAM;
	vmask = VROM_size-1;
}

static void NONE_write(word A,byte V)
{
//	printf("cannot ROM write at:%04x,%02x\n",A,V);
}

static void ROM32_init(void)
{
	ROM_BANK32(0x8000,0);
}

/* 1: nintendo MMC1 */
static byte MMC1_reg[4],MMC1_sft,MMC1_buf; //MMC1_sft[4],MMC1_buf[4];
static void MMC1_init(void)
{
	int i;
	for(i=0;i<4;i++) MMC1_reg[i]=0; //MMC1_sft[i]=MMC1_buf[i]=0;
	MMC1_sft = MMC1_buf = 0;
	ROM16_INIT();
}
#define	MIRROR_H	0
#define	MIRROR_V	1
#define	MIRROR_ONE	2

/* not finish */
static void MMC1_write(word A,byte V)
{
	int n=(A>>13)-4;
	if (V&0x80) { MMC1_sft=MMC1_buf=0;MMC1_reg[0]|=0xC; return;}
	MMC1_buf|=(V&1)<<(MMC1_sft++);
  if (MMC1_sft==5) {
	MMC1_reg[n]=V=MMC1_buf;
	MMC1_sft=MMC1_buf=0;
	switch(n){
	case 0:
	/*	if (!(V&2)) Mirroring = MIRROR_ONE;
		else*/ MIRROR_SET(V&1);
	/*	if (!(V&8)) ROM_BANK32(0x8000,MMC1_reg[3]>>1);
		else if (V&4) {
			ROM_BANK16(0x8000,MMC1_reg[3]);
			ROM_BANK16(0xC000,LAST_BANK);
		} else {
			ROM_BANK16(0xC000,MMC1_reg[3]);
			ROM_BANK16(0x8000,0);
		}*/
		break;
	case 1:
		if (MMC1_reg[0]&0x10) VROM_BANK4(0x0000,V);
		else VROM_BANK8(0x0000,V>>1);
		break;
	case 2:
		if (MMC1_reg[0]&0x10) VROM_BANK4(0x1000,V);
		break;
	case 3:
		if ((MMC1_reg[0]&8)) {
			if ((MMC1_reg[0]&4)) ROM_BANK16(0x8000,V);
			else ROM_BANK16(0xC000,V);
		} else {
			ROM_BANK32(0x8000,V>>1);
		}
		break;
	}
  }
}

/* 2:74HC161/74HC32 */
static void iNES2_write(word A,byte V)
{
	//printf("%04x %02x\n",A,V);
	ROM_BANK16(0x8000,V);
}

/* 3:VROM switch */
static void VROMsw_write(word A,byte V)
{
	VROM_BANK8(0x0000,V&vmask);
}

/* 4:nintendo MMC3 */
/* not finish */
static byte MMC3_cmd;
static int MMC3_IRQ(int scanline)
{
	if (IRQ_flg) {
		if (--IRQ_counter==0) return TRUE;
	}
	return FALSE;
}

static void MMC3_init(void)
{
}

static void MMC3_write(word A,byte V)
{
  switch(A)
  {
  case 0x8000:MMC3_cmd = V;break;
  case 0x8001:
    switch(MMC3_cmd&0xC7)
    {
    case 0:V&=0xFE; VROM_BANK1(0x0000,V);VROM_BANK1(0x0400,V+1); break;
    case 1:V&=0xFE; VROM_BANK1(0x0800,V);VROM_BANK1(0x0C00,V+1); break;
    case 2:VROM_BANK1(0x1000,V); break;
    case 3:VROM_BANK1(0x1400,V); break;
    case 4:VROM_BANK1(0x1800,V); break;
    case 5:
      if (MMC3_cmd&0x40) {
        ROM_BANK8(0xC000,V);
        ROM_BANK8(0x8000,(ROM_size-1)*2);
      } else {
        ROM_BANK8(0x8000,V);
        ROM_BANK8(0xC000,(ROM_size-1)*2);
      }
      break;
    case 6:
      if (MMC3_cmd&0x40) {
        ROM_BANK8(0xA000,V);
        ROM_BANK8(0x8000,(ROM_size-1)*2);
      } else {
        ROM_BANK8(0xA000,V);
        ROM_BANK8(0xC000,(ROM_size-1)*2);
      }
      break;
    }
    break;
  case 0xA000:MIRROR_SET(V&1); break;
  case 0xA001:/* ?? */ break;
  case 0xE000:V=IRQ_ratch;
  case 0xC000:IRQ_counter=V;if(V==0) IRQ_flg=DISABLE; break;
  case 0xC001:IRQ_ratch=V;break;
  case 0xE001:IRQ_flg=ENABLE;break;
  }
}

/* 5: nintendo MMC5 */
/* not finish */
static void MMC5_write(word A,byte V)
{
}

/* 6: FFE F4xxx */
/* not finish */
static void FFEF4_write(word A,byte V)
{
	switch(A){
	case 0x42FC:
	case 0x42FD:
	case 0x42EE:
	case 0x42FF:
	case 0x43FE:
	case 0x4500:
	case 0x4501:
	case 0x4502:
	case 0x4503:
	case 0x8000:
	case 0xA000:
	case 0xC000:
	case 0xE000:
	}
}

/* 7: ROM switch  */
static void ROMsw_init(void)
{
	ROM32_init();
}
static void ROMsw_write(word A,byte V)
{
	ROM_BANK32(0x8000,V&0xf);
//??	if ((V&0x10)==0) nametbl=0x2000; else nametbl=0x2400;
}

/* 8: FFE F3xxx */
static void FFEF3_init(void)
{
	ROM_BANK16(0x8000,0);
	ROM_BANK16(0xC000,1);
	VROM8_INIT();
}
static void FFEF3_write(word A,byte V)
{
	ROM_BANK16(0x8000,V>>3);
	VROM_BANK8(0x0000,V&7);
}

/* 9: nintendo MMC2 */
static byte MMC2_latch;
static void MMC2_init(void)
{
	MMC2_latch=0xFE;
	ROM_BANK16(0x8000,LAST_BANK-1);
	ROM_BANK16(0xC000,LAST_BANK);
	ROM_BANK8(0x8000,0);
}
static void MMC2_write(word A,byte V)
{
	switch(A&0xF000){
	case 0xA000:
		ROM_BANK8(0x8000,V);
	case 0xB000:
	case 0xC000:
		VROM_BANK4(0x0000,V);
		break;
	case 0xD000:
		if (MMC2_latch==0xFD) VROM_BANK4(0x1000,V);
		break;
	case 0xE000:
		if (MMC2_latch==0xFE) VROM_BANK4(0x1000,V);
		break;
	case 0xF000:
		MIRROR_SET(V&1);
		break;
	}
}

/* 10: nintendo MMC4 */
static byte MMC4_latch1,MMC4_latch2;
static void MMC4_init(void)
{
	MMC4_latch1 = MMC4_latch2=0xFE;
	ROM16_INIT();
}
static void MMC4_write(word A,byte V)
{
	switch(A&0xF000){
	case 0xA000:
		ROM_BANK16(0x8000,V);
		break;
	case 0xB000:
		if (MMC4_latch1==0xFD) VROM_BANK4(0x0000,V);
		break;
	case 0xC000:
		if (MMC4_latch1==0xFE) VROM_BANK4(0x0000,V);
		break;
	case 0xD000:
		if (MMC4_latch2==0xFD) VROM_BANK4(0x1000,V);
		break;
	case 0xE000:
		if (MMC4_latch2==0xFE) VROM_BANK4(0x1000,V);
		break;
	case 0xF000:
		MIRROR_SET(V&1);
		break;
	}
}


/* 11: Color dreams */
static void iNES11_init(void)
{
	ROM32_init();
	VROM8_INIT();
}
static void iNES11_write(word A,byte V)
{
	ROM_BANK32(0x8000,V&15);
	VROM_BANK8(0x0000,V>>4);
}

/* 15: 100 in 1 */
static void in1_init(void)
{
	ROM32_init();
}
static void in1_write(word A,byte V)
{
	switch(A){
	case 0x8000:
		MIRROR_SET((V>>6)&1);
		V&=0x3F;
		ROM_BANK16(0x8000,V);
		ROM_BANK16(0xC000,V+1);
		// swap ??
		break;
	case 0x8003:
		MIRROR_SET((V>>6)&1);
	case 0x8001:
		if (V&0x80) {
			V&=0x3F;
			ROM_BANK8(0xC000,V*2+1);
			ROM_BANK8(0xE000,V*2);
		} else {
			ROM_BANK16(0xC000,V);
		}
		break;
	case 0x8002:
		//??
		if (V&0x80) {
			//ROM_BANK8();
		} else {
			//ROM_BANK8();
		}
		break;
	}
}

/* 16: bandai */
static void Bandai_init(void)
{
	IRQ_flg = IRQ_counter = 0;
	ROM16_INIT();
}
static void Bandai_write(word A,byte V)
{
  /* A=0x6000..0xFFFF
  	 use 0x600x,0x7FFx,0x800x
  */
	switch(A&0xF){
	case 0x0: VROM_BANK1(0x0000,V);break;
	case 0x1: VROM_BANK1(0x0400,V);break;
	case 0x2: VROM_BANK1(0x0800,V);break;
	case 0x3: VROM_BANK1(0x0C00,V);break;
	case 0x4: VROM_BANK1(0x1000,V);break;
	case 0x5: VROM_BANK1(0x1400,V);break;
	case 0x6: VROM_BANK1(0x1800,V);break;
	case 0x7: VROM_BANK1(0x1C00,V);break;
	case 0x8: ROM_BANK16(0x8000,V);break;
	case 0x9: //mirror; break;
	case 0xA: IRQ_flg=V&1;break;
	case 0xB: IRQ_counter=(IRQ_counter&0xFF00)|V;break;
	case 0xC: IRQ_counter=(IRQ_counter&0xFF)|(V<<8);break;
	case 0xD: /* WRAM read */
	}
}
static byte Bandai_read(word A)
{
	/* WRAM write */
	if (A&15) return 0xFF; /* NODATA */
	return 0xFF;
}


static void Bandai1_write(word A,byte V)
{
	ROM_BANK32(0x8000,(V>>4)&3);
	VROM_BANK8(0x0000,V&3);
}

static void Bandai2_write(word A,byte V)
{
	MIRROR_SET(V>>7);
	ROM_BANK32(0x8000,(V>>4)&7);
	VROM_BANK8(0x0000,V&15);
}

/* 17: FFE F8xxx */
static void FFEF8_init(void)
{
	ROM16_INIT();
}
static void FFEF8_write(word A,byte V)
{
	switch(A){
	case 0x42FC: //??
	case 0x42FD: //??
	case 0x42FE: /* mirror*/; break;
	case 0x42FF: MIRROR_SET((V>>4)&1); break;
	case 0x4501: IRQ_flg=DISABLE;break;
	case 0x4502: IRQ_counter=(IRQ_counter&0xFF00)|V;break;
	case 0x4503: IRQ_counter=(IRQ_counter&0xFF)|(V<<8);IRQ_flg=ENABLE;break;
	case 0x4504: ROM_BANK8(0x8000,V);break;
	case 0x4505: ROM_BANK8(0xA000,V);break;
	case 0x4506: ROM_BANK8(0xC000,V);break;
	case 0x4507: ROM_BANK8(0xE000,V);break;
	case 0x4510: VROM_BANK1(0x0000,V);break;
	case 0x4511: VROM_BANK1(0x0400,V);break;
	case 0x4512: VROM_BANK1(0x0800,V);break;
	case 0x4513: VROM_BANK1(0x0C00,V);break;
	case 0x4514: VROM_BANK1(0x1000,V);break;
	case 0x4515: VROM_BANK1(0x1400,V);break;
	case 0x4516: VROM_BANK1(0x1800,V);break;
	case 0x4517: VROM_BANK1(0x1C00,V);break;
	}
}

#if 1
/* 18: Jaleco */
static byte V0,V1,V2,P0,P1,P2,P3;
static void Jaleco_init(void)
{
	ROM16_INIT();
}
static void Jaleco_write(word A,byte V)
{
	switch(A){
	case 0x8000: V0 = (V0&0xF0)|(V&15); ROM_BANK8(0x8000,V0); break;//RAM?
	case 0x8001: V0 = (V0&0xF) |(V<<4); ROM_BANK8(0x8000,V0); break;
	case 0x8002: V1 = (V1&0xF0)|(V&15); ROM_BANK8(0xA000,V1); break;
	case 0x8003: V1 = (V1&0xF) |(V<<4); ROM_BANK8(0xA000,V1); break;
	case 0x9000: V2 = (V2&0xF0)|(V&15); ROM_BANK8(0xC000,V2); break;
	case 0x9001: V2 = (V2&0xF) |(V<<4); ROM_BANK8(0xC000,V2); break;
	case 0xA000: P0 = (P0&0xF0)|(V&15); VROM_BANK1(0x0000,P0); break;
	case 0xA001: P0 = (P0&0xF) |(V<<4); VROM_BANK1(0x0000,P0); break;
	case 0xA002: P1 = (P1&0xF0)|(V&15); VROM_BANK1(0x0400,P1); break;
	case 0xA003: P1 = (P1&0xF) |(V<<4); VROM_BANK1(0x0400,P1); break;
	// ВҐВЅВѕВҐ
	case 0xE000: IRQ_counter=(IRQ_counter&0xFF00)|V;break;
	case 0xE001: IRQ_counter=(IRQ_counter&0xFF)|(V<<8);break;
	}
}

/* 19: Namcot 106 */
static void Namco_init(void)
{
	ROM16_INIT();
	// if (vromsize) VROM_BANK8(0x0000,0);
}
static void Namco_write(word A,byte V)
{
	switch(A&0xF800){
	case 0x5000: IRQ_counter=(IRQ_counter&0xFF00)|V;break;
	case 0x5800: IRQ_counter=(IRQ_counter&0xFF)|((V&0x7F)<<8);IRQ_flg=V&1;break;
	case 0x8000: VROM_BANK1(0x0000,V);break;
	case 0x8800: VROM_BANK1(0x0400,V);break;
	case 0x9000: VROM_BANK1(0x0800,V);break;
	case 0x9800: VROM_BANK1(0x0C00,V);break;
	case 0xA000: VROM_BANK1(0x1000,V);break;
	case 0xA800: VROM_BANK1(0x1400,V);break;
	case 0xB000: VROM_BANK1(0x1800,V);break;
	case 0xB800: VROM_BANK1(0x1C00,V);break;
	case 0xC000: VROM_BANK1(0x2000,V);break;
	case 0xC800: VROM_BANK1(0x2400,V);break;
	case 0xD000: VROM_BANK1(0x2800,V);break;
	case 0xD800: VROM_BANK1(0x2C00,V);break;
	case 0xE000: ROM_BANK8(0x8000,V);break;
	case 0xE800: ROM_BANK8(0xA000,V);break;
	case 0xF000: ROM_BANK8(0xC000,V);break;
	}
}

/* 21: Konami VRC4 */

/* 22: Konami VRC2 type A */
static void Konami2A_init(void)
{
	ROM16_INIT();
	VROM8_INIT();
}

static void Konami2A_write(word A,byte V)
{
	switch(A){
	case 0x8000: ROM_BANK8(0x8000,V);break;
	case 0x9000: //mirror
	case 0xA000: ROM_BANK8(0xA000,V);break;
	case 0xB000: VROM_BANK1(0x0000,V>>1);break;
	case 0xB001: VROM_BANK1(0x0400,V>>1);break;
	case 0xC000: VROM_BANK1(0x0800,V>>1);break;
	case 0xC001: VROM_BANK1(0x0C00,V>>1);break;
	case 0xD000: VROM_BANK1(0x1000,V>>1);break;
	case 0xD001: VROM_BANK1(0x1400,V>>1);break;
	case 0xE000: VROM_BANK1(0x1800,V>>1);break;
	case 0xE001: VROM_BANK1(0x1C00,V>>1);break;
	}
}

/* 23: Konami VRC2 type B */

/* 32: Irem */

/* 33: Taito */
static void Taito_write(word A,byte V)
{
	switch(A){
	case 0x8000:ROM_BANK8(0x8000,V&0x1F); MIRROR_SET((V>>6)&1); break;
	case 0x8001:ROM_BANK8(0xA000,V&0x1F); break;
	case 0x8002:V*=2;VROM_BANK1(0x0000,V);VROM_BANK1(0x0400,V); break;
	case 0x8003:V*=2;VROM_BANK1(0x0800,V);VROM_BANK1(0x0C00,V); break; //0x8002
	case 0xA000:VROM_BANK1(0x1000,V); break;
	case 0xA001:VROM_BANK1(0x1400,V); break;
	case 0xA002:VROM_BANK1(0x1800,V); break;
	case 0xA003:VROM_BANK1(0x1C00,V); break;
	/* type 5 */
	case 0xC000:IRQ_counter = V;break;
	case 0xC001:IRQ_flg = ENABLE;break;
	case 0xC002:IRQ_flg = DISABLE;break;
	case 0xC003:IRQ_counter = V;break;
	case 0xE000:MIRROR_SET(V&1);break;
	}
}
static void Taito2_write(word A,byte V)
{
	MIRROR_SET(V>>7);
	ROM_BANK16(0x8000,(V>>4)&7);
	VROM_BANK8(0x0000,V&15);
}
static void Taito3_write(word A,byte V)
{
	switch(A){
	case 0x7EF0:/*MIRROR_SET(V>>7)*/;
		V=(V*0x7F);VROM_BANK1(0x0000,V);VROM_BANK1(0x0400,V+1);break;
	case 0x7EF1:/*MIRROR_SET(V>>7)*/;
		V=(V*0x7F);VROM_BANK1(0x0800,V);VROM_BANK1(0x0C00,V+1);break;
	case 0x7EF2:VROM_BANK1(0x1000,V);break;
	case 0x7EF3:VROM_BANK1(0x1400,V);break;
	case 0x7EF4:VROM_BANK1(0x1800,V);break;
	case 0x7EF5:VROM_BANK1(0x1C00,V);break;
	case 0x7EFA:
	case 0x7EFB:ROM_BANK8(0x8000,V);break;
	case 0x7EFC:
	case 0x7EFD:ROM_BANK8(0xA000,V);break;
	//case 0x7EFC:
	//case 0x7EFD:ROM_BANK8(0xC000,V);break;
	}
}

/* 34: iNES #34 */
//static void iNES34_write(word A,byte V)
//{
//	if (A>=0x8000 || A==0x7FFD) {
//		ROM_BANK32(0x8000,V);
//	} else
//	switch(A){
//	case 0x7FFE:
//		VROM_BANK4(0x0000,V);
//		break;
//	case 0x7FFF:
//		VROM_BANK4(0x1000,V);
//		break;
//	}
//}
#endif

int MMC_init(int type)
{
	NONE_init();
	MMC_write = NONE_write;
	IRQ_flg = 0;
	IRQ_counter = 0;
	switch(type)
        {
	case 0: break;
	case  1: MMC1_init(); MMC_write = MMC1_write; break;
	case  2: MMC_write = iNES2_write; break;
	case  3: MMC_write = VROMsw_write; break;
	case  4: MMC_write = MMC3_write; break;
	case  5: MMC_write = MMC5_write; break;
	case  6: MMC_write = FFEF4_write; break;
	case  7: ROM32_init(); MMC_write = ROMsw_write; break;
	case  8: FFEF3_init();  MMC_write = FFEF3_write; break;
	case  9: MMC2_init();  MMC_write = MMC2_write; break;
	case 10: MMC4_init();  MMC_write = MMC4_write; break;
	case 11: ROM32_init(); MMC_write = iNES11_write; break;
	case 15: ROM32_init(); MMC_write = in1_write; break;
	case 16: MMC_write = Bandai_write; break;
	case 17: MMC_write = FFEF8_write; break;
#if 1
	case 18: MMC_write = Jaleco_write; break;
	case 19: MMC_write = Namco_write; break;
	//case 21: MMC_write = Konami4_write; break;
	case 22: MMC_write = Konami2A_write; break;
	//case 23: MMC_write = Konami2B_write; break;
	//case 32: MMC_write = Irem_write; break;
	case 33: MMC_write = Taito_write; break;
	case 34: MMC_write = MMC34_write; break;//iNES34_write; break;
#endif
        //ƒобавлено из FCEU
        case 107: MMC_write = MMC107_write; break;
        case 156: MMC_write = MMC156_write; break;
        case 140: MMC_write = MMC140_write; break;
        case 94: MMC_write = MMC94_write; break;
        case 93: MMC_write = MMC93_write; break;
        case 87: MMC_write = MMC87_write; break;
        case 78: MMC_write = MMC78_write; break;
        case 70: MMC_write = MMC70_write; break;
        case 152: MMC_write = MMC152_write; break;
        case 66: MMC_write = MMC66_write; break;
        //case 34: MMC_write = MMC34_write; break;
        case 112: MMC_write = MMC112_write; break;
        case 113: MMC113_init(); MMC_write = MMC113_write; break;
        case 200: MMC_write = MMC200_write; break;
        case 180: MMC_write = MMC180_write; break;
        case 182: MMC_write = MMC182_write; break;
        case 184: MMC_write = MMC184_write; break;
        case 151: MMC_write = MMC151_write; break;
        case 202: MMC202_init();MMC_write = MMC202_write; break;
        case 203: MMC203_init();MMC_write = MMC203_write; break;
        case 228: MMC_write = MMC228_write; break;
        case 229: MMC_write = MMC229_write; break;
        case 231: MMC231_init();MMC_write = MMC231_write; break;
        case 201: MMC201_init();MMC_write = MMC201_write; break;
        case 240: MMC_write = MMC240_write; break;
        case 241: MMC_write = MMC241_write; break;
        case 244: MMC_write = MMC244_write; break;
        case 246: MMC_write = MMC246_write; break;
        case 72: MMC_write = MMC72_write; break;
        case 57: MMC_write = MMC57_write; break;
        case 58: MMC58_init();MMC_write = MMC58_write; break;
        case 60: MMC_write = MMC60_write; break;
        case 61: MMC_write = MMC61_write; break;
        case 62: MMC_write = MMC62_write; break;
        case 71: MMC_write = MMC71_write; break;
        case 79: MMC79_init();MMC_write = MMC79_write; break;
//      case 8: MMC8_init(); MMC_write = MMC8_write; break;
        case 86: MMC_write = MMC86_write; break;
        case 89: MMC_write = MMC89_write; break;



	default:
		/*  */
//		printf("Not Support Mapper %d\n",type);
		return -1;
	}
	return 0;
}
