#include "mydisp.h"

#define UPDATE_TIME 1000/25
#define I_SID(a) TextID_CreateIntegerID(a);

wchar_t const *ImgNames[ImgCount]={L"Background.png",L"ProgressBar.png",L"NoCover.png"};

int MOD(int a)
{
	return(a>0?a:-a);
}

int W_SID(wchar_t *s)
{
	int i,len=wstrlen(s);
	if(WIN1251_SUPPORT)//На случай	WIN1251
	{
		for(i=0;i<len;i++)
		{
			if(s[i]>=0xC0 && s[i]<=0xFF) s[i]+=0x350; //Рус.
			else if(s[i]==0xA8) s[i]=0x401; //Ё
			else if(s[i]==0xB8) s[i]=0x451;
			//Бел.
			else if(s[i]==0xA1) s[i]=0x40E;//Ў
			else if(s[i]==0xA2) s[i]=0x45E;
			else if(s[i]==0xB2) s[i]=0x406;//І
			else if(s[i]==0xB3) s[i]=0x456;
			//Укр.
			else if(s[i]==0xAA) s[i]=0x404; //Є
			else if(s[i]==0xBA) s[i]=0x454;
			else if(s[i]==0xAF) s[i]=0x407; //Ї
			else if(s[i]==0xBF) s[i]=0x457;
		}
	}
	return(TextID_Create(s,ENC_UCS2,len));
}

char* CMyDisp::getName()
{
	return "PlayerPanelDisp";
}

void CMyDisp::TrackInfoInit()
{
	audiobook=FindBook(get_IsAudioPlayerBook());
	track_desc=TrackDesc_Get(audiobook);
	mdata = new CMetaData(track_desc->path,track_desc->name);
	HEADER_INFO header_info = ReadMP3Information(track_desc->path,track_desc->name);

	strid[ARTIST]     = W_SID(mdata->Artist());
	strid[ALBUM]      = W_SID(mdata->Album());
	wchar_t *title    = mdata->Title();
	strid[TITLE]      = W_SID(title ? title : track_desc->name);
	strid[YEAR]       = W_SID(mdata->Year());
	strid[GENRE]      = W_SID(mdata->Genre());
	strid[CHANNEL]    = W_SID(GetChannel(header_info.mode));

	wchar_t bitrate[64];
	snwprintf(bitrate,63,BITRATE_MASK,header_info.bitrate);
	strid[BITRATE] = W_SID(bitrate);

	wchar_t frequency[64];
	snwprintf(frequency,63,FREQUENCY_MASK,header_info.frequency);
	strid[FREQUENCY] = W_SID(frequency);

	int currenttime = *track_desc->time;
	int left_time=(track_desc->fulltime) - currenttime;

	wchar_t current_time[64];
	snwprintf(current_time,63,CURRENT_TIME_MASK,currenttime/60,currenttime%60);
	strid[CURRENT_TIME] = W_SID(current_time);

	wchar_t lefttime[64];
	snwprintf(lefttime,63,LEFT_TIME_MASK,left_time/60,left_time%60);
	strid[LEFT_TIME] = W_SID(lefttime);

	wchar_t full_time[64];
	snwprintf(full_time,63,FULL_TIME_MASK,track_desc->fulltime/60,track_desc->fulltime%60);
	strid[FULL_TIME] = W_SID(full_time);

	wchar_t track_count[64];
	snwprintf(track_count,63,TRACK_COUNT_MASK,track_desc->tracks_count);
	strid[TRACK_COUNT] = W_SID(frequency);

	wchar_t current_track[64];
	snwprintf(current_track,63,CURRENT_TRACK_MASK,track_desc->pos);
	strid[CURRENT_TRACK] = W_SID(current_track);
	cover=mdata->Cover();
}

void CMyDisp::onRefresh()
{
	CGuiBase* gui = GetGUI();
	gui->GetBook()->UI_Event(CANCEL_EVENT);
}

void CMyDisp::TrackInfoDestroy()
{
	for(int k=0;k<strid_count;k++)
	{
		if(strid[k]!=EMPTY_TEXTID)
		{
			TextID_Destroy(strid[k]);
			strid[k]=EMPTY_TEXTID;
		}
	}
	delete mdata;
	TrackDesc_Free(track_desc);
}

int CMyDisp::onCreate()
{
	timer=0;
	SetLayerColor(BACKGROUND_COLOR);
	for (int k=0;k<ImgCount;k++) ImageID_Get(dir,ImgNames[k],&ImgID[k]);
	for(int k=0;k<strid_count;k++) strid[k]=EMPTY_TEXTID;
	TrackInfoInit();
	if(AUTO_HIDE_TIME) SetRefreshTimer(AUTO_HIDE_TIME);
	return 1;
}

