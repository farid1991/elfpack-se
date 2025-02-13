
/* Mapper #68 (Sunsoft mapper #4). */
/* This mapper is fully supported. */


int sunsoft4_init (void);
void sunsoft4_reset (void);

void sunsoft4_save_state (PACKFILE , int);
void sunsoft4_load_state (PACKFILE , int);


const MMC mmc_sunsoft4 =
{
    68, "Sunsoft mapper #4",
    sunsoft4_init, sunsoft4_reset,


    "SUNSOFT4",
    sunsoft4_save_state, sunsoft4_load_state
};


UINT8 sunsoft4_name_table_banks [2];

UINT8 sunsoft4_name_table_control = 0;


UINT8 sunsoft4_prg_bank;
UINT8 sunsoft4_chr_bank [4];


void sunsoft4_fixup_name_tables (void)
{
    if (! (sunsoft4_name_table_control & 0x10))
    {
        /* VRAM name tables */

        if (! (sunsoft4_name_table_control & 2))
        {
            /* Horizontal/vertical mirroring. */

            ppu_set_mirroring (((sunsoft4_name_table_control & 1) ?
                MIRRORING_HORIZONTAL : MIRRORING_VERTICAL));
        }
        else
        {
            /* One-screen mirroring. */

            ppu_set_mirroring (((sunsoft4_name_table_control & 1) ?
                MIRRORING_ONE_SCREEN_2400 : MIRRORING_ONE_SCREEN_2000));
        }
    }
    else
    {
        /* CHR-ROM name tables. */

        if (! (sunsoft4_name_table_control & 2))
        {
            /* Horizontal/vertical mirroring. */

            if (! (sunsoft4_name_table_control & 1))
            {
                /* Vertical mirroring. */

                ppu_set_name_table_address_vrom (0, (sunsoft4_name_table_banks [0] | 0x80));

                ppu_set_name_table_address_vrom (1, (sunsoft4_name_table_banks [1] | 0x80));

                ppu_set_name_table_address_vrom (2, (sunsoft4_name_table_banks [0] | 0x80));

                ppu_set_name_table_address_vrom (3, (sunsoft4_name_table_banks [1] | 0x80));
            }
            else
            {
                /* Horizontal mirroring. */

                ppu_set_name_table_address_vrom (0, (sunsoft4_name_table_banks [0] | 0x80));

                ppu_set_name_table_address_vrom (1, (sunsoft4_name_table_banks [0] | 0x80));

                ppu_set_name_table_address_vrom (2, (sunsoft4_name_table_banks [1] | 0x80));

                ppu_set_name_table_address_vrom (3, (sunsoft4_name_table_banks [1] | 0x80));
            }
        }
        else
        {
            /* One-screen mirroring. */

            ppu_set_name_table_address_vrom (0, (sunsoft4_name_table_banks
                [sunsoft4_name_table_control & 1] | 0x80));

            ppu_set_name_table_address_vrom (1, (sunsoft4_name_table_banks
                [sunsoft4_name_table_control & 1] | 0x80));

            ppu_set_name_table_address_vrom (2, (sunsoft4_name_table_banks
                [sunsoft4_name_table_control & 1] | 0x80));

            ppu_set_name_table_address_vrom (3, (sunsoft4_name_table_banks
               [sunsoft4_name_table_control & 1] | 0x80));
        }
    }
}


void sunsoft4_update_prg_bank (void)
{
    /* Select 16k page in lower 16k. */

    cpu_set_read_address_16k_rom_block (0x8000, sunsoft4_prg_bank);
}


void sunsoft4_update_chr_bank (int bank)
{
    /* Calculate PPU address. */

    int address = (bank * 0x800);


    /* Convert 2k page # to 1k. */

    int value = sunsoft4_chr_bank [bank] << 1;


    /* Select 2k page at PPU address. */

    ppu_set_ram_1k_pattern_vrom_block (address, value);

    ppu_set_ram_1k_pattern_vrom_block ((address + 0x400), (value + 1));
}


void sunsoft4_write (UINT16 address, UINT8 value)
{
    address >>= 12;


    switch (address)
    {
        case (0x8000 >> 12):

        case (0x9000 >> 12):

        case (0xa000 >> 12):

        case (0xb000 >> 12):


            /* Select 2k page at PPU address. */

            sunsoft4_chr_bank [address - 8] = value;

            sunsoft4_update_chr_bank (address - 8);

            break;


        case (0xc000 >> 12):

        case (0xd000 >> 12):

            sunsoft4_name_table_banks [address - (0xc000 >> 12)] = value;

            sunsoft4_fixup_name_tables ();


            break;


        case (0xe000 >> 12):

            if ((sunsoft4_name_table_control & 0x13) != (value & 0x13))
            {
                /* Name tables changed? */

                sunsoft4_name_table_control = value;

                sunsoft4_fixup_name_tables ();
            }


            break;


        case (0xf000 >> 12):

            /* Select 16k page in lower 16k. */

            sunsoft4_prg_bank = value;

            sunsoft4_update_prg_bank ();


            break;
    }
}


void sunsoft4_reset (void)
{
    /* Select first 16k page in lower 16k. */

    sunsoft4_prg_bank = 0;

    sunsoft4_update_prg_bank ();


    /* Select last 16k page in upper 16k. */

    cpu_set_read_address_16k (0xc000, LAST_ROM_PAGE);
}


int sunsoft4_init (void)
{
    int index;

    /* Mapper requires some CHR ROM */
    if (mmc_pattern_vram_in_use)
    {
        return -1;
    }


    /* Set initial name table mappings. */

    sunsoft4_name_table_banks [0] = sunsoft4_name_table_banks [1] = 0;

    sunsoft4_name_table_control = ((ppu_get_mirroring () == MIRRORING_VERTICAL) ? 0 : 1);


    /* Set initial pattern table mappings. */

    for (index = 0; index < 4; index++)
    {
        sunsoft4_chr_bank [index] = index;

        sunsoft4_update_chr_bank (index);
    }


    /* Set initial mappings. */

    sunsoft4_reset ();


    /* Install write handler. */

    cpu_set_write_handler_32k (0x8000, sunsoft4_write);


    return (0);
}


void sunsoft4_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Save PRG banking */
    pack_putc (sunsoft4_prg_bank, file_chunk);

    /* Save CHR banking */
    pack_fwrite (sunsoft4_chr_bank, 4, file_chunk);

    /* Save name table banking */
    pack_putc (sunsoft4_name_table_control, file_chunk);
    pack_fwrite (sunsoft4_name_table_banks, 2, file_chunk);


    pack_fclose_chunk (file_chunk);
}


void sunsoft4_load_state (PACKFILE file, int version)
{
    int index;

    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Restore PRG banking */
    sunsoft4_prg_bank = pack_getc (file_chunk);

    sunsoft4_update_prg_bank ();


    /* Restore CHR banking */
    pack_fread (sunsoft4_chr_bank, 4, file_chunk);

    for (index = 0; index < 4; index++)
    {
        sunsoft4_chr_bank [index] = index;

        sunsoft4_update_chr_bank (index);
    }


    /* Restore name table banking */
    sunsoft4_name_table_control = pack_getc (file_chunk);
    pack_fread (sunsoft4_name_table_banks, 2, file_chunk);

    sunsoft4_fixup_name_tables ();



    pack_fclose_chunk (file_chunk);
}

