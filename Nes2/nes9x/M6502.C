/** M6502: portable 6502 emulator ****************************/
/**                                                         **/
/**                         M6502.c                         **/
/**                                                         **/
/** This file contains implementation for 6502 CPU. Don't   **/
/** forget to provide Rd6502(), Wr6502(), Loop6502(), and   **/
/** possibly Op6502() functions to accomodate the emulated  **/
/** machine's architecture.                                 **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1996                      **/
/**               Alex Krasivsky  1996                      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/

#include "M6502.h"
#include "Tables.h"

/** INLINE ***************************************************/
/** Different compilers inline C functions differently.     **/
/*************************************************************/
#define INLINE inline


/** System-Dependent Stuff ***********************************/
/** This is system-dependent code put here to speed things  **/
/** up. It has to stay inlined to be fast.                  **/
/*************************************************************/
#if 1 //def INES
#define FAST_RDOP
extern byte *Page[];
extern byte RAM[];
#if 1
INLINE byte Op6502(register unsigned A) { return(Page[A>>13][A]); }
INLINE unsigned Op6502w(register unsigned A) { return(Page[A>>13][A])|(Page[(A+1)>>13][A+1]<<8); }
INLINE unsigned RdRAMw(register unsigned A) { return RAM[A]|(RAM[A+1]<<8); }

/*
INLINE void Wr6502(unsigned A,byte V)
{
	if (A<0x800) RAM[A]=V; else _Wr6502(A,V);
}

INLINE byte Rd6502(unsigned A)
{
	if (A<0x800 || A>=0x8000) return Page[A>>13][A]; else return _Rd6502(A);
}
*/

#define	RdRAM(A)	RAM[A]
#define	WrRAM(A,V)	RAM[A]=V
#define	AdrRAM(A)	&RAM[A]
#else
#define	Op6502(A)	Rd6502(A)
#define	Op6502w(A)	(Op6502(A)|(Op6502(A+1)<<8))
#define	RdRAM(A)	Rd6502(A)
#define	WrRAM(A,V)	Wr6502(A,V)
#endif

#endif

/** FAST_RDOP ************************************************/
/** With this #define not present, Rd6502() should perform  **/
/** the functions of Rd6502().                              **/
/*************************************************************/
#ifndef FAST_RDOP
#define Op6502(A) Rd6502(A)
#endif

#define	C_SFT	0
#define	Z_SFT	1
#define	I_SFT	2
#define	D_SFT	3
#define	B_SFT	4
#define	R_SFT	5
#define	V_SFT	6
#define	N_SFT	7

#define	_A	R->A
#define	_P	R->P
#define	_X	R->X
#define	_Y	R->Y
#define	_S	R->S
#define	_PC	R->PC
#define	_PC_	R->PC.W
#define	_IPeriod	R->IPeriod
#define	_ICount		R->ICount
#define	_IRequest	R->IRequest
#define	_IPeriod	R->IPeriod
#define	_IBackup	R->IBackup
#define	_TrapBadOps	R->TrapBadOps
#define	_Trace	R->Trace
#define	_Trap	R->Trap
#define	_AfterCLI	R->AfterCLI
#define	_User	R->User

#define	ZP	0
#define	SP	0x100

/** Addressing Methods ***************************************/
/** These macros calculate and return effective addresses.  **/
/*************************************************************/
#define MCZp()	(Op6502(_PC_++))
#define MCZx()	(byte)(Op6502(_PC_++)+_X)
#define MCZy()	(byte)(Op6502(_PC_++)+_Y)
//#define MCZx()	((Op6502(_PC_++)+_X)&0xff)
//#define MCZy()	((Op6502(_PC_++)+_Y)&0xff)
#define	MCIx()	(RdRAMw(MCZx()))
#define	MCIy()	(RdRAMw(MCZp())+_Y)
#define	MCAb()	(Op6502w(_PC_))
#define MCAx()	(Op6502w(_PC_)+_X)
#define MCAy()	(Op6502w(_PC_)+_Y)

