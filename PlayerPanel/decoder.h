#ifndef _DECODER_H_
	#define _DECODER_H_

#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"

  typedef struct
  {
    int bitrate;
    bool VBR;
    int LengthInSeconds;
    int lngFileSize;
    int id3size;
    int frequency;
    int mode;
  } HEADER_INFO;

  HEADER_INFO ReadMP3Information(wchar_t *path, wchar_t *name);
  wchar_t * GetChannel(int num);
#endif
