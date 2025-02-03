

/* Egads... 380+ lines of crap removed from cpu.h. :) */

INLINE UINT8 FN2A03_Read (UINT16);


INLINE void cpu_set_read_address_2k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((2 << 10) - 1);

    cpu_block_2k_read_address [block_start >> 11] = address - block_start;
    cpu_block_2k_read_handler [block_start >> 11] =
        cpu_read_direct_safeguard;


    /* Check if we are using patches. */
#if 0
    if (cpu_patch_count > 0)
    {
        int index;


        for (index = 0; index < cpu_patch_count; index ++)
        {
            if (cpu_patch_info [index].enabled &&
               (cpu_patch_info [index].address < (block_start + 0x800)) && (cpu_patch_info [index].address >= block_start))
            {
                UINT8 value;


                if (cpu_patch_info [index].active)
                {
                    /* Disable patch. */

                    cpu_patch_info [index].active = FALSE;


                    cpu_patch_table [cpu_patch_info [index].address] = 0;
                }


                value = FN2A03_Read (cpu_patch_info [index].address);


                if (value == cpu_patch_info [index].match_value)
                {
                    /* Enable patch. */

                    cpu_patch_info [index].active = TRUE;


                    cpu_patch_table [cpu_patch_info [index].address] = (cpu_patch_info [index].value - value);
                }
            }
        }
    }
#endif
}


INLINE void cpu_set_read_address_4k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((4 << 10) - 1);

    cpu_set_read_address_2k (block_start, address);
    cpu_set_read_address_2k (block_start + (2 << 10), address + (2 << 10));
}


INLINE void cpu_set_read_address_8k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((8 << 10) - 1);

    cpu_set_read_address_4k (block_start, address);
    cpu_set_read_address_4k (block_start + (4 << 10), address + (4 << 10));
}


INLINE void cpu_set_read_address_16k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((16 << 10) - 1);

    cpu_set_read_address_8k (block_start, address);
    cpu_set_read_address_8k (block_start + (8 << 10), address + (8 << 10));
}


INLINE void cpu_set_read_address_32k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((32 << 10) - 1);

    cpu_set_read_address_16k (block_start * 2, address);
    cpu_set_read_address_16k (block_start + (16 << 10), address + (16 << 10));
}


INLINE void cpu_set_write_address_2k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((2 << 10) - 1);

    cpu_block_2k_write_address [block_start >> 11] = address - block_start;
    cpu_block_2k_write_handler [block_start >> 11] =
        cpu_write_direct_safeguard;
}


INLINE void cpu_set_write_address_4k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((4 << 10) - 1);

    cpu_set_write_address_2k (block_start, address);
    cpu_set_write_address_2k (block_start + (2 << 10), address + (2 << 10));
}


INLINE void cpu_set_write_address_8k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((8 << 10) - 1);

    cpu_set_write_address_4k (block_start, address);
    cpu_set_write_address_4k (block_start + (4 << 10), address + (4 << 10));
}


INLINE void cpu_set_write_address_16k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((16 << 10) - 1);

    cpu_set_write_address_8k (block_start, address);
    cpu_set_write_address_8k (block_start + (8 << 10), address + (8 << 10));
}


INLINE void cpu_set_write_address_32k (UINT16 block_start, UINT8 *address)
{
    /* ignore low bits */
    block_start &= ~((32 << 10) - 1);

    cpu_set_write_address_16k (block_start * 2, address);
    cpu_set_write_address_16k (block_start + (16 << 10), address + (16 << 10));
}


INLINE void cpu_set_read_handler_2k (UINT16 block_start, UINT8 (*handler) (UINT16 address))
{
    /* ignore low bits */
    block_start &= ~((2 << 10) - 1);

    cpu_block_2k_read_address [block_start >> 11] = 0;
    cpu_block_2k_read_handler [block_start >> 11] = handler;
}


INLINE void cpu_set_read_handler_4k (UINT16 block_start, UINT8 (*handler) (UINT16 address))
{
    /* ignore low bits */
    block_start &= ~((4 << 10) - 1);

    cpu_set_read_handler_2k (block_start, handler);
    cpu_set_read_handler_2k (block_start + (2 << 10), handler);
}


INLINE void cpu_set_read_handler_8k (UINT16 block_start, UINT8 (*handler) (UINT16 address))
{
    /* ignore low bits */
    block_start &= ~((8 << 10) - 1);

    cpu_set_read_handler_4k (block_start, handler);
    cpu_set_read_handler_4k (block_start + (4 << 10), handler);
}


INLINE void cpu_set_read_handler_16k (UINT16 block_start, UINT8 (*handler) (UINT16 address))
{
    /* ignore low bits */
    block_start &= ~((16 << 10) - 1);

    cpu_set_read_handler_8k (block_start, handler);
    cpu_set_read_handler_8k (block_start + (8 << 10), handler);
}


INLINE void cpu_set_read_handler_32k (UINT16 block_start, UINT8 (*handler) (UINT16 address))
{
    /* ignore low bits */
    block_start &= ~((32 << 10) - 1);

    cpu_set_read_handler_16k (block_start, handler);
    cpu_set_read_handler_16k (block_start + (16 << 10), handler);
}


INLINE void cpu_set_write_handler_2k (UINT16 block_start, void (*handler) (UINT16 address, UINT8 value))
{
    /* ignore low bits */
    block_start &= ~((2 << 10) - 1);

    cpu_block_2k_write_address [block_start >> 11] = 0;
    cpu_block_2k_write_handler [block_start >> 11] = handler;
}


