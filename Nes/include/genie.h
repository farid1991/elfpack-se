

/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

genie.h: Game Genie decoder macros and functions.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#ifndef GENIE_H_INCLUDED

#define GENIE_H_INCLUDED


#include <ctype.h>

#include <string.h>


#include "cpu.h"


#include "misc.h"


/*
  Descrambles game-genie codes.  Note that codes are passed in and out
 both as 32-bit integers - the needed fields are simply coalesced by this
 function.
 */
#define GENIE_NYBBLE_HIGH_BITS \
 ((1 << 31) | (1 << 27) | (1 << 23) | (1 << 19) | (1 << 15) | (1 << 11) | \
 (1 << 7) | (1 << 3))

#define GENIE_EVEN_NYBBLES \
 ((15 << 24) | (15 << 16) | (15 << 8) | (15))
#define GENIE_ODD_NYBBLES (GENIE_EVEN_NYBBLES << 4)

#define GENIE_SWAP_ADDRESS_NYBBLE_1 (15 << 12)
#define GENIE_SWAP_ADDRESS_NYBBLE_2 (15 << 16)
#define GENIE_NOT_ADDRESS_SWAP (~(GENIE_SWAP_ADDRESS_NYBBLE_1 | \
    GENIE_SWAP_ADDRESS_NYBBLE_2))

#define GENIE_CODE_SIZE_BIT 23
#define GENIE_SMALL_CODE_VALUE_BIT_3 3
#define GENIE_LARGE_CODE_VALUE_BIT_3 27


static UINT32 genie_descramble (UINT32 scrambled)
{
    UINT32 value, compare, address;

    /* Rotate nybble MSBs right one */
    scrambled =
        (scrambled & ~GENIE_NYBBLE_HIGH_BITS) |
        ((scrambled & GENIE_NYBBLE_HIGH_BITS) >> 4) |
        ((scrambled & (1 << 3)) << 28);

    /* Swap even and odd nybbles */
    scrambled =
        ((scrambled & GENIE_EVEN_NYBBLES) << 4) |
        ((scrambled & GENIE_ODD_NYBBLES) >> 4);

    /* Swap misplaced address nybbles */
    scrambled = (scrambled & GENIE_NOT_ADDRESS_SWAP) |
        ((scrambled & GENIE_SWAP_ADDRESS_NYBBLE_1) << 4) |
        ((scrambled & GENIE_SWAP_ADDRESS_NYBBLE_2) >> 4);

    if (! (scrambled & (1 << GENIE_CODE_SIZE_BIT)))
    {
        scrambled = (scrambled & ~(1 << GENIE_LARGE_CODE_VALUE_BIT_3) &
         ~(1 << GENIE_SMALL_CODE_VALUE_BIT_3)) |
         ((scrambled & (1 << GENIE_SMALL_CODE_VALUE_BIT_3)) <<
         (GENIE_LARGE_CODE_VALUE_BIT_3 - GENIE_SMALL_CODE_VALUE_BIT_3));
    }


    return (scrambled);
}


#define GENIE_MAP_DIGIT(char_check, value)                  \
    case char_check:                                        \
                                                            \
        return (value);                                     \


static int genie_decode_digit (UINT8 digit)
{
    digit = toupper (digit);


    switch (digit)
    {
        /* First set. */

        GENIE_MAP_DIGIT ('A', 0x0);

        GENIE_MAP_DIGIT ('P', 0x1);

        GENIE_MAP_DIGIT ('Z', 0x2);

        GENIE_MAP_DIGIT ('L', 0x3);

        GENIE_MAP_DIGIT ('G', 0x4);

        GENIE_MAP_DIGIT ('I', 0x5);

        GENIE_MAP_DIGIT ('T', 0x6);

        GENIE_MAP_DIGIT ('Y', 0x7);


        /* Second set. */

        GENIE_MAP_DIGIT ('E', 0x8);

        GENIE_MAP_DIGIT ('O', 0x9);

        GENIE_MAP_DIGIT ('X', 0xA);

        GENIE_MAP_DIGIT ('U', 0xB);

        GENIE_MAP_DIGIT ('K', 0xC);

        GENIE_MAP_DIGIT ('S', 0xD);

        GENIE_MAP_DIGIT ('V', 0xE);

        GENIE_MAP_DIGIT ('N', 0xF);


        default:

            break;
    }


    return (0);
}


static int genie_decode (const UINT8 * code, UINT16 * address, UINT8 * value, UINT8 * match_value)
{
    int index;


    int length;


    UINT32 decoded = 0;


    int shifts;


    length = strlen (code);

    if ((length != 6) && (length != 8))
    {
        return (1);
    }


    shifts = 28;


    for (index = 0; index < length; index ++)
    {
        decoded |= (genie_decode_digit (code [index]) << shifts);


        shifts -= 4;
    }


    decoded = genie_descramble (decoded);


    *address = ((decoded >> 8) & ((1 << 16) - 1));


    *value = ((decoded >> 24) & ((1 << 8) - 1));


    if (length == 8)
    {
        *match_value = (decoded & ((1 << 8) - 1));
    }
    else
    {
        *address += 0x8000;


        *match_value = cpu_read (*address);
    }


    return (0);
}


#endif /* ! GENIE_H_INCLUDED */
