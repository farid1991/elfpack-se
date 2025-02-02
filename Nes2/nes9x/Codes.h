/** M6502: portable 6502 emulator ****************************/
/**                                                         **/
/**                          Codes.h                        **/
/**                                                         **/
/** This file contains implementation for the main table of **/
/** 6502 commands. It is included from 6502.c.              **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1996                      **/
/**               Alex Krasivsky  1996                      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/

case 0x10: if(_P&N_FLAG) _PC_++; else { M_JR; } break; /* BPL * REL */
case 0x30: if(_P&N_FLAG) { M_JR; } else _PC_++; break; /* BMI * REL */
case 0xD0: if(_P&Z_FLAG) _PC_++; else { M_JR; } break; /* BNE * REL */
case 0xF0: if(_P&Z_FLAG) { M_JR; } else _PC_++; break; /* BEQ * REL */
case 0x90: if(_P&C_FLAG) _PC_++; else { M_JR; } break; /* BCC * REL */
case 0xB0: if(_P&C_FLAG) { M_JR; } else _PC_++; break; /* BCS * REL */
case 0x50: if(_P&V_FLAG) _PC_++; else { M_JR; } break; /* BVC * REL */
case 0x70: if(_P&V_FLAG) { M_JR; } else _PC_++; break; /* BVS * REL */

/* RTI */
case 0x40:
  M_POP(_P);_P|=R_FLAG;M_POP(_PC.B.l);M_POP(_PC.B.h);
  break;

/* RTS */
case 0x60:
  M_POP(_PC.B.l);M_POP(_PC.B.h);_PC_++;break;

/* JSR $ssss ABS */
case 0x20:
  K.B.l=Op6502(_PC_++);
  K.B.h=Op6502(_PC_);
  M_PUSH(_PC.B.h);
  M_PUSH(_PC.B.l);
  _PC=K;break;

/* JMP $ssss ABS */
case 0x4C: M_LDWORD(K);_PC=K;break;

/* JMP ($ssss) ABDINDIR */
case 0x6C:
  M_LDWORD(K);
  _PC.B.l=Rd6502(K.W++);
  _PC.B.h=Rd6502(K.W);
  break;

/* BRK */
case 0x00:
  _PC_++;
  M_PUSH(_PC.B.h);M_PUSH(_PC.B.l);
  M_PUSH(_P|B_FLAG);
  _P=(_P|I_FLAG)&~D_FLAG;
#ifdef HuC6280
  _PC.B.l=Op6502(0xFFF6);
  _PC.B.h=Op6502(0xFFF7);
#else
  _PC.B.l=Op6502(0xFFFE);
  _PC.B.h=Op6502(0xFFFF);
#endif
  break;

/* CLI */
case 0x58:
  if((_IRequest!=INT_NONE)&&(_P&I_FLAG))
  {
    _AfterCLI=1;
    _IBackup=_ICount;
    _ICount=1;
  }
  _P&=~I_FLAG;
  break;

/* PLP */
case 0x28:
  M_POP(I);
  if((_IRequest!=INT_NONE)&&((I^_P)&~I&I_FLAG))
  {
    _AfterCLI=1;
    _IBackup=_ICount;
    _ICount=1;
  }
  _P=I|R_FLAG;
  break;

case 0x08: M_PUSH(_P);break;               /* PHP */
case 0x18: _P&=~C_FLAG;break;              /* CLC */
case 0xB8: _P&=~V_FLAG;break;              /* CLV */
case 0xD8: _P&=~D_FLAG;break;              /* CLD */
case 0x38: _P|=C_FLAG;break;               /* SEC */
case 0xF8: _P|=D_FLAG;break;               /* SED */
case 0x78: _P|=I_FLAG;break;               /* SEI */
case 0x48: M_PUSH(_A);break;               /* PHA */
case 0x68: M_POP(_A);M_FL(_A);break;     /* PLA */
case 0x98: _A=_Y;M_FL(_A);break;       /* TYA */
case 0xA8: _Y=_A;M_FL(_Y);break;       /* TAY */
case 0xC8: _Y++;M_FL(_Y);break;          /* INY */
case 0x88: _Y--;M_FL(_Y);break;          /* DEY */
case 0x8A: _A=_X;M_FL(_A);break;       /* TXA */
case 0xAA: _X=_A;M_FL(_X);break;       /* TAX */
case 0xE8: _X++;M_FL(_X);break;          /* INX */
case 0xCA: _X--;M_FL(_X);break;          /* DEX */
case 0xEA: break;                            /* NOP */
case 0x9A: _S=_X;break;                  /* TXS */
case 0xBA: _X=_S;break;                  /* TSX */

