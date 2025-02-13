/* Mapper #16 (Bandai). */
/* This mapper is only partially supported. */

/* Todo: Add EEPROM support. */


#include "mmc/shared.h"


int bandai_init (void);
void bandai_reset (void);

void bandai_save_state (PACKFILE , int);
void bandai_load_state (PACKFILE , int);


const MMC mmc_bandai =
{
    16, "Bandai",

    bandai_init, bandai_reset,


    "BANDAI\0\0",

    bandai_save_state, bandai_load_state
};


const char bandai_mirroring_table [] =
{
    MIRRORING_VERTICAL, MIRRORING_HORIZONTAL,

    MIRRORING_ONE_SCREEN_2000, MIRRORING_ONE_SCREEN_2400
};

const UINT8 bandai_mirroring_mask = 0x03;


char bandai_enable_irqs = FALSE;


int bandai_irq_counter = 0;

PAIR bandai_irq_latch;


UINT8 bandai_prg_bank;
UINT8 bandai_chr_bank[8];


int bandai_irq_tick (int line)
{
    if (bandai_enable_irqs)
    {
        bandai_irq_counter -= MMC_PSEUDO_CLOCKS_PER_SCANLINE;


        if (bandai_irq_counter <= 0)
        {
            bandai_irq_counter = 0;


            bandai_enable_irqs = FALSE;

            return CPU_INTERRUPT_IRQ_MMC;
        }
        else
        {
            return CPU_INTERRUPT_NONE;
        }
    }

    return CPU_INTERRUPT_NONE;
}


INLINE void bandai_update_prg_bank (void)
{
    cpu_set_read_address_16k_rom_block (0x8000, bandai_prg_bank);
}


INLINE void bandai_update_chr_bank (int bank)
{
    if (ROM_CHR_ROM_PAGES > 0)
    {
        /* set new VROM banking */
        ppu_set_ram_1k_pattern_vrom_block (bank << 10, bandai_chr_bank[bank]);
    }
}


void bandai_write (UINT16 address, UINT8 value)
{
    /* Extract write port index. */

    address &= 0x000f;


    if (address > 0x000d)
    {
        return;
    }


    if (address <= 7)
    {
        /* Set requested 1k CHR-ROM page. */
        bandai_chr_bank[address] = value;

        bandai_update_chr_bank(address);
    }
    else
    {
        /* Convert $600X and $7ffX to $800X. */

        address += 0x8000;


        switch (address)
        {
            case 0x8008:

                /* Set requested 16k ROM page at $8000. */
                bandai_prg_bank = value;

                bandai_update_prg_bank ();


                break;


            case 0x8009:

                /* Mirroring select. */

                /* Mask off upper 6 bits. */

                value &= bandai_mirroring_mask;


                /* Use value from LUT. */

                ppu_set_mirroring (bandai_mirroring_table [value]);


                break;


            case 0x800a:

                /* Enable/disable IRQs. */

                cpu_clear_interrupt (CPU_INTERRUPT_IRQ_MMC);

                bandai_enable_irqs = (value & 0x01);

                bandai_irq_counter = bandai_irq_latch.word;


                break;


            case 0x800b:

                /* Low byte of IRQ counter. */

                bandai_irq_latch.bytes.low = value;


                break;


            case 0x800c:

                /* High byte of IRQ counter. */

                bandai_irq_latch.bytes.high = value;


                break;
        }
    }
}


void bandai_reset (void)
{
    /* Select first 16k page in lower 16k. */
    bandai_prg_bank = 0;

    bandai_update_prg_bank ();

    cpu_set_read_address_16k (0x8000, FIRST_ROM_PAGE);


    /* Select last 16k page in upper 16k. */

    cpu_set_read_address_16k (0xc000, LAST_ROM_PAGE);
}


int bandai_init (void)
{
    int index;


    /* Set initial mappings. */

    for (index = 0; index < 8; index++)
    {
        bandai_chr_bank[index] = index;

        bandai_update_chr_bank (index);
    }


    bandai_reset ();


    /* Install write handlers. */

    cpu_set_write_handler_8k (0x6000, bandai_write);

    cpu_set_write_handler_32k (0x8000, bandai_write);


    /* Install IRQ tick handler. */

    mmc_scanline_end = bandai_irq_tick;


    return (0);
}


void bandai_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Save IRQ registers */
    pack_iputw (bandai_irq_counter, file_chunk);
    pack_iputw (bandai_irq_latch.word, file_chunk);

    pack_putc (bandai_enable_irqs, file_chunk);


    /* Save banking */
    pack_putc (bandai_prg_bank, file_chunk);
    pack_fwrite (bandai_chr_bank, 8, file_chunk);


    pack_fclose_chunk (file_chunk);
}


void bandai_load_state (PACKFILE file, int version)
{
    int index;

    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Restore IRQ registers */
    bandai_irq_counter = pack_igetw (file_chunk);
    bandai_irq_latch.word = pack_igetw (file_chunk);

    bandai_enable_irqs = pack_getc (file_chunk);


    /* Restore banking */
    bandai_prg_bank = pack_getc (file_chunk);
    pack_fread (bandai_chr_bank, 8, file_chunk);


    bandai_update_prg_bank ();

    for (index = 0; index < 8; index++)
    {
        bandai_update_chr_bank (index);
    }


    pack_fclose_chunk (file_chunk);
}

