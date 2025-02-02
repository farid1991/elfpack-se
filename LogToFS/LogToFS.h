#ifndef _REGEDITOR_H_
#define _REGEDITOR_H_

int (*MassStorage_isAble_w705)(char * isable)=(int(*)(char * isable))0x141F2CC5;
int (*PrintServer_LogToFS_GetState_w705)()=(int(*)())0x4BA9DBAD;
void (*PrintServer_LogToFS_SetState_w705)(int state)=(void(*)(int state))0x4BA9DB95;
//int (*MassStorage_isAble_w995)(char * isable)=(int(*)(char * isable))0x142F390D;
//int (*PrintServer_LogToFS_GetState_w995)()=(int(*)())0x4BA9ECC1;
//void (*PrintServer_LogToFS_SetState_w995)(int state)=(void(*)(int state))0x4BA9DB95;

int TerminateElf(void * ,BOOK * book);
int ShowAuthorInfo(void *mess ,BOOK * book);
int LogToFSBook_MainPage_Enter(void * r0,BOOK * book);

#define DUMP_PATH L"/tpa/user/other/"
#define DUMP_NAME L"ForceDumpOn.cfg"

#define SETTINGS1 "Off"
#define SETTINGS2 "Log to internal FS"
#define SETTINGS3 "Circular log"
#define SETTINGS4 "Log to Memstick"

#define DUMP_SETTINGS1 "Off "
#define DUMP_SETTINGS2 "On"

typedef enum
{
  REAL = 0,
  STRING = 1,
  INTEGER = 2,
  PHONE_NUMBER = 3,
  DIGITAL_PASS = 4,
  DIGITAL_IP = 5,
  URL = 6,
  UNSIGNED_DIGIT = 9
}SI_INPUT_MODE;


typedef struct
{
  BOOK * book;
}MSG;


typedef struct _MYBOOK : BOOK
{
  GUI_LIST * list_gui;
  GUI_ONEOFMANY * oofm_gui;
  GUI * stringinput_gui;
  char dump_state;
  int (*MassStorage_isAble)(char * isable);
  int (*PrintServer_LogToFS_GetState)();
  void (*PrintServer_LogToFS_SetState)(int state);
}LogToFSBook;



#endif
