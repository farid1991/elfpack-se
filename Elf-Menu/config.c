#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
#include "CFile.h"

void Config_Read()
{
  DELETE(SKIN);
  SKIN=new wchar_t[wstrlen(L"skin.mm")+1];
  wstrcpy(SKIN,L"skin.mm");
  char* pcfg=NULL;
  CFile * file=new CFile(settings_path);
  if(file->Stat_Result()==0)
  {
    if(file->Open(WA_Read)>=0)
    {
      file->Read();
      pcfg=file->Content();
      if(char* vibr=manifest_GetParam(pcfg,"[vibr]",0))
      {
        VIBRA=intget(vibr,'\0');
        DELETE(vibr);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_fullscreen]",0))
      {
        IS_FULLSCREEN=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_nosoftkeys]",0))
      {
        IS_NOSOFTKEYS=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_dyntitle]",0))
      {
        IS_DYNTITLE=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_editor]",0))
      {
        IS_EDITOR=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_joy]",0))
      {
        IS_JOY=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_exit]",0))
      {
        IS_EXIT=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* isfc=manifest_GetParam(pcfg,"[is_move]",0))
      {
        IS_MOVE=intget(isfc,'\0');
        DELETE(isfc);
      }
       if(char* isfc=manifest_GetParam(pcfg,"[is_main_dyntitle]",0))
      {
        IS_MAIN_DYNTITLE=intget(isfc,'\0');
        DELETE(isfc);
      }
      if(char* lang=manifest_GetParam(pcfg,"[skin]",0))
      {
        DELETE(SKIN);
        SKIN=utf82wstr(lang);
        DELETE(lang);
      }
      file->Close();
    }
  }
  DELETE(file);
  DELETE(pcfg);
}

void Config_Save()
{
  CFile * file=new CFile(settings_path);
  if(file->Open(WA_Read+WA_Write+WA_Create+WA_Truncate)>=0)
  {
    char temp_buf[512];
    sprintf(temp_buf, "[vibr] %d\n",VIBRA);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_fullscreen] %d\n",IS_FULLSCREEN);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_nosoftkeys] %d\n",IS_NOSOFTKEYS);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_dyntitle] %d\n",IS_DYNTITLE);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_editor] %d\n",IS_EDITOR);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_joy] %d\n",IS_JOY);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_exit] %d\n",IS_EXIT);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_move] %d\n",IS_MOVE);
    file->Write(temp_buf,strlen(temp_buf));

    sprintf(temp_buf, "[is_main_dyntitle] %d\n",IS_MAIN_DYNTITLE);
    file->Write(temp_buf,strlen(temp_buf));

    if(SKIN)
    {
      char* buf=wstr2utf8(SKIN);
      sprintf(temp_buf, "[skin] %s\n",buf);
      file->Write(temp_buf,strlen(temp_buf));
      DELETE(buf);
    }
    file->Close();
  }
  DELETE(file);
}
