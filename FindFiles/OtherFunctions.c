#include "mcl.h"

int wstrncmp2(wchar_t*str,wchar_t*sub_str)
{
  FUNCTION
  int len=wstrlen(str);
  int sub_len=wstrlen(sub_str);
  int flag;
  for(int i=0;i<(len-sub_len+1);i++)
  {
    flag=i;
    for(int j=0;j<sub_len;j++)
    {
      if(str[i+j]!=sub_str[j])
      {
        flag=-1;
        break;
      }
    }
    if(flag!=-1) break;
  }
  return flag;
}


bool equ(wchar_t*str,wchar_t*sub_str)
{
  FUNCTION
  wstrnlwr(str,wstrlen(str));wstrnlwr(sub_str,wstrlen(sub_str));
  char buf[512];
  char buf2[512];
  debug_printf("\nequ in %s is %s",wstr2strn(buf,str,512),wstr2strn(buf2,sub_str,512));
  int len=wstrlen(str);
  int sub_len=wstrlen(sub_str);
  if(sub_str[sub_len-1]=='*' && sub_len<=len+1 && sub_len!=1)
  {
    wchar_t*sub_strr=new wchar_t[sub_len];
    for(int i=0;i<sub_len-1;i++) sub_strr[i]=sub_str[i];
    sub_strr[sub_len-1]=0;
    bool flag=true;
    for(int i=0;i<sub_len-1;i++)
    {
      if(sub_strr[i]!=str[i])
      {
        flag=false;
        break;
      }
    }
    delete[] sub_strr;
    return flag;
  }
  if(sub_str[0]=='*' && sub_len<=len+1 && sub_len!=1)
  {
    wchar_t*sub_strr=new wchar_t[sub_len];
    for(int i=1;i<sub_len;i++) sub_strr[i-1]=sub_str[i];
    sub_strr[sub_len-1]=0;
    bool flag=true;
    int j=len;
    for(int i=wstrlen(sub_strr)-1;i>=0;i--)
    {
      j--;
      if(sub_strr[i]!=str[j])
      {
        debug_printf("\nin %i - %i",i,j);
        flag=false;
        break;
      }
    }
    delete[] sub_strr;
    return flag;
  }
  if(sub_len>len) return false;
  if(wstrncmp2(str,sub_str)!=-1) return true;
  return false;
}

bool isDir(const wchar_t*dir,const wchar_t*sub_dir)
{
  FUNCTION
  wchar_t*folder=new wchar_t[wstrlen(dir)+wstrlen(sub_dir)+2];
  wstrcpy(folder,dir);
  wstrcat(folder,SLASH);
  wstrcat(folder,sub_dir);
  W_FSTAT fs;
  w_fstat(folder,&fs);
  delete[] folder;
  return(fs.attr&0x4000);
}

void elf_exit(void)
{
  trace_done();
  FUNCTION
  kill_data(&ELF_BEGIN,(void(*)(void*))mfree_adr());
};

wchar_t GetIcon(wchar_t *name)
{
  FUNCTION
  wchar_t icon;
  unsigned short d;
  SUB_EXECUTE *data;
  FILEITEM *fi=FILEITEM_Create();
  FILEITEM_SetFname(fi,name);
  data=DataBrowser_CreateSubExecute(-1, fi);
  DataBrowser_ExecuteSubroutine(data,DB_CMD_SETSMALLICON,&d);
  DataBrowser_ExecuteSubroutine(data,DB_CMD_DESTROY_SUBROUTINE,0);
  FILEITEM_Destroy(fi);
  icon=d;
  return icon;
}
