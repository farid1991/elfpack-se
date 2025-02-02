#ifndef _PHONE_H_
#define _PHONE_H_

enum phone_t {
    UNKNOWN      = 0,
    phoneStart,
    W710_R1JC002 = phoneStart,
    W300_R4EA031,
    K750_R1CA021,
    W800_R1BD001,
    phoneEnd,
    phoneStartDB3150 = phoneEnd,
    K850_R1FA035 = phoneStartDB3150,
    C702_R3EF001,
    W595_R3EF001,
    W760_R3EF001,
    W890_R1FA035,
    W910_R1FA037,
    phoneEndDB3150,
    phoneStartDB3200 = phoneEndDB3150,
    J105_R1HA035 = phoneStartDB3200,
    phoneEndDB3200,
    phoneStartDB3210 = phoneEndDB3200,
    C510_R1HA035 = phoneStartDB3210,
    C901_R1GA028,
    C905_R1FA035,
    W705_R1GA031,
    W715_R1GA030,
    W995_R1GA026,
    phoneEndDB3210
};

typedef struct {
  phone_t type;
  char *addr;
  char *firmware;
  void *net;
}PhoneDB_t;

phone_t GetPhoneType();
void *GetNetInfoAddr(phone_t phoneType);

#endif //_PHONE_H_
