#include "Code_Huffman.h"

char Bits2Byte(char* bits)
{
	char byte=0;
	char mask=128;
	for(int i=0;i<8;i++)
	{
		byte+=mask*bits[i];
		mask=mask/2;
	};
	return byte;
};

void RECORD2BITRECORD(RECORD* rec, BITRECORD* bitrec)
{
	char* bits=new char[904];
	memset(bits,0,904);
	char* color2bits;
	int pos=0;
	for(int i=0;i<15;i++)
		for(int j=0;j<20;j++)
		{
			color2bits=Color2Bits(rec->colors[i][j]);
			for(int j=0;j<3;j++)
			{
				bits[pos]=color2bits[j];
				pos++;
			};
			delete[] color2bits;
		};
	char* bitbyte=bits;
	for(int i=0;i<113;i++)
	{
		bitrec->byte[i]=Bits2Byte(bitbyte);
		bitbyte+=8;
	};
	delete[] bits;
	bitrec->point=rec->point;
};

void BITRECORD2RECORD(BITRECORD* bitrec, RECORD* rec)
{
	char* bits=new char[904];
	char* byte2bit;
	int pos=0;
	for(int i=0;i<113;i++)
	{
		byte2bit=Byte2Bits(bitrec->byte[i]);
		for(int j=0;j<8;j++)
		{
			bits[pos]=byte2bit[j];
			pos++;
		};
		delete[] byte2bit;
	};
	char* bitcol=bits;
	for(int i=0;i<15;i++)
		for(int j=0;j<20;j++)
		{
			rec->colors[i][j]=Bits2Color(bitcol);
			bitcol+=3;
		};
	delete[] bits;
	rec->point=bitrec->point;
};

COLOR Bits2Color(char* bits)
{
	if(bits[0]==0 && bits[1]==0 && bits[2]==0) return COLOR_RED;
	if(bits[0]==1 && bits[1]==0 && bits[2]==0) return COLOR_GREEN;
	if(bits[0]==1 && bits[1]==0 && bits[2]==1) return COLOR_BLUE;
	if(bits[0]==1 && bits[1]==1 && bits[2]==0) return COLOR_YELLOW;
	if(bits[0]==1 && bits[1]==1 && bits[2]==1) return COLOR_PURPLE;
	return COLOR_WHITE;
};

char* Byte2Bits(char byte)
{
	char* bits=new char[8];
	char mask=128;
	for(int i=0;i<8;i++)
	{
		if((byte & mask)==mask) bits[i]=1;
		else bits[i]=0;
		mask=mask/2;
	};
	return bits;
};

char* Color2Bits(COLOR color)
{
	char* bits=new char[3];
	switch(color)
	{
	case COLOR_RED:
		bits[0]=0;
		bits[1]=0;
		bits[2]=0;
		break;
	case COLOR_GREEN:
		bits[0]=1;
		bits[1]=0;
		bits[2]=0;
		break;
	case COLOR_BLUE:
		bits[0]=1;
		bits[1]=0;
		bits[2]=1;
		break;
	case COLOR_YELLOW:
		bits[0]=1;
		bits[1]=1;
		bits[2]=0;
		break;
	case COLOR_PURPLE:
		bits[0]=1;
		bits[1]=1;
		bits[2]=1;
		break;
	default:
		bits[0]=0;
		bits[1]=0;
		bits[2]=1;
	};
	return bits;
};
