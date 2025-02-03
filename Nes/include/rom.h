/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

rom.c: Implementation of the ROM file handling.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#ifndef ROM_H_INCLUDED
#define ROM_H_INCLUDED


#include "misc.h"


typedef struct _ROM ROM;


#include "mmc.h"


extern ROM global_rom;
extern int rom_is_loaded;


#define ROM_TRAINER_SIZE    512


#define ROM_CTRL_MIRRORING      0x01
#define ROM_CTRL_BATTERY        0x02
#define ROM_CTRL_TRAINER        0x04
#define ROM_CTRL_FOUR_SCREEN    0x08


struct _ROM
{
    const MMC * current_mmc;            /* Memory mapping controller. */

    UINT8 * trainer;
    UINT32 trainer_crc32;               /* Checksum for trainer. */

    UINT8 * prg_rom;
    UINT32 prg_rom_crc32;               /* Checksum for PRG ROM. */

    UINT8 * chr_rom;
    UINT32 chr_rom_crc32;                /* Checksum for CHR ROM. */

    UINT8 * chr_rom_cache;              /* CHR-ROM cache buffer. */
    UINT8 * chr_rom_cache_tag;          /* CHR-ROM cache tag buffer. */

    int prg_rom_pages;
    int chr_rom_pages;

    UINT8 control_byte_1;
    UINT8 control_byte_2;

    int mapper_number;

    UINT8 chr_rom_page_overflow_mask;   /* CHR-ROM bank # wrapping mask. */
    UINT8 chr_rom_page_lookup [256];

    UINT8 prg_rom_page_overflow_mask;   /* PRG-ROM bank # wrapping mask. */
    UINT8 prg_rom_page_lookup [256];

    int sram_flag;

    wchar_t filename [256];
};


typedef struct _INES_HEADER
{
    UINT8 signature [4];

    UINT8 prg_rom_pages;
    UINT8 chr_rom_pages;

    UINT8 control_byte_1;
    UINT8 control_byte_2;

    UINT8 reserved [8];
} INES_HEADER;


int load_rom (const wchar_t *, ROM *);
void free_rom (const ROM *);


#define ROM_PRG_ROM_PAGES   global_rom.prg_rom_pages
#define ROM_CHR_ROM_PAGES   global_rom.chr_rom_pages

#define ROM_MAPPER_NUMBER   global_rom.mapper_number
#define ROM_CURRENT_MMC     global_rom.current_mmc

#define ROM_PRG_ROM         global_rom.prg_rom
#define ROM_CHR_ROM         global_rom.chr_rom

#define ROM_CHR_ROM_CACHE       global_rom.chr_rom_cache
#define ROM_CHR_ROM_CACHE_TAG   global_rom.chr_rom_cache_tag

#define ROM_PRG_ROM_PAGE_LOOKUP             global_rom.prg_rom_page_lookup
#define ROM_PRG_ROM_PAGE_OVERFLOW_MASK      global_rom.prg_rom_page_overflow_mask

#define ROM_CHR_ROM_PAGE_LOOKUP             global_rom.chr_rom_page_lookup
#define ROM_CHR_ROM_PAGE_OVERFLOW_MASK      global_rom.chr_rom_page_overflow_mask

#define ROM_PAGE_16K(index)     (ROM_PRG_ROM + (index) * 0x4000)
#define ROM_PAGE_8K(index)      (ROM_PRG_ROM + (index) * 0x2000)

#define FIRST_ROM_PAGE  ROM_PRG_ROM
#define LAST_ROM_PAGE   (ROM_PAGE_16K (ROM_PRG_ROM_PAGES - 1))


#endif /* ! ROM_H_INCLUDED */
