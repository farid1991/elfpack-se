/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

rom.c: Implementation of the ROM file handling.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/

#include "ppu.h"
#include "rom.h"
#include "cpu.h"

#include "misc.h"


int rom_is_loaded = FALSE;
ROM global_rom;


int load_rom (const wchar_t * filename, ROM * rom)
{
    INES_HEADER ines_header;
    int rom_file;


    /* Initialize the ROM context. */

    memset (rom, NIL, sizeof (ROM));


    /* Check if ROM is inside a ZIP file. */

   /* if ((strncmp (get_extension (filename), "zip", 3) == 0) ||
        (strncmp (get_extension (filename), "ZIP", 3) == 0))
    {
        return (load_rom_from_zip (filename, rom));
    }*/


    /* Open the file. */

    rom_file = w_fopen(filename, 0xB, 0x1FF, 0);

    if (rom_file == ~0)
    {
        return (1);
    }


    /* Read the header. */

    w_fread(rom_file, &ines_header, sizeof (INES_HEADER));


    /* Verify the signature. */

    if (strncmp ((char *) ines_header.signature, "NES\x1a", 4))
    {
        w_fclose(rom_file);
        return (1);
    }


    /* Verify that PRG-ROM exists. */

    if (ines_header.prg_rom_pages == 0)
    {
        w_fclose(rom_file);
        return (1);
    }


    /* Check for 'DiskDude!'. */

    if ((ines_header.control_byte_2 == 'D') &&
        (! (strncmp ((char *) ines_header.reserved, "iskDude!", 8))))
    {
        ines_header.control_byte_2 = 0;
    }


    /* Read page/bank count. */

    rom -> prg_rom_pages = ines_header.prg_rom_pages;
    rom -> chr_rom_pages = ines_header.chr_rom_pages;


    /* Read control bytes. */

    rom -> control_byte_1 = ines_header.control_byte_1;
    rom -> control_byte_2 = ines_header.control_byte_2;


    /* Derive mapper number. */

    rom -> mapper_number = ((rom -> control_byte_2 & 0xf0) | ((rom -> control_byte_1 & 0xf0) >> 4));


    mmc_request (rom);


    /* Allocate and load trainer. */

    if ((rom -> control_byte_1 & ROM_CTRL_TRAINER))
    {
        rom -> trainer = (UINT8 *)malloc (ROM_TRAINER_SIZE);

        if (! rom -> trainer)
        {
          w_fclose(rom_file);
          return (1);
        }


        /* initialize area not present in image to a known value */
        memset (rom -> trainer, 0xFF, ROM_TRAINER_SIZE);

        /* read in trainer */
        w_fread (rom_file, rom -> trainer, ROM_TRAINER_SIZE);
    }


    /* Allocate and load PRG-ROM. */

    if (! cpu_get_prg_rom_pages (rom))
    {
        free_rom (rom);

        w_fclose(rom_file);
        return (1);
    }


    /* read in PRG ROM */
    w_fread (rom_file, rom -> prg_rom, (rom -> prg_rom_pages * 0x4000));


    /* Allocate and load CHR-ROM. */

    if (rom -> chr_rom_pages)
    {
        if (! ppu_get_chr_rom_pages (rom))
        {
          free_rom (rom);

          w_fclose(rom_file);
          return (1);
        }

        /* read in CHR ROM */
        w_fread (rom_file, rom -> chr_rom, (rom -> chr_rom_pages * 0x2000));
    }


    /* Fill in extra stuff. */

    wstrncpy(rom -> filename, filename, sizeof(rom -> filename)/sizeof(rom -> filename[0]));

    rom -> sram_flag = (rom -> control_byte_1 & ROM_CTRL_BATTERY);


    if ((rom -> control_byte_1 & ROM_CTRL_FOUR_SCREEN))
    {
        ppu_set_mirroring (MIRRORING_FOUR_SCREEN);
    }
    else
    {
        ppu_set_mirroring (((rom -> control_byte_1 & ROM_CTRL_MIRRORING)
            ? MIRRORING_VERTICAL : MIRRORING_HORIZONTAL));
    }


    /* Close the file. */

    w_fclose (rom_file);
    return (0);
}


void free_rom (const ROM * rom)
{
    if (rom -> trainer)
    {
        free (rom -> trainer);
    }

    cpu_free_prg_rom (rom);
    ppu_free_chr_rom (rom);
}