case 0x24: MR_Zp(I);M_BIT(I);break;       /* BIT $ss ZP */
case 0x2C: MR_Ab(I);M_BIT(I);break;       /* BIT $ssss ABS */

case 0x05: MR_Zp(I);M_ORA(I);break;       /* ORA $ss ZP */
case 0x06: MM_Zp(M_ASL);break;            /* ASL $ss ZP */
case 0x25: MR_Zp(I);M_AND(I);break;       /* AND $ss ZP */
case 0x26: MM_Zp(M_ROL);break;            /* ROL $ss ZP */
case 0x45: MR_Zp(I);M_EOR(I);break;       /* EOR $ss ZP */
case 0x46: MM_Zp(M_LSR);break;            /* LSR $ss ZP */
case 0x65: MR_Zp(I);M_ADC(I);break;       /* ADC $ss ZP */
case 0x66: MM_Zp(M_ROR);break;            /* ROR $ss ZP */
case 0x84: MW_Zp(_Y);break;             /* STY $ss ZP */
case 0x85: MW_Zp(_A);break;             /* STA $ss ZP */
case 0x86: MW_Zp(_X);break;             /* STX $ss ZP */
case 0xA4: MR_Zp(_Y);M_FL(_Y);break;  /* LDY $ss ZP */
case 0xA5: MR_Zp(_A);M_FL(_A);break;  /* LDA $ss ZP */
case 0xA6: MR_Zp(_X);M_FL(_X);break;  /* LDX $ss ZP */
case 0xC4: MR_Zp(I);M_CMP(_Y,I);break;  /* CPY $ss ZP */
case 0xC5: MR_Zp(I);M_CMP(_A,I);break;  /* CMP $ss ZP */
case 0xC6: MM_Zp(M_DEC);break;            /* DEC $ss ZP */
case 0xE4: MR_Zp(I);M_CMP(_X,I);break;  /* CPX $ss ZP */
case 0xE5: MR_Zp(I);M_SBC(I);break;       /* SBC $ss ZP */
case 0xE6: MM_Zp(M_INC);break;            /* INC $ss ZP */

case 0x0D: MR_Ab(I);M_ORA(I);break;       /* ORA $ssss ABS */
case 0x0E: MM_Ab(M_ASL);break;            /* ASL $ssss ABS */
case 0x2D: MR_Ab(I);M_AND(I);break;       /* AND $ssss ABS */
case 0x2E: MM_Ab(M_ROL);break;            /* ROL $ssss ABS */
case 0x4D: MR_Ab(I);M_EOR(I);break;       /* EOR $ssss ABS */
case 0x4E: MM_Ab(M_LSR);break;            /* LSR $ssss ABS */
case 0x6D: MR_Ab(I);M_ADC(I);break;       /* ADC $ssss ABS */
case 0x6E: MM_Ab(M_ROR);break;            /* ROR $ssss ABS */
case 0x8C: MW_Ab(_Y);break;             /* STY $ssss ABS */
case 0x8D: MW_Ab(_A);break;             /* STA $ssss ABS */
case 0x8E: MW_Ab(_X);break;             /* STX $ssss ABS */
case 0xAC: MR_Ab(_Y);M_FL(_Y);break;  /* LDY $ssss ABS */
case 0xAD: MR_Ab(_A);M_FL(_A);break;  /* LDA $ssss ABS */
case 0xAE: MR_Ab(_X);M_FL(_X);break;  /* LDX $ssss ABS */
case 0xCC: MR_Ab(I);M_CMP(_Y,I);break;  /* CPY $ssss ABS */
case 0xCD: MR_Ab(I);M_CMP(_A,I);break;  /* CMP $ssss ABS */
case 0xCE: MM_Ab(M_DEC);break;            /* DEC $ssss ABS */
case 0xEC: MR_Ab(I);M_CMP(_X,I);break;  /* CPX $ssss ABS */
case 0xED: MR_Ab(I);M_SBC(I);break;       /* SBC $ssss ABS */
case 0xEE: MM_Ab(M_INC);break;            /* INC $ssss ABS */