void CMyDisp::onDestroy()
{
	TrackInfoDestroy();
	for (int k=0;k<ImgCount;k++) ImageID_Free(ImgID[k]);
	if(timer) Timer_Kill(&timer);
	KillRefreshTimer();
}

void CMyDisp::OnPlay()
{
	if (audiobook=FindBook(get_IsAudioPlayerBook()))
	{
		TRACK_DESC *new_track_desc=TrackDesc_Get(audiobook);
		if (!TrackDesc_Compare(track_desc,new_track_desc))
		{
			TrackInfoDestroy();
			TrackInfoInit();
			if(isDraw) InvalidateRect(0);
		}
		TrackDesc_Free(new_track_desc);
	}
}

void CMyDisp::OnPlayingTime(PLAY_TIME *play_time)
{
	if(isDraw)
	{
		TextID_Destroy(strid[CURRENT_TIME]);
		TextID_Destroy(strid[LEFT_TIME]);
		int left_time=track_desc->fulltime - *track_desc->time;

		wchar_t current_time[64];
		snwprintf(current_time,63,CURRENT_TIME_MASK,*track_desc->time/60,*track_desc->time%60);
		strid[CURRENT_TIME] = W_SID(current_time);

		wchar_t lefttime[64];
		snwprintf(lefttime,63,LEFT_TIME_MASK,left_time/60,left_time%60);
		strid[LEFT_TIME] = W_SID(lefttime);
		InvalidateRect(0);
	}
}

int MoveXY(int TendsToX,int TendsToY,int * CurrentX,int * CurrentY,int step)
{
	int ret=End;
	int _LenX=*CurrentX-TendsToX;
	int _LenY=*CurrentY-TendsToY;
	int LenX=MOD(_LenX);
	int LenY=MOD(_LenY);
	float tg=(float)LenX/(float)LenY;
	LenY-=step;
	LenX=LenY*tg;
	int SignX=_LenX>0?1:-1;
	int SignY=_LenY>0?1:-1;
	if(LenY>=0)
	{
		ret=Resume;
		*CurrentX=TendsToX + LenX*SignX;
		*CurrentY=TendsToY + LenY*SignY;
	}
	else if(LenY<0)
	{
		ret=End;
		*CurrentX=TendsToX;
		*CurrentY=TendsToY;
	}
	return ret;
}

void CMyDisp::onAnimationTimer(u16 timerID,LPARAM lparam)
{
	CMyDisp *cmd=(CMyDisp*)lparam;
	switch(cmd->AnimType)
	{
		case UP:
		{
			if(cmd->StartY < cmd->WinY){ cmd->StartY+=STEP; }
			else {Timer_Kill(&cmd->timer); cmd->isDraw=true; return;}
			break;
		}
		case DOWN:
		{
			if(cmd->StartY > cmd->WinY){ cmd->StartY-=STEP; }
			else if(cmd->StartY < cmd->WinY){ cmd->StartY++; }
			else {Timer_Kill(&cmd->timer); cmd->isDraw=true; return;}
			break;
		}
		case LEFT:
		{
			if(cmd->StartX < cmd->WinX){ cmd->StartX+=STEP; }
			else if(cmd->StartX > cmd->WinX){ cmd->StartX--; }
			else {Timer_Kill(&cmd->timer); cmd->isDraw=true; return;}
			break;
		}
		case RIGHT:
		{
			if(cmd->StartX > cmd->WinX){ cmd->StartX-=STEP; }
			else if(cmd->StartX < cmd->WinX){ cmd->StartX++; }
			else {Timer_Kill(&cmd->timer); cmd->isDraw=true; return;}
			break;
		}
		case UPLEFT:
		case UPRIGHT:
		case DOWNLEFT:
		case DOWNRIGHT:
		{
			if(cmd->AnimReturn)
			{
				cmd->AnimReturn=MoveXY(cmd->WinX,cmd->WinY,&cmd->StartX,&cmd->StartY,cmd->AnimReturn == Resume ? STEP : 0 );
			}
			else {Timer_Kill(&cmd->timer); cmd->isDraw=true; return;}
			break;
		}
	}
	DispObject_WindowSetPosition(cmd,cmd->StartX,cmd->StartY);
	Timer_ReSet(&cmd->timer,UPDATE_TIME,onAnimationTimer,lparam);
}

