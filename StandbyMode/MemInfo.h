#ifndef _MEMINFO_H_
  #define _MEMINFO_H_

enum meminfo_t {
    NOT_SHOW = 0,
    FREE_PERCENT,
    FREE_MB,
    FREE_GB,
    USED_PERCENT,
    USED_MB,
    USED_GB
};

enum fractional_t {
    ZERO_CHAR = 0,
    ONE_CHAR,
    TWO_CHAR
};

void ShowMemInfo(int y, int location);
void UpdateMemInfoParams(void);
void UpdateMemShowParams(void);
void FreeMemInfoParams(void);

#endif //_MEMINFO_H_
