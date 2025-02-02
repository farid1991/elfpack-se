//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "MyFUNC.h"
#include "StructBook.h"

void SaveSettings(SOUND*s, IMGN*i, TAB*t, int col)
{
  int f=_fopen(GetConfigPath(), L"Settings.gms",0x2,0x180,0);
  fwrite(f, s, sizeof(SOUND));
  fwrite(f, i, sizeof(IMGN));
  fwrite(f, &col, 4);
  fwrite(f, MyBK()->nast->colors, 4*8);
  fwrite(f, MyBK()->nast->buttons, 4*9*2);
  fclose(f);
  f=_fopen(GetConfigPath(), L"Resultats.tab",0x2,0x180,0);
  fwrite(f, t->te, sizeof(TABL));
  fwrite(f, t->tn, sizeof(TABL));
  fwrite(f, t->th, sizeof(TABL));
  fclose(f);
}

void OpenSettings(MyBook*bk)
{
  int f;
  if(!FindFile(GetConfigPath(), L"Settings.gms"))
  {
    if(FileSize(GetConfigPath(), L"Settings.gms")==(sizeof(SOUND)+sizeof(IMGN)+4+4*8+4*9*2))
    {
      f=_fopen(GetConfigPath(), L"Settings.gms",0x1,0x180,0);
      fread(f, bk->nast->sound, sizeof(SOUND));
      fread(f, bk->nast->im, sizeof(IMGN));
      fread(f, &(bk->nast->coloron), 4);
      fread(f, bk->nast->colors, 4*8);
      fread(f, bk->nast->buttons, 4*9*2);
      fclose(f);
    }
    else
    {
      SaveSettings(MyBK()->nast->sound, MyBK()->nast->im, MyBK()->nast->rez, MyBK()->nast->coloron);
    }
  }
  else
  {
    SaveSettings(MyBK()->nast->sound, MyBK()->nast->im, MyBK()->nast->rez, MyBK()->nast->coloron);
  }
  if(!FindFile(GetConfigPath(), L"Resultats.tab"))
  {
    f=_fopen(GetConfigPath(), L"Resultats.tab",0x1,0x180,0);
    fread(f, bk->nast->rez->te, sizeof(TABL));
    fread(f, bk->nast->rez->tn, sizeof(TABL));
    fread(f, bk->nast->rez->th, sizeof(TABL));
    fclose(f);
  }else{
    f=_fopen(GetConfigPath(), L"Resultats.tab",0x2,0x180,0);
    fwrite(f, bk->nast->rez->te, sizeof(TABL));
    fwrite(f, bk->nast->rez->tn, sizeof(TABL));
    fwrite(f, bk->nast->rez->th, sizeof(TABL));
    fclose(f);
  }
}