#define MC_Ab(Rg)	M_LDWORD(Rg)
#define MC_Zp(Rg)	Rg.B.l=Op6502(_PC_++);Rg.B.h=0
#define MC_Zx(Rg)	Rg.B.l=Op6502(_PC_++)+R->X;Rg.B.h=0
#define MC_Zy(Rg)	Rg.B.l=Op6502(_PC_++)+R->Y;Rg.B.h=0
#define MC_Ax(Rg)	M_LDWORD(Rg);Rg.W+=_X
#define MC_Ay(Rg)	M_LDWORD(Rg);Rg.W+=_Y
#if 1
#define MC_Ix(Rg)	K.B.l=Op6502(_PC_++)+R->X;K.B.h=0; \
			Rg.B.l=RdRAM(K.W);Rg.B.h=RdRAM(K.W+1)
#define MC_Iy(Rg)	K.B.l=Op6502(_PC_++);K.B.h=0; \
			Rg.B.l=RdRAM(K.W);Rg.B.h=RdRAM(K.W+1); \
			Rg.W+=R->Y
#else
#define	MC_Ix(Rg)	Rg.W=(RdRAMw(MCZx()))
#define	MC_Iy(Rg)	Rg.W=(RdRAMw(MCZp())+_Y)
#endif
/*
#define MC_Ix(Rg)	{byte *p=AdrRAM(MCZx()); \
			Rg.B.l=p[0];Rg.B.h=p[1]; }
#define MC_Iy(Rg)	{byte *p=AdrRAM(MCZp()); \
			Rg.B.l=p[0];Rg.B.h=p[1]; } \
			Rg.W+=_Y;
*/

/** Reading From Memory **************************************/
/** These macros calculate address and read from it.        **/
/*************************************************************/
#define MR_Ab(Rg)	MC_Ab(J);Rg=Rd6502(J.W)
//#define MR_Ab(Rg)	Rg=Rd6502(MCAb());_PC_+=2
#define MR_Im(Rg)	Rg=Op6502(_PC_++)
#define	MR_Zp(Rg)	Rg=RdRAM(MCZp())
#define MR_Zx(Rg)	Rg=RdRAM(MCZx())
#define MR_Zy(Rg)	Rg=RdRAM(MCZy())
#define	MR_Ax(Rg)	MC_Ax(J);Rg=Rd6502(J.W)
#define MR_Ay(Rg)	MC_Ay(J);Rg=Rd6502(J.W)
//#define MR_Ax(Rg)	Rg=Rd6502(MCAx());_PC_+=2;
//#define MR_Ay(Rg)	Rg=Rd6502(MCAy());_PC_+=2;
//#define MR_Ix(Rg)	Rg=Rd6502(MCIx())
//#define MR_Iy(Rg)	Rg=Rd6502(MCIy())
#define MR_Ix(Rg)	MC_Ix(J);Rg=Rd6502(J.W)
#define MR_Iy(Rg)	MC_Iy(J);Rg=Rd6502(J.W)

/** Writing To Memory ****************************************/
/** These macros calculate address and write to it.         **/
/*************************************************************/
//#define MW_Ab(Rg)	Wr6502(MCAb(),Rg);_PC_+=2
#define MW_Ab(Rg)	MC_Ab(J);Wr6502(J.W,Rg)
#define MW_Zp(Rg)	WrRAM(MCZp(),Rg)
#define MW_Zx(Rg)	WrRAM(MCZx(),Rg)
#define MW_Zy(Rg)	WrRAM(MCZy(),Rg)
#define MW_Ax(Rg)	MC_Ax(J);Wr6502(J.W,Rg)
#define MW_Ay(Rg)	MC_Ay(J);Wr6502(J.W,Rg)
//#define MW_Ax(Rg)	Wr6502(MCAx(),Rg);_PC_+=2
//#define MW_Ay(Rg)	Wr6502(MCAy(),Rg);_PC_+=2
//#define MW_Ix(Rg)	Wr6502(MCIx(),Rg)
//#define MW_Iy(Rg)	Wr6502(MCIy(),Rg)
#define MW_Ix(Rg)	MC_Ix(J);Wr6502(J.W,Rg)
#define MW_Iy(Rg)	MC_Iy(J);Wr6502(J.W,Rg)

