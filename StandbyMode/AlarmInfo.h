#ifndef _ALARMINFO_H_
  #define _ALARMINFO_H_

typedef struct
{
  char  type;
  char  days[7];
}WEEK;

int Alarm_GetCurrentAlarmTime(TIME *AlarmTime);

void ShowAlarmInfo(int y, int location);
void UpdateAlarmInfoParams(void);
void UpdateAlarmShowParams(void);
void FreeAlarmInfoParams(void);

#endif //_ALARMINFO_H_
