#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
#include "CFile.h"

void Read_Lang()
{
  wchar_t*tmp=get_path(0);
  wchar_t*path=new wchar_t[wstrlen(tmp)+wstrlen(L"lang.txt")+1+1];
  wstrcpy(path,tmp);
  wstrcat(path,L"/");
  wstrcat(path,L"lang.txt");

  CFile * file=new CFile(path);
  if(file->Stat_Result()==0)
  {
    if(file->Open(WA_Read)>=0)
    {
      wchar_t * buf=file->ReadUTF8();
      int pos=0;
      for(int i=0;i<50;i++)
      {
        names[i]=GetWchar(buf,&pos,'\r');
        pos++;
      }
      file->Close();
      DELETE(buf);
    }
    else
    {
      MESSAGEBOX(STR("Fatal error!\nCan't open lang.txt!"));
    }
  }
  else
  {
    MESSAGEBOX(STR("Fatal error!\nlang.txt didn't found!"));
  }
  DELETE(file);
  DELETE(path);
  DELETE(tmp);
}
