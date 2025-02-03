#include "..\\include\Lib_Clara.h"
#include "..\include\zlib.h"

#include "id3tag.h"
#include "util.h"
#include "file.h"

#ifndef DEF_WBITS
#  define DEF_WBITS MAX_WBITS
#endif

int ZEXPORT inflateInit_(z_streamp strm, const char *version, int stream_size)
{
    return inflateInit2_(strm, DEF_WBITS, version, stream_size);
}

int ZEXPORT uncompress (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit(&stream);
    if (err != Z_OK) return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}

/*
 * NAME:	util->unsynchronise()
 * DESCRIPTION:	perform (in-place) unsynchronisation
 */
id3_length_t id3_util_unsynchronise(id3_byte_t *data, id3_length_t length)
{
  id3_length_t bytes = 0, count;
  id3_byte_t *end = data + length;
  id3_byte_t const *ptr;

  if (length == 0)
    return 0;

  for (ptr = data; ptr < end - 1; ++ptr) {
    if (ptr[0] == 0xff && (ptr[1] == 0x00 || (ptr[1] & 0xe0) == 0xe0))
      ++bytes;
  }

  if (bytes) {
    ptr  = end;
    end += bytes;

    *--end = *--ptr;

    for (count = bytes; count; *--end = *--ptr) {
      if (ptr[-1] == 0xff && (ptr[0] == 0x00 || (ptr[0] & 0xe0) == 0xe0)) {
	*--end = 0x00;
	--count;
      }
    }
  }

  return length + bytes;
}

/*
 * NAME:	util->deunsynchronise()
 * DESCRIPTION:	undo unsynchronisation (in-place)
 */
id3_length_t id3_util_deunsynchronise(id3_byte_t *data, id3_length_t length)
{
  id3_byte_t const *old, *end = data + length;
  id3_byte_t *_new;

  if (length == 0)
    return 0;

  for (old = _new = data; old < end - 1; ++old) {
    *_new++ = *old;
    if (old[0] == 0xff && old[1] == 0x00)
      ++old;
  }

  *_new++ = *old;

  return _new - data;
}

/*
 * NAME:	util->compress()
 * DESCRIPTION:	perform zlib deflate method compression
 */
id3_byte_t *id3_util_compress(id3_byte_t const *data, id3_length_t length,
			      id3_length_t *newlength)
{
  id3_byte_t *compressed;

  *newlength  = length + 12;
  *newlength += *newlength / 1000;

  compressed = (id3_byte_t *)malloc(*newlength);
  if (compressed) {
    if (compress2(compressed, newlength, data, length,
		  Z_BEST_COMPRESSION) != Z_OK ||
	*newlength >= length) {
      mfree(compressed);
      compressed = 0;
    }
    else {
      id3_byte_t *resized;

      resized = (id3_byte_t *)realloc(compressed, *newlength ? *newlength : 1);
      if (resized)
	compressed = resized;
    }
  }

  return compressed;
}

/*
 * NAME:	util->decompress()
 * DESCRIPTION:	undo zlib deflate method compression
 */
id3_byte_t *id3_util_decompress(id3_byte_t const *data, id3_length_t length,
				id3_length_t newlength)
{
  id3_byte_t *decompressed;

  decompressed = (id3_byte_t *)malloc(newlength ? newlength : 1);
  if (decompressed) {
    id3_length_t size;

    size = newlength;

    if (uncompress(decompressed, &size, data, length) != Z_OK ||
	size != newlength) {
      mfree(decompressed);
      decompressed = 0;
    }
  }

  return decompressed;
}
