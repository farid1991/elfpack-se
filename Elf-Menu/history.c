#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

LIST* history;

void History_Free()
{
  if(history)
  {
    while(history->FirstFree>1)
    {
      int tmp=history->FirstFree;
      ITEM* it=(ITEM*)List_RemoveAt(history,history->FirstFree-1);
      if(tmp==1)
      {
        Menu_Free(&it->menu);
      }
    }
    ITEM* it=(ITEM*)List_RemoveAt(history,0);
    DELETE(it);
    LIST_FREE(history);
  }
}

void History_Get()
{
  if(history)
  {
    while(history->FirstFree>1)
    {
      ITEM*ditem=(ITEM*)List_RemoveAt(history,history->FirstFree-1);
    }
  }
}

void History_Create()
{
  LIST_FREE(history);
  history=List_Create();
  ITEM *item=new ITEM;
  memset(item,0,sizeof(ITEM));
  List_InsertLast(history, item);
}
