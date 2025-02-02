#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

typedef void* PFileStream;

typedef struct
{
  PFileStream pFS;
  wchar_t *fp;
  int unk;
  int rights;
  char dummy[0x8];
}FS_STRUCT;

typedef struct
{
  char dummy[0x8];
  char isBusy;
  int unk;
  wchar_t *fp;
  int _unk;
}W_STREAM;

wchar_t *FSX_GetFileName(int file)
{
  int *fsram=(int*)0x2009DE14;
  if (file<0x32)
  {
    FS_STRUCT *fss=(FS_STRUCT*)*(fsram+file);
    if (fss) return (fss->fp);
  }
  return 0;
}

wchar_t* WFSX_GetFileName(int file)
{
  W_STREAM *ws=(W_STREAM*)0x2003CECC;
  if ((ws+file)->isBusy)
  {
    return ((ws+file)->fp);
  }
  return 0;
}

int main()
{
  int file;
  int i=0,j=0;
  char buf[1000];
  if((file = w_fopen(L"/card/other/FS Stream List.txt",WA_Create|WA_Write,0x1FF,0)) >= 0)
    {
      for(;i<0x32;i++)
      {
        sprintf(buf,"%02x: %ls\r\n",i,FSX_GetFileName(i) ? FSX_GetFileName(i) : L"Free Stream");
        w_fwrite(file,(void*)buf,strlen(buf));
      }
      w_fclose(file);
    }
  if((file = w_fopen(L"/card/other/W_FS Stream List.txt",WA_Create|WA_Write,0x1FF,0)) >= 0)
  {
    for(;j<0x40;j++)
    {
      sprintf(buf,"%02x: %ls\r\n",j,WFSX_GetFileName(j) ? WFSX_GetFileName(j) : L"Free W_Stream");
      w_fwrite(file,(void*)buf,strlen(buf));
    }
    w_fclose(file);
  }
  return 0;
}

