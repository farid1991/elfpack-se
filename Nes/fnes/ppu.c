/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

ppu.c: Implementation of the PPU emulation.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/

#include "cpu.h"
#include "input.h"
#include "mmc.h"
#include "ppu.h"
#include "rom.h"
//#include "video.h"

#include "misc.h"

#include "timing.h"
#include "crc32.h"

#include "../gui.h"

/* delay for sprite 0 collision detection in PPU clocks */
/* should be <= SCANLINE_CLOCKS - 256 */
#define DOTS_HBLANK_BEFORE_RENDER 0

/* VRAM and sprite RAM. */

UINT8 * ppu_vram_block_read_address [8];
UINT8 * ppu_vram_block_background_cache_address [8];
UINT8 * ppu_vram_block_background_cache_tag_address [8];
UINT8 * ppu_vram_block_sprite_cache_address [8];
UINT8 * ppu_vram_block_sprite_cache_tag_address [8];
UINT8 * ppu_vram_block_write_address [8];

/*
 vram block identifiers
  0-7 = pattern VRAM
  8+  = pattern VROM
*/
#define FIRST_VROM_BLOCK 8
UINT32 ppu_vram_block [8];

INT32 ppu_vram_dirty_set_begin [8];
INT32 ppu_vram_dirty_set_end [8];
INT8 ppu_vram_cache_needs_update;


UINT8 ppu_vram_dummy_write [1024];

UINT8 ppu_pattern_vram [8 * 1024];
UINT8 ppu_pattern_vram_cache [8 * 1024 / 2 * 8];
UINT8 ppu_pattern_vram_cache_tag [8 * 1024 / 2];


UINT8 ppu_name_table_vram [4 * 1024];
UINT8 *name_tables_read [4];
UINT8 *name_tables_write [4];


UINT8 ppu_palette [32];


UINT8 ppu_spr_ram [256];

int ppu_enable_sprite_layer_a = TRUE;
int ppu_enable_sprite_layer_b = TRUE;
int ppu_enable_background_layer = TRUE;

#define ppu_background_palette ppu_palette
#define ppu_sprite_palette (ppu_palette + 16)

void do_spr_ram_dma(UINT8 page);

int ppu_mirroring;

int ppu_scanline;
int ppu_frame_last_line;

UINT8 ppu_register_2000;
UINT8 ppu_register_2001;

UINT8 * one_screen_base_address;

#define PPU_GET_LINE_ADDRESS(bitmap, y) \
    (&bitmap[y*256])

#define PPU_PUTPIXEL(bitmap, x, y, color) \
    (bitmap[y*256+x] = color)

#define PPU_GETPIXEL(bitmap, x, y) \
    (bitmap[y*256+x])


int vram_address = 0;
UINT8 buffered_vram_read = 0;

int address_write = 0;
int address_temp = 0;
int x_offset = 0;
int address_increment = 1;


UINT8 spr_ram_address = 0;
int sprite_height = 8;

int want_vblank_nmi = FALSE;

int vblank_occurred = FALSE;

UINT8 hit_first_sprite = 0;
int first_sprite_this_line = 0;

int background_tileset = 0;
int sprite_tileset = 0;

#ifdef ALLEGRO_I386
UINT32 attribute_table [4];
#else
UINT8 attribute_table [4];
#endif

INT8 background_pixels [8 + 256 + 8];

#include "ppu/tiles.h"

#include "ppu/backgrnd.h"
#include "ppu/sprites.h"


INLINE UINT8 vram_read (UINT16 address)
{
    if (address < 0x2000)
    {
        return ppu_vram_block_read_address [address >> 10] [address & 0x3FF];
    }
    else
    {
        return name_tables_read[(address >> 10) & 3]
            [address & 0x3FF];
    }
}


void ppu_free_chr_rom (const ROM *rom)
{
    if (rom -> chr_rom) free (rom -> chr_rom);
    if (rom -> chr_rom_cache) free (rom -> chr_rom_cache);
    if (rom -> chr_rom_cache_tag) free (rom -> chr_rom_cache_tag);
}


