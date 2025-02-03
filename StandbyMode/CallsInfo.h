#ifndef _CALLSINFO_H_
  #define _CALLSINFO_H_

int onCallManager(void * CallManStruct, BOOK *);
void ShowCallsInfo(int y, int location);
void UpdateCallsInfoParams(void);
void UpdateCallsShowParams(void);
void FreeCallsInfoParams(void);

#endif //_CALLSINFO_H_
