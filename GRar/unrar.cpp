#include "main.h"

#define bool int
#define false 0
#define true 1


void *memset(void *buf, int val, int size){
  char *b=(char*)buf;
  while(--size>=0) *b++=val;
  return buf;
}

inline int abs(int a){ return (a<0)?-a:a;}

//typedef unsigned char    byte;   //8 bits
//typedef unsigned int     uint;   //32 bits or more

int in, outhandle; //my add
unsigned int err;

//int loadfilesize=0, readoff=0;
//unsigned long writebytes; //tmp
//char *readdata; // Исходный rar

//char FileName[100];
//int HeadType, Flags, HeadSize, PackSize, UnpSize, Version, Method, NameSize, NextPos;
int Flags;
//int Attrib, AllSize;

const int MAX_SIZE = 0x8000;
int MAXWINSIZE;// = 0x100000;  //80000=512kb
int MAXWINMASK;// = 0x0FFFFF;//(MAXWINSIZE-1);!!!!

//const  int SIZEOF_SHORTBLOCKHEAD =  7;
//const  int LHD_SOLID  = 0x10;
//const  int ATTR_SUB  = 0x10;

const int LOW_DIST_REP_COUNT = 16;
const int BLOCK_LZ=0, BLOCK_PPM=1;

char LDecode[]={0,1,2,3,4,5,6,7,8,10,12,14,16,20,24,28,32,40,48,56,64,80,96,112,(char)128,(char)160,(char)192,(char)224};
char LBits[]=  {0,0,0,0,0,0,0,0,1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5};

int DDecode[]={0,1,2,3,4,6,8,12,16,24,32,48,64,96,128,192,256,384,512,768,1024,1536,2048,3072,4096,6144,8192,12288,16384,24576,32768U,49152U,65536,98304,131072,196608,262144,327680,393216,458752,524288,589824,655360,720896,786432,851968,917504,983040};
char DBits[]=  {0,0,0,0,1,1,2, 2, 3, 3, 4, 4, 5, 5,  6,  6,  7,  7,  8,  8,   9,   9,  10,  10,  11,  11,  12,   12,   13,   13,    14,    14,   15,   15,    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,    16};

int DBitLengthCounts[]= {4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,14,0,12};
char SDDecode[]={0,4,8,16,32,64,(char)128,(char)192};
char SDBits[]=  {2,2,3, 4, 5, 6,  6,  6};


#define NC 299
#define DC  60
#define LDC 17
#define RC  28
#define HUFF_TABLE_SIZE (NC+DC+RC+LDC)
#define BC  20

#define NC20 298
#define DC20 48
#define RC20 28
#define BC20 19
#define MC20 257

int LLSize[]={NC,DC,RC,BC,LDC, NC20,DC20,RC20,BC20,MC20,MC20,MC20,MC20};
int DecodeN[NC+DC+RC+BC+LDC+ NC20+DC20+RC20+BC20+MC20*4];

struct Decode{
  int MaxNum;
  int DecodeLen[16];
  int DecodePos[16];
  int DecodePtr;//add
  int DecodeCnt;//add
} LL[13];

void InitLL(){
  int i,j;
  for(i=j=0;i<13;i++){
    LL[i].DecodePtr=j;
    LL[i].DecodeCnt=LLSize[i];
    j+=LLSize[i];
  }
}

struct AudioVariables
{
  int K1,K2,K3,K4,K5;
  int D1,D2,D3,D4;
  int LastDelta;
  int Dif[11];
  int ByteCount;
  int LastChar;
};
/***************************** Unpack v 2.0 *********************************/


    int UnpRead(char *Window,int UnpPtr);
    void UnpWrite(char *Window,int UnpPtr);

    char UnpOldTable[HUFF_TABLE_SIZE];
    char *Window=0;
    char *InBuf=0;
    int InAddr, InBit;

    void InitBitInput(){
      InAddr=InBit=0;
    }
    void addbits(int Bits){
      Bits+=InBit;
      InAddr+=Bits>>3;
      InBit=Bits&7;
    }
    int getbits(){
      int BitField=(InBuf[InAddr]&0xff) << 16;
      BitField|=(InBuf[InAddr+1]&0xff) << 8;
      BitField|=(InBuf[InAddr+2]&0xff);
      BitField >>= (8-InBit);
      return(BitField & 0xffff);
    }

    void Unpack29(bool Solid);
    bool UnpReadBuf();
    void UnpWriteBuf();
    void UnpWriteArea(int StartPtr,int EndPtr);
    void UnpWriteData(char *Data,int Size);
    bool ReadTables();
    void MakeDecodeTables(char *LenTab,int pos);
    int DecodeNumber(int pos);
    void UnpInitData(int Solid);
    void CopyString();
    bool ReadEndOfBlock();
    void CopyString(int Length,int Distance);
    inline void InsertOldDist(int Distance);
    inline void InsertLastMatch(int Length,int Distance);
    int PPMEscChar;
    int LastFilter;
    bool TablesRead;
    int OldDist[4],OldDistPtr;
    int LastDist,LastLength;
    int UnpPtr,WrPtr;
    int ReadTop;
    int ReadBorder;

    int UnpBlockType;
    int DestUnpSize;

    bool Suspended;
    bool UnpAllBuf;
    bool UnpSomeRead;
    int WrittenFileSize;
    bool FileExtracted;
    bool PPMError;

    int PrevLowDist,LowDistRepCount;

/***************************** Unpack v 1.5 *********************************/

    void Unpack15(bool Solid);
    void ShortLZ();
    void LongLZ();
    void HuffDecode();
    void GetFlagsBuf();
    void OldUnpInitData(int Solid);
    void InitHuff();
    void CorrHuff(int *CharSet,int *NumToPlace);
    void OldCopyString(int Distance,int Length);
    int DecodeNum(int Num,int StartPos,
      int *DecTab,int *PosTab);
    void OldUnpWriteBuf();

    int ChSet[256],ChSetA[256],ChSetB[256],ChSetC[256];
    int Place[256],PlaceA[256],PlaceB[256],PlaceC[256];
    int NToPl[256],NToPlB[256],NToPlC[256];
    int FlagBuf,AvrPlc,AvrPlcB,AvrLn1,AvrLn2,AvrLn3;
    int Buf60,NumHuf,StMode,LCount,FlagsCnt;
    int Nhfb,Nlzb,MaxDist3;
/***************************** Unpack v 2.0 *********************************/
    void Unpack20(bool Solid);
    char UnpOldTable20[MC20*4];
    int UnpAudioBlock,UnpChannels,UnpCurChannel,UnpChannelDelta;
    void CopyString20(int Length,int Distance);
    bool ReadTables20();
    void UnpInitData20(int Solid);
    void ReadLastTables();
    char DecodeAudio(int Delta);

    struct AudioVariables AudV[4];