UINT8 * ppu_get_chr_rom_pages (ROM *rom)
{
    int num_pages = rom -> chr_rom_pages;
    int copycount, missing, count, next, pages_mirror_size;

    /* Compute a mask used to wrap invalid CHR ROM page numbers.
     *  As CHR ROM uses a 8k page size, this mask is based
     *  on a 8k page size.
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
           CHR ROM page count, and use that to compute the mask */
        for (i = 0; (num_pages >> i) > 0; i++);

        pages_mirror_size = (1 << i);
    }

    rom -> chr_rom_page_overflow_mask = pages_mirror_size - 1;


    /* identify-map all the present pages */
    for (copycount = 0; copycount < num_pages; copycount++)
    {
        rom -> chr_rom_page_lookup [copycount] = copycount;
    }


    /* mirror-map all the not-present pages */
    for (next = num_pages, missing = pages_mirror_size - num_pages,
        count = 1; missing; count <<= 1, missing >>= 1)
    {
        if (missing & 1)
        {
            for (copycount = count; copycount; copycount--, next++)
            {
                rom -> chr_rom_page_lookup[next] =
                    rom -> chr_rom_page_lookup[next - count];
            }
        }
    }


    /* 8k CHR ROM page size */
    rom -> chr_rom = (UINT8 *)malloc (num_pages * 0x2000);

    /* 2-bit planar tiles converted to 8-bit chunky */
    rom -> chr_rom_cache = (UINT8 *)malloc ((num_pages * 0x2000) / 2 * 8);
    rom -> chr_rom_cache_tag = (UINT8 *)malloc ((num_pages * 0x2000) / 2);

    if (rom -> chr_rom == 0 || rom -> chr_rom_cache == 0 ||
     rom -> chr_rom_cache_tag == 0)
    {
        if (rom -> chr_rom) free (rom -> chr_rom);
        if (rom -> chr_rom_cache) free (rom -> chr_rom_cache);
        if (rom -> chr_rom_cache_tag) free (rom -> chr_rom_cache_tag);

        rom -> chr_rom = NIL;
        rom -> chr_rom_cache = NIL;
        rom -> chr_rom_cache_tag = NIL;
    }
    else
    {
        /* initialize to a known value for areas not present in image */
        memset (rom -> chr_rom, 0xFF, (num_pages * 0x2000));
    }

    return rom -> chr_rom;
}


void ppu_set_ram_1k_pattern_vram_block (UINT16 block_address, int vram_block)
{
    ppu_vram_block [block_address >> 10] = vram_block;

    ppu_vram_block_read_address [block_address >> 10] =
        ppu_pattern_vram + (vram_block << 10);
    ppu_vram_block_write_address [block_address >> 10] =
        ppu_pattern_vram + (vram_block << 10);

    ppu_vram_block_background_cache_address [block_address >> 10] =
    ppu_vram_block_sprite_cache_address [block_address >> 10] =
        ppu_pattern_vram_cache + ((vram_block << 10) / 2 * 8);

    ppu_vram_block_background_cache_tag_address [block_address >> 10] =
    ppu_vram_block_sprite_cache_tag_address [block_address >> 10] =
        ppu_pattern_vram_cache_tag + ((vram_block << 10) / 2);
}


void ppu_set_ram_1k_pattern_vrom_block (UINT16 block_address, int vrom_block)
{
    vrom_block = (vrom_block & 7) + ROM_CHR_ROM_PAGE_LOOKUP
        [(vrom_block / 8) & ROM_CHR_ROM_PAGE_OVERFLOW_MASK] * 8;


    ppu_vram_block [block_address >> 10] =
        FIRST_VROM_BLOCK + vrom_block;

    ppu_vram_block_read_address [block_address >> 10] =
        ROM_CHR_ROM + (vrom_block << 10);
    ppu_vram_block_write_address [block_address >> 10] =
        ppu_vram_dummy_write;

    ppu_vram_block_background_cache_address [block_address >> 10] =
    ppu_vram_block_sprite_cache_address [block_address >> 10] =
        ROM_CHR_ROM_CACHE + ((vrom_block << 10) / 2 * 8);

    ppu_vram_block_background_cache_tag_address [block_address >> 10] =
    ppu_vram_block_sprite_cache_tag_address [block_address >> 10] =
        ROM_CHR_ROM_CACHE_TAG + ((vrom_block << 10) / 2);
}


