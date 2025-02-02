#ifndef _BATTTEMPINFO_H_
  #define _BATTTEMPINFO_H_

#define BATT_IMAGE_COUNT 11

int onBatteryCapacityChanged(void* mess,BOOK* b);
int onBatteryChargerConnected(void* mess,BOOK* b);
int onBatteryChargerDisconnected(void* mess,BOOK* b);
int onBatteryChargingState(void* mess,BOOK* b);

void ShowBattTempInfo(int y, int location);
void ShowNonStandbyBattTempInfo(int y, int location);
void UpdateBattTempInfo(void);
void UpdateBattTempInfoParams(void);
void UpdateBattTempShowParams(void);
void FreeBattTempInfoImages(void);
void FreeBattTempInfoParams(void);

#endif //_BATTTEMPINFO_H_
