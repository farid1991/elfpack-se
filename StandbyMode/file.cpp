#include "..\\include\Lib_Clara.h"
#include "id3tag.h"
#include "file.h"
#include "tag.h"
#include "field.h"

#ifndef _FPOS_T
#define _FPOS_T
typedef int fpos_t;
#endif /* _FPOS_T */

#ifndef SEEK_SET
#define SEEK_SET  (0x0000)
#define SEEK_CUR  (0x0001)
#define SEEK_END  (0x0002)
#endif

struct filetag {
  struct id3_tag *tag;
  unsigned long location;
  id3_length_t length;
};

struct id3_file {
  int  iofile;

  int flags;

  struct id3_tag *primary;

  unsigned int ntags;
  struct filetag *tags;
};

enum {
  ID3_FILE_FLAG_ID3V1 = 0x0001
};

static void swap_item(char *src,
                      char *dest,
                      size_t size)
{
    while (size--)
    {
        int temp = *src;
        *src++   = *dest;
        *dest++  = temp;
    }
}

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
   char     *basep  = (char *)base; /* POINTER TO ARRAY OF ELEMENTS */
   unsigned  i      = 0;            /* left scan index  */
   unsigned  j      = nmemb - 1;    /* right scan index */
   unsigned  pivot  = (nmemb / 2);
   char     *pivp   = basep + (pivot * size);

   if (nmemb <= 1) return;

   while( i < j )
   {
      while( (*compar) (basep + (i * size), pivp) < 0 ) ++i;
      while( (*compar) (basep + (j * size), pivp) > 0 ) --j;
      if( i < j )
      {
	 swap_item(basep + (i * size), basep + (j * size), size);
	 if     ( pivot == i ) { pivot = j; pivp = basep + (pivot * size); }
	 else if( pivot == j ) { pivot = i; pivp = basep + (pivot * size); }
	 ++i; --j;
      }
      else if ( i == j ) { ++i; --j;  break; }
   }

   if( j > 0)       qsort(basep, j + 1, size, compar);
   if( i < nmemb-1) qsort(basep + (i * size), nmemb - i, size, compar);
}

long ftell(int _fp)
{
   fpos_t  _pos;
   if(_fp < 0) return (-1L);
   _pos = lseek(_fp, 0, SEEK_CUR);
   if(_pos < 0) return (-1L);
   return (_pos);
}

void  rewind(int _fp)
{
  lseek(_fp, 0, SEEK_SET);
}

int fgetpos(int _fp, fpos_t *_pos)
{
   if((*_pos = ftell(_fp)) != -1L) return (0);
   else return (-1);
}

int fsetpos(int _fp, const fpos_t *_pos)
{
   fpos_t pos = lseek(_fp, *_pos, SEEK_SET);
   if(pos >= 0) return (0);
   return (-1);
}

void *(realloc)(void *ptr, size_t size_arg)
{	/* reallocate a data object on the heap */
	if (ptr == 0)
		return (malloc(size_arg));
	if (size_arg == 0)
	{	/* free if zero size */
		mfree(ptr);
		return (0);
	}
	/* try to buy larger cell */
	void *new_p;

	if ((new_p = malloc(size_arg)) == 0) return (0);
	else
	{	/* copy to new cell and free old */
		memcpy(new_p, ptr, size_arg);
		mfree(ptr);
		return (new_p);
	}
}

void *memchr(const void *cs, int c, size_t n)
{
   if (n)
   {
      const unsigned char *mem = (unsigned char *)cs;
      unsigned char        ch  = c;

      do
         if ( *mem == ch ) return (void *)mem;
         else mem++;
      while (--n);
   }
   return NULL;
}

/*
 * NAME:	query_tag()
 * DESCRIPTION:	check for a tag at a file's current position
 */
static
signed long query_tag(int iofile)
{
  fpos_t save_position;
  id3_byte_t query[ID3_TAG_QUERYSIZE];
  signed long size;

  if (fgetpos(iofile, &save_position) == -1)
    return 0;

  fread(iofile, query, sizeof(query));
  size = id3_tag_query(query, sizeof(query));

  if (fsetpos(iofile, &save_position) == -1)
    return 0;

  return size;
}

/*
 * NAME:	read_tag()
 * DESCRIPTION:	read and parse a tag at a file's current position
 */
static
struct id3_tag *read_tag(int iofile, id3_length_t size)
{
  id3_byte_t *data;
  struct id3_tag *tag = 0;