/***************************** Unpack v 2.0 *********************************/
    void DoUnpack(int Method,bool Solid);
    bool IsFileExtracted() {return(FileExtracted);}
    void SetDestSize(int DestSize) {DestUnpSize=DestSize;FileExtracted=false;}
    void SetSuspended(bool Suspend) {Suspended=Suspend;}
/*
    unsigned int GetChar()
    {
      if (InAddr>MAX_SIZE-30)
        UnpReadBuf();
      return(InBuf[InAddr++]);
    }
*/

//#include "rar.hpp"
//#include "unpack15.cpp"
//#include "unpack20.cpp"

void UnpackInit()
{
  InAddr=InBit=0;
  Suspended=false;
  UnpAllBuf=false;
  UnpSomeRead=false;
  InBuf=(char*)malloc(MAX_SIZE);
  MAXWINSIZE = 0x400000;
  if((Window=(char*)malloc(MAXWINSIZE))==0)
  {
    MAXWINSIZE = 0x100000;
    Window=(char*)malloc(MAXWINSIZE);
  }
  MAXWINMASK = MAXWINSIZE-1;
  InitLL();
  UnpInitData(false);
}


void DoUnpack(int Method,bool Solid)
{
  switch(Method)
  {
    case 14: case 13: case 12: case 11: case 10://add
    case 15: // rar 1.5 compression
      Unpack15(Solid);
      break;
    case 20: // rar 2.x compression
    case 26: // files larger than 2GB
      Unpack20(Solid);
      break;
    case 29: // rar 3.x compression
    case 36: // alternative hash
      Unpack29(Solid);
      break;
  }
}


inline void InsertOldDist(int Distance)
{
  OldDist[3]=OldDist[2];
  OldDist[2]=OldDist[1];
  OldDist[1]=OldDist[0];
  OldDist[0]=Distance;
}


inline void InsertLastMatch(int Length,int Distance)
{
  LastDist=Distance;
  LastLength=Length;
}


void CopyString(int Length,int Distance)
{
  int DestPtr=UnpPtr-Distance;
  if (DestPtr<MAXWINSIZE-260 && UnpPtr<MAXWINSIZE-260)
  {
    Window[UnpPtr++]=Window[DestPtr++];
    while (--Length>0)
      Window[UnpPtr++]=Window[DestPtr++];
  }
  else
    while (Length--)
    {
      Window[UnpPtr]=Window[DestPtr++ & MAXWINMASK];
      UnpPtr=(UnpPtr+1) & MAXWINMASK;
    }
}


int DecodeNumber(int pos)
{
  int Bits;
  int BitField=getbits() & 0xfffe;
  if (BitField<LL[pos].DecodeLen[8])
    if (BitField<LL[pos].DecodeLen[4])
      if (BitField<LL[pos].DecodeLen[2])
        if (BitField<LL[pos].DecodeLen[1])
          Bits=1;
        else
          Bits=2;
      else
        if (BitField<LL[pos].DecodeLen[3])
          Bits=3;
        else
          Bits=4;
    else
      if (BitField<LL[pos].DecodeLen[6])
        if (BitField<LL[pos].DecodeLen[5])
          Bits=5;
        else
          Bits=6;
      else
        if (BitField<LL[pos].DecodeLen[7])
          Bits=7;
        else
          Bits=8;
  else
    if (BitField<LL[pos].DecodeLen[12])
      if (BitField<LL[pos].DecodeLen[10])
        if (BitField<LL[pos].DecodeLen[9])
          Bits=9;
        else
          Bits=10;
      else
        if (BitField<LL[pos].DecodeLen[11])
          Bits=11;
        else
          Bits=12;
    else
      if (BitField<LL[pos].DecodeLen[14])
        if (BitField<LL[pos].DecodeLen[13])
          Bits=13;
        else
          Bits=14;
      else
        Bits=15;

  addbits(Bits);
  int N=LL[pos].DecodePos[Bits]+((BitField-LL[pos].DecodeLen[Bits-1])>>(16-Bits));
  if (N<0 || N>=LL[pos].MaxNum)
    N=0;
  return(DecodeN[LL[pos].DecodePtr+N]);
}


void Unpack29(bool Solid)
{
  int Bits;

  FileExtracted=true;

  if (!Suspended)
  {
    UnpInitData(Solid);
    if (!UnpReadBuf())
      return;
    if ((!Solid || !TablesRead) && !ReadTables())
      return;
  }

  if (PPMError)
    return;

  while (true)
  {
    UnpPtr&=MAXWINMASK;

    if (InAddr>ReadBorder)
    {
      if (!UnpReadBuf())
        break;
    }
    if (((WrPtr-UnpPtr) & MAXWINMASK)<260 && WrPtr!=UnpPtr)
    {
      UnpWriteBuf();
      if (WrittenFileSize>DestUnpSize)
        return;
      if (Suspended)
      {
        FileExtracted=false;
        return;
      }
    }

    if (UnpBlockType==BLOCK_PPM)
      return;//add

    int Number=DecodeNumber(0);
    if (Number<256)
    {
      Window[UnpPtr++]=Number;
      continue;
    }
    if (Number>=271)
    {
      int Length=(LDecode[Number-=271]&0xff)+3;
      if ((Bits=(LBits[Number]&0xff))>0)
      {
        Length+=getbits()>>(16-Bits);
        addbits(Bits);
      }

      int DistNumber=DecodeNumber(1);
      int Distance=DDecode[DistNumber]+1;
      if ((Bits=(DBits[DistNumber]&0xff))>0)
      {
        if (DistNumber>9)
        {
          if (Bits>4)
          {
            Distance+=((getbits()>>(20-Bits))<<4);
            addbits(Bits-4);
          }
          if (LowDistRepCount>0)
          {
            LowDistRepCount--;
            Distance+=PrevLowDist;
          }
          else
          {
            int LowDist=DecodeNumber(4);
            if (LowDist==16)
            {
              LowDistRepCount=LOW_DIST_REP_COUNT-1;
              Distance+=PrevLowDist;
            }
            else
            {
              Distance+=LowDist;
              PrevLowDist=LowDist;
            }
          }
        }
        else
        {
          Distance+=getbits()>>(16-Bits);
          addbits(Bits);
        }
      }

      if (Distance>=0x2000)
      {
        Length++;
        if (Distance>=0x40000L)
          Length++;
      }

      InsertOldDist(Distance);
      InsertLastMatch(Length,Distance);
      CopyString(Length,Distance);
      continue;
    }
    if (Number==256)
    {
      if (!ReadEndOfBlock())
        break;
      continue;
    }
    if (Number==257)
    {
      //if (!ReadVMCode())
        break;
      //continue;
    }
    if (Number==258)
    {
      if (LastLength!=0)
        CopyString(LastLength,LastDist);
      continue;
    }
    if (Number<263)
    {
      int DistNum=Number-259;
      int Distance=OldDist[DistNum];
      for (int I=DistNum;I>0;I--)
        OldDist[I]=OldDist[I-1];
      OldDist[0]=Distance;

      int LengthNumber=DecodeNumber(2);
      int Length=(LDecode[LengthNumber]&0xff)+2;
      if ((Bits=(LBits[LengthNumber]&0xff))>0)
      {
        Length+=getbits()>>(16-Bits);
        addbits(Bits);
      }
      InsertLastMatch(Length,Distance);
      CopyString(Length,Distance);
      continue;
    }
    if (Number<272)
    {
      int Distance=(SDDecode[Number-=263]&0xff)+1;
      if ((Bits=(SDBits[Number]&0xff))>0)
      {
        Distance+=getbits()>>(16-Bits);
        addbits(Bits);
      }
      InsertOldDist(Distance);
      InsertLastMatch(2,Distance);
      CopyString(2,Distance);
      continue;
    }
  }
  UnpWriteBuf();
}


