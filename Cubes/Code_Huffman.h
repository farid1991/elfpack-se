#include "Types.h"
#include "Types.h"
#ifndef CODE_HUFFMAN_H_
#define CODE_HUFFMAN_H_
char Bits2Byte(char* bits);
void RECORD2BITRECORD(RECORD* rec, BITRECORD* bitrec);
void BITRECORD2RECORD(BITRECORD* bitrec, RECORD* rec);
COLOR Bits2Color(char* bits);
char* Byte2Bits(char byte);
char* Color2Bits(COLOR color);
#endif
