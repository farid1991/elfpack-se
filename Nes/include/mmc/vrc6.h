
/* Mappers #24 (Konami VRC6). */
/* This mapper is fully supported. */

/* Mapper #26 (Konami VRC6V). */
/* This mapper is fully supported. */


#include "mmc/shared.h"


int vrc6_init (void);
void vrc6_reset (void);


void vrc6_save_state (PACKFILE , int);
void vrc6_load_state (PACKFILE , int);


const MMC mmc_vrc6 =
{
    24, "Konami VRC6 + ExSound",
    vrc6_init, vrc6_reset,


    "VRC6\0\0\0\0",
    vrc6_save_state, vrc6_load_state
};


int vrc6v_init (void);


const MMC mmc_vrc6v =
{
    26, "Konami VRC6V + ExSound",
    vrc6v_init, vrc6_reset,


    "VRC6V\0\0\0",
    vrc6_save_state, vrc6_load_state
};


const char vrc6_mirroring_table [] =
{
    MIRRORING_VERTICAL, MIRRORING_HORIZONTAL,

    MIRRORING_ONE_SCREEN_2000, MIRRORING_ONE_SCREEN_2400
};

const UINT8 vrc6_mirroring_mask = 0x0c;


char vrc6_enable_irqs = FALSE;


UINT8 vrc6_irq_counter = 0;
UINT8 vrc6_irq_latch = 0;


UINT8 vrc6_prg_bank[2];
UINT8 vrc6_chr_bank[8];


int vrc6_irq_tick (int line)
{
    if (vrc6_enable_irqs & 0x02)
    {
        if (vrc6_irq_counter == 0xff)
        {
            vrc6_irq_counter = vrc6_irq_latch;


            return CPU_INTERRUPT_IRQ;
        }
        else
        {
            vrc6_irq_counter ++;
        }
    }


    return CPU_INTERRUPT_NONE;
}


char vrc6_swap_address_pins = FALSE;


INLINE void vrc6_update_prg_bank (int bank)
{
    if (bank == 0)
    /* 16k ROM page select. */
    {
        cpu_set_read_address_16k_rom_block (0x8000, vrc6_prg_bank[0]);
    }
    else
    /* 8k ROM page select. */
    {
        cpu_set_read_address_8k_rom_block (0xC000, vrc6_prg_bank[1]);
    }
}


INLINE void vrc6_update_chr_bank (int bank)
{
    if (ROM_CHR_ROM_PAGES > 0)
    {
        /* set new VROM banking */
        ppu_set_ram_1k_pattern_vrom_block (bank << 10, vrc6_chr_bank[bank]);
    }
}


void vrc6_write (UINT16 address, UINT8 value)
{
    int major;

    int minor;


    /* Swap address pins. */

    if (vrc6_swap_address_pins)
    {
        address = ((address & 0xfffc) | ((address >> 1) & 1) | ((address << 1) & 2));
    }


    /* Extract command indexes. */

    major = (address & 0xf000);

    minor = (address & 0x000f);


    switch (major)
    {
        case 0x8000:

            if (minor == 0x0000)
            {
                /* Set requested 16k ROM page at $8000. */

                vrc6_prg_bank[0] = value;

                vrc6_update_prg_bank (0);
            }


            break;


        case 0xb000:

            if (minor == 0x0003)
            {
                /* Mirroring select. */

                /* Discard unused bits. */

                value &= vrc6_mirroring_mask;

                value >>= 2;


                /* Use value from LUT. */

                ppu_set_mirroring (vrc6_mirroring_table [value]);
            }


            break;


        case 0xc000:

            if (minor == 0x0000)
            {
                /* Set requested 8k ROM page at $C000. */

                vrc6_prg_bank[1] = value;

                vrc6_update_prg_bank (1);
            }


            break;


        case 0xd000:

            if (minor < 0x0004)
            {
                /* Set requested 1k CHR-ROM page. */
                vrc6_chr_bank[minor] = value;

                vrc6_update_chr_bank (minor);
            }


            break;


        case 0xe000:

            if (minor < 0x0004)
            {
                /* Set requested 1k CHR-ROM page. */

                vrc6_chr_bank[minor + 4] = value;

                vrc6_update_chr_bank (minor + 4);
            }


            break;


        case 0xf000:

            if (minor == 0x0000)
            {
                /* Both (?) bytes of IRQ counter. */

                vrc6_irq_latch = value;
            }
            else if (minor == 0x0001)
            {
                /* Enable/disable IRQs. */

                vrc6_enable_irqs = (value & 0x03);


                if (vrc6_enable_irqs & 0x02)
                {
                    vrc6_irq_counter = vrc6_irq_latch;
                }
            }
            else if (minor == 0x0002)
            {
                /* ?? */

                if (vrc6_enable_irqs & 0x01)
                {
                    vrc6_enable_irqs |= 0x02;
                }
                else
                {
                    vrc6_enable_irqs &= 0x01;
                }
            }


            break;


        default:


            break;
    }


    //papu_exwrite (address, value);
}


void vrc6_reset (void)
{
    /* Reset PRG banking. */

    /* Select first 16k page in lower 16k. */
    vrc6_prg_bank[0] = 0;

    vrc6_update_prg_bank (0);

    /* Select last 16k page in upper 16k. */
    cpu_set_read_address_16k (0xc000, LAST_ROM_PAGE);

    vrc6_prg_bank[1] = (ROM_PRG_ROM_PAGES * 2) - 2;

    vrc6_update_prg_bank (1);
}


int vrc6_base_init (void)
{
    //int index;


    /* Set initial mappings. */

    vrc6_reset ();


    /* Install write handler. */

    cpu_set_write_handler_32k (0x8000, vrc6_write);


    /* Install IRQ tick handler. */

    mmc_scanline_end = vrc6_irq_tick;


    /* Select ExSound chip. */

    //papu_set_exsound (PAPU_EXSOUND_VRC6);


    return (0);
}


int vrc6_init (void)
{
    /* Disable address pin swap. */

    vrc6_swap_address_pins = FALSE;


    return (vrc6_base_init ());
}


int vrc6v_init (void)
{
    /* Pins A0 and A1 are swapped in VRC6V. */

    vrc6_swap_address_pins = TRUE;


    return (vrc6_base_init ());
}


void vrc6_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Restore IRQ registers */
    pack_putc (vrc6_irq_counter, file_chunk);
    pack_putc (vrc6_irq_latch, file_chunk);

    pack_putc (vrc6_enable_irqs, file_chunk);


    /* Restore banking */
    pack_fwrite (vrc6_prg_bank, 2, file_chunk);
    pack_fwrite (vrc6_chr_bank, 8, file_chunk);


    pack_fclose_chunk (file_chunk);
}


void vrc6_load_state (PACKFILE file, int version)
{
    int index;

    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Restore IRQ registers */
    vrc6_irq_counter = pack_getc (file_chunk);
    vrc6_irq_latch = pack_getc (file_chunk);

    vrc6_enable_irqs = pack_getc (file_chunk);


    /* Restore banking */
    pack_fread (vrc6_prg_bank, 2, file_chunk);
    pack_fread (vrc6_chr_bank, 8, file_chunk);


    vrc6_update_prg_bank (0);
    vrc6_update_prg_bank (1);


    for (index = 0; index < 8; index++)
    {
        vrc6_update_chr_bank (index);
    }


    pack_fclose_chunk (file_chunk);
}

