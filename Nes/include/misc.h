/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

misc.h: Global miscellaneous definitions.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include "../../include/Lib_Clara.h"
#include "../../include/Dir.h"

#define free mfree
#define INLINE inline
#define PACKFILE int

#define LSB_FIRST

#define HERE debug_printf( "\nHERE file %s line %d\n", __FILE__, __LINE__ );

typedef unsigned char UINT8;
typedef signed char INT8;

typedef unsigned short UINT16;
typedef signed short INT16;

typedef unsigned int UINT32;
typedef signed int INT32;


typedef union
{
    struct
    {
#ifdef LSB_FIRST
        UINT8 low, high;
#else
        UINT8 high, low;
#endif
    }
    bytes;

    UINT16 word;
} PAIR;


#undef  TRUE
#define TRUE    1


#undef  FALSE
#define FALSE   (! TRUE)


/* NIL has replaced NULL. */

#define NIL     0


//FILE * log_file;

inline bool exists(wchar_t *file)
{
  W_FSTAT fs;
  return w_fstat(file, &fs) == 0;
}

inline PACKFILE pack_fopen_chunk( PACKFILE file, int )
{
  return file;
}

inline void pack_fclose_chunk( PACKFILE )
{
}

#define piput( name, type ) inline int name( type w, PACKFILE f ) \
  { \
    return w_fwrite( f, &w, sizeof(type) ); \
  }

piput( pack_iputw, UINT16 )
piput( pack_iputl, int )
piput( pack_putc, UINT8 )

#define piget( name, type ) inline type name( PACKFILE f ) \
  { \
    type T; \
    w_fread( f, &T, sizeof(type) ); \
    return T; \
  }

piget( pack_igetw, UINT16 )
piget( pack_igetl, int )
piget( pack_getc, UINT8 )

#define pack_fwrite( buf, size, file ) w_fwrite( file, (void *)buf, size )
#define pack_fread( buf, size, file ) w_fread( file, (void *)buf, size )



#define printf debug_printf


#endif /* ! MISC_H_INCLUDED */
