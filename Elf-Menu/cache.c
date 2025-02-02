#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
//TEXTID names_table[101];

wchar_t*skin_path_int;
wchar_t*skin_path_ext;
wchar_t*settings_path;

wchar_t * get_path(int is_card)
{
  wchar_t * path=0;
  if(is_card==0)
  {
    path = new wchar_t[wstrlen(CONFIG_INT)+8+1];
    wstrcpy(path,CONFIG_INT);
    wstrcat(path,L"/ElfMenu");
  }
  else if(is_card==1)
  {
    path = new wchar_t[wstrlen(CONFIG_EXT)+8+1];
    wstrcpy(path,CONFIG_EXT);
    wstrcat(path,L"/ElfMenu");
  }
  return(path);
}
wchar_t * get_style_path(int is_card)
{
  wchar_t * path=0;
  if(is_card==0)
  {
    path = new wchar_t[wstrlen(CONFIG_INT)+15+1];
    wstrcpy(path,CONFIG_INT);
    wstrcat(path,L"/ElfMenu/Styles");
  }
  else if(is_card==1)
  {
    path = new wchar_t[wstrlen(CONFIG_EXT)+15+1];
    wstrcpy(path,CONFIG_EXT);
    wstrcat(path,L"/ElfMenu/Styles");
  }
  return(path);
}

wchar_t * get_save_path(int is_card)
{
  wchar_t * path=0;
  wchar_t temp[]=L"/ElfMenu/";
  if(is_card==0)
  {
    path = new wchar_t[wstrlen(CONFIG_INT)+wstrlen(temp)+wstrlen(SKIN)+1];
    wstrcpy(path,CONFIG_INT);
    wstrcat(path,temp);
    wstrcat(path,SKIN);
  }
  else if(is_card==1)
  {
    path = new wchar_t[wstrlen(CONFIG_EXT)+wstrlen(temp)+wstrlen(SKIN)+1];
    wstrcpy(path,CONFIG_EXT);
    wstrcat(path,temp);
    wstrcat(path,SKIN);
  }
  return(path);
}


wchar_t * get_path_skin(int is_card)
{
  wchar_t * path;
  if(is_card==0)
  {
    path = new wchar_t[wstrlen(CONFIG_INT)+(wstrlen(SKIN))+9+1];
    wstrcpy(path,CONFIG_INT);
  }
  else
  {
    path = new wchar_t[wstrlen(CONFIG_EXT)+(wstrlen(SKIN))+9+1];
    wstrcpy(path,CONFIG_EXT);
  }
  wstrcat(path,L"/ElfMenu/");

  wchar_t *str_d=new wchar_t[wstrlen(SKIN)+1];
  wstrcpy(str_d,SKIN);
  *(wstrrchr(str_d,L'.'))=0;

  wstrcat(path,str_d);
  DELETE(str_d);
  return(path);
}

wchar_t * GetPath_Menu()
{
  W_FSTAT stat;
  wchar_t*path;
  wchar_t*tmp=get_save_path(0);
  wchar_t*tmp2=get_save_path(1);
  if(w_fstat(tmp2,&stat)==0)
  {
    path=new wchar_t[wstrlen(tmp2)+1];
    wstrcpy(path,tmp2);
  }
  else
  {
    path=new wchar_t[wstrlen(tmp)+1];
    wstrcpy(path,tmp);
  }
  DELETE(tmp);
  DELETE(tmp2);
  return(path);
}

void Cache_SkinPath()
{
  Cache_SkinPath_Del();
  skin_path_int=get_path_skin(0);
  skin_path_ext=get_path_skin(1);
}

void Cache_SkinPath_Del()
{
  DELETE(skin_path_int);
  DELETE(skin_path_ext);
}

void Cache_Lables_Del()
{
  int i;
  for(i=0;i<LCNT;i++)
    DELETE(names[i]);
}

int wstr2id(wchar_t*str)
{
  int strid=0x6FFFFFFF;
  if(str&&(wstrlen(str)>0))
  {
    strid=TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN);
  }
  else
  {
    strid=TextID_Create(names[2],ENC_UCS2,TEXTID_ANY_LEN);
  }
  return(strid);
}

wchar_t * mstr(wchar_t * w)
{
  wchar_t * res=new wchar_t[wstrlen(w)+1];
  wstrcpy(res,w);
  return(res);
}
