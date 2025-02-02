#ifndef HEADER_H
#define HEADER_H

union SIGNAL
{
  SIGSELECT signo;
};

struct DATACOUNTERS_SIGNAL {
  SIGSELECT signo;
  int unk; //0x4
  u16 unk2; //0x8
  u16 unk3; //0xA
  u16 unk4; //0xC
  u16 unk5; //0xE
  int connectiontime; //0x10
  int sendbytes;      //0x14
  int receivedbytes;  //0x18
};

#define EVTProcess_Kill_Sig 0x6FFF
#define EVTProcess_Kill_Response_Sig 0x7000

#endif
