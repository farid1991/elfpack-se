#include "..\\include\Lib_Clara.h"
# include "id3tag.h"
# include "parse.h"
# include "latin1.h"
# include "utf16.h"
# include "utf8.h"
# include "file.h"

signed long id3_parse_int(id3_byte_t const **ptr, unsigned int bytes)
{
  signed long value = 0;

  if(!(bytes >= 1 && bytes <= 4)) return 0;

  if (**ptr & 0x80)
    value = ~0;

  switch (bytes) {
  case 4: value = (value << 8) | *(*ptr)++;
  case 3: value = (value << 8) | *(*ptr)++;
  case 2: value = (value << 8) | *(*ptr)++;
  case 1: value = (value << 8) | *(*ptr)++;
  }

  return value;
}

unsigned long id3_parse_uint(id3_byte_t const **ptr, unsigned int bytes)
{
  unsigned long value = 0;

  if(!(bytes >= 1 && bytes <= 4)) return 0;

  switch (bytes) {
  case 4: value = (value << 8) | *(*ptr)++;
  case 3: value = (value << 8) | *(*ptr)++;
  case 2: value = (value << 8) | *(*ptr)++;
  case 1: value = (value << 8) | *(*ptr)++;
  }

  return value;
}

unsigned long id3_parse_syncsafe(id3_byte_t const **ptr, unsigned int bytes)
{
  unsigned long value = 0;

  if(!(bytes == 4 || bytes == 5)) return 0;

  switch (bytes) {
  case 5: value = (value << 4) | (*(*ptr)++ & 0x0f);
  case 4: value = (value << 7) | (*(*ptr)++ & 0x7f);
          value = (value << 7) | (*(*ptr)++ & 0x7f);
	  value = (value << 7) | (*(*ptr)++ & 0x7f);
	  value = (value << 7) | (*(*ptr)++ & 0x7f);
  }

  return value;
}

void id3_parse_immediate(id3_byte_t const **ptr, unsigned int bytes,
			 char *value)
{
  if(!value) return;
  if(!(bytes == 8 || bytes == 4 || bytes == 3)) return;

  switch (bytes) {
  case 8: *value++ = *(*ptr)++;
          *value++ = *(*ptr)++;
	  *value++ = *(*ptr)++;
	  *value++ = *(*ptr)++;
  case 4: *value++ = *(*ptr)++;
  case 3: *value++ = *(*ptr)++;
          *value++ = *(*ptr)++;
	  *value++ = *(*ptr)++;
  }

  *value = 0;
}

id3_latin1_t *id3_parse_latin1(id3_byte_t const **ptr, id3_length_t length,
			       int full)
{
  id3_byte_t const *end;
  int terminated = 0;
  id3_latin1_t *latin1;

  end = (id3_byte_t const *)memchr(*ptr, 0, length);
  if (end == 0)
    end = *ptr + length;
  else {
    length = end - *ptr;
    terminated = 1;
  }

  latin1 = (id3_latin1_t *)malloc(length + 1);
  if (latin1) {
    memcpy(latin1, *ptr, length);
    latin1[length] = 0;

    if (!full) {
      id3_latin1_t *check;

      for (check = latin1; *check; ++check) {
	if (*check == '\n')
	  *check = ' ';
      }
    }
  }

  *ptr += length + terminated;

  return latin1;
}

id3_ucs4_t *id3_parse_string(id3_byte_t const **ptr, id3_length_t length,
			     enum id3_field_textencoding encoding, int full)
{
  id3_ucs4_t *ucs4 = 0;
  enum id3_utf16_byteorder byteorder = ID3_UTF16_BYTEORDER_ANY;

  switch (encoding) {
  case ID3_FIELD_TEXTENCODING_ISO_8859_1:
    ucs4 = id3_latin1_deserialize(ptr, length);
    break;

  case ID3_FIELD_TEXTENCODING_UTF_16BE:
    byteorder = ID3_UTF16_BYTEORDER_BE;
  case ID3_FIELD_TEXTENCODING_UTF_16:
    ucs4 = id3_utf16_deserialize(ptr, length, byteorder);
    break;

  case ID3_FIELD_TEXTENCODING_UTF_8:
    ucs4 = id3_utf8_deserialize(ptr, length);
    break;
  }

  if (ucs4 && !full) {
    id3_ucs4_t *check;

    for (check = ucs4; *check; ++check) {
      if (*check == '\n')
	*check = ' ';
    }
  }

  return ucs4;
}

id3_byte_t *id3_parse_binary(id3_byte_t const **ptr, id3_length_t length)
{
  id3_byte_t *data;

  if (length == 0)
    return (id3_byte_t *)malloc(1);

  data = (id3_byte_t *)malloc(length);
  if (data)
    memcpy(data, *ptr, length);

  *ptr += length;

  return data;
}