case 0x09: MR_Im(I);M_ORA(I);break;       /* ORA #$ss IMM */
case 0x29: MR_Im(I);M_AND(I);break;       /* AND #$ss IMM */
case 0x49: MR_Im(I);M_EOR(I);break;       /* EOR #$ss IMM */
case 0x69: MR_Im(I);M_ADC(I);break;       /* ADC #$ss IMM */
case 0xA0: MR_Im(_Y);M_FL(_Y);break;  /* LDY #$ss IMM */
case 0xA2: MR_Im(_X);M_FL(_X);break;  /* LDX #$ss IMM */
case 0xA9: MR_Im(_A);M_FL(_A);break;  /* LDA #$ss IMM */
case 0xC0: MR_Im(I);M_CMP(_Y,I);break;  /* CPY #$ss IMM */
case 0xC9: MR_Im(I);M_CMP(_A,I);break;  /* CMP #$ss IMM */
case 0xE0: MR_Im(I);M_CMP(_X,I);break;  /* CPX #$ss IMM */
case 0xE9: MR_Im(I);M_SBC(I);break;       /* SBC #$ss IMM */

case 0x15: MR_Zx(I);M_ORA(I);break;       /* ORA $ss,x ZP,x */
case 0x16: MM_Zx(M_ASL);break;            /* ASL $ss,x ZP,x */
case 0x35: MR_Zx(I);M_AND(I);break;       /* AND $ss,x ZP,x */
case 0x36: MM_Zx(M_ROL);break;            /* ROL $ss,x ZP,x */
case 0x55: MR_Zx(I);M_EOR(I);break;       /* EOR $ss,x ZP,x */
case 0x56: MM_Zx(M_LSR);break;            /* LSR $ss,x ZP,x */
case 0x75: MR_Zx(I);M_ADC(I);break;       /* ADC $ss,x ZP,x */
case 0x76: MM_Zx(M_ROR);break;            /* ROR $ss,x ZP,x */
case 0x94: MW_Zx(_Y);break;             /* STY $ss,x ZP,x */
case 0x95: MW_Zx(_A);break;             /* STA $ss,x ZP,x */
case 0x96: MW_Zy(_X);break;             /* STX $ss,y ZP,y */
case 0xB4: MR_Zx(_Y);M_FL(_Y);break;  /* LDY $ss,x ZP,x */
case 0xB5: MR_Zx(_A);M_FL(_A);break;  /* LDA $ss,x ZP,x */
case 0xB6: MR_Zy(_X);M_FL(_X);break;  /* LDX $ss,y ZP,y */
case 0xD5: MR_Zx(I);M_CMP(_A,I);break;  /* CMP $ss,x ZP,x */
case 0xD6: MM_Zx(M_DEC);break;            /* DEC $ss,x ZP,x */
case 0xF5: MR_Zx(I);M_SBC(I);break;       /* SBC $ss,x ZP,x */
case 0xF6: MM_Zx(M_INC);break;            /* INC $ss,x ZP,x */

