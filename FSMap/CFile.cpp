#include "CFile.h"

#define BOM8A 0xEF
#define BOM8B 0xBB
#define BOM8C 0xBF

wchar_t *utf82wstr(char *string){
  long b=0,c=0;
  if (string[0]==BOM8A && string[1]==BOM8B && string[2]==BOM8C)
  {
    string+=3;
  }
  for (const char *a=string;*a;a++)
    if ((*a)<128 || (*a&192)==192)
      c++;
  wchar_t *res=new wchar_t[c+1];
  res[c]=0;
  for (char *a=string;*a;a++)
  {
    if (!(*a&128))
      res[b]=*a;
    else if ((*a&192)==128)
      continue;
    else if ((*a&224)==192)
      res[b]=((*a&31)<<6)|a[1]&63;
    else if ((*a&240)==224)
      res[b]=((*a&15)<<12)|((a[1]&63)<<6)|a[2]&63;
    else if ((*a&248)==240){
      res[b]='?';
    }
    b++;
  }
  res[b]='\0';
  return res;
}

char * wstr2utf8(wchar_t* src)
{
  char * dest=0;

  int len=0;
  for (size_t i = 0; i < wstrlen(src); i++)
  {
    wchar_t w = src[i];
    if (w <= 0x7f)
    {
      len++;
    }
    else if (w <= 0x7ff){
      len+=2;
    }
    else if (w <= 0xffff){
      len+=3;
    }
    else if (w <= 0x10ffff){
      len+=4;
    }
    else
    {
      len++;
    }
  }
  dest = new char[len+1];
  int c=0;
  for (size_t i = 0; i < wstrlen(src); i++){
    wchar_t w = src[i];
    if (w <= 0x7f)
    {
      dest[c]=((char)w);
      c++;
    }
    else if (w <= 0x7ff){
      dest[c]=(0xc0 | ((w >> 6)& 0x1f));
      c++;
      dest[c]=(0x80| (w & 0x3f));
      c++;
    }
    else if (w <= 0xffff){
      dest[c]=(0xe0 | ((w >> 12)& 0x0f));
      c++;
      dest[c]=(0x80| ((w >> 6) & 0x3f));
      c++;
      dest[c]=(0x80| (w & 0x3f));
      c++;
    }
    else if (w <= 0x10ffff){
      dest[c]=(0xf0 | ((w >> 18)& 0x07));
      c++;
      dest[c]=(0x80| ((w >> 12) & 0x3f));
      c++;
      dest[c]=(0x80| ((w >> 6) & 0x3f));
      c++;
      dest[c]=(0x80| (w & 0x3f));
      c++;
    }
    else
    {
      dest[c]=('?');
      c++;
    }
  }
  dest[c]='\0';
  return(dest);
}

CFile::CFile(wchar_t * path)
{
  buffer=0;
  file_path = new wchar_t [wstrlen(path)+1];
  wstrcpy(file_path,path);
  stat_result=w_fstat(file_path, &info);
}

CFile::CFile(wchar_t * path,wchar_t * fname)
{
  buffer=0;
  file_path = new wchar_t [wstrlen(path)+wstrlen(fname)+1];
  wstrcpy(file_path,path);
  wstrcat(file_path,fname);
  stat_result=w_fstat(file_path, &info);
}

CFile::~CFile(void)
{
  if(file_path)
  {
    delete(file_path);
  }
  if(buffer)
  {
    delete(buffer);
  }
  Close();
}

char * CFile::Content()
{
  char * result = 0;
  int len=strlen(buffer);
  if((buffer)&&(len>0))
  {
    result=new char[len+1];
    strcpy(result,buffer);
    return(result);
  }
  return(0);
}

int CFile::Open(int attr)
{
  if((file=w_fopen(file_path, attr, 0x1FF, 0))>=0)
  {
    return(1);
  }
  return(0);
}

int CFile::Close()
{
  return(w_fclose(file));
}

int CFile::Write(void * buf, int len)
{
  return(w_fwrite(file,buf,len));
}

int CFile::WriteNum(int d)
{
  char buf[8];
  int len=sprintf(buf,"%d",d);
  return(w_fwrite(file,buf,len));
}

int CFile::WriteUTF8_BOM()
{
  char buf[3];
  buf[0]=BOM8A;
  buf[1]=BOM8B;
  buf[2]=BOM8C;
  return(w_fwrite(file,buf,3));
}

int CFile::WriteUTF8(wchar_t * buf)
{
  int res;
  char * temp=wstr2utf8(buf);
  res=w_fwrite(file,temp,strlen(temp));
  if(temp)
  {
    delete(temp);
  }
  return(res);
}

//[param]: //

int CFile::WriteParam(wchar_t * param, wchar_t * buf)
{
  wchar_t * tempbuf= new wchar_t[wstrlen(param)+wstrlen(buf)+5];
  wchar_t br[]=L"[]: ";
  wstrcpy(tempbuf,&br[0]);
  wstrcat(tempbuf,param);
  wstrcat(tempbuf,&br[1]);
  wstrcat(tempbuf,&br[2]);
  wstrcat(tempbuf,&br[3]);
  int res;
  char * temp=wstr2utf8(buf);
  res=w_fwrite(file,temp,strlen(temp));
  if(temp)
  {
    delete(temp);
  }
  if(tempbuf)
  {
    delete(tempbuf);
  }
  return(res);
}

void CFile::Read()
{
  if(buffer)
  {
    delete(buffer);
  }
  int len=info.st_size;
  buffer=(char*)malloc(len+1);
  w_fread(file,buffer,len);
}

char * CFile::ReadN(int n)
{
  char*result=new char[n+1];
  w_fread(file,result,n);
  return(result);
}

wchar_t * CFile::ReadUTF8()
{
  if(!buffer)
  {
    int len=info.st_size;
    buffer=(char*)malloc(len+1);
    w_fread(file,buffer,len);
  }
  return(utf82wstr(buffer));
}

wchar_t * CFile::ReadParam(wchar_t * param)
{
  int flen=info.st_size;
  char * temp=(char*)malloc(flen+1);
  w_fread(file,temp,flen);

  wchar_t * buf=utf82wstr(temp);
  wchar_t * value=0;
  wchar_t * result=0;
  if((value=wstrwstr(buf,param))!=0)
  {
    int i=0;
    int len=0;
    int pos=0;
    while((value[i]!=':')&&
          (value[i]!='=')&&
          (value[i]!='\0'))

    {
      pos++;
      i++;
    }
    pos++;
    i++;
    while((value[i]!='\n')&&
          (value[i]!='\r')&&
          (value[i]!=';')&&
          (value[i]!='\0'))

    {
      len++;
      i++;
    }
    result=new wchar_t[len+1];
    wstrncpy(result,value+pos,len);
    result[len]='\0';
  }
  if(temp)
  {
    delete(temp);
  }
  if(buf)
  {
    delete(buf);
  }
  return(result);
}

int CFile::Size()
{
  return(info.st_size);
}
int CFile::Stat_Result()
{
  return(stat_result);
}
bool CFile::Exists()
{
  if(stat_result==0)return true;
  return false;
}

W_FSTAT CFile::FStat()
{
  return(info);
}
int CFile::FileID()
{
  return(file);
}

void CFile::ChangePath(wchar_t * path)
{
  if(file_path)
  {
    delete(file_path);
  }
  file_path = new wchar_t [wstrlen(path)+1];
  wstrcpy(file_path,path);
  stat_result=w_fstat(file_path, &info);
}

