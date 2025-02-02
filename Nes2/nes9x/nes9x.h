#ifndef __NES9X_H__
#define __NES9X_H__

#include "../../include/Lib_Clara.h"
#define free mfree

/* nes9x.c */
extern byte *Page[],*VPage[];
extern byte RAM[],SPRAM[];
extern byte *ROM,*VROM,*WRAM,*VRAM;

extern int ROM_size;
extern int VROM_size;
extern int ROM_type;
extern int MMC_type;
extern int Mirroring,Mirrorxor;

/* mapper.c */
extern void (*MMC_write)(word A,byte V);
extern int MMC_init(int type);

/* other */
#ifndef TRUE
#define	TRUE	1
#define	FALSE	0
#endif

#endif
