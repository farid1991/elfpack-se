
#include "LNGparser.h"

LNG_RES_T Lang;  // языковой ресурс

// чтение файла €зыкового ресурса
int ReadLang(wchar_t *file)
{
  FILE_HANDLE f;
  int fSize;
  
  Lang.res = NULL;
  
  W_FSTAT w_fst;
  if(w_fstat(file, &w_fst)) return(RESULT_FAIL);
  fSize = w_fst.st_size;
  
  Lang.res = (wchar_t *)malloc(fSize+2);
  
  f = w_fopen(file, WA_Read, 0x1FF, 0);
  if(f == FILE_HANDLE_INVALID) return(RESULT_FAIL);
  w_fread(f, Lang.res, fSize);
  w_fclose(f);
  
  if(Lang.res[0] == 0xFEFF) Lang.str[0] = Lang.res+1;
  else Lang.str[0] = Lang.res;
  
  for(int i=0, numStr=1; (i < fSize/2)&&(numStr < LNG_MAX); i++)
  {
    if(Lang.res[i] == '\r') Lang.res[i] = 0;
    if(Lang.res[i] == '\n')
    {
      Lang.res[i] = 0;
      Lang.str[numStr] = Lang.res+i+1;
      numStr++;
    }
  }
  Lang.res[(fSize/2)] = 0;
  return(RESULT_OK);
}

void CloseLang(void)
{
  if(Lang.res != NULL)
  {
    mfree(Lang.res);
    Lang.res = NULL;
  }
}

wchar_t *RecLang(LNG_RES_ID_T num)
{
  if(num >= LNG_MAX) return(NULL);
  return(Lang.str[num]);
}
