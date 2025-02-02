//#include "mem2.h"
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "Menu.h"
#include "StructBook.h"
#include "Rec_Check.h"
#include "OpenNastroiki.h"
#include "Menu.h"
#include "ErrorIAR.h"
#include "MyFUNC.h"
#include "Game.h"
#include "Pages.h"

MyBook*Game;

MyBook * GetBookID()
{
  return Game;
}



#pragma segment="ELFBEGIN"
void elf_exit(void){
  //trace_done();
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}

static void onMyBookClose(BOOK * bk)
{
  SaveSettings(((MyBook*)bk)->nast->sound, ((MyBook*)bk)->nast->im, ((MyBook*)bk)->nast->rez, ((MyBook*)bk)->nast->coloron);
  ImagesDel(bk);
  SUBPROC(elf_exit);
}

int main(wchar_t*elfname,wchar_t*path,wchar_t*name)
{
  //trace_init();
  if((BOOK*)MyBK())
  {
    BookObj_SetFocus((BOOK*)MyBK(),0);
    SUBPROC(elf_exit);
  }
  else
  {
    int i;
    Game=new MyBook;
    memset(Game,0,sizeof(MyBook));
    CreateBook((BOOK*)Game,onMyBookClose,&bk_base,MyElf,-1,0);
    Game->guido=0;
    Game->gui=0;
    Game->image=new IMAGES;
    for(i=0; i<7; i++)
    {
      Game->image->im[i]=new IMG;
      Game->image->fi[i]=0;
    }
    Game->lvl=0;
    Game->speed=0;
    Game->point=0;
    Game->nast=new NASTROIKI;
    Game->nast->im=new IMGN;
    Game->nast->sound=new SOUND;
    Game->nast->rez=new TAB;
    for(i=0; i<6; i++) Game->nast->sound->status[i]=1;
    Game->nast->sound->sound=1;
    i=0;
    wstrcpy(Game->nast->sound->names[i], L"start.mp3");
    i++;
    wstrcpy(Game->nast->sound->names[i], L"OK.mp3");
    i++;
    wstrcpy(Game->nast->sound->names[i], L"gameover.mp3");
    i++;
    wstrcpy(Game->nast->sound->names[i], L"newrecord.mp3");
    i++;
    wstrcpy(Game->nast->sound->names[i], L"pause.mp3");
    i++;
    wstrcpy(Game->nast->sound->names[i], L"hide.mp3");
    i=0;
    wstrcpy(Game->nast->im->names[i], L"fon.png");
    i++;
    wstrcpy(Game->nast->im->names[i], L"fongame.png");
    i++;
    wstrcpy(Game->nast->im->names[i], L"cub.png");
    i++;
    wstrcpy(Game->nast->im->names[i], L"cub.png");
    i++;
    wstrcpy(Game->nast->im->names[i], L"cub.png");
    i++;
    wstrcpy(Game->nast->im->names[i], L"cub.png");
    i++;
    wstrcpy(Game->nast->im->names[i], L"nextcub.png");
    Game->nast->rez->te=new TABL;
    Game->megaspeed=1;
    Game->nast->im->on=0;
    Game->nast->rez->tn=new TABL;
    Game->nast->rez->th=new TABL;
    for(i=0; i<10; i++)
    {
      Game->nast->rez->te->rezultat[i]=0;
      Game->nast->rez->tn->rezultat[i]=0;
      Game->nast->rez->th->rezultat[i]=0;
      wstrcpy(Game->nast->rez->te->name[i], L"Unknow");
      wstrcpy(Game->nast->rez->tn->name[i], L"Unknow");
      wstrcpy(Game->nast->rez->th->name[i], L"Unknow");
    }
    Game->dis[0]=new DISPLAY;
    Game->dis[0]->x=176;
    Game->dis[0]->y=220;
    Game->dis[0]->cub=10;
    Game->dis[0]->FP=FONT_E_30R;
    Game->dis[0]->FS=FONT_E_12R;
    Game->dis[1]=new DISPLAY;
    Game->dis[1]->x=240;
    Game->dis[1]->y=320;
    Game->dis[1]->cub=14;
    Game->dis[1]->FP=FONT_E_40R;
    Game->dis[1]->FS=FONT_E_14R;
    Game->dis[2]=new DISPLAY;
    Game->dis[2]->x=128;
    Game->dis[2]->y=160;
    Game->dis[2]->cub=7;
    Game->dis[2]->FP=FONT_E_18R;
    Game->dis[2]->FS=FONT_D_7R;
    Game->pause=0;
    Game->gtimer=0;
    Game->iptimer=0;
    Game->nast->coloron=0;
    int k=0;
    Game->nast->colors[k]=0xff000000;
    k++;
    Game->nast->colors[k]=0xffff0000;
    k++;
    Game->nast->colors[k]=0xff00ff00;
    k++;
    Game->nast->colors[k]=0xff0000ff;
    k++;
    Game->nast->colors[k]=0xffffffff;
    k++;
    Game->nast->colors[k]=0xff000000;
    k++;
    Game->nast->colors[k]=0xffffffff;
    k++;
    Game->nast->colors[k]=0xffff0000;
    DefaultButtons(Game);
    MyBK()->r=new RECT;
    OpenSettings(Game);
    CheckResurs(Game);
    if(path && name)
    {
      wstrcpy(Game->path, path);
      wstrcpy(Game->name, name);
      BookObj_GotoPage((BOOK *)Game,&bk_gs);
    }
    else BookObj_GotoPage((BOOK *)Game,&bk_gm);
  }
  return 0;
}
