#ifndef _STATUSINFO_H_
  #define _STATUSINFO_H_

int onSessionActive(void* mess,BOOK* b);
int onSessionActivity(void* mess,BOOK* b);
int onSessionSuspended(void* mess,BOOK* b);

int onBluetoothActivityChanged(void* mess,BOOK* b);
int onBluetoothStateChanged(void* mess,BOOK* b);

int onUSB_Connected(void* mess,BOOK* b);
int onUSB_DisConnected(void* mess,BOOK* b);

int onCHF_Connected(void* mess,BOOK* b);
int onCHF_DisConnected(void* mess,BOOK* b);

int onSmsSent(void* mess,BOOK* b);
int onSmsSending(void* mess,BOOK* b);
int onMmsSent(void* mess,BOOK* b);
int onMmsSending(void* mess,BOOK* b);

int onDownloadProgressHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16);
int onDownloadDoneHook(void *msg, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16);

void ShowStatusInfo(int y, int location);
void ShowNonStandbyStatusInfo(int y, int location);
void UpdateStatusInfoParams(void);
void UpdateStatusShowParams(void);
void FreeStatusInfoParams(void);

#endif //_STATUSINFO_H_