bool ReadEndOfBlock()
{
  int BitField=getbits();
  bool NewTable,NewFile=false;
  if (BitField & 0x8000)
  {
    NewTable=true;
    addbits(1);
  }
  else
  {
    NewFile=true;
    NewTable=(BitField & 0x4000);
    addbits(2);
  }
  TablesRead=!NewTable;
  return !(NewFile || NewTable && !ReadTables());
}

bool UnpReadBuf()
{
  int DataSize=ReadTop-InAddr;
  if (DataSize<0)
    return(false);
  if (InAddr>MAX_SIZE/2)
  {
    if (DataSize>0)
      //memmove(InBuf,InBuf+InAddr,DataSize);!!!!
      memcpy(InBuf,InBuf+InAddr,DataSize);
    InAddr=0;
    ReadTop=DataSize;
  }
  else
    DataSize=ReadTop;
  int ReadCode=UnpRead(InBuf+DataSize,(MAX_SIZE-DataSize)&~0xf);
  if (ReadCode>0)
    ReadTop+=ReadCode;
  ReadBorder=ReadTop-30;
  return(ReadCode!=-1);
}


void UnpWriteBuf()
{
  int WrittenBorder=WrPtr;
  //int WriteSize=(UnpPtr-WrittenBorder)&MAXWINMASK;
  UnpWriteArea(WrittenBorder,UnpPtr);
  WrPtr=UnpPtr;
}

void UnpWriteArea(int StartPtr,int EndPtr)
{
  if (EndPtr!=StartPtr)
    UnpSomeRead=true;
/*
  if (EndPtr<StartPtr)
  {
    UnpWriteData(&Window[StartPtr],-StartPtr & MAXWINMASK);
    UnpWriteData(Window,EndPtr);
    UnpAllBuf=true;
  }
  else*/
    UnpWriteData(&Window[StartPtr],EndPtr-StartPtr);
}


void UnpWriteData(char *Data,int Size)
{
  if (WrittenFileSize>=DestUnpSize)
    return;
  int WriteSize=Size;
  int LeftToWrite=DestUnpSize-WrittenFileSize;
  if (WriteSize>LeftToWrite)
    WriteSize=LeftToWrite;
  UnpWrite(Data,WriteSize);
  WrittenFileSize+=Size;
}


bool ReadTables()
{
  int I;
  char BitLength[BC];
  char Table[HUFF_TABLE_SIZE];
  if (InAddr>ReadTop-25)
    if (!UnpReadBuf())
      return(false);
  addbits((8-InBit)&7);
  int BitField=getbits();
  if (BitField & 0x8000)
  {
    UnpBlockType=BLOCK_PPM;
    //return(PPM.DecodeInit(this,PPMEscChar));
    return false;//add
  }
  UnpBlockType=BLOCK_LZ;

  PrevLowDist=0;
  LowDistRepCount=0;

  if (!(BitField & 0x4000))
    memset(UnpOldTable,0,sizeof(UnpOldTable));
  addbits(2);

  for (int I=0;I<BC;I++)
  {
    int Length=(getbits() >> 12);
    addbits(4);
    if (Length==15)
    {
      int ZeroCount=(getbits() >> 12);
      addbits(4);
      if (ZeroCount==0)
        BitLength[I]=15;
      else
      {
        ZeroCount+=2;
        while (ZeroCount-- > 0 && I<sizeof(BitLength)/sizeof(BitLength[0]))
          BitLength[I++]=0;
        I--;
      }
    }
    else
      BitLength[I]=Length;
  }
  MakeDecodeTables((char*)BitLength,3);

  const int TableSize=HUFF_TABLE_SIZE;
  for (I=0;I<TableSize;)
  {
    if (InAddr>ReadTop-5)
      if (!UnpReadBuf())
        return(false);
    int Number=DecodeNumber(3);
    if (Number<16)
    {
      Table[I]=(Number+UnpOldTable[I]) & 0xf;
      I++;
    }
    else
      if (Number<18)
      {
        int N;
        if (Number==16)
        {
          N=(getbits() >> 13)+3;
          addbits(3);
        }
        else
        {
          N=(getbits() >> 9)+11;
          addbits(7);
        }
        while (N-- > 0 && I<TableSize)
        {
          Table[I]=Table[I-1];
          I++;
        }
      }
      else
      {
        int N;
        if (Number==18)
        {
          N=(getbits() >> 13)+3;
          addbits(3);
        }
        else
        {
          N=(getbits() >> 9)+11;
          addbits(7);
        }
        while (N-- > 0 && I<TableSize)
          Table[I++]=0;
      }
  }
  TablesRead=true;
  if (InAddr>ReadTop)
    return(false);
  MakeDecodeTables(&Table[0],0);
  MakeDecodeTables(&Table[NC],1);
  MakeDecodeTables(&Table[NC+DC],4);
  MakeDecodeTables(&Table[NC+DC+LDC],2);
  memcpy(UnpOldTable,Table,sizeof(UnpOldTable));
  return(true);
}


void UnpInitData(int Solid)
{
  if (!Solid)
  {
    TablesRead=false;
    memset(OldDist,0,sizeof(OldDist));
    OldDistPtr=0;
    LastDist=LastLength=0;
    memset(Window,0,MAXWINSIZE);
    memset(UnpOldTable,0,sizeof(UnpOldTable));
    UnpPtr=WrPtr=0;
    PPMEscChar=2;

    //InitFilters();
  }
  InitBitInput();
  PPMError=false;
  WrittenFileSize=0;
  ReadTop=0;
  ReadBorder=0;
  UnpInitData20(Solid);
}

