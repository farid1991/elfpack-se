//include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Game.h"
#include "Pages.h"



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

void vibra()
{
  PAudioControl pAC = AudioControl_Init();
  if( !pAC ) pAC = *GetAudioControlPtr();
  AudioControl_Vibrate(pAC, 200, 50, 200);
}


wchar_t*NamesOfPunkts(int x)
{
  switch(x)
  {
#ifdef ENG_LANG
  case 0:
    return L"Звук старта";
  case 1:
    return L"Звук очка";
  case 2:
    return L"Звук конца игры";
  case 3:
    return L"Звук рекорда";
  case 4:
    return L"Звук паузы";
  case 5:
    return L"Звук перемещения";
#else
  case 0:
    return L"Sound start";
  case 1:
    return L"Sound point";
  case 2:
    return L"Sound game over";
  case 3:
    return L"Sound record";
  case 4:
    return L"Sound pause";
  case 5:
    return L"Sound displacement";
#endif
  }
  return L" ";
}


int GetStrokaIMG(wchar_t*stroka, int x)
{
  int met[2];
  if(x) iconidname2id(L"CHECKMARK_IN_BOX_ICN",-1, &met[0]);
  else iconidname2id(L"CHECKBOX_ICN", -1, &met[0]);
  met[0]=(0x78000000+met[0]);
  met[1]=Str2ID(stroka, ENC_UCS2, TEXTID_ANY_LEN);
  return (Str2ID(met, ENC_TEXTID, 2));
}

BOOK*isBookX(char const*isbook, int x)
{
  int j, i, k;
  BOOK*bk;
  j=root_list_get_session_count();
  if(x)
  {
    bk=(BOOK*)ListElement_GetByIndex(root_list_get_session(j-1)->listbook,root_list_get_session(j-1)->listbook->FirstFree-1);
    if(!strcmp(bk->xbook->name,isbook)) return bk;
  }
  else
  {
    for(i=0; i<j; i++)
    {
      for(k=0;k<root_list_get_session(i)->listbook->FirstFree;k++)
      {
        bk=(BOOK*)ListElement_GetByIndex(root_list_get_session(i)->listbook,k);
        if(!strcmp(bk->xbook->name,isbook)) return bk;
      }
    }
  }
  return(0);
}

MyBook*MyBK()
{
  return ((MyBook*)isBookX(NameMyElf,0));
}

int FileSize(wchar_t*path, wchar_t*name)
{
  FSTAT fs;
  fstat(path, name, &fs);
  return fs.fsize;
}

wchar_t * StatusSettings1(int s)
{
#ifdef ENG_LANG
  if(s==0) return L"Вкл.";
  return L"Откл.";
#else
  if(s==0) return L"On";
  return L"Off";
#endif
}

wchar_t * StatusSettings(int s)
{
#ifdef ENG_LANG
  if(s==1) return L"Включенно";
  return L"Отключенно";
#else
  if(s==1) return L"On";
  return L"Off";
#endif

}

int Record(TABL*tab, int new_result, int N)
{
  int i=0;
  while(i<N)
  {
    if(new_result>(tab->rezultat[i]))
    {
      return i+1;
    }
    i++;
  }
  return 0;
}
int Perestava(TABL*tab, wchar_t new_name[8], int new_result, int N)
{
  int i=0;
  int j;
  while(i<N)
  {
    if(new_result>(tab->rezultat[i]))
    {
      for(j=N-2; j>=i; j+=-1)
      {
        wstrcpy(tab->name[j+1], tab->name[j]);
        tab->rezultat[j+1]=tab->rezultat[j];
      }
      tab->rezultat[i]=new_result;
      wstrcpy(tab->name[i], new_name);
      return i+1;
    }
    i++;
  }
  return 0;
}

wchar_t * GetConfigPath()
{
  wchar_t*cpath=new wchar_t[wstrlen(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL))+wstrlen(L"/Games/Tetris")+1];
  wstrcpy(cpath,GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
  wstrcat(cpath,L"/Games/Tetris");
  return(cpath);
};