  data = (id3_byte_t *)malloc(size);
  if (data) {
//    if (fread(iofile, data, size) == 1)
      fread(iofile, data, size);
      tag = id3_tag_parse(data, size);

    mfree(data);
  }

  return tag;
}

/*
 * NAME:	update_primary()
 * DESCRIPTION:	update the primary tag with data from a new tag
 */
static
int update_primary(struct id3_tag *tag, struct id3_tag const *_new)
{
  unsigned int i;
  struct id3_frame *frame;

  if (_new) {
    if (!(_new->extendedflags & ID3_TAG_EXTENDEDFLAG_TAGISANUPDATE))
      id3_tag_clearframes(tag);

    i = 0;
    while ((frame = id3_tag_findframe(_new, 0, i++))) {
      if (id3_tag_attachframe(tag, frame) == -1)
	return -1;
    }
  }

  return 0;
}

/*
 * NAME:	tag_compare()
 * DESCRIPTION:	tag sort function for qsort()
 */
static
int tag_compare(const void *a, const void *b)
{
  struct filetag const *tag1 = (struct filetag const *)a, *tag2 = (struct filetag const *)b;

  if (tag1->location < tag2->location)
    return -1;
  else if (tag1->location > tag2->location)
    return +1;

  return 0;
}

/*
 * NAME:	add_filetag()
 * DESCRIPTION:	add a new file tag entry
 */
static
int add_filetag(struct id3_file *file, struct filetag const *filetag)
{
  struct filetag *tags;

  tags = (struct filetag *)realloc(file->tags, (file->ntags + 1) * sizeof(*tags));
  if (tags == 0)
    return -1;

  file->tags = tags;
  file->tags[file->ntags++] = *filetag;

  /* sort tags by location */

  if (file->ntags > 1)
    qsort(file->tags, file->ntags, sizeof(file->tags[0]), tag_compare);

  return 0;
}

/*
 * NAME:	del_filetag()
 * DESCRIPTION:	delete a file tag entry
 */
# if defined(HAVE_FTRUNCATE)
static
void del_filetag(struct id3_file *file, unsigned int index)
{
  assert(index < file->ntags);

  while (index < file->ntags - 1) {
    file->tags[index] = file->tags[index + 1];
    ++index;
  }

  --file->ntags;
}
#endif

/*
 * NAME:	add_tag()
 * DESCRIPTION:	read, parse, and add a tag to a file structure
 */
static
struct id3_tag *add_tag(struct id3_file *file, id3_length_t length)
{
  long location;
  unsigned int i;
  struct filetag filetag;
  struct id3_tag *tag;

  location = ftell(file->iofile);
  if (location == -1) return 0;

  /* check for duplication/overlap */
  {
    unsigned long begin1, end1, begin2, end2;

    begin1 = location;
    end1   = begin1 + length;

    for (i = 0; i < file->ntags; ++i) {
      begin2 = file->tags[i].location;
      end2   = begin2 + file->tags[i].length;

      if (begin1 == begin2 && end1 == end2)
	return file->tags[i].tag;  /* duplicate */

      if (begin1 < end2 && end1 > begin2)
	return 0;  /* overlap */
    }
  }

  tag = read_tag(file->iofile, length);

  filetag.tag      = tag;
  filetag.location = location;
  filetag.length   = length;

  if (add_filetag(file, &filetag) == -1 ||
      update_primary(file->primary, tag) == -1) {
    if (tag)
      id3_tag_delete(tag);
    return 0;
  }

  if (tag)
    id3_tag_addref(tag);

  return tag;
}

/*
 * NAME:	search_tags()
 * DESCRIPTION:	search for tags in a file
 */
