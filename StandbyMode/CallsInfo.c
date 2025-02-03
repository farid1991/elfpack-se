#include "..\include\Lib_Clara.h"
#include "CallsInfo.h"
#include "MediaInfo.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"

enum
{
    CALLMANAGER_IDLE            =0,
    CALLMANAGER_CALL_CONNECTED  =1, /*Поднятие трубки*/
    CALLMANAGER_CALL_SETUP      =2, /*Нажатие вызвать*/
    CALLMANAGER_CALL_ALERT      =4, /*Входящий вызов*/
    CALLMANAGER_CALL_WAITING    =5,
    CALLMANAGER_CALL_TERMINATED =6  /*Завершение соединения*/
};

typedef struct
{
  wchar_t * PNUM;
  wchar_t * Name;
  int first_callstate;
  int last_callstate;
  DATETIME * startdatetime;
  DATETIME * begindatetime;
  char line;
}myList_elem;

LIST * myList = 0;

TEXTID callID = EMPTY_TEXTID;
u16 timer_callInfo = 0;
int call_show = 0;

IMAGEID call_img = NOIMAGE;

int call_height = 0;
int call_width = 0;
unsigned int call_x = 0;
unsigned int call_y = 0;
unsigned int call_img_x = 0;
unsigned int call_img_y = 0;


//Функция поиска элемента в листе звонков
int myList_Find(void *r0,void *r1)
{
  if (((myList_elem*)r0)->line==((myList_elem*)r1)->line) return(0);
  return(1);
}

//Убийство моего элемента
void myList_elem_Free(void * r)
{
  myList_elem * r0=(myList_elem *)r;
  if (r0)
  {
    if (r0->PNUM)
    {
      delete(r0->PNUM);
    }
    if (r0->Name)
    {
      delete(r0->Name);
    }
    if (r0->startdatetime)
    {
      delete(r0->startdatetime);
    }
    if (r0->begindatetime)
    {
      delete(r0->begindatetime);
    }
    delete(r0);
  }
}

//Фильтр для убийства элементов при убийстве листа (всех ненулевых)
int myList_elem_Filter(void * r0)
{
  if (r0) return(1);
  return(0);
}

void onTimer_callInfo (u16 timerID, LPARAM lparam)
{
  if(callID != EMPTY_TEXTID)
  {
    TextID_Destroy(callID);
    callID = EMPTY_TEXTID;
  }
}