void ppu_set_ram_1k_pattern_vrom_block_ex (UINT16 block_address,
 int vrom_block, int map_type)
{
    vrom_block = (vrom_block & 7) + ROM_CHR_ROM_PAGE_LOOKUP
        [(vrom_block / 8) & ROM_CHR_ROM_PAGE_OVERFLOW_MASK] * 8;

    if (map_type & PPU_MAP_RAM)
    {
        ppu_vram_block [block_address >> 10] =
            FIRST_VROM_BLOCK + vrom_block;

        ppu_vram_block_read_address [block_address >> 10] =
            ROM_CHR_ROM + (vrom_block << 10);
        ppu_vram_block_write_address [block_address >> 10] =
            ppu_vram_dummy_write;
    }

    if (map_type & PPU_MAP_BACKGROUND)
    {
        ppu_vram_block_background_cache_address [block_address >> 10] =
            ROM_CHR_ROM_CACHE + ((vrom_block << 10) / 2 * 8);

        ppu_vram_block_background_cache_tag_address [block_address >> 10] =
            ROM_CHR_ROM_CACHE_TAG + ((vrom_block << 10) / 2);
    }

    if (map_type & PPU_MAP_SPRITES)
    {
        ppu_vram_block_sprite_cache_address [block_address >> 10] =
            ROM_CHR_ROM_CACHE + ((vrom_block << 10) / 2 * 8);

        ppu_vram_block_sprite_cache_tag_address [block_address >> 10] =
            ROM_CHR_ROM_CACHE_TAG + ((vrom_block << 10) / 2);
    }
}


void ppu_set_ram_8k_pattern_vram (void)
{
    ppu_set_ram_1k_pattern_vram_block (0x0000, 0);
    ppu_set_ram_1k_pattern_vram_block (0x0400, 1);
    ppu_set_ram_1k_pattern_vram_block (0x0800, 2);
    ppu_set_ram_1k_pattern_vram_block (0x0C00, 3);
    ppu_set_ram_1k_pattern_vram_block (0x1000, 4);
    ppu_set_ram_1k_pattern_vram_block (0x1400, 5);
    ppu_set_ram_1k_pattern_vram_block (0x1800, 6);
    ppu_set_ram_1k_pattern_vram_block (0x1C00, 7);
}


int ppu_init (void)
{
    int i;

    /* compute CRC32 for CHR ROM */
    global_rom.chr_rom_crc32 = crc32_calculate (global_rom.chr_rom,
        (global_rom.chr_rom_pages * 0x2000));


    /* calculate the attribute lookup table */
    for (i = 0; i < 4; i++)
    {
#ifdef ALLEGRO_I386
        UINT32 attribute = (i << 2) | 3;

        attribute |= (attribute << 8) | (attribute << 16) |
            (attribute << 24);

        attribute_table [i] = attribute;
#else
        attribute_table [i] = (i << 2) | 3;
#endif
    }

    ppu_cache_init ();
    ppu_cache_chr_rom_pages ();

    ppu_reset ();


    return (0);
}


void ppu_exit (void)
{

}


int ppu_get_mirroring (void)
{
  return ppu_mirroring;
}

void ppu_set_name_table_internal (int table, int select)
{
    ppu_set_name_table_address (table, ppu_name_table_vram + (select << 10));
}


void ppu_set_name_table_address (int table, UINT8 *address)
{
    name_tables_read[table] = address;
    name_tables_write[table] = address;
}