void MakeDecodeTables(char *LenTab,int pos)
{
  int LenCount[16],TmpPos[16],I,Size=LL[pos].DecodeCnt;
  long M,N;
  memset(LenCount,0,sizeof(LenCount));
  memset(DecodeN+LL[pos].DecodePtr,0,Size*sizeof(int));
  for (I=0;I<Size;I++)
    LenCount[LenTab[I] & 0xF]++;

  LenCount[0]=TmpPos[0]=0;
  LL[pos].DecodePos[0]=LL[pos].DecodeLen[0]=0;
  for (N=0,I=1;I<16;I++)
  {
    N=2*(N+LenCount[I]);
    M=N<<(15-I);
    if (M>0xFFFF)
      M=0xFFFF;
    LL[pos].DecodeLen[I]=(int)M;
    TmpPos[I]=LL[pos].DecodePos[I]=LL[pos].DecodePos[I-1]+LenCount[I-1];
  }

  for (I=0;I<Size;I++)
    if (LenTab[I]!=0)
      DecodeN[LL[pos].DecodePtr+(TmpPos[LenTab[I] & 0xF]++)]=I;
  LL[pos].MaxNum=Size;
}


/////////////////unpack15
#define STARTL1  2
static int DecL1[]={0x8000,0xa000,0xc000,0xd000,0xe000,0xea00,
                             0xee00,0xf000,0xf200,0xf200,0xffff};
static int PosL1[]={0,0,0,2,3,5,7,11,16,20,24,32,32};

#define STARTL2  3
static int DecL2[]={0xa000,0xc000,0xd000,0xe000,0xea00,0xee00,
                             0xf000,0xf200,0xf240,0xffff};
static int PosL2[]={0,0,0,0,5,7,9,13,18,22,26,34,36};

#define STARTHF0  4
static int DecHf0[]={0x8000,0xc000,0xe000,0xf200,0xf200,0xf200,
                              0xf200,0xf200,0xffff};
static int PosHf0[]={0,0,0,0,0,8,16,24,33,33,33,33,33};


#define STARTHF1  5
static int DecHf1[]={0x2000,0xc000,0xe000,0xf000,0xf200,0xf200,
                              0xf7e0,0xffff};
static int PosHf1[]={0,0,0,0,0,0,4,44,60,76,80,80,127};


#define STARTHF2  5
static int DecHf2[]={0x1000,0x2400,0x8000,0xc000,0xfa00,0xffff,
                              0xffff,0xffff};
static int PosHf2[]={0,0,0,0,0,0,2,7,53,117,233,0,0};


#define STARTHF3  6
static int DecHf3[]={0x800,0x2400,0xee00,0xfe80,0xffff,0xffff,
                              0xffff};
static int PosHf3[]={0,0,0,0,0,0,0,2,16,218,251,0,0};


#define STARTHF4  8
static int DecHf4[]={0xff00,0xffff,0xffff,0xffff,0xffff,0xffff};
static int PosHf4[]={0,0,0,0,0,0,0,0,0,255,0,0,0};


void Unpack15(bool Solid)
{
  if (Suspended)
    UnpPtr=WrPtr;
  else
  {
    UnpInitData(Solid);
    OldUnpInitData(Solid);
    UnpReadBuf();
    if (!Solid)
    {
      InitHuff();
      UnpPtr=0;
    }
    else
      UnpPtr=WrPtr;
    --DestUnpSize;
  }
  if (DestUnpSize>=0)
  {
    GetFlagsBuf();
    FlagsCnt=8;
  }

  while (DestUnpSize>=0)
  {
    UnpPtr&=MAXWINMASK;

    if (InAddr>ReadTop-30 && !UnpReadBuf())
      break;
    if (((WrPtr-UnpPtr) & MAXWINMASK)<270 && WrPtr!=UnpPtr)
    {
      OldUnpWriteBuf();
      if (Suspended)
        return;
    }
    if (StMode)
    {
      HuffDecode();
      continue;
    }

    if (--FlagsCnt < 0)
    {
      GetFlagsBuf();
      FlagsCnt=7;
    }

    if (FlagBuf & 0x80)
    {
      FlagBuf<<=1;
      if (Nlzb > Nhfb)
        LongLZ();
      else
        HuffDecode();
    }
    else
    {
      FlagBuf<<=1;
      if (--FlagsCnt < 0)
      {
        GetFlagsBuf();
        FlagsCnt=7;
      }
      if (FlagBuf & 0x80)
      {
        FlagBuf<<=1;
        if (Nlzb > Nhfb)
          HuffDecode();
        else
          LongLZ();
      }
      else
      {
        FlagBuf<<=1;
        ShortLZ();
      }
    }
  }
  OldUnpWriteBuf();
}


void OldUnpWriteBuf()
{
  if (UnpPtr!=WrPtr)
    UnpSomeRead=true;
  if (UnpPtr<WrPtr)
  {
    UnpWrite(&Window[WrPtr],-WrPtr & MAXWINMASK);
    UnpWrite(Window,UnpPtr);
    UnpAllBuf=true;
  }
  else
    UnpWrite(&Window[WrPtr],UnpPtr-WrPtr);
  WrPtr=UnpPtr;
}