int GetDis()
{
  switch(Display_GetHeight(0))
  {
  case 220:
    return 0;
  case 320:
    return 1;
  }
  return 2;
}

int GetFSize(int f)
{
  if(GetDis()==0 && f==FONT_E_30R) return 30;
  if(GetDis()==0 && f==FONT_E_12R) return 12;
  if(GetDis()==1 && f==FONT_E_40R) return 40;
  if(GetDis()==1 && f==FONT_E_14R) return 14;
  if(GetDis()==2 && f==FONT_E_18R) return 18;
  if(GetDis()==2 && f==FONT_D_7R) return 7;
  return 0;
}

int FindFile(wchar_t*path, wchar_t const*name)
{
  FSTAT _fstat;
  if(fstat(path,name,&_fstat)==0) return 0;
  return 1;
}

int CheckMusic(int x)
{
  int result=!FindFile(GetConfigPath(),MyBK()->nast->sound->names[x])*MyBK()->nast->sound->sound*MyBK()->nast->sound->status[x]*!FindBook(isFmRadioBook())*!FindBook(isAudioPlayerBook());
  return result;
}

void DELTABL(TABL*t)
{
  for(int i=0; i<10; i++)
  {
    t->rezultat[i]=0;
    wstrcpy(t->name[i], L"Unknow");
  }
}

int PlayMyMusic(int x)
{
  if(CheckMusic(x)) PlayFile(GetConfigPath(),MyBK()->nast->sound->names[x]);
  return 0;
}

wchar_t*chislo(int x)
{
  int ID=int2strID(x);
  wchar_t*text=new wchar_t[TextGetLength(ID)+1];
  TextID2wstr(ID,text,TextGetLength(ID));
  return text;
}

void SaveTempGame(GAME*g, FIGURA*f)
{
  int ifile=_fopen(GetConfigPath(), L"TEMP.sav", 0x2,0x180,0);
  fwrite(ifile, g, sizeof(GAME));
  fwrite(ifile, f, sizeof(FIGURA));
  fclose(ifile);
  wstrcpy(MyBK()->path, GetConfigPath());
  wstrcpy(MyBK()->name, L"TEMP.sav");
  BookObj_GotoPage((BOOK*)(MyBK()), &bsave);
}
int FreeElf(void*, BOOK*bk)
{
  if(MyBK()->gtimer)
  {
    Timer_Kill(&(MyBK()->ftimer));
  }
  if(MyBK()->iptimer) Timer_Kill(&(MyBK()->ptimer));
  FreeBook(bk);
  return 0;
}

void DefaultButtons(MyBook*Game)
{
  Game->nast->buttons[0][0]=KEY_LEFT;//лево
  Game->nast->buttons[0][1]=KEY_RIGHT;//право
  Game->nast->buttons[0][2]=KEY_ENTER;//ускорение
  Game->nast->buttons[0][3]=KEY_LEFT_SOFT;//мгновенный сброс
  Game->nast->buttons[0][4]=KEY_UP;//поворот по часовой
  Game->nast->buttons[0][5]=KEY_DOWN;//поворот против часовой
  Game->nast->buttons[0][6]=KEY_DEL;//свернуть
  Game->nast->buttons[0][7]=KEY_RIGHT_SOFT;//вкл/откл звук
  Game->nast->buttons[0][8]=KEY_ESC;//пауза/выход
  Game->nast->buttons[1][0]=KEY_DIGITAL_0+4;//лево
  Game->nast->buttons[1][1]=KEY_DIGITAL_0+6;//право
  Game->nast->buttons[1][2]=KEY_DIGITAL_0+5;//ускорение
  Game->nast->buttons[1][3]=KEY_DIGITAL_0+8;//мгновенный сброс
  Game->nast->buttons[1][4]=KEY_DIGITAL_0+1;//поворот по часовой
  Game->nast->buttons[1][5]=KEY_DIGITAL_0+3;//поворот против часовой
  Game->nast->buttons[1][6]=KEY_STAR;//свернуть
  Game->nast->buttons[1][7]=KEY_DIEZ;//вкл/откл звук
  Game->nast->buttons[1][8]=KEY_DIGITAL_0;//пауза/выход
}
