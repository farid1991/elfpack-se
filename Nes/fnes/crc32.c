/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

crc32.c: Implementation of the CRC32 calculation.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/

#include "misc.h"
#include "crc32.h"

unsigned long crc_table[256];

int crc32_initialized = FALSE;


void crc32_init (void)
{
  unsigned long crc;
  for (int i = 0; i < 256; i++)
  {
    crc = i;
    for (int j = 0; j < 8; j++)
      crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
    crc_table[i] = crc;
  }

  crc32_initialized = TRUE;
}


UINT32 crc32_calculate(unsigned char *buf, unsigned long len)
{
  unsigned long crc;

  if (!crc32_initialized) crc32_init ();

  crc = 0xFFFFFFFFUL;
  while (len--)
    crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
  return crc ^ 0xFFFFFFFFUL;
}