/** Modifying Memory *****************************************/
/** These macros calculate address and modify it.           **/
/*************************************************************/
#define MM_Ab(Cmd)	MC_Ab(J);I=Rd6502(J.W);Cmd(I);Wr6502(J.W,I)
#define MM_Zp(Cmd)	{unsigned A=MCZp();I=RdRAM(A);Cmd(I);WrRAM(A,I); }
#define MM_Zx(Cmd)	{unsigned A=MCZx();I=RdRAM(A);Cmd(I);WrRAM(A,I); }
//#define MM_Zp(Cmd)	{byte *p=AdrRAM(MCZp());I=*p;Cmd(I);*p=I;}
//#define MM_Zx(Cmd)	{byte *p=AdrRAM(MCZx());I=*p;Cmd(I);*p=I;}
#define MM_Ax(Cmd)	MC_Ax(J);I=Rd6502(J.W);Cmd(I);Wr6502(J.W,I)

/** Other Macros *********************************************/
/** Calculating flags, stack, jumps, arithmetics, etc.      **/
/*************************************************************/
#define M_FL(Rg)	_P=(_P&~(Z_FLAG|N_FLAG))|ZNTable[Rg]
#define M_LDWORD(Rg)	Rg.B.l=Op6502(_PC_);Rg.B.h=Op6502(_PC_+1);_PC_+=2

#define M_PUSH(Rg)	WrRAM(SP+_S,Rg);_S--
#define M_POP(Rg)	_S++;Rg=RdRAM(SP+_S)
#define M_JR		_PC_+=(offset)Op6502(_PC_)+1;_ICount--

#define M_ADC(Rg) \
  if(_P&D_FLAG) \
  { \
    K.B.l=(_A&0x0F)+(Rg&0x0F)+(_P&C_FLAG); \
    K.B.h=(_A>>4)+(Rg>>4)+(K.B.l>15? 1:0); \
    if(K.B.l>9) { K.B.l+=6;K.B.h++; } \
    if(K.B.h>9) K.B.h+=6; \
    _A=(K.B.l&0x0F)|(K.B.h<<4); \
    _P=(_P&~C_FLAG)|(K.B.h>15? C_FLAG:0); \
  } \
  else \
  { \
    K.W=_A+Rg+(_P&C_FLAG); \
    _P&=~(N_FLAG|V_FLAG|Z_FLAG|C_FLAG); \
    _P|=((~(_A^Rg)&(_A^K.B.l)&0x80)?V_FLAG:0)| \
          (K.B.h? C_FLAG:0)|ZNTable[K.B.l]; \
    _A=K.B.l; \
  }

/* Warning! C_FLAG is inverted before SBC and after it */
#define M_SBC(Rg) \
  if(_P&D_FLAG) \
  { \
    K.B.l=(_A&0x0F)-(Rg&0x0F)-(~_P&C_FLAG); \
    if(K.B.l&0x10) K.B.l-=6; \
    K.B.h=(_A>>4)-(Rg>>4)-(K.B.l&0x10); \
    if(K.B.h&0x10) K.B.h-=6; \
    _A=(K.B.l&0x0F)|(K.B.h<<4); \
    _P=(_P&~C_FLAG)|(K.B.h>15? 0:C_FLAG); \
  } \
  else \
  { \
    K.W=_A-Rg-(~_P&C_FLAG); \
    _P&=~(N_FLAG|V_FLAG|Z_FLAG|C_FLAG); \
    _P|=(((_A^Rg)&(_A^K.B.l)&0x80)?V_FLAG:0)| \
          (K.B.h? 0:C_FLAG)|ZNTable[K.B.l]; \
    _A=K.B.l; \
  }

#define M_CMP(Rg1,Rg2) \
  K.W=Rg1-Rg2; \
  _P&=~(N_FLAG|Z_FLAG|C_FLAG); \
  _P|=ZNTable[K.B.l]|(K.B.h? 0:C_FLAG)
#define M_BIT(Rg) \
  _P&=~(N_FLAG|V_FLAG|Z_FLAG); \
  _P|=(Rg&(N_FLAG|V_FLAG))|(Rg&_A? 0:Z_FLAG)

#define M_AND(Rg)	_A&=Rg;M_FL(_A)
#define M_ORA(Rg)	_A|=Rg;M_FL(_A)
#define M_EOR(Rg)	_A^=Rg;M_FL(_A)
#define M_INC(Rg)	Rg++;M_FL(Rg)
#define M_DEC(Rg)	Rg--;M_FL(Rg)