int CallManager(void * CallManStruct)
{
  if(cfg_call_show == 0) return(1);

  //Ловим всех кроме idle
  if (((CALLMANAGER_EVENT_DATA*)CallManStruct)->CallState)
  {
    //Создаем структурку
    myList_elem* temp_elem = new(myList_elem);
    int length = PNUM_len(((CALLMANAGER_EVENT_DATA*)CallManStruct)->PNUM);
    //Проверяем наличие элемента в листе
    if (((CALLMANAGER_EVENT_DATA*)CallManStruct)->CallID == 0xFF)
    {
      if (myList->FirstFree)
      {
        temp_elem->line = 1;
      }
      else
      {
        temp_elem->line = 0;
      }
    }
    else
    {
      temp_elem->line = ((CALLMANAGER_EVENT_DATA*)CallManStruct)->CallID;
    }
    temp_elem->PNUM = new wchar_t[length + 1];
    char * sp = new char[length + 1];
    memset(sp, 0, length + 1);
    PNUM2str(sp, ((CALLMANAGER_EVENT_DATA*)CallManStruct)->PNUM,length,length+1);
    str2wstr(temp_elem->PNUM, sp);
    delete(sp);
    if (ListElement_Find(myList,temp_elem,myList_Find) == LIST_ERROR)
    {
      temp_elem->startdatetime = new DATETIME;
      REQUEST_DATEANDTIME_GET(SYNC,temp_elem->startdatetime);
      temp_elem->first_callstate = ((CALLMANAGER_EVENT_DATA*)CallManStruct)->CallState;
      temp_elem->last_callstate  =   CALLMANAGER_IDLE;
      wchar_t * CallName = CallStatusDesc_GetName(CallID_GetCallStatusDesc(temp_elem->line));
      if (CallName)
      {
        if (CallName[0])
        {
          temp_elem->Name = new wchar_t[wstrlen(CallName+1)];
          wstrcpy(temp_elem->Name, CallName+2);
        }
        else
        {
          temp_elem->Name = new wchar_t[14];
          temp_elem->Name[0] = 0;
        }
      }
      else
      {
        temp_elem->Name = new wchar_t[20];
        temp_elem->Name[0] = 0;
        wstrcpy(temp_elem->Name,L"Emergency");
      }

      temp_elem->begindatetime = new DATETIME;
      temp_elem->begindatetime->time.hour = 0;
      temp_elem->begindatetime->time.min  = 0;
      temp_elem->begindatetime->time.sec  = 0;
      ListElement_AddtoTop(myList,temp_elem);
    }
    else
    {
      if (((CALLMANAGER_EVENT_DATA*)CallManStruct)->CallState == CALLMANAGER_CALL_TERMINATED)
      {
        DATETIME * enddatetime = new DATETIME;
        REQUEST_DATEANDTIME_GET(SYNC,enddatetime);
        myList_elem * elem = (myList_elem*)ListElement_Remove(myList,ListElement_Find(myList,temp_elem,myList_Find));
        int sub;
        int m;
        int s;
        if (!elem->Name[0])
        {
          snwprintf(elem->Name,8,L"NoName");
        }
        if (elem->last_callstate == CALLMANAGER_IDLE)
        {
          m = 0;
          s = 0;
        }
        else
        {
          //Если вызов начался в 23, а закончился в 00, меняем 00 на 24
          if ((elem->begindatetime->time.hour == 23) && (enddatetime->time.hour == 0)) enddatetime->time.hour = 24;
          //Получаем время разговора в секундах
          sub = (enddatetime->time.hour-elem->begindatetime->time.hour)*3600 + (enddatetime->time.min-elem->begindatetime->time.min)*60 + enddatetime->time.sec-elem->begindatetime->time.sec;
          //Выделяем минуты и секунды
          m = sub/60;
          s = sub%60;
        }
        wchar_t call_msg[32];
        snwprintf(call_msg,MAXELEMS(call_msg)-1,L"%02d:%02d", m, s);
        if(callID != EMPTY_TEXTID) TextID_Destroy(callID);
        callID = TextID_Create(call_msg,ENC_UCS2,MAXELEMS(call_msg));
        Timer_ReSet(&timer_callInfo,cfg_call_show*1000,onTimer_callInfo,0);

        delete(enddatetime);
        myList_elem_Free(elem);
      }
      if (((CALLMANAGER_EVENT_DATA*)CallManStruct)->CallState == CALLMANAGER_CALL_CONNECTED)
      {
        myList_elem * elem=(myList_elem*)ListElement_GetByIndex(myList,ListElement_Find(myList,temp_elem,myList_Find));
        if (elem->last_callstate != CALLMANAGER_CALL_CONNECTED)
        {
          elem->last_callstate = CALLMANAGER_CALL_CONNECTED;
          elem->begindatetime=new DATETIME;
          REQUEST_DATEANDTIME_GET(SYNC,elem->begindatetime);
        }
      }
      //Убиваем временный элемент если не засунули в лист
      delete(temp_elem->PNUM);
      delete(temp_elem);
    }
  }

  return(1);
}