void ppu_set_name_table_address_rom (int table, UINT8 *address)
{
    name_tables_read[table] = address;
    name_tables_write[table] = ppu_vram_dummy_write;
}

void ppu_set_name_table_address_vrom (int table, int vrom_block)
{
    vrom_block = (vrom_block & 7) + ROM_CHR_ROM_PAGE_LOOKUP
        [(vrom_block / 8) & ROM_CHR_ROM_PAGE_OVERFLOW_MASK] * 8;

    ppu_set_name_table_address_rom (table, ROM_CHR_ROM + (vrom_block << 10));
}

void ppu_set_mirroring_one_screen (void)
{
    ppu_set_name_table_address (0, one_screen_base_address);
    ppu_set_name_table_address (1, one_screen_base_address);
    ppu_set_name_table_address (2, one_screen_base_address);
    ppu_set_name_table_address (3, one_screen_base_address);
}

void ppu_set_mirroring (int mirroring)
{
    ppu_mirroring = mirroring;

    switch (ppu_mirroring)
    {
     case MIRRORING_ONE_SCREEN:
        ppu_set_mirroring_one_screen();

        break;

     case MIRRORING_ONE_SCREEN_2000:
        one_screen_base_address = ppu_name_table_vram;
        ppu_set_mirroring_one_screen();

        break;

     case MIRRORING_ONE_SCREEN_2400:
        one_screen_base_address = ppu_name_table_vram + 0x400;
        ppu_set_mirroring_one_screen();

        break;

     case MIRRORING_ONE_SCREEN_2800:
        one_screen_base_address = ppu_name_table_vram + 0x800;
        ppu_set_mirroring_one_screen();

        break;

     case MIRRORING_ONE_SCREEN_2C00:
        one_screen_base_address = ppu_name_table_vram + 0xC00;
        ppu_set_mirroring_one_screen();

        break;

     case MIRRORING_VERTICAL:
        ppu_set_name_table_internal (0, 0);
        ppu_set_name_table_internal (1, 1);
        ppu_set_name_table_internal (2, 0);
        ppu_set_name_table_internal (3, 1);

        break;

     case MIRRORING_HORIZONTAL:
        ppu_set_name_table_internal (0, 0);
        ppu_set_name_table_internal (1, 0);
        ppu_set_name_table_internal (2, 1);
        ppu_set_name_table_internal (3, 1);

        break;

     case MIRRORING_FOUR_SCREEN:
        ppu_set_name_table_internal (0, 0);
        ppu_set_name_table_internal (1, 1);
        ppu_set_name_table_internal (2, 2);
        ppu_set_name_table_internal (3, 3);

        break;
    }

}


void ppu_invert_mirroring (void)   /* '/' key. */
{
    switch (ppu_mirroring)
    {
        case MIRRORING_HORIZONTAL:
            ppu_set_mirroring (MIRRORING_VERTICAL);
            break;

        case MIRRORING_VERTICAL:
            ppu_set_mirroring (MIRRORING_HORIZONTAL);
            break;
    }
}

void ppu_reset (void)
{
    //int i;

    memset (ppu_pattern_vram, NIL, sizeof (ppu_pattern_vram));
    memset (ppu_name_table_vram, NIL, sizeof (ppu_name_table_vram));
    memset (ppu_spr_ram, NIL, sizeof (ppu_spr_ram));


    ppu_cache_all_vram ();


    ppu_write (0x2000, 0);

    ppu_write (0x2001,
     PPU_BACKGROUND_SHOW_LEFT_EDGE_BIT |
     PPU_SPRITES_SHOW_LEFT_EDGE_BIT |
     PPU_BACKGROUND_ENABLE_BIT | PPU_SPRITES_ENABLE_BIT);


    spr_ram_address = 0;
    sprite_list_needs_recache = TRUE;


    vram_address = 0;
    address_temp = 0;

    x_offset = 0;

    address_write = FALSE;

    buffered_vram_read = 0;


    ppu_set_mirroring(ppu_mirroring);

    ppu_clear ();
}


