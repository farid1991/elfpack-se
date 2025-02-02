#include "..\\include\Lib_Clara.h"
#include "id3tag.h"
#include "render.h"
#include "ucs4.h"
#include "latin1.h"
#include "utf16.h"
#include "utf8.h"

id3_length_t id3_render_immediate(id3_byte_t **ptr,
				  char const *value, unsigned int bytes)
{
  if(!value) return 0;
  if(!(bytes == 8 || bytes == 4 || bytes == 3)) return 0;

  if (ptr) {
    switch (bytes) {
    case 8: *(*ptr)++ = *value++;
            *(*ptr)++ = *value++;
	    *(*ptr)++ = *value++;
	    *(*ptr)++ = *value++;
    case 4: *(*ptr)++ = *value++;
    case 3: *(*ptr)++ = *value++;
            *(*ptr)++ = *value++;
	    *(*ptr)++ = *value++;
    }
  }

  return bytes;
}

id3_length_t id3_render_syncsafe(id3_byte_t **ptr,
				 unsigned long num, unsigned int bytes)
{
  if(!(bytes == 4 || bytes == 5)) return 0;

  if (ptr) {
    switch (bytes) {
    case 5: *(*ptr)++ = (num >> 28) & 0x0f;
    case 4: *(*ptr)++ = (num >> 21) & 0x7f;
            *(*ptr)++ = (num >> 14) & 0x7f;
	    *(*ptr)++ = (num >>  7) & 0x7f;
	    *(*ptr)++ = (num >>  0) & 0x7f;
    }
  }

  return bytes;
}

id3_length_t id3_render_int(id3_byte_t **ptr,
			    signed long num, unsigned int bytes)
{
  if(!(bytes >= 1 && bytes <= 4)) return 0;

  if (ptr) {
    switch (bytes) {
    case 4: *(*ptr)++ = num >> 24;
    case 3: *(*ptr)++ = num >> 16;
    case 2: *(*ptr)++ = num >>  8;
    case 1: *(*ptr)++ = num >>  0;
    }
  }

  return bytes;
}

id3_length_t id3_render_binary(id3_byte_t **ptr,
			       id3_byte_t const *data, id3_length_t length)
{
  if (data == 0)
    return 0;

  if (ptr) {
    memcpy(*ptr, data, length);
    *ptr += length;
  }

  return length;
}

id3_length_t id3_render_latin1(id3_byte_t **ptr,
			       id3_latin1_t const *latin1, int terminate)
{
  id3_length_t size;

  if (latin1 == 0)
    latin1 = "";

  size = id3_latin1_size(latin1);
  if (!terminate)
    --size;

  if (ptr) {
    memcpy(*ptr, latin1, size);
    *ptr += size;
  }

  return size;
}

id3_length_t id3_render_string(id3_byte_t **ptr, id3_ucs4_t const *ucs4,
			       enum id3_field_textencoding encoding,
			       int terminate)
{
  enum id3_utf16_byteorder byteorder = ID3_UTF16_BYTEORDER_ANY;

  if (ucs4 == 0)
    ucs4 = id3_ucs4_empty;

  switch (encoding) {
  case ID3_FIELD_TEXTENCODING_ISO_8859_1:
    return id3_latin1_serialize(ptr, ucs4, terminate);

  case ID3_FIELD_TEXTENCODING_UTF_16BE:
    byteorder = ID3_UTF16_BYTEORDER_BE;
  case ID3_FIELD_TEXTENCODING_UTF_16:
    return id3_utf16_serialize(ptr, ucs4, byteorder, terminate);

  case ID3_FIELD_TEXTENCODING_UTF_8:
    return id3_utf8_serialize(ptr, ucs4, terminate);
  }

  return 0;
}

id3_length_t id3_render_padding(id3_byte_t **ptr, id3_byte_t value,
				id3_length_t length)
{
  if (ptr) {
    memset(*ptr, value, length);
    *ptr += length;
  }

  return length;
}

/*
 * NAME:	render->paddedstring()
 * DESCRIPTION:	render a space-padded string using latin1 encoding
 */
id3_length_t id3_render_paddedstring(id3_byte_t **ptr, id3_ucs4_t const *ucs4,
				     id3_length_t length)
{
  id3_ucs4_t padded[31], *data, *end;

  /* latin1 encoding only (this is used for ID3v1 fields) */

  if(!(length <= 30)) return 0;

  data = padded;
  end  = data + length;

  if (ucs4) {
    while (*ucs4 && end - data > 0) {
      *data++ = *ucs4++;

      if (data[-1] == '\n')
	data[-1] = ' ';
    }
  }

  while (end - data > 0)
    *data++ = ' ';

  *data = 0;

  return id3_latin1_serialize(ptr, padded, 0);
}
