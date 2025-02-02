#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "main.h"
#include "tagReader.h"
#include "id3tag.h"
#include "utf16.h"
#include "config_data.h"
#include "MediaInfo.h"

static void FreeAllData()
{
  hastags=false;

  if(nameID != EMPTY_TEXTID) TextID_Destroy(nameID);
  if(albumID != EMPTY_TEXTID) TextID_Destroy(albumID);
  if(artistID != EMPTY_TEXTID) TextID_Destroy(artistID);
  if(yearID != EMPTY_TEXTID) TextID_Destroy(yearID);
  if(genreID != EMPTY_TEXTID) TextID_Destroy(genreID);
  nameID = albumID = artistID = yearID = genreID = EMPTY_TEXTID;
  if(cover_image != NOIMAGE) ImageID_Free(cover_image);
  cover_image = NOIMAGE;

  nameWidth = albumWidth = artistWidth = yearWidth = genreWidth = coverWidth = coverHeight = 0;
  nameX = albumX = artistX = yearX = genreX = 0;
}

void GetNameWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_title_font);
  nameWidth = Disp_GetTextIDWidth(nameID,TextID_GetLength(nameID));
  SetFont(oldfont);
}

static void GetAlbumWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_album_font);
  albumWidth = Disp_GetTextIDWidth(albumID,TextID_GetLength(albumID));
  SetFont(oldfont);
}

static void GetArtistWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_artist_font);
  artistWidth = Disp_GetTextIDWidth(artistID,TextID_GetLength(artistID));
  SetFont(oldfont);
}

static void GetYearWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_year_font);
  yearWidth = Disp_GetTextIDWidth(yearID,TextID_GetLength(yearID));
  SetFont(oldfont);
}

static void GetGenreWidth()
{
  int oldfont;
  oldfont = SetFont(cfg_player_genre_font);
  genreWidth = Disp_GetTextIDWidth(genreID,TextID_GetLength(genreID));
  SetFont(oldfont);
}

static void GetCoverSize()
{
  int Width  = GetImageWidth(cover_image);
  int Height = GetImageHeight(cover_image);
  int Width0  = cfg_player_cover.x2 - cfg_player_cover.x1;
  int Height0 = cfg_player_cover.y2 - cfg_player_cover.y1;
  if((coverWidth = (Width * Height0) / Height) <= Width0)
  {
    coverHeight = Height0;
  }
  else
  {
    coverWidth  = Width0;
    coverHeight = (Height * Width0) / Width;
  }
}

static void GetExtCover(wchar_t * path)
{
  FSTAT _fstat;
  if(fstat(path,cover_file,&_fstat)==0)
  {
    if(ImageID_Get(path,cover_file,&cover_image)<0)
    {
      cover_image = NOIMAGE;
    }
    else
    {
      GetCoverSize();
    }
  }
}

