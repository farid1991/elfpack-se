#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

void kickbadsymbols(wchar_t *string, int len)
{
  char *str=(char*)string;
  if (len>0)
  {
    int x;
    for (x=0;x<(len*2);x++)
    {
      if (str[x]==0xFE || str[x]==0xFF)
      {
        str[x]=' ';
      }
    }
  }
};

void win12512unicode(wchar_t *ws, char *s, int len)
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c==0xA8) c=0x401;
    if (c==0xAA) c=0x404;
    if (c==0xAF) c=0x407;
    if (c==0xB8) c=0x451;
    if (c==0xBA) c=0x454;
    if (c==0xBF) c=0x457;
    if ((c>=0xC0)&&(c<0x100)) c+=0x350;
    *ws++=c;
  }
  *ws=0;
}
void *memmem(const unsigned char *haystack, size_t haystacklen, const void *needle, size_t needlelen)
{
  const unsigned char *cursor;
  const unsigned char *last_possible_needle_location = (unsigned char *)haystack + haystacklen - needlelen;
  if (needlelen > haystacklen) return(NULL);
  if (needle == NULL) return(NULL);
  if (haystack == NULL) return(NULL);
  if (needlelen == 0) return(NULL);
  if (haystacklen == 0) return(NULL);

  for (cursor = haystack; cursor <= last_possible_needle_location; cursor++) {
    if (memcmp(needle, cursor, needlelen) == 0) {
      return((void *) cursor);
    }
  }
  return(NULL);
};

char *getparam(char *buf, char *mask, char end, int fsize)
{
  char *str=0;
  if (str=(char*)memmem((unsigned char const*)buf,fsize,mask,strlen(mask)))
  {
    char *ret=0;;
    int addr=(int)str;
    str=(char*)(addr+strlen(mask)+2);
    int x=0;
    int len=0;
    while (str[x]!=end)
    {
      x++;
    }
    len=x+1;
    ret=new char[len+1];
    memcpy(ret,str,len);
    ret[len]='\0';
    return ret;
  }
  return 0;
};


wchar_t *getwchr(char *line, int *pos, char end)
{
  int start,en;
  start=pos[0];
  while (line[pos[0]]!=end)
  {
    pos[0]++;
  }
  en=pos[0];
  pos[0]++;
  if ((en-start)<=0)
  {
    return 0;
  }
  int len=en-start;
  wchar_t *wret=new wchar_t[len/2+1];
  memcpy(wret,&line[start], len);
  wret[len/2]=L'\0';
  return wret;
};

int intget(char *line, char end)
{
  int pos=0;
  bool minus=false;
  if (line[pos]=='-')
  {
    minus=true;
    pos++;
  }
  int ret=0;
  while (line[pos]!=end)
  {
    ret=(ret*10)+(line[pos]-0x30);
    pos++;
  }
  pos++;
  if (minus)ret=-ret;
  return ret;
};

int getint(char *line, int *pos,char end)
{
  bool minus=false;
  if (line[pos[0]]=='-')
  {
    minus=true;
    pos[0]++;
  }
  int ret=0;
  while (line[pos[0]]!=end)
  {
    ret=(ret*10)+(line[pos[0]]-0x30);
    pos[0]++;
  }
  pos[0]++;
  if (minus)ret=-ret;
  return ret;
};

char * unicode2win1251(char *s, wchar_t *ws, int len)
{
  char *d=s;
  int c;
  while((c=*ws++)&&((len--)>0))
  {
    if (c==0x401) c=0xA8;
    if (c==0x404) c=0xAA;
    if (c==0x407) c=0xAF;
    if (c==0x451) c=0xB8;
    if (c==0x454) c=0xBA;
    if (c==0x457) c=0xBF;
    if ((c>=0x410)&&(c<0x450)) c-=0x350;
    *s++=c;
  }
  *s=0;
  return(d);
}

void Book_Refresh()
{
  BookObj_Hide((BOOK*)mbk,0);
  BookObj_Show((BOOK*)mbk,0);
}

int h2i(char * h)
{
  char c;
  int res=0;
  while((*h++)!='x');
  do
  {
    c=*h++;
    if (c>0x60) c-=0x20; else c-=0x30;
    if (c>9) c-=7;
    res<<=4;
    res|=c;
  }
  while(*h!=';');
  return(res);
}

int GetInt(wchar_t * buf,int * x, wchar_t end)
{
  int i=0;
  while((buf[*x+i]!=end)&&(buf[*x+i]!='\0'))
  {
    i++;
  }
  wchar_t * temp=new wchar_t[i+1];
  i=0;
  while((buf[*x+i]!=end)&&(buf[*x+i]!='\0'))
  {
    temp[i]=buf[*x+i];
    i++;
  }
  temp[i]='\0';
  int res=0;
  wtoi(temp,i,&res);
  if((wstrlen(buf)-1)>(*x+i+1))
  {
  *x+=i+1;
  }
  else
  {
  *x=wstrlen(buf)-1;
  }
  DELETE(temp);
  return(res);
}

wchar_t * GetWchar(wchar_t * buf,int * x, wchar_t end)
{
  int i=0;
  while((buf[*x+i]!=end)&&(buf[*x+i]!='\0'))
  {
    i++;
  }
  wchar_t * temp=new wchar_t[i+1];
  i=0;
  while((buf[*x+i]!=end)&&(buf[*x+i]!='\0'))
  {
    temp[i]=buf[*x+i];
    i++;
  }
  temp[i]='\0';
  if((wstrlen(buf)-1)>(*x+i+1))
  {
  *x+=i+1;
  }
  else
  {
  *x=wstrlen(buf)-1;
  }
  return(temp);
}

