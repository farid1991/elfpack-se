#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

int CreateFolderDB(void *data, BOOK * book)
{
  ITEM *it=Menu_GetSelected();
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  folder_list[0]=it->link;
  folder_list[1]=0;
  DataBrowserDesc_SetHeaderText(DB_Desc,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,1);
  DataBrowserDesc_Menu_AddFSFunctions(DB_Desc,0);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);
  return(0);
}

const PAGE_MSG SelectFolder_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateFolderDB,
  PREVIOUS_EVENT_TAG,onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG,onCancel_MainMenu_DB,
  NIL_EVENT_TAG,0
};

PAGE_DESC SelectFolder_page ={"BookManager_ChangeShortcuts_SelectFolder_Page",0,SelectFolder_PageEvents};

void MakeVC(wchar_t *num)
{
  char vc[256];
  VCALL_Init(vc);
  VCALL_SetNumber(vc,num, wstrlen(num));
  MakeVoiceCall(0,vc,1);
};

void Link_Execute()
{
  if(VIBRA)AudioControl_Vibrate(pAC,100,100,100);
  Menu_Get()->last=GET_SELECTED();
  ITEM *it=Menu_GetSelected();
  if(!move)
  {
    if(it->type==0)
    {
      Shortcut_Run(it->link);
    }
    else if(it->type==1)
    {
        int ID=JavaName_GetID(it->link);
        char sp_flag;
        REQUEST_UI_OAF_START_APPLICATION(ASYNC,ID+0x10000,&sp_flag);
    }
    else if(it->type==2)
    {
      W_FSTAT _fstat;
      if (w_fstat(it->link,&_fstat)==0)
      {
        elfload(it->link,0,0,0);
      }
    }
    else if(it->type==3)
    {
      //очищаем заголовок и ставим новый
      DELETE(cur_title);
      cur_title=mstr(it->name);
      /*//создаём новый пункт в истории
      ITEM *item=new ITEM;
      memset(item,0,sizeof(ITEM));
      //копируем в него меню из выбранного пункта
      item=it;*/
      //добавляем в историю
      List_InsertLast(history, it);
      //обновляем все это дело
      Menu_Refresh();
    }
    else if(it->type==4)
    {
      char*param=new char[wstrlen(it->link)+2];
      wstr2strn(param,it->link,wstrlen(it->link));
      strcat(param,";");
      int ev=h2i(param);
      UI_Event(ev);
      DELETE(param);
    }
    else if(it->type==5)
    {
      MakeVC(it->link);
    }
    else if(it->type==6)
    {
      BookObj_CallPage(&mbk->book,&SelectFolder_page);
    }

    if((it->type!=3)&&(it->type!=6))
    {
      if(IS_EXIT&&(history->FirstFree==1))Return_ToStandby();
    }

  }
  else
  {
    Menu_Item_Move();
    move=0;
    //History_Refresh();
    Book_Refresh();
  }
}