int tagReader(wchar_t * path, wchar_t * fname) //Читаем тэги ID3
{
  struct id3_file *id3file = 0;
  struct id3_tag *id3tag = 0;
  struct id3_frame const *frame = 0;
  id3_ucs4_t const *ucs4 = 0;
  id3_utf16_t *utf16 = 0;
  union id3_field const *field = 0;

  FreeAllData();

  if(fname == 0) return 0;
  if(path == 0) return 0;

  id3file = id3_file_open(path, fname, ID3_FILE_MODE_READONLY);

  if(id3file)
  {
    id3tag = id3_file_tag(id3file);
    // Title
    if(cfg_player_title_show)
    {
      if(frame = id3_tag_findframe(id3tag, ID3_FRAME_TITLE, 0))
      {
        if(field = id3_frame_field(frame, 1))
        {
          if(ucs4 = id3_field_getstrings(field, 0))
          {
            if(utf16 = id3_ucs4_utf16duplicate(ucs4))
            {
              nameID = TextID_Create(utf16,ENC_UCS2,id3_utf16_size(utf16));
              mfree(utf16);
              if(cfg_player_title_scroll) GetNameWidth();
            }
          }
        }
      }
      else
      {
        nameID = TextID_Create(fname,ENC_UCS2,wstrlen(fname));
        if(cfg_player_title_scroll) GetNameWidth();
      }
    }
    // Album
    if(cfg_player_album_show)
    {
      if(frame = id3_tag_findframe(id3tag, ID3_FRAME_ALBUM, 0))
      {
        if(field = id3_frame_field(frame, 1))
        {
          if(ucs4 = id3_field_getstrings(field, 0))
          {
            if(utf16 = id3_ucs4_utf16duplicate(ucs4))
            {
              albumID = TextID_Create(utf16,ENC_UCS2,id3_utf16_size(utf16));
              mfree(utf16);
              if(cfg_player_album_scroll) GetAlbumWidth();
            }
          }
        }
      }
    }
    // Artist
    if(cfg_player_artist_show)
    {
      if(frame = id3_tag_findframe(id3tag, ID3_FRAME_ARTIST, 0))
      {
        if(field = id3_frame_field(frame, 1))
        {
          if(ucs4 = id3_field_getstrings(field, 0))
          {
            if(utf16 = id3_ucs4_utf16duplicate(ucs4))
            {
              artistID = TextID_Create(utf16,ENC_UCS2,id3_utf16_size(utf16));
              mfree(utf16);
              if(cfg_player_artist_scroll) GetArtistWidth();
            }
          }
        }
      }
    }
    // Year
    if(cfg_player_year_show)
    {
      if(frame = id3_tag_findframe(id3tag, ID3_FRAME_YEAR, 0))
      {
        if(field = id3_frame_field(frame, 1))
        {
          if(ucs4 = id3_field_getstrings(field, 0))
          {
            if(utf16 = id3_ucs4_utf16duplicate(ucs4))
            {
              yearID = TextID_Create(utf16,ENC_UCS2,id3_utf16_size(utf16));
              mfree(utf16);
              if(cfg_player_year_scroll) GetYearWidth();
            }
          }
        }
      }
    }
    // Genre
    if(cfg_player_genre_show)
    {
      if(frame = id3_tag_findframe(id3tag, ID3_FRAME_GENRE, 0))
      {
        if(field = id3_frame_field(frame, 1))
        {
          if(ucs4 = id3_field_getstrings(field, 0))
          {
            if(ucs4 = id3_genre_name(ucs4))
            {
              if(utf16 = id3_ucs4_utf16duplicate(ucs4))
              {
                genreID = TextID_Create(utf16,ENC_UCS2,id3_utf16_size(utf16));
                mfree(utf16);
                if(cfg_player_genre_scroll) GetGenreWidth();
              }
            }
          }
        }
      }
    }
    // Cover
    if(cfg_player_cover_show)
    {
      id3_length_t len = 0;
      id3_byte_t const *coverart = 0;
      int i;
      // "Attached picture"
      for(i = 0; (frame = id3_tag_findframe(id3tag, "APIC", i)) != 0; i++)
      {
        union id3_field *field = id3_frame_field (frame, 2);
        if(field)
        {
          int pictype = id3_field_getint(field);
          /* We'll prefer type 3 (cover) over type 0 (other) */
          if (pictype == 3)
          {
            // Get the picture data
            coverart = id3_field_getbinarydata(id3_frame_field(frame, 4), &len);
            break;
          }
          if ((pictype == 0) && !coverart)
          {
            // Get the picture data
            coverart = id3_field_getbinarydata(id3_frame_field(frame, 4), &len);
            break;
          }
        }
      }
      if(coverart)
      {
        int pic_file;
        if ((pic_file=_fopen(GetDir(DIR_INI),L"cover.png",FSX_O_WRONLY,FSX_S_IREAD|FSX_S_IWRITE,0)) >= 0)
        {
          fwrite(pic_file,coverart,len);
          fclose(pic_file);
          if(ImageID_Get(GetDir(DIR_INI),L"cover.png",&cover_image)<0)
          {
            cover_image = NOIMAGE;
          }
          else
          {
            GetCoverSize();
          }
        }
      }
      if(cover_image == NOIMAGE) GetExtCover(path);
    }

    hastags=true;
    id3_file_close(id3file);
    return (1);
  }
  return (0);
}

