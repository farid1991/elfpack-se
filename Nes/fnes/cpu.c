/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

cpu.c: Implementation of the CPU abstraction.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#include "cpu.h"
#include "input.h"
#include "mmc.h"
#include "papu.h"
#include "ppu.h"
#include "rom.h"

#include "core.h"
#include "misc.h"

#include "crc32.h"

UINT8 cpu_sram [8192];
UINT8 cpu_ram [VIRTUAL_RAM_SIZE];
FN2A03 cpu_context;

UINT8 * cpu_block_2k_read_address [MAX_BLOCKS];
UINT8 * cpu_block_2k_write_address [MAX_BLOCKS];
UINT8 (* cpu_block_2k_read_handler [MAX_BLOCKS]) (UINT16);
void (* cpu_block_2k_write_handler [MAX_BLOCKS]) (UINT16, UINT8);

UINT16 * cpu_active_pc;

UINT8 dummy_read [(8 << 10)];
UINT8 dummy_write [(8 << 10)];

#if 0
static UINT8 * get_patches_filename (UINT8 * buffer, const UINT8 * rom_filename, int buffer_size)
{
    memset (buffer, NIL, buffer_size);


    strcat (buffer, get_config_string ("gui", "save_path", "./"));

    put_backslash (buffer);


    strcat (buffer, get_filename (rom_filename));


    replace_extension (buffer, buffer, "fpt", buffer_size);


    return (buffer);
}


static UINT8 cpu_patch_titles [MAX_PATCHES] [16];

void patches_load (const char * rom_filename)
{
    UINT8 buffer [256];

    int index;

    memset (cpu_patch_titles, NIL, sizeof (cpu_patch_titles));

    for (index = 0; index < MAX_PATCHES; index ++)
    {
        cpu_patch_info [index].title = cpu_patch_titles [index];
    }

    get_patches_filename (buffer, rom_filename, sizeof (buffer));

    if (exists (buffer))
    {
        int version;


        push_config_state ();


        set_config_file (buffer);


        version = get_config_hex ("header", "version", 0x0100);


        if (version > 0x100)
        {
            pop_config_state ();


            return;
        }


        cpu_patch_count = get_config_int ("header", "patch_count", 0);


        if (cpu_patch_count > MAX_PATCHES)
        {
            cpu_patch_count = MAX_PATCHES;
        }
        else if (cpu_patch_count < 0)
        {
            cpu_patch_count = 0;
        }


        for (index = 0; index < cpu_patch_count; index ++)
        {
            char * title;


            memset (buffer, NIL, sizeof (buffer));


            sprintf (buffer, "patch%02d", index);


            strncat (cpu_patch_info [index].title, get_config_string (buffer, "title", "?"), 15);


            cpu_patch_info [index].address = get_config_hex (buffer, "address", 0xffff);


            cpu_patch_info [index].value = get_config_hex (buffer, "value", 0xff);

            cpu_patch_info [index].match_value = get_config_hex (buffer, "match_value", 0xff);


            cpu_patch_info [index].enabled = get_config_int (buffer, "enabled", FALSE);


            cpu_patch_info [index].active = FALSE;
        }


        pop_config_state ();
    }
}


void patches_save (const char * rom_filename)
{
    int index;


    UINT8 buffer [256];


    get_patches_filename (buffer, rom_filename, sizeof (buffer));


    /* Delete old records. */

    remove (buffer);


    if (cpu_patch_count == 0)
    {
        return;
    }


    push_config_state ();


    set_config_file (buffer);


    set_config_hex ("header", "version", 0x0100);


    set_config_int ("header", "patch_count", cpu_patch_count);


    for (index = 0; index < cpu_patch_count; index ++)
    {
        memset (buffer, NIL, sizeof (buffer));


        sprintf (buffer, "patch%02d", index);


        if (cpu_patch_info [index].title)
        {
            set_config_string (buffer, "title", cpu_patch_info [index].title);
        }


        set_config_hex (buffer, "address", cpu_patch_info [index].address);


        set_config_hex (buffer, "value", cpu_patch_info [index].value);

        set_config_hex (buffer, "match_value", cpu_patch_info [index].match_value);


        set_config_int (buffer, "enabled", cpu_patch_info [index].enabled);
    }


    pop_config_state ();
}
#endif