UINT8 ppu_vram_read()
{
    UINT16 address = vram_address & 0x3FFF;
    UINT8 temp = buffered_vram_read;

    /* VRAM Read I/O. */

    if (address >= 0x2000)
    {
        /* name tables */
        {
            buffered_vram_read = name_tables_read[(address >> 10) & 3]
             [address & 0x3FF];
        }
    }
    else
    /* pattern tables */
    {
        /* If the mapper's behavior is affected by PPU bus activity, *
         *  it will need to install a handler in the function        *
         *  pointer mmc_check_latches.                               */

        /* This code is currently configured to report accesses to   *
         *  0FD0-0FEF and 1FD0-1FEF only, for MMC2 and MMC4.         */

        /* If a mapper needs to watch other PPU address ranges,      *
         *  this code will need to be changed.                       */
        if (mmc_check_latches)
        {
            if ((address & 0xFFF) >= 0xFD0 && (address & 0xFFF) <= 0xFEF)
            {
                mmc_check_latches(address);
            }
        }

        buffered_vram_read =
            ppu_vram_block_read_address [address >> 10] [address & 0x3FF];
    }

    vram_address += address_increment;


    return (temp);

}


void ppu_vram_write(UINT8 value)
{
    UINT16 address = vram_address & 0x3FFF;

    /* VRAM Write I/O. */

    if (address >= 0x2000)
    {
        if (address >= 0x3F00)
        /* palettes */
        {
            if ((address & 0x03) == 0)
            {
                ppu_background_palette [(address & 0x0F)] = (value & 0x3F) + 1;
                ppu_sprite_palette [(address & 0x0F)] = (value & 0x3F) + 1;
            }
            else
            {
                ppu_palette [address & 0x1F] = (value & 0x3F) + 1;
            }
        }
        else
        /* name tables */
        {
            name_tables_write[(address >> 10) & 3]
             [address & 0x3FF] = value;
        }
    }
    else
    /* pattern tables */
    {
        int vram_block;

        if (mmc_check_latches)
        {
            if ((address & 0xFFF) >= 0xFD0 && (address & 0xFFF) <= 0xFEF)
            {
                mmc_check_latches(address);
            }
        }

        vram_block = ppu_vram_block [address >> 10];

        if (vram_block < FIRST_VROM_BLOCK)
        /* if block is writable (VRAM) */
        {
            int this_tile;

            if (ppu_vram_block_write_address [address >> 10]
                [address & 0x3FF] != value)
            /* VRAM changed, track for cache update */
            {
                ppu_vram_block_write_address [address >> 10]
                    [address & 0x3FF] = value;

                this_tile = (address & 0x3FF) / 16;

                if (ppu_vram_dirty_set_end [vram_block] != this_tile)
                {
                    if (ppu_vram_dirty_set_end [vram_block] == this_tile - 1)
                    {
                        ppu_vram_dirty_set_end [vram_block] ++;
                    }
                    else
                    {
                        if (vram_set_needs_recache(vram_block))
                        {
                            recache_vram_set (vram_block);
                        }

                        ppu_vram_dirty_set_begin [vram_block] =
                            ppu_vram_dirty_set_end [vram_block] = this_tile;

                        ppu_vram_cache_needs_update = TRUE;
                    }
                }
            }
        }
    }

    vram_address += address_increment;

}


static UINT8 last_ppu_write_value;

