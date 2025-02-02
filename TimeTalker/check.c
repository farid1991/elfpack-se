#include "..\include\Lib_Clara.h"
#include "check.h"
#include "main.h"
#include "config_data.h"

int CheckForAutoPlay(void)
{
	if(AP_CAMERA && FindBook(get_IsCameraBook())) return 1;
	if(AP_MPLAYER && FindBook(get_IsAudioPlayerBook())) return 1;
	if(AP_VPLAYER && FindBook(get_IsMediaPlayerVideoBook())) return 1;
	if(AP_FMRADIO && FindBook(get_IsFmRadioBook())) return 1;
	if(AP_SRECODER && FindBook(get_IsSoundRecorderBook())) return 1;
	if(AP_SILENCE && GetSilent()) return 1;
  	return 0;
}

int CheckForPlay(void)
{
	if(CAMERA && FindBook(get_IsCameraBook())) return 1;
	if(MPLAYER && FindBook(get_IsAudioPlayerBook())) return 1;
	if(VPLAYER && FindBook(get_IsMediaPlayerVideoBook())) return 1;
	if(FMRADIO && FindBook(get_IsFmRadioBook())) return 1;
	if(SRECODER && FindBook(get_IsSoundRecorderBook())) return 1;
	if(SILENCE && GetSilent()) return 1;
  	return 0;
}

int CheckForSleepMode(char h, char m)
{
	if (SLEEP)
	{
		char h1 = SLEEP_START.hour;
		char m1 = SLEEP_START.min;
		char h2 = SLEEP_END.hour;
		char m2 = SLEEP_END.min;
		if (h1<h2)
		{
			if (h1==h && m1<=m) return 1;
			if (h1<h && h<h2) return 1;
			if (h==h2 && m<=m2) return 1;
		}
		if (h1>h2)
		{
			if (h1==h && m1<=m) return 1;
			if (h1<h || h<h2) return 1;
			if (h==h2 && m<=m2) return 1;
		}
		if (h1==h2)
		{
			if (m1<m2)
			{
				if (h1==h && m1<=m && m<=m2) return 1;
			}
			if (m1>m2)
			{
				if (!(h1==h && m1<m && m<m2)) return 1;
			}
			if (m1==m2)
			{
				if (h1==h) return 1;
			}
		}
	}
	return 0;
}