INLINE void cpu_set_write_handler_4k (UINT16 block_start, void (*handler) (UINT16 address, UINT8 value))
{
    /* ignore low bits */
    block_start &= ~((4 << 10) - 1);

    cpu_set_write_handler_2k (block_start, handler);
    cpu_set_write_handler_2k (block_start + (2 << 10), handler);
}


INLINE void cpu_set_write_handler_8k (UINT16 block_start, void (*handler) (UINT16 address, UINT8 value))
{
    /* ignore low bits */
    block_start &= ~((8 << 10) - 1);

    cpu_set_write_handler_4k (block_start, handler);
    cpu_set_write_handler_4k (block_start + (4 << 10), handler);
}


INLINE void cpu_set_write_handler_16k (UINT16 block_start, void (*handler) (UINT16 address, UINT8 value))
{
    /* ignore low bits */
    block_start &= ~((16 << 10) - 1);

    cpu_set_write_handler_8k (block_start, handler);
    cpu_set_write_handler_8k (block_start + (8 << 10), handler);
}


INLINE void cpu_set_write_handler_32k (UINT16 block_start, void (*handler) (UINT16 address, UINT8 value))
{
    /* ignore low bits */
    block_start &= ~((32 << 10) - 1);

    cpu_set_write_handler_16k (block_start, handler);
    cpu_set_write_handler_16k (block_start + (16 << 10), handler);
}


/* ----- ROM-specific banking ----- */


INLINE void cpu_set_read_address_8k_rom_block (UINT16 block_start, int rom_block)
{
    rom_block = (rom_block & 1) + ROM_PRG_ROM_PAGE_LOOKUP
        [(rom_block / 2) & ROM_PRG_ROM_PAGE_OVERFLOW_MASK] * 2;

    cpu_set_read_address_8k (block_start, ROM_PRG_ROM + (rom_block << 13));
}


INLINE void cpu_set_read_address_16k_rom_block (UINT16 block_start, int rom_block)
{
    rom_block = ROM_PRG_ROM_PAGE_LOOKUP
        [rom_block & ROM_PRG_ROM_PAGE_OVERFLOW_MASK];

    cpu_set_read_address_16k (block_start, ROM_PRG_ROM + (rom_block << 14));
}


INLINE void cpu_set_read_address_32k_rom_block (UINT16 block_start, int rom_block)
{
    rom_block <<= 1;

    cpu_set_read_address_16k_rom_block (block_start, rom_block);
    cpu_set_read_address_16k_rom_block (block_start + 0x4000, rom_block + 1);
}


/* ----- FN2A03 Routines ----- */


INLINE UINT8 FN2A03_Fetch (UINT16 Addr)
{
    // printf ("FN2A03_Fetch at $%04x.\n", Addr);

    if (cpu_block_2k_read_address [Addr >> 11] != 0)
    {
        return cpu_block_2k_read_address [Addr >> 11] [Addr] /*+ cpu_patch_table [Addr]*/;
    }
    else
    {
        return cpu_block_2k_read_handler [Addr >> 11] (Addr);
    }
}


INLINE UINT8 FN2A03_Read (UINT16 Addr)
{
    // printf ("FN2A03_Read at $%04x.\n", Addr);

    if (cpu_block_2k_read_address [Addr >> 11] != 0)
    {
        return cpu_block_2k_read_address [Addr >> 11] [Addr]/* + cpu_patch_table [Addr]*/;
    }
    else
    {
        return cpu_block_2k_read_handler [Addr >> 11] (Addr);
    }
}


INLINE void FN2A03_Write (UINT16 Addr, UINT8 Value)
{
    // printf ("FN2A03_Write at $%04x ($%02x).\n", Addr, Value);

    if (cpu_block_2k_write_address [Addr >> 11] != 0)
    {
        cpu_block_2k_write_address [Addr >> 11] [Addr] = Value;
    }
    else
    {
        cpu_block_2k_write_handler [Addr >> 11] (Addr, Value);
    }
}


#ifndef INLINE_WITH_MACROS


INLINE UINT8 FN2A03_Read_Stack (UINT8 S)
{
    return (cpu_ram [0x100 + S]);
}


INLINE void FN2A03_Write_Stack (UINT8 S, UINT8 Value)
{
    cpu_ram [0x100 + S] = Value;
}


INLINE UINT8 FN2A03_Read_ZP (UINT8 S)
{
    return (cpu_ram [S]);
}


INLINE void FN2A03_Write_ZP (UINT8 S, UINT8 Value)
{
    cpu_ram [S] = Value;
}


#else   /* ! INLINE_WITH_MACROS */


#define FN2A03_Read_Stack(S)        (cpu_ram [0x100 + (UINT8) (S)])
#define FN2A03_Write_Stack(S,Value) (cpu_ram [0x100 + (UINT8) (S)] = Value)


#define FN2A03_Read_ZP(S)           (cpu_ram [(UINT8) (S)])
#define FN2A03_Write_ZP(S,Value)    (cpu_ram [(UINT8) (S)] = Value)


#endif


INLINE UINT8 cpu_read (UINT16 address)
{
    return (FN2A03_Read (address));
}


INLINE void cpu_write (UINT16 address, UINT8 value)
{
    FN2A03_Write (address, value);
}


INLINE void cpu_execute (int cycles)
{
    cpu_context.ICount += cycles;


    FN2A03_Run (&cpu_context);
}


INLINE void cpu_consume_cycles (int cycles)
{
    FN2A03_consume_cycles (&cpu_context, cycles);
}
