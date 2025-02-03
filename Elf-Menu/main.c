//Проект Elf-Menu
//Главное меню на эльфе
//(c)arban

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
PAudioControl pAC;
MyBOOK*mbk;
wchar_t Slash[]=L"/";

void All_Gui_Free()
{
  FREE_GUI(mbk->menu);
  FREE_GUI(mbk->edit);
  FREE_GUI(mbk->type);
  FREE_GUI(mbk->set);
  FREE_GUI(mbk->style);
  FREE_GUI(mbk->cstyle);
  FREE_GUI(mbk->gstyle);
  FREE_GUI(mbk->skin);
  FREE_GUI(mbk->selectf);
  FREE_GUI(mbk->str_input);
  FREE_GUI(mbk->ev_input);
  FREE_GUI(mbk->vc_input);
  FREE_GUI(mbk->skn_input);
  FREE_GUI(mbk->g_del);
  FREE_GUI(mbk->gmenu);
  FREE_GUI(mbk->style_tab);
  FREE_GUI(mbk->sn_input);
}

void Gui_Pointers2NULL()
{
  mbk->menu=NULL;
  mbk->edit=NULL;
  mbk->type=NULL;
  mbk->set=NULL;
  mbk->style_tab=NULL;
  mbk->cstyle=NULL;
  mbk->gstyle=NULL;
  mbk->style=NULL;
  mbk->skin=NULL;
  mbk->selectf=NULL;
  mbk->str_input=NULL;
  mbk->sn_input=NULL;
  mbk->ev_input=NULL;
  mbk->vc_input=NULL;
  mbk->skn_input=NULL;
  mbk->g_del=NULL;
  mbk->gmenu=NULL;
}

int TerminateElf(void * ,BOOK* book)
{
  Menu_Save();
  Timer_Kill(&timer);
  All_Gui_Free();
  Softkey_SetAction(0);
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MessageBox(0x6fFFFFFF,STR(AUTHOR),0, 1 ,0,0);
  return(1);
}

const PAGE_MSG HW_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  0,0
};

PAGE_DESC base_page ={"HW_BasePage",0,HW_PageEvents};

void elf_exit(void)
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

void onMyBookClose(BOOK * book)
{
  if (book)
  {
    CustomStyles_List_Destroy();
    Styles_List_Destroy();
    History_Free();
    DELETE(SKIN);
    DELETE(cur_title);
    DELETE(settings_path);
    Cache_Lables_Del();
    Cache_SkinPath_Del();
    elf_exit();
  }
}

int isMyBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"Elf-Menu")) return(1);
  return(0);
}

int Platform;


int Start()
{
  mbk=new MyBOOK;
  memset(mbk,0,sizeof(MyBOOK));
  if (!CreateBook(&mbk->book,onMyBookClose,&base_page,"Elf-Menu",-1,0))
  {
    delete mbk;
    SUBPROC(elf_exit);
    return (1);
  }
  Gui_Pointers2NULL();
  Config_Read();
  Cache_SkinPath();
  Menu_Read();
  return(0);
}

int main (void)
{
  //деликер
  trace_init();
  //кэшируем текстовые метки
  Read_Lang();
  //создаём историю
  History_Create();
  //проверяем и создаем основные папки для эльфменю
  wchar_t*path=get_style_path(0);
  W_FSTAT _fstat;
  if (w_fstat(path,&_fstat)!=0)
  {
    mkdir(path);
  }
  DELETE(path);
  //кешируем путь до конфига
  wchar_t*tmp=get_path(0);
  settings_path=new wchar_t[wstrlen(tmp)+wstrlen(L"settings.ini")+1+1];
  wstrcpy(settings_path,tmp);
  wstrcat(settings_path,L"/");
  wstrcat(settings_path,L"settings.ini");
  DELETE(tmp);
  //проверяем платформу
  switch(GetChipID()&CHIPID_MASK)
  {
  case CHIPID_DB2010:
    Platform = 2010; //DB2010
    break;
  case CHIPID_DB2020:
    Platform = 2020; //DB2020
    break;
  default:
    Platform=200;
    break;
  }
  //инициализируем аудиоконтрол
  pAC = AudioControl_Init();
  if( !pAC )
    pAC = *GetAudioControlPtr();
  //запускаем монстра
  if(!FindBook(isMyBook))
  {
    if(Start())return 0;
    Softkey_SetAction(1);
  }
  else
  {
    FreeBook(FindBook(isMyBook));
    if(Start())return 0;
    Softkey_SetAction(1);
    MENU_CREATE_GUI();
  }
  return(0);
}