int CallManagerA2(void * CallManStruct)
{
  if(cfg_call_show == 0) return(1);

  //Ловим всех кроме idle
  if (((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->CallState)
  {
    //Создаем структурку
    myList_elem* temp_elem = new(myList_elem);
    int length = PNUM_len(((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->PNUM);
    //Проверяем наличие элемента в листе
    if (((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->CallID == 0xFF)
    {
      if (myList->FirstFree)
      {
        temp_elem->line = 1;
      }
      else
      {
        temp_elem->line = 0;
      }
    }
    else
    {
      temp_elem->line = ((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->CallID;
    }
    temp_elem->PNUM = new wchar_t[length + 1];
    char * sp = new char[length + 1];
    memset(sp, 0, length + 1);
    PNUM2str(sp, ((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->PNUM,length,length+1);
    str2wstr(temp_elem->PNUM, sp);
    delete(sp);
    if (ListElement_Find(myList,temp_elem,myList_Find) == LIST_ERROR)
    {
      temp_elem->startdatetime = new DATETIME;
      REQUEST_DATEANDTIME_GET(SYNC,temp_elem->startdatetime);
      temp_elem->first_callstate = ((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->CallState;
      temp_elem->last_callstate  =   CALLMANAGER_IDLE;
      wchar_t * CallName = CallStatusDesc_GetName(CallID_GetCallStatusDesc(temp_elem->line));
      if (CallName)
      {
        if (CallName[0])
        {
          temp_elem->Name = new wchar_t[wstrlen(CallName+1)];
          wstrcpy(temp_elem->Name, CallName+2);
        }
        else
        {
          temp_elem->Name = new wchar_t[14];
          temp_elem->Name[0] = 0;
        }
      }
      else
      {
        temp_elem->Name = new wchar_t[20];
        temp_elem->Name[0] = 0;
        wstrcpy(temp_elem->Name,L"Emergency");
      }

      temp_elem->begindatetime = new DATETIME;
      temp_elem->begindatetime->time.hour = 0;
      temp_elem->begindatetime->time.min  = 0;
      temp_elem->begindatetime->time.sec  = 0;
      ListElement_AddtoTop(myList,temp_elem);
    }
    else
    {
      if (((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->CallState == CALLMANAGER_CALL_TERMINATED)
      {
        DATETIME * enddatetime = new DATETIME;
        REQUEST_DATEANDTIME_GET(SYNC,enddatetime);
        myList_elem * elem = (myList_elem*)ListElement_Remove(myList,ListElement_Find(myList,temp_elem,myList_Find));
        int sub;
        int m;
        int s;
        if (!elem->Name[0])
        {
          snwprintf(elem->Name,8,L"NoName");
        }
        if (elem->last_callstate == CALLMANAGER_IDLE)
        {
          m = 0;
          s = 0;
        }
        else
        {
          //Если вызов начался в 23, а закончился в 00, меняем 00 на 24
          if ((elem->begindatetime->time.hour == 23) && (enddatetime->time.hour == 0)) enddatetime->time.hour = 24;
          //Получаем время разговора в секундах
          sub = (enddatetime->time.hour-elem->begindatetime->time.hour)*3600 + (enddatetime->time.min-elem->begindatetime->time.min)*60 + enddatetime->time.sec-elem->begindatetime->time.sec;
          //Выделяем минуты и секунды
          m = sub/60;
          s = sub%60;
        }
        wchar_t call_msg[32];
        snwprintf(call_msg,MAXELEMS(call_msg)-1,L"%02d:%02d", m, s);
        if(callID != EMPTY_TEXTID) TextID_Destroy(callID);
        callID = TextID_Create(call_msg,ENC_UCS2,MAXELEMS(call_msg));
        Timer_ReSet(&timer_callInfo,cfg_call_show*1000,onTimer_callInfo,0);

        delete(enddatetime);
        myList_elem_Free(elem);
      }
      if (((CALLMANAGER_EVENT_DATA_A2*)CallManStruct)->CallState == CALLMANAGER_CALL_CONNECTED)
      {
        myList_elem * elem=(myList_elem*)ListElement_GetByIndex(myList,ListElement_Find(myList,temp_elem,myList_Find));
        if (elem->last_callstate != CALLMANAGER_CALL_CONNECTED)
        {
          elem->last_callstate = CALLMANAGER_CALL_CONNECTED;
          elem->begindatetime=new DATETIME;
          REQUEST_DATEANDTIME_GET(SYNC,elem->begindatetime);
        }
      }
      //Убиваем временный элемент если не засунули в лист
      delete(temp_elem->PNUM);
      delete(temp_elem);
    }
  }

  return(1);
}

int onCallManager(void * CallManStruct, BOOK *)
{
   switch (ChipID)
   {
   case CHIPID_DB2000:
   case CHIPID_DB2010:
   case CHIPID_DB2020:
                 CallManager(CallManStruct);
                 break;
   case CHIPID_DB3150:
   case CHIPID_DB3200:
   case CHIPID_DB3210:
   case CHIPID_DB3350:
                 CallManagerA2(CallManStruct);
                 break;
   }
   return 1;
}

void ShowCallsInfo(int y, int location)
{
   if (call_show)
   {
     if(callID != EMPTY_TEXTID)
     {
       if(cfg_call_align == RIGHT)
          myDrawString(cfg_call_font, callID, cfg_call_align, 1, call_y - y, call_x, DisplayHeight, cfg_call_border, cfg_call_color, cfg_call_highlight);
       else
          myDrawString(cfg_call_font, callID, cfg_call_align, call_x, call_y - y, DisplayWidth, DisplayHeight, cfg_call_border, cfg_call_color, cfg_call_highlight);

       if(cfg_call_icon_show && (call_img != NOIMAGE))
       {
         GC *GC_DISP=get_DisplayGC ();
         putchar(GC_DISP, call_img_x, call_img_y - y, call_height, call_height, call_img);
       }
     }
   }
}

void UpdateCallsShowParams(void)
{
   call_show = cfg_call_show && (!(PlayerIsStarted && cfg_call_not_show_if_player));
}

void FreeCallsImages(void)
{
  if(call_img != NOIMAGE)
  {
    ImageID_Free(call_img);
    call_img = NOIMAGE;
  }
}

void UpdateCallsInfoParams(void)
{
  if(myList == 0) myList=List_New();

    FSTAT _fstat;

    FreeCallsImages();
    if(callID != EMPTY_TEXTID)
    {
       TextID_Destroy(callID);
       callID = EMPTY_TEXTID;
    }

    if(cfg_call_show && cfg_call_icon_show)
    {
        extractdir(tmppath, tmpname, cfg_call_file);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          MessageBox(EMPTY_TEXTID, STR("Иконка для времени разговора не найдена"),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&call_img)<0) call_img = NOIMAGE;
    }

    int font_old;
    TEXTID strID;

    font_old = SetFont(cfg_call_font);
    strID = TextID_Create(L"00:00",ENC_UCS2,5);
    call_height = GetImageHeight(strID);
    call_width = Disp_GetTextIDWidth(strID,TextID_GetLength(strID));
    TextID_Destroy(strID);
    SetFont(font_old);

    if(cfg_call_icon_show)
    {
      if(cfg_call_align == RIGHT)
      {
        call_img_x = cfg_call_x - call_width - call_height - 1;
        call_x = cfg_call_x;
      }
      else if(cfg_call_align == LEFT)
      {
        call_img_x = cfg_call_x;
        call_x = cfg_call_x + call_height + 1;
      }
      else
      {
        call_img_x = cfg_call_x + ((DisplayWidth - call_width - call_height) >> 1) - 1;
        call_x = cfg_call_x + (call_height >> 1) + 2;
      }
      call_y = call_img_y = cfg_call_y;
    }
    else
    {
      call_x = cfg_call_x;
      call_y = cfg_call_y;
    }
}

void FreeCallsInfoParams(void)
{
  if(timer_callInfo)
  {
      Timer_Kill(&timer_callInfo);
      timer_callInfo = 0;
  }
  if(callID != EMPTY_TEXTID)
  {
     TextID_Destroy(callID);
     callID = EMPTY_TEXTID;
  }
  FreeCallsImages();

  List_FreeElements(myList,myList_elem_Filter,myList_elem_Free);
  List_Free(myList);
}


