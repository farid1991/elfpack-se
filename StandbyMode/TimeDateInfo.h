#ifndef _TIMEDATEINFO_H_
  #define _TIMEDATEINFO_H_

extern const int cfg_clock2_nonstandby_show;

void ShowTimeDateInfo(int y, int location);
void ShowNonStandbyTimeInfo(int y, int location);
void UpdateTimeDateShowParams(void);
void UpdateTimeDateInfoParams(void);
void FreeTimeDateInfoParams(void);

#endif //_TIMEDATEINFO_H_