wchar_t * get_sram_filename (wchar_t *buffer, const wchar_t * rom_filename, int buffer_size)
{
    memset (buffer, NIL, buffer_size*sizeof(wchar_t));

    wstrcpy (buffer, rom_filename);

    wchar_t *p = wstrrchr(buffer, L'.');
    if(p)
     wstrcpy(++p, L"sav");

    return (buffer);
}


void sram_load (const wchar_t *rom_filename)
{
    wchar_t buffer[256];

    int sram_file;

    get_sram_filename(buffer, rom_filename, sizeof(buffer));

    if (exists(buffer))
    {
        sram_file = w_fopen(buffer, 0xB, 0x1FF, 0);

        if (sram_file >= 0)
        {
            w_fread (sram_file, cpu_sram, sizeof (cpu_sram));
            w_fclose (sram_file);
        }
    }
}


void sram_save (const wchar_t *rom_filename)
{
    wchar_t buffer [256];

    int sram_file;

    get_sram_filename(buffer, rom_filename, sizeof(buffer));

    sram_file = w_fopen(buffer, 0xB, 0x1FF, 0);
    if (sram_file >= 0)
    {
        w_fwrite (sram_file, cpu_sram, sizeof (cpu_sram));
        w_fclose (sram_file);
    }
}


int cpu_init (void)
{
    memset (cpu_ram, NIL, sizeof (cpu_ram));
    memset (cpu_sram, NIL, sizeof (cpu_sram));

    /* Trainer support - copy the trainer into the memory map */
    if ((global_rom.control_byte_1 & ROM_CTRL_TRAINER))
    {
        memcpy (cpu_sram + 0x1000, global_rom.trainer, 512);
    }


    cpu_active_pc = &cpu_context.PC.word;


    if (global_rom.sram_flag)
    {
        sram_load (global_rom.filename);
    }


    cpu_context.TrapBadOps = TRUE;


    memset(dummy_read, 0, sizeof(dummy_read));
    memset(dummy_write, 0, sizeof(dummy_write));


    return (0);
}


UINT8 cpu_read_direct_safeguard(UINT16 address)
{
    return cpu_block_2k_read_address [address >> 11] [address] /*+ cpu_patch_table [address]*/;
}


void cpu_write_direct_safeguard(UINT16 address, UINT8 value)
{
    cpu_block_2k_write_address [address >> 11] [address] = value;
}


UINT8 ppu_read_2000_3FFF (UINT16 address)
{
    return ppu_read(address);
}

UINT8 ppu_read_4000_47FF (UINT16 address)
{
    if (address == 0x4014)
        return (ppu_read(address));
    else if (address <= 0x4015)
    {
        return /*(papu_read (address))*/0;
    }
    else if ((address == 0x4016) || (address == 0x4017))
    {
        return (input_read (address));
    }

    return (0);
}

void ppu_write_2000_3FFF (UINT16 address, UINT8 value)
{
    ppu_write(address, value);
}

void ppu_write_4000_47FF (UINT16 address, UINT8 value)
{
    if (address == 0x4014)
    {
        ppu_write (address, value);
    }
    else if (address <= 0x4015)
    {
        //papu_write (address, value);
    }
    else if ((address == 0x4016) || (address == 0x4017))
    {
        input_write (address, value);
    }
}

