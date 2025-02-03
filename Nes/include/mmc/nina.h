
/* Mapper #34 (NINA-001). */
/* This mapper is fully supported. */


int nina_init (void);
void nina_reset (void);

void nina_save_state (PACKFILE , int);
void nina_load_state (PACKFILE , int);


const MMC mmc_nina =
{
    34, "NINA-001",
    nina_init, nina_reset,


    "NINA\0\0\0\0",
    nina_save_state, nina_load_state
};


UINT8 nina_prg_bank;
UINT8 nina_chr_bank[2];


void nina_update_prg_bank (void)
{
    /* Set requested 32k ROM page at $8000. */

    cpu_set_read_address_32k_rom_block (0x8000, nina_prg_bank);
}


void nina_update_chr_bank (int bank)
{
    int index, page_base;

    /* Verify that CHR-ROM is present. */

    if (ROM_CHR_ROM_PAGES == 0)
    {
        return;
    }


    /* Convert 4k CHR-ROM page index to 1k. */

    page_base = nina_chr_bank[bank] * 4;


    /* Set requested 4k CHR-ROM page. */

    for (index = 0; index < 4; index ++)
    {
        ppu_set_ram_1k_pattern_vrom_block (((bank << 12) + (index << 10)),
         (page_base + index));
    }
}


void nina_write_low (UINT16 address, UINT8 value)
{
    //int index;


    /* Check if address is out-of-bounds. */

    if (address < 0x7ffd)
    {
        return;
    }


    switch (address)
    {
        case 0x7ffe:
        case 0x7fff:

            /* Set requested 4k CHR-ROM page. */

            nina_chr_bank[address & 1] = value;

            nina_update_chr_bank (address & 1);


            break;


        case 0x7ffd:

            /* Set requested 32k ROM page at $8000. */

            nina_prg_bank = value;

            nina_update_prg_bank ();


            break;


        default:


            break;
    }
}


void nina_write_high (UINT16 address, UINT8 value)
{
    /* Set requested 32k ROM page at $8000. */

    nina_prg_bank = value;

    nina_update_prg_bank ();
}


void nina_reset (void)
{
    /* Select first 32k ROM page at $8000. */
    nina_prg_bank = 0;

    nina_update_prg_bank ();


    /* Select first 8k CHR-ROM page at PPU $0000. */
    nina_chr_bank[0] = 0;
    nina_chr_bank[1] = 1;

    nina_update_chr_bank (0);
    nina_update_chr_bank (1);
}


int nina_init (void)
{
    /* Set initial mappings. */

    nina_reset ();


    /* Install write handlers. */

    cpu_set_write_handler_2k (0x7800, nina_write_low);

    cpu_set_write_handler_32k (0x8000, nina_write_high);


    return (0);
}


void nina_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Save banking */
    pack_putc (nina_prg_bank, file_chunk);
    pack_fwrite (nina_chr_bank, 2, file_chunk);


    pack_fclose_chunk (file_chunk);
}


static void nina_load_state (PACKFILE file, int version)
{
    //int index;

    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Restore banking */
    nina_prg_bank = pack_getc (file_chunk);
    pack_fread (nina_chr_bank, 2, file_chunk);


    nina_update_prg_bank ();

    nina_update_chr_bank (0);
    nina_update_chr_bank (1);


    pack_fclose_chunk (file_chunk);
}

