
/* Mapper #11 (Color Dreams). */
/* This mapper is fully supported. */


#include "mmc/shared.h"


int dreams_init (void);
void dreams_reset (void);

void dreams_save_state (PACKFILE , int);
void dreams_load_state (PACKFILE , int);


const MMC mmc_dreams =
{
    11, "Color Dreams",

    dreams_init, dreams_reset,


    "DREAMS\0\0",
    dreams_save_state, dreams_load_state
};


UINT8 dreams_last_write = 0;


#define DREAMS_PRG_ROM_MASK     0x0f
#define DREAMS_CHR_ROM_MASK     0x70


void dreams_write (UINT16 address, UINT8 value)
{
    int index;


    int prg_page;

    int chr_page;


    /* Store page #s for state saving. */

    dreams_last_write = value;


    /* Extract ROM page # (0000xxxx). */

    prg_page = (value & DREAMS_PRG_ROM_MASK);


    /* Select requested 32k ROM page. */

    cpu_set_read_address_32k_rom_block (0x8000, prg_page);


    /* Extract CHR-ROM page # (0xxx0000). */

    chr_page = ((value & DREAMS_CHR_ROM_MASK) >> 4);


    /* Convert 8k page # to 1k. */

    chr_page *= 8;


    /* Select requested 8k CHR-ROM page. */

    for (index = 0; index < 8; index ++)
    {
        ppu_set_ram_1k_pattern_vrom_block ((index << 10), (chr_page + index));
    }
}


void dreams_reset (void)
{
    int index;


    /* Select first 32k ROM page. */

    cpu_set_read_address_32k_rom_block (0x8000, MMC_FIRST_ROM_BLOCK);


    /* Select first 8k CHR-ROM page. */

    for (index = 0; index < 8; index ++)
    {
        ppu_set_ram_1k_pattern_vrom_block ((index << 10), index);
    }
}


int dreams_init (void)
{
    /* Mapper requires some CHR ROM */
    if (mmc_pattern_vram_in_use)
    {
        return -1;
    }


    /* Set initial mappings. */

    dreams_reset ();


    /* Install write handler. */

    cpu_set_write_handler_32k (0x8000, dreams_write);


    return (0);
}


void dreams_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    pack_putc (dreams_last_write, file_chunk);


    pack_fclose_chunk (file_chunk);
}


void dreams_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    dreams_write (0x8000, pack_getc (file_chunk));


    pack_fclose_chunk (file_chunk);
}