void ShortLZ()
{
  static int ShortLen1[]={1,3,4,4,5,6,7,8,8,4,4,5,6,6,4,0};
  static int ShortXor1[]={0,0xa0,0xd0,0xe0,0xf0,0xf8,0xfc,0xfe,
                                   0xff,0xc0,0x80,0x90,0x98,0x9c,0xb0};
  static int ShortLen2[]={2,3,3,3,4,4,5,6,6,4,4,5,6,6,4,0};
  static int ShortXor2[]={0,0x40,0x60,0xa0,0xd0,0xe0,0xf0,0xf8,
                                   0xfc,0xc0,0x80,0x90,0x98,0x9c,0xb0};


  int Length,SaveLength;
  int LastDistance;
  int Distance;
  int DistancePlace;
  NumHuf=0;

  int BitField=getbits();
  if (LCount==2)
  {
    addbits(1);
    if (BitField >= 0x8000)
    {
      OldCopyString((int)LastDist,LastLength);
      return;
    }
    BitField <<= 1;
    LCount=0;
  }

  BitField>>=8;

  ShortLen1[1]=ShortLen2[3]=Buf60+3;

  if (AvrLn1<37)
  {
    for (Length=0;;Length++)
      if (((BitField^ShortXor1[Length]) & (~(0xff>>ShortLen1[Length])))==0)
        break;
    addbits(ShortLen1[Length]);
  }
  else
  {
    for (Length=0;;Length++)
      if (((BitField^ShortXor2[Length]) & (~(0xff>>ShortLen2[Length])))==0)
        break;
    addbits(ShortLen2[Length]);
  }

  if (Length >= 9)
  {
    if (Length == 9)
    {
      LCount++;
      OldCopyString((int)LastDist,LastLength);
      return;
    }
    if (Length == 14)
    {
      LCount=0;
      Length=DecodeNum(getbits(),STARTL2,DecL2,PosL2)+5;
      Distance=(getbits()>>1) | 0x8000;
      addbits(15);
      LastLength=Length;
      LastDist=Distance;
      OldCopyString(Distance,Length);
      return;
    }

    LCount=0;
    SaveLength=Length;
    Distance=OldDist[(OldDistPtr-(Length-9)) & 3];
    Length=DecodeNum(getbits(),STARTL1,DecL1,PosL1)+2;
    if (Length==0x101 && SaveLength==10)
    {
      Buf60 ^= 1;
      return;
    }
    if (Distance > 256)
      Length++;
    if (Distance >= MaxDist3)
      Length++;

    OldDist[OldDistPtr++]=Distance;
    OldDistPtr = OldDistPtr & 3;
    LastLength=Length;
    LastDist=Distance;
    OldCopyString(Distance,Length);
    return;
  }

  LCount=0;
  AvrLn1 += Length;
  AvrLn1 -= AvrLn1 >> 4;

  DistancePlace=DecodeNum(getbits(),STARTHF2,DecHf2,PosHf2) & 0xff;
  Distance=ChSetA[DistancePlace];
  if (--DistancePlace != -1)
  {
    PlaceA[Distance]--;
    LastDistance=ChSetA[DistancePlace];
    PlaceA[LastDistance]++;
    ChSetA[DistancePlace+1]=LastDistance;
    ChSetA[DistancePlace]=Distance;
  }
  Length+=2;
  OldDist[OldDistPtr++] = ++Distance;
  OldDistPtr = OldDistPtr & 3;
  LastLength=Length;
  LastDist=Distance;
  OldCopyString(Distance,Length);
}


void LongLZ()
{
  int Length;
  int Distance;
  int DistancePlace,NewDistancePlace;
  int OldAvr2,OldAvr3;

  NumHuf=0;
  Nlzb+=16;
  if (Nlzb > 0xff)
  {
    Nlzb=0x90;
    Nhfb >>= 1;
  }
  OldAvr2=AvrLn2;

  int BitField=getbits();
  if (AvrLn2 >= 122)
    Length=DecodeNum(BitField,STARTL2,DecL2,PosL2);
  else
    if (AvrLn2 >= 64)
      Length=DecodeNum(BitField,STARTL1,DecL1,PosL1);
    else
      if (BitField < 0x100)
      {
        Length=BitField;
        addbits(16);
      }
      else
      {
        for (Length=0;((BitField<<Length)&0x8000)==0;Length++)
          ;
        addbits(Length+1);
      }

  AvrLn2 += Length;
  AvrLn2 -= AvrLn2 >> 5;

  BitField=getbits();
  if (AvrPlcB > 0x28ff)
    DistancePlace=DecodeNum(BitField,STARTHF2,DecHf2,PosHf2);
  else
    if (AvrPlcB > 0x6ff)
      DistancePlace=DecodeNum(BitField,STARTHF1,DecHf1,PosHf1);
    else
      DistancePlace=DecodeNum(BitField,STARTHF0,DecHf0,PosHf0);

  AvrPlcB += DistancePlace;
  AvrPlcB -= AvrPlcB >> 8;
  while (1)
  {
    Distance = ChSetB[DistancePlace & 0xff];
    NewDistancePlace = NToPlB[Distance++ & 0xff]++;
    if (!(Distance & 0xff))
      CorrHuff(ChSetB,NToPlB);
    else
      break;
  }

  ChSetB[DistancePlace]=ChSetB[NewDistancePlace];
  ChSetB[NewDistancePlace]=Distance;

  Distance=((Distance & 0xff00) | (getbits() >> 8)) >> 1;
  addbits(7);

  OldAvr3=AvrLn3;
  if (Length!=1 && Length!=4)
    if (Length==0 && Distance <= MaxDist3)
    {
      AvrLn3++;
      AvrLn3 -= AvrLn3 >> 8;
    }
    else
      if (AvrLn3 > 0)
        AvrLn3--;
  Length+=3;
  if (Distance >= MaxDist3)
    Length++;
  if (Distance <= 256)
    Length+=8;
  if (OldAvr3 > 0xb0 || AvrPlc >= 0x2a00 && OldAvr2 < 0x40)
    MaxDist3=0x7f00;
  else
    MaxDist3=0x2001;
  OldDist[OldDistPtr++]=Distance;
  OldDistPtr = OldDistPtr & 3;
  LastLength=Length;
  LastDist=Distance;
  OldCopyString(Distance,Length);
}


void HuffDecode()
{
  int CurByte,NewBytePlace;
  int Length;
  int Distance;
  int BytePlace;

  int BitField=getbits();

  if (AvrPlc > 0x75ff)
    BytePlace=DecodeNum(BitField,STARTHF4,DecHf4,PosHf4);
  else
    if (AvrPlc > 0x5dff)
      BytePlace=DecodeNum(BitField,STARTHF3,DecHf3,PosHf3);
    else
      if (AvrPlc > 0x35ff)
        BytePlace=DecodeNum(BitField,STARTHF2,DecHf2,PosHf2);
      else
        if (AvrPlc > 0x0dff)
          BytePlace=DecodeNum(BitField,STARTHF1,DecHf1,PosHf1);
        else
          BytePlace=DecodeNum(BitField,STARTHF0,DecHf0,PosHf0);
  BytePlace&=0xff;
  if (StMode)
  {
    if (BytePlace==0 && BitField > 0xfff)
      BytePlace=0x100;
    if (--BytePlace==-1)
    {
      BitField=getbits();
      addbits(1);
      if (BitField & 0x8000)
      {
        NumHuf=StMode=0;
        return;
      }
      else
      {
        Length = (BitField & 0x4000) ? 4 : 3;
        addbits(1);
        Distance=DecodeNum(getbits(),STARTHF2,DecHf2,PosHf2);
        Distance = (Distance << 5) | (getbits() >> 11);
        addbits(5);
        OldCopyString(Distance,Length);
        return;
      }
    }
  }
  else
    if (NumHuf++ >= 16 && FlagsCnt==0)
      StMode=1;
  AvrPlc += BytePlace;
  AvrPlc -= AvrPlc >> 8;
  Nhfb+=16;
  if (Nhfb > 0xff)
  {
    Nhfb=0x90;
    Nlzb >>= 1;
  }

  Window[UnpPtr++]=(ChSet[BytePlace]>>8);
  --DestUnpSize;

  while (1)
  {
    CurByte=ChSet[BytePlace];
    NewBytePlace=NToPl[CurByte++ & 0xff]++;
    if ((CurByte & 0xff) > 0xa1)
      CorrHuff(ChSet,NToPl);
    else
      break;
  }

  ChSet[BytePlace]=ChSet[NewBytePlace];
  ChSet[NewBytePlace]=CurByte;
}


