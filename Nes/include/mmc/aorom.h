/* Mapper #7 (AOROM). */
/* This mapper is fully supported. */

int aorom_init (void);
void aorom_reset (void);

void aorom_save_state (PACKFILE , int);
void aorom_load_state (PACKFILE , int);

const MMC mmc_aorom =
{
    7, "AOROM",
    aorom_init, aorom_reset,

    "AOROM\0\0\0",
    aorom_save_state, aorom_load_state
};


UINT8 aorom_last_write = 0;


#define AOROM_MIRRORING_BIT     (1 << 4)

void aorom_write (UINT16 address, UINT8 value)
{
    //int index;


    /* Save page # and mirroring for state saving. */
    aorom_last_write = value;


    /* Is this reversed? */
    ppu_set_mirroring (((value & AOROM_MIRRORING_BIT) ? MIRRORING_ONE_SCREEN_2400 : MIRRORING_ONE_SCREEN_2000));


    /* Mask off upper 4 bits. */
    value &= 0x0f;


    /* Select requested 32k page. */
    cpu_set_read_address_32k_rom_block (0x8000, value);
}


void aorom_reset (void)
{
    //int index;

    /* Select first 32k page. */
    cpu_set_read_address_32k_rom_block (0x8000, 0);
}


int aorom_init (void)
{
    /* No VROM hardware. */

    ppu_set_ram_8k_pattern_vram ();

    mmc_pattern_vram_in_use = TRUE;



    /* Set the default mirroring. */

    mmc_name_table_count = 2;
    ppu_set_mirroring (MIRRORING_ONE_SCREEN_2000);


    /* Set initial mappings. */

    aorom_reset ();


    /* Install write handler. */

    cpu_set_write_handler_32k (0x8000, aorom_write);


    return (0);
}


void aorom_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;

    file_chunk = pack_fopen_chunk (file, FALSE);

    pack_putc (aorom_last_write, file_chunk);

    pack_fclose_chunk (file_chunk);
}


void aorom_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;

    file_chunk = pack_fopen_chunk (file, FALSE);

    aorom_write (0x8000, pack_getc (file_chunk));

    pack_fclose_chunk (file_chunk);
}
