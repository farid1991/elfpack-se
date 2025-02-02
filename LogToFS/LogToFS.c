#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\include\var_arg.h"
#include "LogToFS.h"
#include "Pages.h"


void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


int TerminateElf(void * ,BOOK * book)
{
  FreeBook(book);
  return(1);
}


int ShowAuthorInfo(void *mess ,BOOK * book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("LogToFS Book, v1\n\n(c) IronMaster\n\n"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


void oofm_gui_dump_cancel(BOOK * book, GUI * gui)
{
  FreeBook(book);
}


void oofm_gui_dump_previous(BOOK * book, GUI * gui)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  GUIObject_Destroy(gui);
  LTFBook->oofm_gui=0;
}


void oofm_gui_dump_select(BOOK * book, GUI * gui)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  int selected=OneOfMany_GetSelected(gui);
  
  if (LTFBook->dump_state!=selected)
  {
    if (!selected) FileDelete(DUMP_PATH,DUMP_NAME,0);
    else fclose(_fopen(DUMP_PATH,DUMP_NAME,FSX_O_CREAT,FSX_S_IRUSR|FSX_S_IWUSR,0));
  }
  
  GUIObject_Destroy(gui);
  LTFBook->oofm_gui=0;
}


int oofm_gui_dump_callback(GUI_MESSAGE * msg)
{
  char * item_name=0;
  if (GUIonMessage_GetMsg(msg)==LISTMSG_GetItem)
  {
    switch(GUIonMessage_GetCreatedItemIndex(msg))
    {
    case 0:
      item_name=DUMP_SETTINGS1;
      break;
    case 1:
      item_name=DUMP_SETTINGS2;
      break;
    }
    GUIonMessage_SetMenuItemText(msg,TextID_Create(item_name,ENC_LAT1,TEXTID_ANY_LEN));
    return 1;
  }
  return 0;
}


void LogToFSBook_DumpSettings(BOOK * book)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  int active;
  
  if (fstat(DUMP_PATH,DUMP_NAME,0)>=0) active=1;
  else active=0;
  
  LTFBook->dump_state=active;
  
  LTFBook->oofm_gui=CreateOneOfMany(LTFBook);
  OneOfMany_SetItemCount(LTFBook->oofm_gui,2);
  GUIObject_SetTitleText(LTFBook->oofm_gui,TextID_Create("Dump Settings",ENC_LAT1,TEXTID_ANY_LEN));
  OneOfMany_SetChecked(LTFBook->oofm_gui,active);
  OneOfMany_SetOnMessage(LTFBook->oofm_gui,oofm_gui_dump_callback);
  GUIObject_SoftKeys_SetAction(LTFBook->oofm_gui,ACTION_SELECT1,oofm_gui_dump_select);
  GUIObject_SoftKeys_SetAction(LTFBook->oofm_gui,ACTION_BACK,oofm_gui_dump_previous);
  GUIObject_SoftKeys_SetAction(LTFBook->oofm_gui,ACTION_LONG_BACK,oofm_gui_dump_cancel);
  GUIObject_Show(LTFBook->oofm_gui);
}


void stringinput_gui_accept(BOOK * book, wchar_t * string, int len)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  char buf[61];
  wstr2strn(buf,string,len);
  debug_printf("\n\n-------- %s --------\n\n",buf);
  
  GUIObject_Destroy(LTFBook->stringinput_gui);
  LTFBook->stringinput_gui=0;
}


void stringinput_gui_cancel(BOOK * book)
{
  FreeBook(book);
}


void stringinput_gui_previous(BOOK * book)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  GUIObject_Destroy(LTFBook->stringinput_gui);
  LTFBook->stringinput_gui=0;
}


