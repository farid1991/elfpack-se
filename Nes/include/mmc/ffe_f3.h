
/* Mapper #8 (FFE F3xxx). */
/* This mapper is fully supported. */


#include "mmc/shared.h"


int ffe_f3_init (void);
void ffe_f3_reset (void);

void ffe_f3_save_state (PACKFILE , int);
void ffe_f3_load_state (PACKFILE , int);


const MMC mmc_ffe_f3 =
{
    8, "FFE F3xxx",
    ffe_f3_init, ffe_f3_reset,

    "FFE_F3\0\0",
    ffe_f3_save_state, ffe_f3_load_state
};


UINT8 ffe_f3_last_write = 0;

#define FFE_F3_PRG_ROM_MASK     0xf8
#define FFE_F3_CHR_ROM_MASK     0x07


void ffe_f3_write (UINT16 address, UINT8 value)
{
    int index;


    int chr_page;

    int prg_page;


    /* Store page #s for state saving. */

    ffe_f3_last_write = value;


    /* Extract ROM page # (xxxxx000). */

    prg_page = ((value & FFE_F3_PRG_ROM_MASK) >> 3);


    /* Select requested 16k ROM page at $8000. */

    cpu_set_read_address_16k_rom_block (0x8000, prg_page);


    /* Extract CHR-ROM page # (00000xxx). */

    chr_page = (value & FFE_F3_CHR_ROM_MASK);


    /* Convert 8k page # to 1k. */

    chr_page *= 8;


    /* Select requested 8k CHR-ROM page. */

    for (index = 0; index < 8; index ++)
    {
        ppu_set_ram_1k_pattern_vrom_block ((index << 10), (chr_page + index));
    }
}


void ffe_f3_reset (void)
{
    int index;


    /* Note: Upper 16k ROM bank is hard-wired. */

    /* Select first 32k ROM page. */

    cpu_set_read_address_32k_rom_block (0x8000, MMC_FIRST_ROM_BLOCK);


    /* Select first 8k CHR-ROM page. */

    for (index = 0; index < 8; index ++)
    {
        ppu_set_ram_1k_pattern_vrom_block ((index << 10), index);
    }
}


int ffe_f3_init (void)
{
    /* Mapper requires some CHR ROM */
    if (mmc_pattern_vram_in_use)
    {
        return -1;
    }


    /* Set initial mappings. */

    ffe_f3_reset ();


    /* Install write handler. */

    cpu_set_write_handler_32k (0x8000, ffe_f3_write);


    return (0);
}


void ffe_f3_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    pack_putc (ffe_f3_last_write, file_chunk);


    pack_fclose_chunk (file_chunk);
}


void ffe_f3_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    ffe_f3_write (0x8000, pack_getc (file_chunk));


    pack_fclose_chunk (file_chunk);
}
