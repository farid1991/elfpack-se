#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "headerReader.h"
#include "main.h"
#include "header.hpp"
#include "config_data.h"
#include "MediaInfo.h"

int headerReader(wchar_t * path, wchar_t * fname) //Читаем header
{
  MP3Header mp3hdr;
  int oldfont;
  hasheaders=false;

  if(frequencyID != EMPTY_TEXTID) TextID_Destroy(frequencyID);
  if(bitrateID != EMPTY_TEXTID) TextID_Destroy(bitrateID);
  if(channelsID != EMPTY_TEXTID) TextID_Destroy(channelsID);

  frequencyID = bitrateID = channelsID = EMPTY_TEXTID;
  frequencyWidth = bitrateWidth = channelsWidth = 0;
  frequencyX = bitrateX = channelsX = 0;

  if(fname == 0) return 0;
  if(path == 0) return 0;

  mp3hdr.ReadMP3Information(path, fname);
  if(mp3hdr.hasHeader)
  {
    frequencyID = TextID_Create(mp3hdr.wstrFrequency,ENC_UCS2,wstrlen(mp3hdr.wstrFrequency));
    bitrateID = TextID_Create(mp3hdr.wstrBitRate,ENC_UCS2,wstrlen(mp3hdr.wstrBitRate));
    channelsID = TextID_Create(mp3hdr.wstrMode,ENC_UCS2,wstrlen(mp3hdr.wstrMode));

    if(cfg_player_frequency_scroll)
    {
      oldfont = SetFont(cfg_player_frequency_font);
      frequencyWidth = Disp_GetTextIDWidth(frequencyID,TextID_GetLength(frequencyID));
      SetFont(oldfont);
    }

    if(cfg_player_bitrate_scroll)
    {
      oldfont = SetFont(cfg_player_bitrate_font);
      bitrateWidth = Disp_GetTextIDWidth(bitrateID,TextID_GetLength(bitrateID));
      SetFont(oldfont);
    }

    if(cfg_player_channels_scroll)
    {
      oldfont = SetFont(cfg_player_channels_font);
      channelsWidth = Disp_GetTextIDWidth(channelsID,TextID_GetLength(channelsID));
      SetFont(oldfont);
    }

    hasheaders = true;
    return (1);
  }
  return (0);
}