void CMyDisp::CreateAnimation(int type,RECT window)
{
	if(!timer)
	{
		CGuiBase* gui = GetGUI();
		DispObject_WindowSetSize(gui->GetDISPObj(),window.x2-window.x1,window.y2-window.y1);
		int ScrWidth=Display_GetWidth(0);
		int ScrHeight=Display_GetHeight(0);
		AnimType=type;
		switch(type)
		{
			case UP: { DispObject_WindowSetPosition(this,StartX=window.x1,StartY=-(window.y2-window.y1)); break;}
			case DOWN:{ DispObject_WindowSetPosition(this,StartX=window.x1,StartY=ScrHeight); break;}
			case LEFT: { DispObject_WindowSetPosition(this,StartX=-(window.x2-window.x1),StartY=window.y1); break;}
			case RIGHT: { DispObject_WindowSetPosition(this,StartX=ScrWidth,StartY=window.y1); break;}
			case UPLEFT: { DispObject_WindowSetPosition(this,StartX=-(window.x2-window.x1),StartY=-(window.y2-window.y1)); break;}
			case UPRIGHT: { DispObject_WindowSetPosition(this,StartX=ScrWidth,StartY=-(window.y2-window.y1)); break;}
			case DOWNLEFT: { DispObject_WindowSetPosition(this,StartX=-(window.x2-window.x1),StartY=ScrHeight); break;}
			case DOWNRIGHT: { DispObject_WindowSetPosition(this,StartX=ScrWidth,StartY=ScrHeight); break;}
		}
		WinX=window.x1;
		WinY=window.y1;
		InvalidateRect(0);
		isDraw=false;
		AnimReturn=Resume;
		timer=Timer_Set(UPDATE_TIME,onAnimationTimer,(LPARAM)this);
	}
}

void CMyDisp::onAnimation2Timer(u16 timerID,LPARAM lparam)
{
	CMyDisp *cmd=(CMyDisp*)lparam;
	switch(cmd->AnimType)
	{
		case UP:
		{
			if(cmd->StartY > cmd->OldStartY){ cmd->StartY-=STEP; }
			else {Timer_Kill(&cmd->timer); cmd->OnDestr(cmd); return;}
			break;
		}
		case DOWN:
		{
			if(cmd->StartY < cmd->OldStartY){ cmd->StartY+=STEP; }
			else {Timer_Kill(&cmd->timer); cmd->OnDestr(cmd); return;}
			break;
		}
		case LEFT:
		{
			if(cmd->StartX > cmd->OldStartX){ cmd->StartX-=STEP; }
			else {Timer_Kill(&cmd->timer); cmd->OnDestr(cmd); return;}
			break;
		}
		case RIGHT:
		{
			if(cmd->StartX < cmd->OldStartX){ cmd->StartX+=STEP; }
			else {Timer_Kill(&cmd->timer); cmd->OnDestr(cmd); return;}
			break;
		}
		case UPLEFT:
		case UPRIGHT:
		case DOWNLEFT:
		case DOWNRIGHT:
		{
			if(cmd->AnimReturn)
			{
				cmd->AnimReturn=MoveXY(cmd->OldStartX,cmd->OldStartY,&cmd->StartX,&cmd->StartY,cmd->AnimReturn == Resume ? STEP : 0 );
			}
			else {Timer_Kill(&cmd->timer); cmd->OnDestr(cmd); return;}
			break;
		}
	}
	DispObject_WindowSetPosition(cmd,cmd->StartX,cmd->StartY);
	Timer_ReSet(&cmd->timer,UPDATE_TIME,onAnimation2Timer,lparam);
}

void CMyDisp::DestroyAnimation(int type,RECT window,void (*OnDestroy)(CMyDisp*))
{
	if(!timer)
	{
		int ScrWidth=Display_GetWidth(0);
		int ScrHeight=Display_GetHeight(0);
		WinX=window.x1;
		WinY=window.y1;
		AnimType=type;
		OnDestr=OnDestroy;
		switch(type)
		{
			case UP: { OldStartX=window.x1; OldStartY=-(window.y2-window.y1); break;}
			case DOWN:{ OldStartX=window.x1; OldStartY=ScrHeight; break;}
			case LEFT: { OldStartX=-(window.x2-window.x1); OldStartY=window.y1; break;}
			case RIGHT: { OldStartX=ScrWidth; OldStartY=window.y1; break;}
			case UPLEFT: { OldStartX=-(window.x2-window.x1); OldStartY=-(window.y2-window.y1); break;}
			case UPRIGHT: { OldStartX=ScrWidth; OldStartY=-(window.y2-window.y1); break;}
			case DOWNLEFT: { OldStartX=-(window.x2-window.x1); OldStartY=ScrHeight; break;}
			case DOWNRIGHT: { OldStartX=ScrWidth; OldStartY=ScrHeight;break;}
		}
		InvalidateRect(0);
		isDraw=false;
		AnimReturn=Resume;
		timer=Timer_Set(UPDATE_TIME,onAnimation2Timer,(LPARAM)this);
	}
}