void GetFlagsBuf()
{
  int Flags,NewFlagsPlace;
  int FlagsPlace=DecodeNum(getbits(),STARTHF2,DecHf2,PosHf2);

  while (1)
  {
    Flags=ChSetC[FlagsPlace];
    FlagBuf=Flags>>8;
    NewFlagsPlace=NToPlC[Flags++ & 0xff]++;
    if ((Flags & 0xff) != 0)
      break;
    CorrHuff(ChSetC,NToPlC);
  }

  ChSetC[FlagsPlace]=ChSetC[NewFlagsPlace];
  ChSetC[NewFlagsPlace]=Flags;
}


void OldUnpInitData(int Solid)
{
  if (!Solid)
  {
    AvrPlcB=AvrLn1=AvrLn2=AvrLn3=NumHuf=Buf60=0;
    AvrPlc=0x3500;
    MaxDist3=0x2001;
    Nhfb=Nlzb=0x80;
  }
  FlagsCnt=0;
  FlagBuf=0;
  StMode=0;
  LCount=0;
  ReadTop=0;
}


void InitHuff()
{
  for (int I=0;I<256;I++)
  {
    Place[I]=PlaceA[I]=PlaceB[I]=I;
    PlaceC[I]=(~I+1) & 0xff;
    ChSet[I]=ChSetB[I]=I<<8;
    ChSetA[I]=I;
    ChSetC[I]=((~I+1) & 0xff)<<8;
  }
  memset(NToPl,0,sizeof(NToPl));
  memset(NToPlB,0,sizeof(NToPlB));
  memset(NToPlC,0,sizeof(NToPlC));
  CorrHuff(ChSetB,NToPlB);
}


void CorrHuff(int *CharSet,int *NumToPlace)
{
  int I,J;
  for (I=7;I>=0;I--)
    for (J=0;J<32;J++,CharSet++)
      *CharSet=(*CharSet & ~0xff) | I;
  memset(NumToPlace,0,sizeof(NToPl));
  for (I=6;I>=0;I--)
    NumToPlace[I]=(7-I)*32;
}


void OldCopyString(int Distance,int Length)
{
  DestUnpSize-=Length;
  while (Length--)
  {
    Window[UnpPtr]=Window[(UnpPtr-Distance) & MAXWINMASK];
    UnpPtr=(UnpPtr+1) & MAXWINMASK;
  }
}


int DecodeNum(int Num,int StartPos,
                               int *DecTab,int *PosTab)
{
  int I;
  for (Num&=0xfff0,I=0;DecTab[I]<=Num;I++)
    StartPos++;
  addbits(StartPos);
  return(((Num-(I ? DecTab[I-1]:0))>>(16-StartPos))+PosTab[StartPos]);
}



///////////unpack20

void CopyString20(int Length,int Distance)
{
  LastDist=OldDist[OldDistPtr++ & 3]=Distance;
  LastLength=Length;
  DestUnpSize-=Length;

  int DestPtr=UnpPtr-Distance;
  if (DestPtr<MAXWINSIZE-300 && UnpPtr<MAXWINSIZE-300)
  {
    Window[UnpPtr++]=Window[DestPtr++];
    Window[UnpPtr++]=Window[DestPtr++];
    while (Length>2)
    {
      Length--;
      Window[UnpPtr++]=Window[DestPtr++];
    }
  }
  else
    while (Length--)
    {
      Window[UnpPtr]=Window[DestPtr++ & MAXWINMASK];
      UnpPtr=(UnpPtr+1) & MAXWINMASK;
    }
}


void Unpack20(bool Solid)
{
  int Bits;

  if (Suspended)
    UnpPtr=WrPtr;
  else
  {
    UnpInitData(Solid);
    if (!UnpReadBuf())
      return;
    if (!Solid)
      if (!ReadTables20())
        return;
    --DestUnpSize;
  }

  while (DestUnpSize>=0)
  {
    UnpPtr&=MAXWINMASK;

    if (InAddr>ReadTop-30)
      if (!UnpReadBuf())
        break;
    if (((WrPtr-UnpPtr) & MAXWINMASK)<270 && WrPtr!=UnpPtr)
    {
      OldUnpWriteBuf();
      if (Suspended)
        return;
    }
    if (UnpAudioBlock)
    {
      int AudioNumber=DecodeNumber(9+UnpCurChannel);

      if (AudioNumber==256)
      {
        if (!ReadTables20())
          break;
        continue;
      }
      Window[UnpPtr++]=DecodeAudio(AudioNumber);
      if (++UnpCurChannel==UnpChannels)
        UnpCurChannel=0;
      --DestUnpSize;
      continue;
    }

    int Number=DecodeNumber(5);
    if (Number<256)
    {
      Window[UnpPtr++]=Number;
      --DestUnpSize;
      continue;
    }
    if (Number>269)
    {
      int Length=(LDecode[Number-=270]&0xff)+3;
      if ((Bits=(LBits[Number]&0xff))>0)
      {
        Length+=getbits()>>(16-Bits);
        addbits(Bits);
      }

      int DistNumber=DecodeNumber(6);
      int Distance=DDecode[DistNumber]+1;
      if ((Bits=(DBits[DistNumber]&0xff))>0)
      {
        Distance+=getbits()>>(16-Bits);
        addbits(Bits);
      }

      if (Distance>=0x2000)
      {
        Length++;
        if (Distance>=0x40000L)
          Length++;
      }

      CopyString20(Length,Distance);
      continue;
    }
    if (Number==269)
    {
      if (!ReadTables20())
        break;
      continue;
    }
    if (Number==256)
    {
      CopyString20(LastLength,LastDist);
      continue;
    }
    if (Number<261)
    {
      int Distance=OldDist[(OldDistPtr-(Number-256)) & 3];
      int LengthNumber=DecodeNumber(7);
      int Length=(LDecode[LengthNumber]&0xff)+2;
      if ((Bits=(LBits[LengthNumber]&0xff))>0)
      {
        Length+=getbits()>>(16-Bits);
        addbits(Bits);
      }
      if (Distance>=0x101)
      {
        Length++;
        if (Distance>=0x2000)
        {
          Length++;
          if (Distance>=0x40000)
            Length++;
        }
      }
      CopyString20(Length,Distance);
      continue;
    }
    if (Number<270)
    {
      int Distance=(SDDecode[Number-=261]&0xff)+1;
      if ((Bits=(SDBits[Number]&0xff))>0)
      {
        Distance+=getbits()>>(16-Bits);
        addbits(Bits);
      }
      CopyString20(2,Distance);
      continue;
   }
  }
  ReadLastTables();
  OldUnpWriteBuf();
}


