
/* Mapper #3 (CNROM). */
/* This mapper is fully supported. */


#include "mmc/shared.h"


int cnrom_init (void);
void cnrom_reset (void);

void cnrom_save_state (PACKFILE, int);
void cnrom_load_state (PACKFILE, int);

const MMC mmc_cnrom =
{
    3, "CNROM",
    cnrom_init, cnrom_reset,

    "CNROM\0\0\0",
    cnrom_save_state, cnrom_load_state
};


UINT8 cnrom_last_write = 0;


void cnrom_write (UINT16 address, UINT8 value)
{
    int index;

    /* Store page # for state saving. */
    cnrom_last_write = value;

    /* Convert 8k page # to 1k. */
    value *= 8;

    /* Select requested 8k CHR-ROM page. */
    for (index = 0; index < 8; index ++)
    {
        ppu_set_ram_1k_pattern_vrom_block ((index << 10), (value + index));
    }
}


void cnrom_reset (void)
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


int cnrom_init (void)
{
    /* Mapper requires some CHR ROM */
    if (mmc_pattern_vram_in_use)
    {
        return -1;
    }

    /* Set initial mappings. */
    cnrom_reset ();

    /* Install write handler. */
    cpu_set_write_handler_32k (0x8000, cnrom_write);


    return (0);
}


static void cnrom_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);

    pack_putc (cnrom_last_write, file_chunk);

    pack_fclose_chunk (file_chunk);
}


static void cnrom_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;

    file_chunk = pack_fopen_chunk (file, FALSE);

    cnrom_write (0x8000, pack_getc (file_chunk));

    pack_fclose_chunk (file_chunk);
}
