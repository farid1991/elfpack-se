#include "bookparam.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "shortcuts.h"
#include "main.h"

const char corversion[4]="422";
wchar_t filename[15]=L"PARAMBOOKS.dat";

void WritePARAM(BOOK_PARAM* data, const int n)
{
  FSTAT _fs;
  INFO inf;
  inf.flag[0]='B';
  inf.flag[1]='M';
  inf.version[0]=corversion[0];
  inf.version[1]=corversion[1];
  inf.version[2]=corversion[2];
  wchar_t* path=get_path();
  int f=_fopen(path,filename,FSX_O_WRONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  fwrite(f,&inf,sizeof(INFO));
  if(fstat(path,filename,&_fs)==0)
  {
    if(n==-1) lseek(f,_fs.fsize,0);
    else lseek(f,n*sizeof(BOOK_PARAM)+sizeof(INFO),0);
  }
  fwrite(f,data,sizeof(BOOK_PARAM));
  fclose(f);
  delete path;
}

int Convert4_21toNow(int OldFile)
{
  INFO inf;
  BOOK_PARAM_4_21 oldbp;
  BOOK_PARAM newbp;
  inf.flag[0]='B';
  inf.flag[1]='M';
  inf.version[0]=corversion[0];
  inf.version[1]=corversion[1];
  inf.version[2]=corversion[2];
  wchar_t* path=get_path();
  lseek(OldFile,0,0);
  int size=lseek(OldFile,0,2);
  lseek(OldFile,0,0);
  int N=size/sizeof(BOOK_PARAM_4_21);
  int NewFile=_fopen(path,L"TEMP.dat",FSX_O_WRONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  fwrite(NewFile,&inf,sizeof(INFO));
  for(int i=0;i<N;i++)
  {
    memset(&oldbp,0,sizeof(BOOK_PARAM_4_21));
    memset(&newbp,0,sizeof(BOOK_PARAM));
    fread(OldFile,&oldbp,sizeof(BOOK_PARAM_4_21));
    for(int j=0;j<strlen(oldbp.book);j++)
      newbp.book[j]=oldbp.book[j];
    for(int j=0;j<wstrlen(oldbp.name);j++)
      newbp.name[j]=oldbp.name[j];
    for(int j=0;j<wstrlen(oldbp.i_name);j++)
      newbp.i_name[j]=oldbp.i_name[j];
    //strcpy(newbp.book,oldbp.book);
    //wstrcpy(newbp.name,oldbp.name);
    //wstrcpy(newbp.i_name,oldbp.i_name);
    fwrite(NewFile,&newbp,sizeof(BOOK_PARAM));
  }
  fclose(OldFile);
  fclose(NewFile);
  int error;
  FileDelete(path,filename,&error);
  rename(path,L"TEMP.dat",path,filename,0);
  NewFile=_fopen(path,filename,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  delete(path);
  return NewFile;
}

int GetPARAM(char* BookName, BOOK_PARAM* data)
{
  int n=-1;
  FSTAT _fs;
  wchar_t* path=get_path();
  INFO inf;
  if(fstat(path,L"PARAMBOOKS.dat",&_fs)==0)
  {
    int i;
    int f=_fopen(path,filename,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
    fread(f,&inf,sizeof(INFO));
    if(inf.flag[0]!='B' || inf.flag[1]!='M') f=Convert4_21toNow(f);
    else if(inf.version[0]!=corversion[0] || inf.version[1]!=corversion[1] || inf.version[2]!=corversion[2]) /*конвертация*/;
    lseek(f,0,0);
    int all=(lseek(f,0,2)-sizeof(INFO))/sizeof(BOOK_PARAM);
    lseek(f,sizeof(INFO),0);
    if(f!=-1)
    {
      for(i=0;i<all;i++)
      {
        fread(f,data,sizeof(BOOK_PARAM));
        if(strcmp(BookName,data->book)==0)
        {
          n=i;
          break;
        }
      }
      fclose(f);
    }
  }
  delete path;
  return n;
};