bool ReadTables20()
{
  char BitLength[BC20];
  char Table[MC20*4];
  int TableSize,N,I;
  if (InAddr>ReadTop-25)
    if (!UnpReadBuf())
      return(false);
  int BitField=getbits();
  UnpAudioBlock=(BitField & 0x8000);

  if (!(BitField & 0x4000))
    memset(UnpOldTable20,0,sizeof(UnpOldTable20));
  addbits(2);

  if (UnpAudioBlock)
  {
    UnpChannels=((BitField>>12) & 3)+1;
    if (UnpCurChannel>=UnpChannels)
      UnpCurChannel=0;
    addbits(2);
    TableSize=MC20*UnpChannels;
  }
  else
    TableSize=NC20+DC20+RC20;

  for (I=0;I<BC20;I++)
  {
    BitLength[I]=(getbits() >> 12);
    addbits(4);
  }
  MakeDecodeTables(BitLength,8);
  I=0;
  while (I<TableSize)
  {
    if (InAddr>ReadTop-5)
      if (!UnpReadBuf())
        return(false);
    int Number=DecodeNumber(8);
    if (Number<16)
    {
      Table[I]=(Number+UnpOldTable20[I]) & 0xf;
      I++;
    }
    else
      if (Number==16)
      {
        N=(getbits() >> 14)+3;
        addbits(2);
        while (N-- > 0 && I<TableSize)
        {
          Table[I]=Table[I-1];
          I++;
        }
      }
      else
      {
        if (Number==17)
        {
          N=(getbits() >> 13)+3;
          addbits(3);
        }
        else
        {
          N=(getbits() >> 9)+11;
          addbits(7);
        }
        while (N-- > 0 && I<TableSize)
          Table[I++]=0;
      }
  }
  if (InAddr>ReadTop)
    return(true);
  if (UnpAudioBlock)
    for (I=0;I<UnpChannels;I++)
      MakeDecodeTables(&Table[I*MC20],9+I);
  else
  {
    MakeDecodeTables(&Table[0],5);
    MakeDecodeTables(&Table[NC20],6);
    MakeDecodeTables(&Table[NC20+DC20],7);
  }
  memcpy(UnpOldTable20,Table,sizeof(UnpOldTable20));
  return(true);
}


void ReadLastTables()
{
  if (ReadTop>=InAddr+5)
    if (UnpAudioBlock)
    {
      if (DecodeNumber(9+UnpCurChannel)==256)
        ReadTables20();
    }
    else
      if (DecodeNumber(5)==269)
        ReadTables20();
}


void UnpInitData20(int Solid)
{
  if (!Solid)
  {
    UnpChannelDelta=UnpCurChannel=0;
    UnpChannels=1;
    memset(AudV,0,sizeof(AudV));
    memset(UnpOldTable20,0,sizeof(UnpOldTable20));
  }
}


char DecodeAudio(int Delta)
{
//  struct AudioVariables *V=&AudV[UnpCurChannel];
  AudV[UnpCurChannel].ByteCount++;
  AudV[UnpCurChannel].D4=AudV[UnpCurChannel].D3;
  AudV[UnpCurChannel].D3=AudV[UnpCurChannel].D2;
  AudV[UnpCurChannel].D2=AudV[UnpCurChannel].LastDelta-AudV[UnpCurChannel].D1;
  AudV[UnpCurChannel].D1=AudV[UnpCurChannel].LastDelta;
  int PCh=8*AudV[UnpCurChannel].LastChar+AudV[UnpCurChannel].K1*AudV[UnpCurChannel].D1+AudV[UnpCurChannel].K2*AudV[UnpCurChannel].D2+AudV[UnpCurChannel].K3*AudV[UnpCurChannel].D3+AudV[UnpCurChannel].K4*AudV[UnpCurChannel].D4+AudV[UnpCurChannel].K5*UnpChannelDelta;
  PCh=(PCh>>3) & 0xFF;

  int Ch=PCh-Delta;

  int D=((signed char)Delta)<<3;

  AudV[UnpCurChannel].Dif[0]+=abs(D);
  AudV[UnpCurChannel].Dif[1]+=abs(D-AudV[UnpCurChannel].D1);
  AudV[UnpCurChannel].Dif[2]+=abs(D+AudV[UnpCurChannel].D1);
  AudV[UnpCurChannel].Dif[3]+=abs(D-AudV[UnpCurChannel].D2);
  AudV[UnpCurChannel].Dif[4]+=abs(D+AudV[UnpCurChannel].D2);
  AudV[UnpCurChannel].Dif[5]+=abs(D-AudV[UnpCurChannel].D3);
  AudV[UnpCurChannel].Dif[6]+=abs(D+AudV[UnpCurChannel].D3);
  AudV[UnpCurChannel].Dif[7]+=abs(D-AudV[UnpCurChannel].D4);
  AudV[UnpCurChannel].Dif[8]+=abs(D+AudV[UnpCurChannel].D4);
  AudV[UnpCurChannel].Dif[9]+=abs(D-UnpChannelDelta);
  AudV[UnpCurChannel].Dif[10]+=abs(D+UnpChannelDelta);

  UnpChannelDelta=AudV[UnpCurChannel].LastDelta=(signed char)(Ch-AudV[UnpCurChannel].LastChar);
  AudV[UnpCurChannel].LastChar=Ch;

  if ((AudV[UnpCurChannel].ByteCount & 0x1F)==0)
  {
    int MinDif=AudV[UnpCurChannel].Dif[0],NumMinDif=0;
    AudV[UnpCurChannel].Dif[0]=0;
    for (int I=1;I<sizeof(AudV[UnpCurChannel].Dif)/sizeof(AudV[UnpCurChannel].Dif[0]);I++)
    {
      if (AudV[UnpCurChannel].Dif[I]<MinDif)
      {
        MinDif=AudV[UnpCurChannel].Dif[I];
        NumMinDif=I;
      }
      AudV[UnpCurChannel].Dif[I]=0;
    }
    switch(NumMinDif)
    {
      case 1:
        if (AudV[UnpCurChannel].K1>=-16)
          AudV[UnpCurChannel].K1--;
        break;
      case 2:
        if (AudV[UnpCurChannel].K1<16)
          AudV[UnpCurChannel].K1++;
        break;
      case 3:
        if (AudV[UnpCurChannel].K2>=-16)
          AudV[UnpCurChannel].K2--;
        break;
      case 4:
        if (AudV[UnpCurChannel].K2<16)
          AudV[UnpCurChannel].K2++;
        break;
      case 5:
        if (AudV[UnpCurChannel].K3>=-16)
          AudV[UnpCurChannel].K3--;
        break;
      case 6:
        if (AudV[UnpCurChannel].K3<16)
          AudV[UnpCurChannel].K3++;
        break;
      case 7:
        if (AudV[UnpCurChannel].K4>=-16)
          AudV[UnpCurChannel].K4--;
        break;
      case 8:
        if (AudV[UnpCurChannel].K4<16)
          AudV[UnpCurChannel].K4++;
        break;
      case 9:
        if (AudV[UnpCurChannel].K5>=-16)
          AudV[UnpCurChannel].K5--;
        break;
      case 10:
        if (AudV[UnpCurChannel].K5<16)
          AudV[UnpCurChannel].K5++;
        break;
    }
  }
  return(Ch);
}