#define M_ASL(Rg)	_P&=~C_FLAG;_P|=Rg>>7;Rg<<=1;M_FL(Rg)
#define M_LSR(Rg)	_P&=~C_FLAG;_P|=Rg&C_FLAG;Rg>>=1;M_FL(Rg)
#define M_ROL(Rg)	K.B.l=(Rg<<1)|(_P&C_FLAG); \
			_P&=~C_FLAG;_P|=Rg>>7;Rg=K.B.l; \
			M_FL(Rg)
#define M_ROR(Rg)	K.B.l=(Rg>>1)|(_P<<7); \
			_P&=~C_FLAG;_P|=Rg&C_FLAG;Rg=K.B.l; \
			M_FL(Rg)

/** Reset6502() **********************************************/
/** This function can be used to reset the registers before **/
/** starting execution with Run6502(). It sets registers to **/
/** their initial values.                                   **/
/*************************************************************/
void Reset6502(M6502 *R)
{
#ifdef HuC6280
  R->MPR[7]=0xff; Page[7]=ROMMap[0xff];
#endif
  _A=_X=_Y=0x00;
  _P=Z_FLAG|R_FLAG;
  _S=0xFF;
  _PC.B.l=Op6502(0xFFFC);
  _PC.B.h=Op6502(0xFFFD);
  _ICount=_IPeriod;
  _IRequest=INT_NONE;
  _AfterCLI=0;
}

#if 0
/** Exec6502() ***********************************************/
/** This function will execute a single 6502 opcode. It     **/
/** will then return next PC, and current register values   **/
/** in R.                                                   **/
/*************************************************************/
word Exec6502(M6502 *R)
{
  register pair J,K;
  register byte I;

  I=Op6502(_PC_++);
  _ICount-=Cycles[I];
  switch(I)
  {
#include "Codes.h"
  }

  /* We are done */
  return(_PC_);
}
#endif

/** Int6502() ************************************************/
/** This function will generate interrupt of a given type.  **/
/** INT_NMI will cause a non-maskable interrupt. INT_IRQ    **/
/** will cause a normal interrupt, unless I_FLAG set in R.  **/
/*************************************************************/
void Int6502(M6502 *R,byte Type)
{
  register pair J;

  if((Type==INT_NMI)||((Type==INT_IRQ)&&!(_P&I_FLAG)))
  {
    _ICount-=7;
    M_PUSH(_PC.B.h);
    M_PUSH(_PC.B.l);
    M_PUSH(_P&~B_FLAG);
    _P&=~D_FLAG;
    if(Type==INT_NMI) J.W=0xFFFA; else { _P|=I_FLAG;J.W=0xFFFE; }
    _PC.B.l=Op6502(J.W);
    _PC.B.h=Op6502(J.W+1);
  }
}

extern word PCbuf[];
extern int PCstep,PCstepMask;

int isframeend = 0;

/** Run6502() ************************************************/
/** This function will run 6502 code until Loop6502() call  **/
/** returns INT_QUIT. It will return the PC at which        **/
/** emulation stopped, and current register values in R.    **/
/*************************************************************/
word Run6502(M6502 *R)
{
  register pair J,K;
  register byte I;

  while(!isframeend)
  {

    I=Op6502(_PC_++);
    _ICount-=Cycles[I];
    switch(I)
    {
#ifdef HuC6280
 #include "HuCodes.h"
#endif
#include "Codes.h"
    }

    /* If cycle counter expired... */
    if(_ICount<=0)
    {
      /* If we have come after CLI, get INT_? from IRequest */
      /* Otherwise, get it from the loop handler            */
      if(_AfterCLI)
      {
        I=_IRequest;            /* Get pending interrupt     */
        _ICount+=_IBackup-1;  /* Restore the ICount        */
        _AfterCLI=0;            /* Done with AfterCLI state  */
      }
      else
      {
        I=Loop6502(R);            /* Call the periodic handler */
        _ICount=_IPeriod;     /* Reset the cycle counter   */
      }

      if(I) Int6502(R,I);              /* Interrupt if needed  */
    }
  }

  isframeend = 0;

  /* Execution stopped */
  return(_PC_);
}
