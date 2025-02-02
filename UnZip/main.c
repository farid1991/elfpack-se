#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\zlib.h"

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

wchar_t filename[256]={0};

int fread32(int fh, char * buf, int len)
{
  int clen;
  int rlen;
  int total=0;
  while(len)
  {
    if (len>16384) clen=16384; else clen=len;
    total+=(rlen=fread(fh, buf, clen));
    if (rlen!=clen) break;
    buf+=rlen;
    len-=clen;
  }
  return(total);
}

int fwrite32(int fh, char * buf, int len)
{
  int clen;
  int rlen;
  int total=0;
  while(len)
  {
    if (len>16384) clen=16384; else clen=len;
    total+=(rlen=fwrite(fh, buf, clen));
    if (rlen!=clen) break;
    buf+=rlen;
    len-=clen;
  }
  return(total);
}

int unzip(Byte *compr, uLong comprLen, Byte *uncompr, uLong  uncomprLen)
{
    int err;
    z_stream d_stream; /* decompression stream */
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = compr;
    d_stream.avail_in = (uInt)comprLen;
    err = inflateInit2(&d_stream,-MAX_WBITS);
    if(err!=Z_OK)
    {
   unerr:
      return err;
    }
    d_stream.next_out = uncompr;            /* discard the output */
    d_stream.avail_out = (uInt)uncomprLen;
    err = inflate(&d_stream, 2);
    if(err<0) goto unerr;
    err = inflateEnd(&d_stream);
    if(err<0) goto unerr;
    return 0;
}


int getint(char * ptr)
{
  return ptr[0]|(ptr[1]<<8)|((ptr[2])<<16)|((ptr[3])<<24);
}

void prepname(wchar_t * file)
{
  do
  {
    while(*++file && *file!='/');
    if(!*file) break;
    *file=0;
    mkdir(filename);
    *file='/',0;
  }
  while(1);
}

void bi8t2unicode(wchar_t * ws, char * s, int len)
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c==0xF1) c=0x451;
    if (c==0xF0) c=0x401;
    if ((c>=0x80)&&(c<0xB0)) c+=0x390;
    if ((c>=0xE0)&&(c<0xF0)) c+=0x360;
    *ws++=c;
  }
  *ws=0;
}

int main(wchar_t * exename, wchar_t * path, wchar_t * fname)
{
  int in,out;
  int packsize=1, unpacksize=2, filenamesize=3;
  wchar_t * file=filename;
  char * buf;
  if(fname && (in=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))!=-1)
  {
    wchar_t * sfname = new wchar_t[wstrlen(fname)+1];
    wstrcpy(sfname,fname);
    wchar_t * spath = new wchar_t[wstrlen(path)+1];
    wstrcpy(spath,path);
    wstrcpy(filename,spath);
    wstrcat(filename,_T("/"));
    wstrcat(filename,sfname);
    mfree(spath);
    mfree(sfname);

    file=filename+wstrlen(filename);

    while(--file>filename && *file!='.');
    *file=0;
    wchar_t * dir_name = new wchar_t[wstrlen(filename)+1];
    wstrcpy(dir_name,filename);
    *file++='/';

    char header[30];
    while(1)
    {
      if(fread(in,header,30)!=30) break;
      if(getint(header)!=0x04034b50) break;
      packsize=getint(header+18);
      unpacksize=getint(header+22);
      filenamesize=getint(header+26);

      char * fnameISO = new char[filenamesize+1];
      fread(in,fnameISO,filenamesize);
      fnameISO[filenamesize]=0;
      wchar_t * wfname = new wchar_t[filenamesize+1];
      bi8t2unicode(wfname,fnameISO,filenamesize);
      *file=0;
      wstrcat(file,wfname);
      mfree(fnameISO);

      if(file[wstrlen(wfname)-1]!='/')
      {
        if((buf=(char*)malloc((packsize==unpacksize)?packsize:(packsize+unpacksize)))!=NULL)
        {
          mkdir(dir_name);
          fread32(in,buf,packsize);
          prepname(file);
          if((out=fopen(filename,FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE))!=-1)
          {
            if(packsize==unpacksize)
            {
              fwrite32(out,buf,packsize);
            }
            else
            {
              unzip((Byte*)buf,packsize,(Byte*)(buf+packsize),unpacksize);
              fwrite32(out,buf+packsize,unpacksize);
            }
            fclose(out);
          }
          mfree(buf);
        }
        else
        {
          wchar_t er[256];
          snwprintf(er,255,L"Does not stand out memory!\nBig size packed file \"%ls\"",wfname);
          MessageBox(EMPTY_TEXTID,TextID_Create(er,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,10000,0);
        }
      }
      mfree(wfname);
    }
    mfree(dir_name);
    fclose(in);
  }
  UI_Event_toBookID(DB_UPDATE_EVENT,BookObj_GetBookID(FindBook(get_IsDataBrowserBook())));
  SUBPROC(elf_exit);
  return (0);
}