static
int search_tags(struct id3_file *file)
{
  fpos_t save_position;
  signed long size;

  /*
   * save the current seek position
   *
   * We also verify the stream is seekable by calling fsetpos(), since
   * fgetpos() alone is not reliable enough for this purpose.
   *
   * [Apparently not even fsetpos() is sufficient under Win32.]
   */

  if (fgetpos(file->iofile, &save_position) == -1 ||
      fsetpos(file->iofile, &save_position) == -1)  return -1;

  /* look for an ID3v1 tag */

  if (lseek(file->iofile, -128, SEEK_END) >= 0) {
    size = query_tag(file->iofile);
    if (size > 0) {
      struct id3_tag const *tag;

      tag = add_tag(file, size);

      /* if this is indeed an ID3v1 tag, mark the file so */

      if (tag && (ID3_TAG_VERSION_MAJOR(id3_tag_version(tag)) == 1))
	file->flags |= ID3_FILE_FLAG_ID3V1;
    }
  }

  /* look for a tag at the beginning of the file */

  rewind(file->iofile);

  size = query_tag(file->iofile);
  if (size > 0) {
    struct id3_tag const *tag;
    struct id3_frame const *frame;

    tag = add_tag(file, size);

    /* locate tags indicated by SEEK frames */

    while (tag && (frame = id3_tag_findframe(tag, "SEEK", 0))) {
      long seek;

      seek = id3_field_getint(id3_frame_field(frame, 0));
      if (seek < 0 || lseek(file->iofile, seek, SEEK_CUR) < 0 )
	break;

      size = query_tag(file->iofile);
      tag  = (size > 0) ? add_tag(file, size) : 0;
    }
  }

  /* look for a tag at the end of the file (before any ID3v1 tag) */

  if (lseek(file->iofile, ((file->flags & ID3_FILE_FLAG_ID3V1) ? -128 : 0) +
	    -10, SEEK_END) >= 0) {
    size = query_tag(file->iofile);
    if (size < 0 && lseek(file->iofile, size, SEEK_CUR) >= 0) {
      size = query_tag(file->iofile);
      if (size > 0)
	add_tag(file, size);
    }
  }

  /* restore seek position */

  if (fsetpos(file->iofile, &save_position) == -1)
    return -1;

  /* set primary tag options and target padded length for convenience */

  if ((file->ntags > 0 && !(file->flags & ID3_FILE_FLAG_ID3V1)) ||
      (file->ntags > 1 &&  (file->flags & ID3_FILE_FLAG_ID3V1))) {
    if (file->tags[0].location == 0)
      id3_tag_setlength(file->primary, file->tags[0].length);
    else
      id3_tag_options(file->primary, ID3_TAG_OPTION_APPENDEDTAG, ~0);
  }

  return 0;
}

/*
 * NAME:	finish_file()
 * DESCRIPTION:	release memory associated with a file
 */
static
void finish_file(struct id3_file *file)
{
  if(!file) return;

  unsigned int i;

  if (file->primary) {
    id3_tag_delref(file->primary);
    id3_tag_delete(file->primary);
  }

  for (i = 0; i < file->ntags; ++i) {
    struct id3_tag *tag;

    tag = file->tags[i].tag;
    if (tag) {
      id3_tag_delref(tag);
      id3_tag_delete(tag);
    }
  }

  if (file->tags)
    mfree(file->tags);

  mfree(file);
}

/*
 * NAME:	new_file()
 * DESCRIPTION:	create a new file structure and load tags
 */
static
struct id3_file *new_file(int iofile, wchar_t const *path, wchar_t const *name)
{
  struct id3_file *file;

  file = (struct id3_file *)malloc(sizeof(*file));
  if (file == 0)
    goto fail;

  file->iofile  = iofile;

  file->flags   = 0;

  file->ntags   = 0;
  file->tags    = 0;

  file->primary = id3_tag_new();
  if (file->primary == 0)
    goto fail;

  id3_tag_addref(file->primary);

  /* load tags from the file */

  if (search_tags(file) == -1)
    goto fail;

  id3_tag_options(file->primary, ID3_TAG_OPTION_ID3V1,
		  (file->flags & ID3_FILE_FLAG_ID3V1) ? ~0 : 0);

  if (0) {
  fail:
    if (file) {
      finish_file(file);
      file = 0;
    }
  }

  return file;
}

/*
 * NAME:	file->open()
 * DESCRIPTION:	open a file given its pathname
 */
struct id3_file *id3_file_open(wchar_t const *path, wchar_t const *name, enum id3_file_mode mode)
{
  FSTAT _fstat;
  int ifile;
  struct id3_file *file;

  if(!path) return 0;
  if(!name) return 0;

  if(mode == ID3_FILE_MODE_READONLY)
  {
    if (fstat(path,name,&_fstat)) return 0;
    if ((ifile=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0)) < 0) return 0;
  }
  else
    return 0; //ID3_FILE_MODE_READWRITE mode is not supported yet.

  file = new_file(ifile, path, name);
  if (file == 0)
    fclose(ifile);

  return file;
}

/*
 * NAME:	file->close()
 * DESCRIPTION:	close a file and delete its associated tags
 */
int id3_file_close(struct id3_file *file)
{
  int result = 0;

  if(!file) return -1;

  fclose(file->iofile);

  finish_file(file);

  return result;
}

/*
 * NAME:	file->tag()
 * DESCRIPTION:	return the primary tag structure for a file
 */