void CMyDisp::onKey(int key,int,int repeat,int type)
{
	CGuiBase* gui = GetGUI();
	if(key==KEY_ESC && type==KBD_SHORT_PRESS)
	{
		if(gui && gui->GetBook())
		{
			gui->GetBook()->UI_Event(CANCEL_EVENT); return;
		}
	}
	if(type==KBD_SHORT_RELEASE)
	{
		if(key==KEY_LEFT || key==KEY_DIGITAL_0+4){ UI_Event(UI_MEDIAPLAYER_PREV_TRACK_EVENT); return; }
		if(key==KEY_RIGHT || key==KEY_DIGITAL_0+6){ UI_Event(UI_MEDIAPLAYER_NEXT_TRACK_EVENT); return; }
		if(key==KEY_UP || key==KEY_DIGITAL_0+2){ UI_Event(VOLUMEUPKEY_SHORT_PRESS_EVENT); return; }
		if(key==KEY_DOWN || key==KEY_DIGITAL_0+8){ UI_Event(VOLUMEDOWNKEY_SHORT_PRESS_EVENT); return; }
		if(key==KEY_ENTER || key==KEY_DIGITAL_0+5){ BookObj_SetFocus(FindBook(get_IsAudioPlayerBook()),0); return; }
	}
	if(key==KEY_LEFT || key==KEY_DIGITAL_0+4)
	{
		if(type==KBD_LONG_PRESS){ PlayerControl(audiobook,4); KillRefreshTimer();}
		else if(type==KBD_LONG_RELEASE){ PlayerControl(audiobook,5); if(AUTO_HIDE_TIME) SetRefreshTimer(AUTO_HIDE_TIME);}
		OnPlay();
		OnPlayingTime(0);
	}
	if(key==KEY_RIGHT || key==KEY_DIGITAL_0+6)
	{
		if(type==KBD_LONG_PRESS){ PlayerControl(audiobook,2); KillRefreshTimer();}
		else if(type==KBD_LONG_RELEASE){ PlayerControl(audiobook,3); if(AUTO_HIDE_TIME) SetRefreshTimer(AUTO_HIDE_TIME);}
		OnPlay();
		OnPlayingTime(0);
	}
}

void CMyDisp::DrawElement1(DrawStruct1 DrawElems[],int count)
{
  for (int k=0;k<=count;k++)
	{
		if(DrawElems[k].IsEnabled){
		DrawHighlightString(strid[k],DrawElems[k].Font
										,DrawElems[k].Align
										,DrawElems[k].Rect
										,DrawElems[k].NormalColor
										,DrawElems[k].BorderColor,-BACKGROUND_RC.x1,-BACKGROUND_RC.y1);}
	}
}

void CMyDisp::DrawElement2(DrawStruct2 DrawElems[],int count)
{
  for (int k=0;k<=count;k++)
	{
		if(DrawElems[k].IsEnabled){
		DrawHighlightString(strid[k+BITRATE]
										,DrawElems[k].Font
										,DrawElems[k].Align
										,DrawElems[k].Rect
										,DrawElems[k].NormalColor
										,DrawElems[k].BorderColor,-BACKGROUND_RC.x1,-BACKGROUND_RC.y1);}
	}
}

void CMyDisp::onDraw(int a,int b,int c)
{
	CPaintGC gc;
	DrawPicture(BACKGROUND_RC,-BACKGROUND_RC.x1,-BACKGROUND_RC.y1,ImgID[BACKGROUND]);

	if(COVER_EN) DrawPicture(COVER_RC,-BACKGROUND_RC.x1,-BACKGROUND_RC.y1,cover!=NOIMAGE ? cover : ImgID[NOCOVER]);

	if(PROGRESSBAR_EN)
	{
		int time=*track_desc->time;
		int ftime=track_desc->fulltime;
		DrawProgressBar(PROGRESSBAR_TYPE,PROGRESSBAR_RC
					,PROGRESSBAR_IMG_EN ? ImgID[PROGRESSBAR] : NOIMAGE
					,PROGRESSBAR_BACKGROUND_COLOR,PROGRESSBAR_NORMAL_COLOR,PROGRESSBAR_BORDER_COLOR,-BACKGROUND_RC.x1,-BACKGROUND_RC.y1,time*100/ftime);
	}

	DrawElement1((DrawStruct1*)(&ARTIST_EN),CHANNEL-ARTIST);
	DrawElement2((DrawStruct2*)(&BITRATE_EN),CURRENT_TRACK-BITRATE);
}
