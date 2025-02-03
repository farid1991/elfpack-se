#ifndef _PHONE_H_
#define _PHONE_H_

typedef struct {
  int type;
  char *addr;
  char *firmware;
}PhoneDB_t;

enum phone_t {
    UNKNOWN      = 0,
    K750_R1CA021,
    W800_R1BD001,
    W700_R1CA021,
    phoneNums
};

extern PhoneDB_t PhoneDB[phoneNums];

#endif //_PHONE_H_