void LogToFSBook_MakeMarker(BOOK * book)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  LTFBook->stringinput_gui=CreateStringInputVA(0,
                                                 VAR_HEADER_TEXT( TextID_Create("Marker name",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_FIXED_TEXT( TextID_Create("log marker",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_TEXT (TextID_Create("Marker",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_MAX_LEN( 60 ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( STRING ),
                                                 VAR_BOOK( LTFBook ),
                                                 VAR_PREV_ACTION_PROC( stringinput_gui_previous ),
                                                 VAR_LONG_BACK_PROC( stringinput_gui_cancel ),
                                                 VAR_OK_PROC( stringinput_gui_accept ),
                                                 0 );
  GUIObject_Show(LTFBook->stringinput_gui);
}


void oofm_gui_cancel(BOOK * book, GUI * gui)
{
  FreeBook(book);
}


void oofm_gui_previous(BOOK * book, GUI * gui)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  GUIObject_Destroy(gui);
  LTFBook->oofm_gui=0;
}


void oofm_gui_select(BOOK * book, GUI * gui)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  char isable=0;
  int selected=0;
  
  selected=OneOfMany_GetSelected(gui);
  
  if (selected==2);
  {
    LTFBook->MassStorage_isAble(&isable);
    if (isable==0) selected=0;
  }
  
  LTFBook->PrintServer_LogToFS_SetState(selected);
  
  GUIObject_Destroy(gui);
  LTFBook->oofm_gui=0;
}


int oofm_gui_callback(GUI_MESSAGE * msg)
{
  char * item_name=0;
  if (GUIonMessage_GetMsg(msg)==LISTMSG_GetItem)
  {
    switch(GUIonMessage_GetCreatedItemIndex(msg))
    {
    case 0:
      item_name=SETTINGS1;
      break;
    case 1:
      item_name=SETTINGS2;
      break;
    case 2:
      item_name=SETTINGS3;
      break;
    case 3:
      item_name=SETTINGS4;
      break;
    }
    GUIonMessage_SetMenuItemText(msg,TextID_Create(item_name,ENC_LAT1,TEXTID_ANY_LEN));
    return 1;
  }
  return 0;
}


void LogToFSBook_Settings(BOOK * book)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  char isable=0;
  int active=0;
  
  LTFBook->oofm_gui=CreateOneOfMany(LTFBook);
  GUIObject_SetTitleText(LTFBook->oofm_gui,TextID_Create("Settings",ENC_LAT1,TEXTID_ANY_LEN));
  
  active=LTFBook->PrintServer_LogToFS_GetState();
  
  LTFBook->MassStorage_isAble(&isable);
  
  if (!isable)
  {
    OneOfMany_SetItemCount(LTFBook->oofm_gui,3);
    if (active==3)
    {
      LTFBook->PrintServer_LogToFS_SetState(0);
      active=0;
    }
  }
  else
  {
    OneOfMany_SetItemCount(LTFBook->oofm_gui,4);
  }
  
  OneOfMany_SetChecked(LTFBook->oofm_gui,active);
  OneOfMany_SetOnMessage(LTFBook->oofm_gui,oofm_gui_callback);
  GUIObject_SoftKeys_SetAction(LTFBook->oofm_gui,ACTION_SELECT1,oofm_gui_select);
  GUIObject_SoftKeys_SetAction(LTFBook->oofm_gui,ACTION_BACK,oofm_gui_previous);
  GUIObject_SoftKeys_SetAction(LTFBook->oofm_gui,ACTION_LONG_BACK,oofm_gui_cancel);
  GUIObject_Show(LTFBook->oofm_gui);
}


void list_gui_cancel(BOOK * book, GUI * gui)
{
  FreeBook(book);
}


void list_gui_previous(BOOK * book, GUI * gui)
{
  FreeBook(book);
}


void list_gui_select(BOOK * book, GUI * gui)
{
  switch(ListMenu_GetSelectedItem(gui))
  {
  case 0:
    LogToFSBook_Settings(book);
    break;
  case 1:
    LogToFSBook_MakeMarker(book);
    break;
  case 2:
    LogToFSBook_DumpSettings(book);
    break;
  }
}


int list_gui_callback(GUI_MESSAGE * msg)
{
  char * item_name=0;
  if (GUIonMessage_GetMsg(msg)==LISTMSG_GetItem)
  {
    switch(GUIonMessage_GetCreatedItemIndex(msg))
    {
    case 0:
      item_name="Settings";
      break;
    case 1:
      item_name="Set marker";
      break;
    case 2:
      item_name="Dump Settings";
      break;
    }
    GUIonMessage_SetMenuItemText(msg,TextID_Create(item_name,ENC_LAT1,TEXTID_ANY_LEN));
    return 1;
  }
  return 0;
}


int LogToFSBook_MainPage_Enter(void * r0,BOOK * book)
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  LTFBook->list_gui=CreateListMenu(LTFBook,0);
  GUIObject_SetTitleText(LTFBook->list_gui,TextID_Create("Log to FS",ENC_LAT1,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(LTFBook->list_gui,3);
  ListMenu_SetCursorToItem(LTFBook->list_gui,0);
  ListMenu_SetOnMessage(LTFBook->list_gui,list_gui_callback);
  GUIObject_SoftKeys_SetAction(LTFBook->list_gui,ACTION_SELECT1,list_gui_select);
  GUIObject_SoftKeys_SetAction(LTFBook->list_gui,ACTION_BACK,list_gui_previous);
  GUIObject_SoftKeys_SetAction(LTFBook->list_gui,ACTION_LONG_BACK,list_gui_cancel);
  GUIObject_Show(LTFBook->list_gui);
  
  return 0;
}


void LTFBook_Destroy( BOOK * book )
{
  LogToFSBook * LTFBook = (LogToFSBook*)book;
  
  if (LTFBook->list_gui) GUIObject_Destroy(LTFBook->list_gui);
  if (LTFBook->oofm_gui) GUIObject_Destroy(LTFBook->oofm_gui);
  if (LTFBook->stringinput_gui) GUIObject_Destroy(LTFBook->stringinput_gui);
  
  SUBPROC(elf_exit);
}


void CreateLogToFSBook()
{
  int f;
  wchar_t * path;
  FSTAT fstat_struct;
  
  LogToFSBook * LTFBook = (LogToFSBook*)malloc(sizeof(LogToFSBook));
  CreateBook(LTFBook,LTFBook_Destroy,&LogToFSBook_Base_Page,"LogToFS Book",-1,0);
  LTFBook->list_gui=0;
  LTFBook->oofm_gui=0;
  LTFBook->stringinput_gui=0;
  
  LTFBook->MassStorage_isAble=MassStorage_isAble_w705;
  LTFBook->PrintServer_LogToFS_GetState=PrintServer_LogToFS_GetState_w705;
  LTFBook->PrintServer_LogToFS_SetState=PrintServer_LogToFS_SetState_w705;
  
  path=GetDir( DIR_INI );
  
  if (fstat(path,L"logtofs.ini",&fstat_struct)>=0)
  {
    char buf[50];
    
    f=_fopen(path,L"logtofs.ini",FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
    fread(f,&buf,fstat_struct.fsize);
    buf[fstat_struct.fsize]=0;
    fclose(f);
    
    sscanf(buf,"%*s %x\r\n%*s %x\r\n%*s %x",&LTFBook->MassStorage_isAble,&LTFBook->PrintServer_LogToFS_GetState,&LTFBook->PrintServer_LogToFS_SetState);
  }
  
  BookObj_GotoPage(LTFBook,&LogToFSBook_Main_Page);
}

int isLogToFSBook(BOOK* book)
{
  if(!strcmp(book->xbook->name,"LogToFS Book")) return(1);
  return(0);
}


int main ()
{
  if (FindBook(isLogToFSBook))
  {
    MessageBox(EMPTY_TEXTID, STR("Already runed"), NOIMAGE, 1, 5000, 0);
    SUBPROC(elf_exit);
  }
  else
  {
    CreateLogToFSBook();
  }
  return 0;;
}