void cpu_memmap_init (void)
{
    int index;

#if 0
    /* Clear patch information. */

    cpu_patch_count = 0;


    memset (cpu_patch_info, NIL, sizeof (cpu_patch_info));


    /* Load patches from patch table. */

    if (rom_is_loaded)
    {
        patches_load (global_rom.filename);
    }


    memset (cpu_patch_table, 0, sizeof (cpu_patch_table));
#endif

    /* Start with a clean memory map */
    for (index = 0; index < (64 << 10); index += (2 << 10))
    {
        cpu_set_read_address_2k (index, dummy_read);
        cpu_set_write_address_2k (index, dummy_write);
    }

    /* Map in RAM */
    for (index = 0; index < (8 << 10); index += (2 << 10))
    {
        cpu_set_read_address_2k (index, cpu_ram);
        cpu_set_write_address_2k (index, cpu_ram);
    }

    /* Map in SRAM */
    enable_sram();

    /* Map in registers */
    cpu_set_read_handler_8k (0x2000, ppu_read_2000_3FFF);
    cpu_set_write_handler_8k (0x2000, ppu_write_2000_3FFF);

    cpu_set_read_handler_2k (0x4000, ppu_read_4000_47FF);
    cpu_set_write_handler_2k (0x4000, ppu_write_4000_47FF);


    if ((global_rom.control_byte_1 & ROM_CTRL_TRAINER))
    {
        /* compute CRC32 for trainer */
        global_rom.trainer_crc32 =
            crc32_calculate (global_rom.trainer, ROM_TRAINER_SIZE);
    }
    else
    {
        global_rom.trainer_crc32 = 0;
    }


    /* compute CRC32 for PRG ROM */
    global_rom.prg_rom_crc32 = crc32_calculate (global_rom.prg_rom,
        (global_rom.prg_rom_pages * 0x4000));

}


void cpu_exit (void)
{

#ifdef DEBUG

    FILE * dump_file;

    int address;


    dump_file = fopen ("cpudump.ram", "wb");


    if (dump_file)
    {
        for (address = 0; address < 65536; address ++)
        {
            fputc (cpu_read (address), dump_file);
        }


        fclose (dump_file);
    }

#endif


    if (global_rom.sram_flag)
        sram_save (global_rom.filename);

#if 0
    patches_save (global_rom.filename);
#endif
}


void cpu_free_prg_rom (const ROM *rom)
{
    if (rom -> prg_rom) free (rom -> prg_rom);
}


UINT8 * cpu_get_prg_rom_pages (ROM *rom)
{
    int num_pages = rom -> prg_rom_pages;
    int copycount, missing, count, next, pages_mirror_size;

    /* Compute a mask used to wrap invalid PRG ROM page numbers.
     *  As PRG ROM uses a 16k page size, this mask is based
     *  on a 16k page size.
     */
    if (((num_pages * 2 - 1) & (num_pages - 1)) == (num_pages - 1))
    /* compute mask for even power of two */
    {
        pages_mirror_size = num_pages;
    }
    else
    /* compute mask */
    {
        int i;

        /* compute the smallest even power of 2 greater than
           PRG ROM page count, and use that to compute the mask */
        for (i = 0; (num_pages >> i) > 0; i++);

        pages_mirror_size = (1 << i);
    }

    rom -> prg_rom_page_overflow_mask = pages_mirror_size - 1;


    /* identify-map all the present pages */
    for (copycount = 0; copycount < num_pages; copycount++)
    {
        rom -> prg_rom_page_lookup [copycount] = copycount;
    }


    /* mirror-map all the not-present pages */
    for (next = num_pages, missing = pages_mirror_size - num_pages,
        count = 1; missing; count <<= 1, missing >>= 1)
    {
        if (missing & 1)
        {
            for (copycount = count; copycount; copycount--, next++)
            {
                rom -> prg_rom_page_lookup[next] =
                    rom -> prg_rom_page_lookup[next - count];
            }
        }
    }


    /* 16k PRG ROM page size */
    rom -> prg_rom = (UINT8 *)malloc (num_pages * 0x4000);

    if (rom -> prg_rom != NIL)
    {
        /* initialize to a known value for areas not present in image */
        memset (rom -> prg_rom, 0xFF, (num_pages * 0x4000));
    }

    return rom -> prg_rom;
}


void enable_sram(void)
{
    cpu_set_read_address_8k (0x6000, cpu_sram);
    cpu_set_write_address_8k (0x6000, cpu_sram);
}


void disable_sram(void)
{
    cpu_set_read_address_8k (0x6000, dummy_read);
    cpu_set_write_address_8k (0x6000, dummy_write);
}


