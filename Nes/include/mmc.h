/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

mmc.h: Declarations for the MMC emulations.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#ifndef MMC_H_INCLUDED
#define MMC_H_INCLUDED


#include "misc.h"


typedef struct _MMC
{
    int number;

    const UINT8 * name;

    int (* init) (void);
    void (* reset) (void);

    const UINT8 * id;

    void (* save_state) (PACKFILE, int);
    void (* load_state) (PACKFILE, int);
} MMC;


int mmc_init (void);
void mmc_reset (void);

void mmc_request (ROM *);

extern int (* mmc_hblank_start) (int);
extern int (* mmc_scanline_end) (int);

extern void (* mmc_check_latches) (UINT16);

int mmc_get_name_table_count (void);
int mmc_uses_pattern_vram (void);

void mmc_save_state (PACKFILE, int);
void mmc_load_state (PACKFILE, int);


#endif /* ! MMC_H_INCLUDED */
