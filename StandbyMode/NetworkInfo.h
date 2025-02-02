#ifndef _NETWORKINFO_H_
  #define _NETWORKINFO_H_

#define NET_IMAGE_COUNT 6

typedef struct
{
  char unk1[0xA1];
  char Level;
  wchar_t CurrentIconID;
  char TypeActive;
}STANDBY_K750_R1CA021;

typedef struct
{
  char unk1[0xB5];        //
  char Level;             //0xB5
  wchar_t CurrentIconID;  //0xB6
  char TypeActive;        //0xB8 2-Active 3-Not Aсtive/ не 2 не 3 - usual
  char unk2[3];           //
  char isFlightNotActive; //0xBC 0-active flight 1-not active
  char Type;              //0xBD 2-Edge not 2-gprs
}STANDBY_W300_R4EA031;

typedef struct
{
  char unk1[0xC1];         //
  char Level;              //0xC1 5-5 4-4 3-3 2-2 1-1 0-0
  wchar_t CurrentIconID;   //0xC2
  char TypeActive;         //0xC4 2-Active 3-Not Aсtive/ не 2 не 3 - usual
  char Type;               //0xC5 1-gprs 2-edge/не 1 не 2 какая и была
  char unk2[2];            //
  char isFlightNotActive;  //0xC8 0-active 1-not active
}STANDBY_W710_R1JC002;

typedef struct
{
  char unk1[0xA1];         //
  char Level;              //0xA1 5-5 4-4 3-3 2-2 1-1 0-0
  wchar_t CurrentIconID;   //0xA2
  char TypeActive;         //0xA4 2-Active 3-Not Aсtive/ не 2 не 3 - usual
  char Type;               //0xA5 1-gprs 2-edge/не 1 не 2 какая и была
  char unk2[2];            //
  char isFlightNotActive;  //0xA8 0-active 1-not active
}STANDBY_DB2020;

typedef struct
{
  char unk1[0x99];        //
  char Level;             //0x99
  wchar_t CurrentIconID;  //0x9A
  char TypeActive;        //0x9С
  char unk2[3];           //
  char isFlightNotActive; //0xA0 0-active flight 1-not active
  char Type;              //0xA1 2-Edge not 2-gprs
}STANDBY_DB2010;

typedef struct
{
char unk[0x2];
char Level; //5-5 4-4 3-3 2-2 1-1 0-0
char Type; //01-gprs 2-edge/ia 1 ia 2 eaeay e auea
char TypeActive; //2-Active 3-Not Antive/ ia 2 ia 3 - usual
char unk1[0x1];
wchar_t CurrentIconID; //Oaeouay eeiiea
}STANDBY_DBA2;

typedef union
{
  STANDBY_K750_R1CA021 *K750;
  STANDBY_W300_R4EA031 *W300;
  STANDBY_W710_R1JC002 *W710;
  STANDBY_DB2010       *DB2010;
  STANDBY_DB2020       *DB2020;
  STANDBY_DBA2         *DBA2;
  void                 *addr;
}NetInfoAddr_t;

extern NetInfoAddr_t NetInfoAddr;

void ShowNetworkInfo(int y, int location);
void ShowNonStandbyNetworkInfo(int y, int location);
void UpdateNetworkInfoParams(void);
void UpdateNetworkShowParams(void);
void FreeNetworkInfoImages(void);
void FreeNetworkInfoParams(void);

#endif //_NETWORKINFO_H_