case 0x19: MR_Ay(I);M_ORA(I);break;       /* ORA $ssss,y ABS,y */
case 0x1D: MR_Ax(I);M_ORA(I);break;       /* ORA $ssss,x ABS,x */
case 0x1E: MM_Ax(M_ASL);break;            /* ASL $ssss,x ABS,x */
case 0x39: MR_Ay(I);M_AND(I);break;       /* AND $ssss,y ABS,y */
case 0x3D: MR_Ax(I);M_AND(I);break;       /* AND $ssss,x ABS,x */
case 0x3E: MM_Ax(M_ROL);break;            /* ROL $ssss,x ABS,x */
case 0x59: MR_Ay(I);M_EOR(I);break;       /* EOR $ssss,y ABS,y */
case 0x5D: MR_Ax(I);M_EOR(I);break;       /* EOR $ssss,x ABS,x */
case 0x5E: MM_Ax(M_LSR);break;            /* LSR $ssss,x ABS,x */
case 0x79: MR_Ay(I);M_ADC(I);break;       /* ADC $ssss,y ABS,y */
case 0x7D: MR_Ax(I);M_ADC(I);break;       /* ADC $ssss,x ABS,x */
case 0x7E: MM_Ax(M_ROR);break;            /* ROR $ssss,x ABS,x */
case 0x99: MW_Ay(_A);break;             /* STA $ssss,y ABS,y */
case 0x9D: MW_Ax(_A);break;             /* STA $ssss,x ABS,x */
case 0xB9: MR_Ay(_A);M_FL(_A);break;  /* LDA $ssss,y ABS,y */
case 0xBC: MR_Ax(_Y);M_FL(_Y);break;  /* LDY $ssss,x ABS,x */
case 0xBD: MR_Ax(_A);M_FL(_A);break;  /* LDA $ssss,x ABS,x */
case 0xBE: MR_Ay(_X);M_FL(_X);break;  /* LDX $ssss,y ABS,y */
case 0xD9: MR_Ay(I);M_CMP(_A,I);break;  /* CMP $ssss,y ABS,y */
case 0xDD: MR_Ax(I);M_CMP(_A,I);break;  /* CMP $ssss,x ABS,x */
case 0xDE: MM_Ax(M_DEC);break;            /* DEC $ssss,x ABS,x */
case 0xF9: MR_Ay(I);M_SBC(I);break;       /* SBC $ssss,y ABS,y */
case 0xFD: MR_Ax(I);M_SBC(I);break;       /* SBC $ssss,x ABS,x */
case 0xFE: MM_Ax(M_INC);break;            /* INC $ssss,x ABS,x */

case 0x01: MR_Ix(I);M_ORA(I);break;       /* ORA ($ss,x) INDEXINDIR */
case 0x11: MR_Iy(I);M_ORA(I);break;       /* ORA ($ss),y INDIRINDEX */
case 0x21: MR_Ix(I);M_AND(I);break;       /* AND ($ss,x) INDEXINDIR */
case 0x31: MR_Iy(I);M_AND(I);break;       /* AND ($ss),y INDIRINDEX */
case 0x41: MR_Ix(I);M_EOR(I);break;       /* EOR ($ss,x) INDEXINDIR */
case 0x51: MR_Iy(I);M_EOR(I);break;       /* EOR ($ss),y INDIRINDEX */
case 0x61: MR_Ix(I);M_ADC(I);break;       /* ADC ($ss,x) INDEXINDIR */
case 0x71: MR_Iy(I);M_ADC(I);break;       /* ADC ($ss),y INDIRINDEX */
case 0x81: MW_Ix(_A);break;             /* STA ($ss,x) INDEXINDIR */
case 0x91: MW_Iy(_A);break;             /* STA ($ss),y INDIRINDEX */
case 0xA1: MR_Ix(_A);M_FL(_A);break;  /* LDA ($ss,x) INDEXINDIR */
case 0xB1: MR_Iy(_A);M_FL(_A);break;  /* LDA ($ss),y INDIRINDEX */
case 0xC1: MR_Ix(I);M_CMP(_A,I);break;  /* CMP ($ss,x) INDEXINDIR */
case 0xD1: MR_Iy(I);M_CMP(_A,I);break;  /* CMP ($ss),y INDIRINDEX */
case 0xE1: MR_Ix(I);M_SBC(I);break;       /* SBC ($ss,x) INDEXINDIR */
case 0xF1: MR_Iy(I);M_SBC(I);break;       /* SBC ($ss),y INDIRINDEX */

case 0x0A: M_ASL(_A);break;             /* ASL a ACC */
case 0x2A: M_ROL(_A);break;             /* ROL a ACC */
case 0x4A: M_LSR(_A);break;             /* LSR a ACC */
case 0x6A: M_ROR(_A);break;             /* ROR a ACC */

default:
  /*if(_TrapBadOps) {
    printf
    (
      "[M6502 %lX] Unrecognized instruction: $%02X at PC=$%04X\n",
      _User,Op6502(_PC_-1),(word)(_PC_-1)
    );
    _Trace=1;
  }*/
  break;
