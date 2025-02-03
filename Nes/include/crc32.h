/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

crc32.h: Declarations for the CRC32 calculation.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/
#ifndef CRC32_H_INCLUDED
#define CRC32_H_INCLUDED


#include "misc.h"


UINT32 crc32_calculate(unsigned char *buf, unsigned long len);


#endif /* ! CRC32_H_INCLUDED */