void cpu_reset (void)
{
    if (global_rom.sram_flag)
    {
        //sram_save (global_rom.filename);
    }


    FN2A03_Reset (&cpu_context);
}


void cpu_interrupt (int type)
{
    switch (type)
    {
        case CPU_INTERRUPT_IRQ_SINGLE_SHOT:

            FN2A03_Interrupt (&cpu_context, FN2A03_INT_IRQ_SINGLE_SHOT);

            break;

        case CPU_INTERRUPT_NMI:

            FN2A03_Interrupt (&cpu_context, FN2A03_INT_NMI);

            break;

        default:

            FN2A03_Interrupt (&cpu_context,
                FN2A03_INT_IRQ_SOURCE(type - CPU_INTERRUPT_IRQ_SOURCE(0)));

            break;
    }
}


void cpu_clear_interrupt (int type)
{
    switch (type)
    {
        case CPU_INTERRUPT_IRQ_SINGLE_SHOT:
        case CPU_INTERRUPT_NMI:

            break;

        default:

            FN2A03_Clear_Interrupt (&cpu_context,
                FN2A03_INT_IRQ_SOURCE(type - CPU_INTERRUPT_IRQ_SOURCE(0)));

            break;
    }
}


int scanline_start_cycle;

void cpu_start_new_scanline (void)
{
    scanline_start_cycle = cpu_context.ICount;
}


int cpu_get_cycles_line (void)
{
    return (cpu_context.Cycles - scanline_start_cycle) / CYCLE_LENGTH;
}


int cpu_get_cycles (int reset)
{
    int cycles = cpu_context.Cycles;


    if (reset)
    {
        cpu_context.ICount -= cpu_context.Cycles;
        cpu_context.Cycles = 0;
    }


    return (cycles / CYCLE_LENGTH);
}


void cpu_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Save CPU registers */
    pack_iputw (cpu_context.PC.word, file_chunk);

    pack_putc (cpu_context.A, file_chunk);
    pack_putc (cpu_context.X, file_chunk);
    pack_putc (cpu_context.Y, file_chunk);
    pack_putc (cpu_context.S, file_chunk);
    pack_putc (FN2A03_Pack_Flags(&cpu_context), file_chunk);

    /* Save cycle counters */
    pack_iputl (cpu_context.ICount, file_chunk);
    pack_iputl (cpu_context.Cycles, file_chunk);

    /* Save IRQ state */
    pack_iputl (cpu_context.IRequest, file_chunk);

    /* Save execution state */
    pack_putc (cpu_context.Jammed, file_chunk);


    /* Save NES internal RAM */
    pack_fwrite (cpu_ram, 0x800, file_chunk);

    /* Save cartridge expansion RAM */
    pack_fwrite (cpu_sram, 0x2000, file_chunk);


    pack_fclose_chunk (file_chunk);
}


void cpu_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;

    UINT8 P;


    file_chunk = pack_fopen_chunk (file, FALSE);


    /* Restore CPU registers */
    cpu_context.PC.word = pack_igetw (file_chunk);

    cpu_context.A = pack_getc (file_chunk);
    cpu_context.X = pack_getc (file_chunk);
    cpu_context.Y = pack_getc (file_chunk);
    cpu_context.S = pack_getc (file_chunk);
    P = pack_getc (file_chunk);
    FN2A03_Unpack_Flags(&cpu_context, P);

    /* Restore cycle counters */
    cpu_context.ICount = pack_igetl (file_chunk);
    cpu_context.Cycles = pack_igetl (file_chunk);

    /* Restore IRQ state */
    cpu_context.IRequest = pack_igetl (file_chunk);

    /* Restore execution state */
    cpu_context.Jammed = pack_getc (file_chunk);


    /* Restore NES internal RAM */
    pack_fread (cpu_ram, 0x800, file_chunk);

    /* Restore cartridge expansion RAM */
    pack_fread (cpu_sram, 0x2000, file_chunk);


    pack_fclose_chunk (file_chunk);
}