UINT8 ppu_read (UINT16 address)
{
    int data = 0;


    if (address == 0x4014)
    {
        return (0);
    }


    /* Handle register mirroring. */

    switch (address & 7)
    {
        case 0x2000 & 7:
        case 0x2001 & 7:
        case 0x2003 & 7:
        case 0x2005 & 7:
        case 0x2006 & 7:
            return last_ppu_write_value;

        case 0x2002 & 7:

            /* PPU status register. */

            if (vblank_occurred)
            {
                data |= PPU_VBLANK_FLAG_BIT;
                vblank_occurred = FALSE;
            }

            if (PPU_SPRITES_ENABLED && (ppu_scanline <= LAST_DISPLAYED_LINE))
            {
                if (sprite_list_needs_recache)
                {
                    recache_sprite_list ();
                }

                data |= sprite_overflow_on_line [ppu_scanline];
            }

            if (!hit_first_sprite && first_sprite_this_line)
            {
                if (cpu_get_cycles_line() > ((first_sprite_this_line - 1) / 3))
                {
                    hit_first_sprite = PPU_SPRITE_0_COLLISION_BIT;
                }
            }

            data |= hit_first_sprite;

            address_write = FALSE;

            return (data | (last_ppu_write_value & 0x1F));

        case 0x2004 & 7:

            /* Sprite RAM I/O. */

            return ppu_spr_ram [spr_ram_address++];

        case 0x2007 & 7:

            /* VRAM Read I/O. */

            return ppu_vram_read();

        default:

            break;
    }

    return (0);
}


void ppu_write (UINT16 address, UINT8 value)
{

    if (address == 0x4014)
    {
        /* Sprite RAM DMA. */

        do_spr_ram_dma (value);

        return;
    }


    /* Handle register mirroring. */

    last_ppu_write_value = value;

    switch (address & 7)
    {
        case 0x2000 & 7:

            /* Control register #1. */

            ppu_register_2000 = value;

            {
                int new_sprite_height =
                 ((value & PPU_SPRITE_SIZE_BIT) ? 16 : 8);

                if (sprite_height != new_sprite_height)
                {
                    sprite_height = new_sprite_height;
                    sprite_list_needs_recache = TRUE;
                }
            }


            want_vblank_nmi = (value & PPU_VBLANK_NMI_FLAG_BIT);

            address_increment =
                ((value & PPU_ADDRESS_INCREMENT_BIT) ? 32 : 1);


            background_tileset =
                ((value & PPU_BACKGROUND_TILESET_BIT) ? 0x1000 : 0x0000);

            sprite_tileset =
                ((value & PPU_SPRITE_TILESET_BIT) ? 0x1000 : 0x0000);


            address_temp = (address_temp & ~(3 << 10)) | ((value & 3) << 10);

            break;

        case 0x2001 & 7:

            /* Control register #2. */

            ppu_register_2001 = value;

            break;

        case 0x2003 & 7:

            /* Sprite RAM address. */

            spr_ram_address = value;

            break;

        case 0x2004 & 7:

            /* Sprite RAM I/O. */

            if (ppu_spr_ram [spr_ram_address] != value)
            {
                ppu_spr_ram [spr_ram_address] = value;
                sprite_list_needs_recache = TRUE;
            }
            spr_ram_address++;

            break;

        case 0x2005 & 7:

            /* Horizontal / Vertical offset. */

            if (! address_write)
            {
                /* Horizontal offset. */

                address_write = TRUE;

                address_temp = (address_temp & ~0x1F) | ((value >> 3) & 0x1F);
                x_offset = value & 7;
            }
            else
            {
                /* Vertical offset. */

                address_write = FALSE;

                address_temp = (address_temp & ~(0x1F << 5)) | (((value >> 3) & 0x1F) << 5);
                address_temp = (address_temp & ~(7 << 12)) | ((value & 7) << 12);
            }

            break;

        case 0x2006 & 7:

            /* VRAM address. */

            if (! address_write)
            {
                address_write = TRUE;
                address_temp = (address_temp & 0xFF) | ((value & 0x3F) << 8);
            }
            else
            {
                address_write = FALSE;
                address_temp = (address_temp & ~0xFF) | value;
                vram_address = address_temp;
            }

            break;

        case 0x2007 & 7:

            /* VRAM Write I/O. */

            ppu_vram_write(value);

            break;

        default:
            break;
    }
}