int fread32(int fh, char * buf, int len)
{
  int clen;
  int rlen;
  int total=0;
  while(len)
  {
    if (len>16384) clen=16384; else clen=len;
    total+=(rlen=fread(fh, buf, clen));
    if (rlen!=clen) break;
    buf+=rlen;
    len-=clen;
  }
  return(total);
}

int fwrite32(int fh, char * buf, int len)
{
  int clen;
  int rlen;
  int total=0;
  while(len)
  {
    if (len>16384) clen=16384; else clen=len;
    total+=(rlen=fwrite(fh, buf, clen));
    if (rlen!=clen) break;
    buf+=rlen;
    len-=clen;
  }
  return(total);
}

int UnpRead(char *Addr,int Count)
{
  int i;
  i=fread32(in,Addr,Count);
  if(i<0) return -1;
  return i;
}


void UnpWrite(char *Addr,int Count)
{
  fwrite32(outhandle,Addr,Count);
}

void prepname(wchar_t *file)
{
  FSTAT fst;
  wchar_t *filename = file;
  do{
    while(*++file && *file!='/');
    if(!*file) break;
    *file=0;
    if(!fstat(filename,0,&fst));
    else mkdir(filename);
    *file='/';
  }while(1);
}

void bi8t2unicode(wchar_t *ws, char *s, int len) //(c) Joker XT
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c=='\\') c = '/';
    else if (c==0xF1) c=0x451;
    else if (c==0xF0) c=0x401;
    else if ((c>=0x80)&&(c<0xB0)) c+=0x390;
    else if ((c>=0xE0)&&(c<0xF0)) c+=0x360;
    *ws++=c;
  }
  *ws=0;
}

RAR_INFO_HEADER * RAR_ReadInfoHeader(int file, int file_pos)
{
  #define data(__n__) (data[__n__]&0xff)
  char data[0x20];
  lseek(file,file_pos,SEEK_SET);
  if(fread(file,data,0x20)!=0x20) return 0;
  RAR_INFO_HEADER * header = new RAR_INFO_HEADER;
  header->header_type = data(2);
  header->flags = data(3) + (data(4)<<8);
  header->header_size = data(5) + (data(6)<<8);
  header->compressed_size = data(7) + (data(8)<<8) + (data(9)<<16) + (data(10)<<24);
  header->real_size = data(11) + (data(12)<<8) + (data(13)<<16) + (data(14)<<24);
  header->version = data(24);
  header->method = data(25);
  header->name_size = data(26) + (data(27)<<8);
  header->attributes = data(28);

  int len = header->name_size;
  char * c_name = new char[len+1];
  if(fread(file,c_name,len) != len)
  {
    delete c_name;
    delete header;
    return 0;
  }
  header->name = new wchar_t[len+1];
  bi8t2unicode(header->name, c_name, len);
  delete c_name;
  return header;
}
int RAR_DestroyInfoHeader(RAR_INFO_HEADER * header)
{
  if (header)
  {
    if (header->name) delete header->name;
    delete header;
    return 1;
  }
  return 0;
}

int RAR_ReadName(int file, wchar_t * name, int len)
{
  char * data = new char[len+1];
  if(fread(file,data,len) != len)
  {
    delete data;
    return 0;
  }
  bi8t2unicode(name, data, len);
  delete data;
  return 1;
}
int RAR_ReadMainHeader(int file)
{
  char header[8];
  lseek(file,0,SEEK_SET);
  if(fread(file,header,7)==7  && header[0]==0x52 && header[1]==0x61 && header[2]==0x72)
  {
    return 1;
  }
  return 0;
}
int qwer=0;
int ExtractOne(int file, int file_pos, wchar_t * new_name)
{
  //читаем заголовок, рар ли это?
  if(!RAR_ReadMainHeader(file))
  {
    return 0;
  }
  //читаем заголовок для данных (файл/папка)
  RAR_INFO_HEADER * header = RAR_ReadInfoHeader(file, file_pos);
  //если не удалось, то выхдим
  if (!header)
  {
    return 0;
  }
  //если это папка, то создали
  if(header->attributes & 0x10)
  {
    mkdir(new_name);
  }
  //если файл
  else
  {
    //открываем файл для записи
    if((outhandle=fopen(new_name,FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE))!=-1)
    {
      //перешли к содержимому
      lseek(file, file_pos + header->header_size, SEEK_SET);
      //передали реальный размер файла в глобальную переменную
      DestUnpSize=header->real_size;
      if (!qwer) { qwer = 1; UnpackInit();}
      //если файл сжат то распаковываем
      if(header->real_size!=header->compressed_size && header->real_size>0)
      {
        in = file;
        DoUnpack(header->version,(header->flags & 0x10)!=0);
      }
      //иначе просто читаем данные в файл
      else
      {
        int i;
        int real_size = header->real_size;
        while(real_size>0)
        {
          i=(real_size<MAX_SIZE)?real_size:MAX_SIZE;
          fread32(file,InBuf,i);
          fwrite32(outhandle,InBuf,i);
          real_size-=i;
        }
      }
      fclose(outhandle);
    }
  }
  //подсчитали размер блока = заголовок + даные
  int block_size = header->header_size + header->compressed_size;
  //прибрали заголовок
  RAR_DestroyInfoHeader(header);
  return block_size;
}

void elf_exit(void)
{
  if(Window!=0) mfree(Window);
  if(InBuf!=0) mfree(InBuf);
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

