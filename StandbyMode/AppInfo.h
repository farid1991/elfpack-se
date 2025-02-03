#ifndef _APPINFO_H_
  #define _APPINFO_H_

int onRootListChanged(void * r0, BOOK * bk);

void ShowAppInfo(int y, int location);
void UpdateAppInfoParams(void);
void UpdateAppShowParams(void);
void FreeAppInfoImages(void);
void FreeAppInfoParams(void);

#endif //_APPINFO_H_