static void do_spr_ram_dma(UINT8 page)
{
    /* Sprite RAM DMA. */

    int index;
    unsigned address = page * 0x100;

    cpu_consume_cycles(2);

    for (index = 0; index < 256; index ++)
    {
        int value = cpu_read (address + index);

        cpu_consume_cycles(2);

        if (ppu_spr_ram [spr_ram_address] != value)
        {
            ppu_spr_ram [spr_ram_address] = value;
            sprite_list_needs_recache = TRUE;
        }

        spr_ram_address++;
    }
}


static void vram_address_start_new_frame(void)
{
    if (PPU_BACKGROUND_ENABLED || PPU_SPRITES_ENABLED)
    {
        vram_address = address_temp;
    }
}


void ppu_start_line(void)
{
    if (PPU_BACKGROUND_ENABLED || PPU_SPRITES_ENABLED)
    {
        vram_address = (vram_address & (~0x1F & ~(1 << 10)))
         | (address_temp & (0x1F | (1 << 10)));
    }

    if (first_sprite_this_line)
    {
        hit_first_sprite = PPU_SPRITE_0_COLLISION_BIT;
        first_sprite_this_line = 0;
    }
}

void ppu_end_line(void)
{
    if (PPU_BACKGROUND_ENABLED || PPU_SPRITES_ENABLED)
    {
        vram_address += 0x1000;

        if ((vram_address & (7 << 12)) == 0)
        {
            vram_address += 32;

            switch (vram_address & (0x1F << 5))
            {
             case 0:

                vram_address -= (1 << 10);

                break;

             case (30 << 5):

                vram_address = (vram_address - (30 << 5)) ^ (1 << 11);

                break;
            }
        }
    }
}


void ppu_clear (void)
{
    vblank_occurred = FALSE;

    hit_first_sprite = 0;
    first_sprite_this_line = 0;
}


void ppu_start_frame (void)
{
    vram_address_start_new_frame();

    /*if (input_enable_zapper)
    {
        input_update_zapper_offsets ();
    }*/
}


void ppu_start_render (void)
{
    ppu_start_frame();
}



void ppu_render_line (int line)
{
    //int i;

    if (!PPU_BACKGROUND_ENABLED)
    {
        memset (PPU_GET_LINE_ADDRESS (video_buffer, line),
            ppu_background_palette [0], 256);
    }

    if (!PPU_BACKGROUND_ENABLED && !PPU_SPRITES_ENABLED)
    {
        return;
    }

    if (!PPU_BACKGROUND_ENABLED && PPU_SPRITES_ENABLED)
    {
        /* used for sprite pixel allocation and collision detection */
        memset (background_pixels + 8, 0, 256);
    }

    if (ppu_vram_cache_needs_update)
    {
        recache_vram_sets ();
    }

    if (PPU_BACKGROUND_ENABLED)
    {
        ppu_render_background (line);
    }

    if (PPU_SPRITES_ENABLED)
    {
        ppu_render_sprites (line);
    }

    /* handle zapper emulation */
    /*if (input_enable_zapper && (input_zapper_y_offset == line) &&
        input_zapper_on_screen)
    {
        input_update_zapper ();
    }*/
}

void ppu_stub_render_line (int line)
{
    int first_y, last_y;

    /* draw lines for zapper emulation */

    /*if (input_enable_zapper && (input_zapper_y_offset == line) &&
        input_zapper_on_screen)
    {
        ppu_render_line (line);
        return;
    }*/


    /* draw lines for sprite 0 collision emulation */

    /* if sprites or background are disabled, */
    /* sprite 0 can't collide with background */
    if (!PPU_BACKGROUND_ENABLED || !PPU_SPRITES_ENABLED) return;

    /* if sprite 0 already collided, nothing to detect */
    if (hit_first_sprite) return;

    first_y = ppu_spr_ram [0] + 1;
    last_y = first_y + sprite_height - 1;

    /* if sprite 0 not on this line, nothing to detect */
    if (line < first_y || line > last_y) return;

    ppu_render_line (line);
}


