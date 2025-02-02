#include "..\include\Lib_Clara.h"
#include "phone.h"

#define W710_R1JC002_ADDR (char *)0x459A6D3D
#define W300_R4EA031_ADDR (char *)0x448CA6BD
#define K750_R1CA021_ADDR (char *)0x44670436
#define W800_R1BD001_ADDR (char *)0x4467DFB2

//DB3150
#define K850_R1FA035_ADDR (char *)0x120C7520
#define K850_R1FA035_NET (void *)0x29C51362
#define C702_R3EF001_ADDR (char *)0x1205C922
#define C702_R3EF001_NET (void *)0x2AF91378
#define W595_R3EF001_ADDR (char *)0x11FB40C6
#define W595_R3EF001_NET (void *)0x4BB90DE6
#define W760_R3EF001_ADDR (char *)0x1205A857
#define W760_R3EF001_NET (void *)0x4BB91582
#define W890_R1FA035_ADDR (char *)0x120A4943
#define W890_R1FA035_NET (void *)0x2A252AFE
#define W910_R1FA037_ADDR (char *)0x120F9D78
#define W910_R1FA037_NET (void *)0x2A24BBDE
//DB3200
#define J105_R1HA035_ADDR (char *)0x15BCF2E3
#define J105_R1HA035_NET (void *)0x4BA12F34
//DB3210
#define C510_R1HA035_ADDR (char *)0x15CBE142
#define C510_R1HA035_NET (void *)0x4BA13974
#define C901_R1GA028_ADDR (char *)0x15CA2369
#define C901_R1GA028_NET (void *)0x4BA13A56
#define C905_R1FA035_ADDR (char *)0x15D8C04A
#define C905_R1FA035_NET (void *)0x4BA11E8E
#define W705_R1GA031_ADDR (char *)0x15C9CE75
#define W705_R1GA031_NET (void *)0x4BA134BA
#define W715_R1GA030_ADDR (char *)0x15D3584D
#define W715_R1GA030_NET (void *)0x4BA134DA
#define W995_R1GA026_ADDR (char *)0x15DD8AC1
#define W995_R1GA026_NET (void *)0x4BA13E9E


PhoneDB_t PhoneDB[phoneEndDB3210] = {
  {UNKNOWN, 0, "0000000"},
  {W710_R1JC002, W710_R1JC002_ADDR, "R1JC002", 0},
  {W300_R4EA031, W300_R4EA031_ADDR, "R4EA031", 0},
  {K750_R1CA021, K750_R1CA021_ADDR, "R1CA021", 0},
  {W800_R1BD001, W800_R1BD001_ADDR, "R1BD001", 0},

  {K850_R1FA035, K850_R1FA035_ADDR, "R1FA035", K850_R1FA035_NET},
  {C702_R3EF001, C702_R3EF001_ADDR, "R3EF001", C702_R3EF001_NET},
  {W595_R3EF001, W595_R3EF001_ADDR, "R3EF001", W595_R3EF001_NET},
  {W760_R3EF001, W760_R3EF001_ADDR, "R3EF001", W760_R3EF001_NET},
  {W890_R1FA035, W890_R1FA035_ADDR, "R1FA035", W890_R1FA035_NET},
  {W910_R1FA037, W910_R1FA037_ADDR, "R1FA037", W910_R1FA037_NET},

  {J105_R1HA035, J105_R1HA035_ADDR, "R1HA035", J105_R1HA035_NET},

  {C510_R1HA035, C510_R1HA035_ADDR, "R1HA035", C510_R1HA035_NET},
  {C901_R1GA028, C901_R1GA028_ADDR, "R1GA028", C901_R1GA028_NET},
  {C905_R1FA035, C905_R1FA035_ADDR, "R1FA035", C905_R1FA035_NET},
  {W705_R1GA031, W705_R1GA031_ADDR, "R1GA031", W705_R1GA031_NET},
  {W715_R1GA030, W715_R1GA030_ADDR, "R1GA030", W715_R1GA030_NET},
  {W995_R1GA026, W995_R1GA026_ADDR, "R1GA026", W995_R1GA026_NET},
};

phone_t GetPhoneType()
{
  int start_indx = phoneStart, end_indx = phoneEnd;

  switch(GetChipID()&CHIPID_MASK)
  {
      case CHIPID_DB3150:
           start_indx = phoneStartDB3150;
           end_indx   = phoneEndDB3150;
           break;
      case CHIPID_DB3200:
           start_indx = phoneStartDB3200;
           end_indx   = phoneEndDB3200;
           break;
      case CHIPID_DB3210:
           start_indx = phoneStartDB3210;
           end_indx   = phoneEndDB3210;
           break;
  }

  char tmp[8];
  for(int i=start_indx; i<end_indx; i++)
  {
    if(PhoneDB[i].addr)
    {
      memcpy(tmp,PhoneDB[i].addr,7);
      tmp[7] = 0;
      if(strcmp(tmp,PhoneDB[i].firmware) == 0)
      {
        return PhoneDB[i].type;
      }
    }
  }
  return UNKNOWN;
}

void *GetNetInfoAddr(phone_t phoneType)
{
  if(phoneType < phoneEndDB3210)
  {
    for(int i=phoneStart; i<phoneEndDB3210; i++)
    {
      if(PhoneDB[i].type == phoneType)
      {
        if(PhoneDB[i].net)
          return PhoneDB[i].net;
        else
          return Find_StandbyBook();
      }
    }
  }

  return 0;
}