struct id3_tag *id3_file_tag(struct id3_file const *file)
{
  if(!file) return 0;

  return file->primary;
}

#if 0
/*
 * NAME:	v1_write()
 * DESCRIPTION:	write ID3v1 tag modifications to a file
 */
static
int v1_write(struct id3_file *file,
	     id3_byte_t const *data, id3_length_t length)
{
  assert(!data || length == 128);

  if (data) {
    long location;

    if (fseek(file->iofile, (file->flags & ID3_FILE_FLAG_ID3V1) ? -128 : 0,
	      SEEK_END) == -1 ||
	(location = my_ftell(file->iofile)) == -1 ||
	fwrite(data, 128, 1, file->iofile) != 1 ||
	fflush(file->iofile) == EOF)
      return -1;

    /* add file tag reference */

    if (!(file->flags & ID3_FILE_FLAG_ID3V1)) {
      struct filetag filetag;

      filetag.tag      = 0;
      filetag.location = location;
      filetag.length   = 128;

      if (add_filetag(file, &filetag) == -1)
	return -1;

      file->flags |= ID3_FILE_FLAG_ID3V1;
    }
  }
# if defined(HAVE_FTRUNCATE)
  else if (file->flags & ID3_FILE_FLAG_ID3V1) {
    long length;

    if (fseek(file->iofile, 0, SEEK_END) == -1)
      return -1;

    length = ftell(file->iofile);
    if (length == -1 ||
	(length >= 0 && length < 128))
      return -1;

    if (ftruncate(fileno(file->iofile), length - 128) == -1)
      return -1;

    /* delete file tag reference */

    del_filetag(file, file->ntags - 1);

    file->flags &= ~ID3_FILE_FLAG_ID3V1;
  }
# endif

  return 0;
}

/*
 * NAME:	v2_write()
 * DESCRIPTION:	write ID3v2 tag modifications to a file
 */
static
int v2_write(struct id3_file *file,
	     id3_byte_t const *data, id3_length_t length)
{
  assert(!data || length > 0);

  if (data &&
      ((file->ntags == 1 && !(file->flags & ID3_FILE_FLAG_ID3V1)) ||
       (file->ntags == 2 &&  (file->flags & ID3_FILE_FLAG_ID3V1))) &&
      file->tags[0].length == length) {
    /* easy special case: rewrite existing tag in-place */

    if (fseek(file->iofile, file->tags[0].location, SEEK_SET) == -1 ||
	fwrite(data, length, 1, file->iofile) != 1 ||
	fflush(file->iofile) == EOF)
      return -1;

    goto done;
  }

  /* hard general case: rewrite entire file */

  /* ... */

 done:
  return 0;
}

/*
 * NAME:	file->update()
 * DESCRIPTION:	rewrite tag(s) to a file
 */
int id3_file_update(struct id3_file *file)
{
  int options, result = 0;
  id3_length_t v1size = 0, v2size = 0;
  id3_byte_t id3v1_data[128], *id3v1 = 0, *id3v2 = 0;

  assert(file);

  if (file->mode != ID3_FILE_MODE_READWRITE)
    return -1;

  options = id3_tag_options(file->primary, 0, 0);

  /* render ID3v1 */

  if (options & ID3_TAG_OPTION_ID3V1) {
    v1size = id3_tag_render(file->primary, 0);
    if (v1size) {
      assert(v1size == sizeof(id3v1_data));

      v1size = id3_tag_render(file->primary, id3v1_data);
      if (v1size) {
	assert(v1size == sizeof(id3v1_data));
	id3v1 = id3v1_data;
      }
    }
  }

  /* render ID3v2 */

  id3_tag_options(file->primary, ID3_TAG_OPTION_ID3V1, 0);

  v2size = id3_tag_render(file->primary, 0);
  if (v2size) {
    id3v2 = (id3_byte_t *)malloc(v2size);
    if (id3v2 == 0)
      goto fail;

    v2size = id3_tag_render(file->primary, id3v2);
    if (v2size == 0) {
      free(id3v2);
      id3v2 = 0;
    }
  }

  /* write tags */

  if (v2_write(file, id3v2, v2size) == -1 ||
      v1_write(file, id3v1, v1size) == -1)
    goto fail;

  rewind(file->iofile);

  /* update file tags array? ... */

  if (0) {
  fail:
    result = -1;
  }

  /* clean up; restore tag options */

  if (id3v2)
    free(id3v2);

  id3_tag_options(file->primary, ~0, options);

  return result;
}

#endif