void ppu_vblank (void)
{
    vblank_occurred = TRUE;
}


void ppu_end_render (void)
{
  video2screen();
  DispObject_InvalidateRect(GUIObject_GetDispObject(MyGui), 0);

  ppu_vblank ();
}


void ppu_vblank_nmi (void)
{
    if (want_vblank_nmi)
    {
        cpu_interrupt (CPU_INTERRUPT_NMI);
    }
}


void ppu_save_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    file_chunk = pack_fopen_chunk (file, FALSE);


    pack_putc(ppu_register_2000, file_chunk);
    pack_putc(ppu_register_2001, file_chunk);


    pack_putc(ppu_mirroring, file_chunk);


    pack_putc(spr_ram_address, file_chunk);


    pack_iputw(vram_address, file_chunk);
    pack_putc(buffered_vram_read, file_chunk);

    pack_putc(address_write, file_chunk);
    pack_iputw(address_temp, file_chunk);
    pack_putc(x_offset, file_chunk);

    pack_putc(vblank_occurred, file_chunk);

    pack_putc(hit_first_sprite, file_chunk);
    pack_iputl(first_sprite_this_line, file_chunk);


    pack_putc(mmc_get_name_table_count(), file_chunk);
    pack_putc(mmc_uses_pattern_vram(), file_chunk);


    /* save palette RAM */
    pack_fwrite(ppu_palette, 32, file_chunk);


    /* save sprite RAM */
    pack_fwrite(ppu_spr_ram, 256, file_chunk);

    /* mmc_get_name_table_count() MUST be <= 4 */
    /* values of 2 and 4 are expected */
    if (mmc_get_name_table_count())
    {
     pack_fwrite(ppu_name_table_vram, 1024 * mmc_get_name_table_count(),
      file_chunk);
    }

    if (mmc_uses_pattern_vram())
    {
     pack_fwrite(ppu_pattern_vram, 8192, file_chunk);
    }


    pack_fclose_chunk (file_chunk);
}


void ppu_load_state (PACKFILE file, int version)
{
    PACKFILE file_chunk;


    int state_name_table_count, state_contains_pattern_vram;


    file_chunk = pack_fopen_chunk (file, FALSE);


    ppu_register_2000 = pack_getc(file_chunk);
    ppu_write (0x2000, ppu_register_2000);

    ppu_register_2001 = pack_getc(file_chunk);
    ppu_write (0x2001, ppu_register_2001);


    ppu_mirroring = pack_getc(file_chunk);
    ppu_set_mirroring(ppu_mirroring);


    spr_ram_address = pack_getc(file_chunk);
    sprite_list_needs_recache = TRUE;


    vram_address = pack_igetw(file_chunk);
    buffered_vram_read = pack_getc(file_chunk);

    address_write = pack_getc(file_chunk);
    address_temp = pack_igetw(file_chunk);
    x_offset = pack_getc(file_chunk);

    vblank_occurred = pack_getc(file_chunk);

    hit_first_sprite = pack_getc(file_chunk);
    first_sprite_this_line = pack_igetl(file_chunk);


    state_name_table_count = pack_getc(file_chunk);
    state_contains_pattern_vram = pack_getc(file_chunk);


    /* load palette RAM */
    pack_fread(ppu_palette, 32, file_chunk);


    /* load sprite RAM */
    pack_fread(ppu_spr_ram, 256, file_chunk);

    /* state_name_table_count MUST be <= 4 */
    /* values of 2 and 4 are expected */
    if (state_name_table_count)
    {
     pack_fread(ppu_name_table_vram, 1024 * state_name_table_count,
      file_chunk);
    }

    if (state_contains_pattern_vram)
    {
     pack_fread(ppu_pattern_vram, 8192, file_chunk);
     ppu_cache_all_vram ();
    }


    pack_fclose_chunk (file_chunk);
}