int tagReaderInt(wchar_t * path, wchar_t * fname) //Читаем тэги ID3
{
  FreeAllData();

  if(fname == 0) return 0;
  if(path == 0) return 0;

  void *tags = 0;
  if(tags = MetaData_Desc_Create(path,fname))
  {
    wchar_t *tag;
    // Title
    if(cfg_player_title_show)
    {
      if (tag = MetaData_Desc_GetTags(tags,1))
      {
        if (tag[0]!=0)
        {
          nameID = TextID_Create(tag,ENC_UCS2,wstrlen(tag));
          if(cfg_player_title_scroll) GetNameWidth();
        }
        else
        {
          nameID = TextID_Create(fname,ENC_UCS2,wstrlen(fname));
          if(cfg_player_title_scroll) GetNameWidth();
        }
      }
      else
      {
        nameID = TextID_Create(fname,ENC_UCS2,wstrlen(fname));
        if(cfg_player_title_scroll) GetNameWidth();
      }
    }
    // Album
    if(cfg_player_album_show)
    {
      if (tag=MetaData_Desc_GetTags(tags,2))
      {
        if (tag[0]!=0)
        {
          albumID = TextID_Create(tag,ENC_UCS2,wstrlen(tag));
          if(cfg_player_album_scroll) GetAlbumWidth();
        }
      }
    }
    // Artist
    if(cfg_player_artist_show)
    {
      if (tag=MetaData_Desc_GetTags(tags,0))
      {
        if (tag[0]!=0)
        {
          artistID = TextID_Create(tag,ENC_UCS2,wstrlen(tag));
          if(cfg_player_artist_scroll) GetArtistWidth();
        }
      }
    }
    // Year
    if(cfg_player_year_show)
    {
      if (tag=MetaData_Desc_GetTags(tags,3))
      {
        if (tag[0]!=0)
        {
          yearID = TextID_Create(tag,ENC_UCS2,wstrlen(tag));
          if(cfg_player_year_scroll) GetYearWidth();
        }
      }
    }
    // Genre
    if(cfg_player_genre_show)
    {
      if (tag=MetaData_Desc_GetTags(tags,4))
      {
        if (tag[0]!=0)
        {
          genreID = TextID_Create(tag,ENC_UCS2,wstrlen(tag));
          if(cfg_player_genre_scroll) GetGenreWidth();
        }
      }
    }
    // Cover
    if(cfg_player_cover_show)
    {
      char type;
      int len;
      int offset;
      if (MetaData_Desc_GetCoverInfo(tags,&type,&len,&offset)!=0)
      {
        int f;
        if((f = _fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0x0)) >= 0)
        {
          if (lseek(f,offset,0)>=0)
          {
            char *buf=new char[len+1];
            fread(f,buf,len);

            int pic_file;
            if ((pic_file = _fopen(GetDir(DIR_INI),L"cover.png",FSX_O_WRONLY,FSX_S_IREAD|FSX_S_IWRITE,0)) >= 0)
            {
              fwrite(pic_file,buf,len);
              fclose(pic_file);
              if(ImageID_Get(GetDir(DIR_INI),L"cover.png",&cover_image) < 0)
              {
                cover_image = NOIMAGE;
              }
              else
              {
                GetCoverSize();
              }
            }
            delete(buf);
          }
          fclose(f);
        }
      }
      if(cover_image == NOIMAGE) GetExtCover(path);
    }

    MetaData_Desc_Destroy(tags);

    hastags=true;
    return (1);
  }
  return (0);
}
