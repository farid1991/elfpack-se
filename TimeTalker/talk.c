#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "config_data.h"
#include "check.h"
#include "main.h"

wchar_t **sfile;
int q=0,i=0;
int ti[10];

typedef struct {
	char id_riff[4];
        unsigned int len_riff;

	char id_chuck[4];
	char fmt[3];
	char _space;
	unsigned int len_chuck;

	u16  type;
	u16  channels;
	unsigned int freq;
	unsigned int bytes;
	u16  align;
	u16  bits;

	char id_data[4];
	unsigned int len_data;
} TitleWave;

int wavlength(wchar_t * name)
{
	FSTAT _fstat;
	if (fstat(DIRSOUNDS,name,&_fstat)!=-1)
	{
		int file;
		if ((file=_fopen(DIRSOUNDS,name,0x001,0x180,0))>=0)
		{
			static TitleWave  tw;
			fread(file,&tw,sizeof(tw));
			fclose(file);
			if (!strncmp(tw.id_riff,"RIFF",4) && !strncmp(tw.id_chuck,"WAVEfmt",7) && !strncmp(tw.id_data,"data",4))
			{
				double d = tw.len_data * 1000 / tw.freq / tw.channels;
				return (int)d+1;
			}
		}
	}
	return -1;
}


void onTtimer (u16  unk , void * data)
{
	MyBOOK * book = (MyBOOK *) data;
	if (i<=q)
	{
		if (VOLUME)
		{
  			PlayFileV(DIRSOUNDS,sfile[i],VOLUME);
		}
		else
		{
			PlayFile(DIRSOUNDS,sfile[i]);
		}
		int tt = DELAY;
		if (ti[i]!=-1)
		{
			tt = ti[i];
		}
		TIMER_RESET(&(book->Ttimer),tt,onTtimer,data);
		i++;
	}
	else
	{
		for (int j=0; j<=q; j++)
		{
			delete sfile[j];
		}
		delete sfile;
		i=0;
		book->play=0;
	}
}

wchar_t * print(int n)
{
  	int l = wstrlen(EXTENTION) + 3;
	wchar_t * wstr = new wchar_t[l+1];
  	MyBOOK * myBook = (MyBOOK *) FindBook(isTT);
  	snwprintf(wstr, l, L"%02d.%ls", n, EXTENTION);
  	return(wstr);
}

int ge(int n)
{
	if ((n/10)==1) return 0;
	switch(n%10)
	{
	case 1: return 1;
	case 2: return 2;
	case 3: return 2;
	case 4: return 2;
	default: return 0;
	}
}

void TalkTime(MyBOOK * myBook)
{
	if (!(myBook->play))
  	{
    		myBook->play=1;
    		sfile = new wchar_t* [16];
    		int h,m,j=0;
    		DATETIME dt;
    		REQUEST_DATEANDTIME_GET(0,&dt);
    		h=dt.time.hour;
    		m=dt.time.min;

    		if (h<21) sfile[j]=print(h);
    		if (h>20)
    		{
      			sfile[j]=print(20); j++;
      			sfile[j]=print(h%10);
    		}
		j++;
    		sfile[j]=print(ge(h)+80);
    		j++;
    		if (m<21)
    		{
      			if (m==1) sfile[j]=print(31);
      			if (m==2) sfile[j]=print(32);
      			if (m!=1 && m!=2) sfile[j]=print(m);
			j++;
    		}
    		if (m>20)
    		{
      			sfile[j] = print((m/10)*10); j++;
      			if (m%10==1) {sfile[j]=print(31); j++;}
      			if (m%10==2) {sfile[j]=print(32); j++;}
      			if (m%10!=1 && m%10!=2 && m%10!=0) {sfile[j]=print(m%10); j++;}
    		}
    		sfile[j]=print(ge(m)+90);
   	 	i=0;
    		q=j;
		for (int p=0; p<=j; p++)
		{
			ti[p] = wavlength(sfile[p]);
		}
    		myBook->Ttimer=TIMER_SET(0,onTtimer,myBook);
  	}
}
/*
void TalkTime(void)
{
  	//TalkTimeRU();
	MessageBox(0,STR("-=***=-"),0,1,0,0);
}
*/

// { !!! Таймер !!! }

void onAPtimer (u16  unk , void * data)
{
  	TIMER_RESET(&APtimer,time[REPEAT]*1000,onAPtimer,data);
	DATETIME dt;
  	REQUEST_DATEANDTIME_GET(0,&dt);
      	if (!CheckForSleepMode(dt.time.hour,dt.time.min))
      	{
		if (!CheckForAutoPlay())
        	{
          		TalkTime((MyBOOK*)data);
        	}
      	}
}

int NewKey(int key, int rep_count, int mode, MyBOOK*, DISP_OBJ*)
{
  	if ((KEYUSE1 && mode==KEYMODE1 && key==KEY1) || (KEYUSE2 && mode==KEYMODE2 && key==KEY2))
	{
		if (CheckForPlay()==0)
		{
			TalkTime((MyBOOK *)FindBook(isTT));
			return(-1);
		}
	}
	return(0);
}


/*
void TalkTimeP(void)
{
  MyBOOK * myBook = (MyBOOK *) FindBook(isTTBook);
  if (!(myBook->IsPlaying))
  {
    myBook->IsPlaying=1;
    sfile = new u16* [16];
    for(int n=0; n<16; n++)
    sfile[n] = new u16[8];
    int h,m,j=0;
    DATETIME dt;
    REQUEST_DATEANDTIME_GET(0,&dt);
    h=dt.time.hour;
    m=dt.time.min;

    if (h<21) wstrcpy(sfile[j],print(h));
    if (h>20)
    {
      wstrcpy(sfile[j],print(20)); j++;
      wstrcpy(sfile[j],print(41)); j++;
      wstrcpy(sfile[j],print(h%10));
    }j++;
    if (h==0 || h==1) wstrcpy(sfile[j],print(81));
    else wstrcpy(sfile[j],print(82));
    if (m)
    {
      j++;
      wstrcpy(sfile[j],print(41)); j++;
      if (m<21)
      {
        if (m==1) wstrcpy(sfile[j],print(31));
        if (m==2) wstrcpy(sfile[j],print(32));
        if (m!=1 && m!=2) wstrcpy(sfile[j],print(m));
      }
      if (m>20)
      {
        wstrcpy(sfile[j],print((m/10)*10));j++;
        if (m%10!=0) wstrcpy(sfile[j],print(41)); j++;
        if (m%10==1) wstrcpy(sfile[j],print(31));
        if (m%10==2) wstrcpy(sfile[j],print(32));
        if (m%10!=1 && m%10!=2 && m%10!=0) wstrcpy(sfile[j],print(m%10));
      }j++;
      if (m==1) wstrcpy(sfile[j],print(91));
      else wstrcpy(sfile[j],print(92));
    }
    i=0;
    q=j;
    myBook->Ttimer=Timer_Set(0,onTtimer,0);
    //for (int m=0;m<=j;m++)
   //   MessageBox(0,TextID_Create(sfile[m],0,6),0,1,0,0);
  }
}
*/
