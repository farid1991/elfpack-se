#include "..\\include\Lib_Clara.h"
#include "id3tag.h"
#include "field.h"
#include "frame.h"
#include "render.h"
#include "ucs4.h"
#include "latin1.h"
#include "parse.h"
#include "file.h"

/*
 * NAME:	field->init()
 * DESCRIPTION:	initialize a field to a default value for the given type
 */
void id3_field_init(union id3_field *field, enum id3_field_type type)
{
  if(!field) return;

  switch (field->type = type) {
  case ID3_FIELD_TYPE_TEXTENCODING:
  case ID3_FIELD_TYPE_INT8:
  case ID3_FIELD_TYPE_INT16:
  case ID3_FIELD_TYPE_INT24:
  case ID3_FIELD_TYPE_INT32:
    field->number.value = 0;
    break;

  case ID3_FIELD_TYPE_LATIN1:
  case ID3_FIELD_TYPE_LATIN1FULL:
    field->latin1.ptr = 0;
    break;

  case ID3_FIELD_TYPE_LATIN1LIST:
    field->latin1list.nstrings = 0;
    field->latin1list.strings  = 0;

  case ID3_FIELD_TYPE_STRING:
  case ID3_FIELD_TYPE_STRINGFULL:
    field->string.ptr = 0;
    break;

  case ID3_FIELD_TYPE_STRINGLIST:
    field->stringlist.nstrings = 0;
    field->stringlist.strings  = 0;
    break;

  case ID3_FIELD_TYPE_LANGUAGE:
    strcpy(field->immediate.value, "XXX");
    break;

  case ID3_FIELD_TYPE_FRAMEID:
    strcpy(field->immediate.value, "XXXX");
    break;

  case ID3_FIELD_TYPE_DATE:
    memset(field->immediate.value, 0, sizeof(field->immediate.value));
    break;

  case ID3_FIELD_TYPE_INT32PLUS:
  case ID3_FIELD_TYPE_BINARYDATA:
    field->binary.data   = 0;
    field->binary.length = 0;
    break;
  }
}

/*
 * NAME:	field->finish()
 * DESCRIPTION:	reset a field, deallocating memory if necessary
 */
void id3_field_finish(union id3_field *field)
{
  unsigned int i;

  if(!field) return;

  switch (field->type) {
  case ID3_FIELD_TYPE_TEXTENCODING:
  case ID3_FIELD_TYPE_INT8:
  case ID3_FIELD_TYPE_INT16:
  case ID3_FIELD_TYPE_INT24:
  case ID3_FIELD_TYPE_INT32:
  case ID3_FIELD_TYPE_LANGUAGE:
  case ID3_FIELD_TYPE_FRAMEID:
  case ID3_FIELD_TYPE_DATE:
    break;

  case ID3_FIELD_TYPE_LATIN1:
  case ID3_FIELD_TYPE_LATIN1FULL:
    if (field->latin1.ptr)
      mfree(field->latin1.ptr);
    break;

  case ID3_FIELD_TYPE_LATIN1LIST:
    for (i = 0; i < field->latin1list.nstrings; ++i)
      mfree(field->latin1list.strings[i]);

    if (field->latin1list.strings)
      mfree(field->latin1list.strings);
    break;

  case ID3_FIELD_TYPE_STRING:
  case ID3_FIELD_TYPE_STRINGFULL:
    if (field->string.ptr)
      mfree(field->string.ptr);
    break;

  case ID3_FIELD_TYPE_STRINGLIST:
    for (i = 0; i < field->stringlist.nstrings; ++i)
      mfree(field->stringlist.strings[i]);

    if (field->stringlist.strings)
      mfree(field->stringlist.strings);
    break;

  case ID3_FIELD_TYPE_INT32PLUS:
  case ID3_FIELD_TYPE_BINARYDATA:
    if (field->binary.data)
      mfree(field->binary.data);
    break;
  }

  id3_field_init(field, field->type);
}

/*
 * NAME:	field->type()
 * DESCRIPTION:	return the value type of a field
 */
enum id3_field_type id3_field_type(union id3_field const *field)
{
  return field->type;
}

/*
 * NAME:	field->parse()
 * DESCRIPTION:	parse a field value
 */
int id3_field_parse(union id3_field *field, id3_byte_t const **ptr,
		    id3_length_t length, enum id3_field_textencoding *encoding)
{
  if(!field) return -1;

  id3_field_finish(field);

  switch (field->type) {
  case ID3_FIELD_TYPE_INT32:
    if (length < 4)
      goto fail;

    field->number.value = id3_parse_uint(ptr, 4);
    break;

  case ID3_FIELD_TYPE_INT24:
    if (length < 3)
      goto fail;

    field->number.value = id3_parse_uint(ptr, 3);
    break;

  case ID3_FIELD_TYPE_INT16:
    if (length < 2)
      goto fail;

    field->number.value = id3_parse_uint(ptr, 2);
    break;

  case ID3_FIELD_TYPE_INT8:
  case ID3_FIELD_TYPE_TEXTENCODING:
    if (length < 1)
      goto fail;

    field->number.value = id3_parse_uint(ptr, 1);

    if (field->type == ID3_FIELD_TYPE_TEXTENCODING)
      *encoding = (enum id3_field_textencoding)field->number.value;
    break;

  case ID3_FIELD_TYPE_LANGUAGE:
    if (length < 3)
      goto fail;

    id3_parse_immediate(ptr, 3, field->immediate.value);
    break;

  case ID3_FIELD_TYPE_FRAMEID:
    if (length < 4)
      goto fail;

    id3_parse_immediate(ptr, 4, field->immediate.value);
    break;

  case ID3_FIELD_TYPE_DATE:
    if (length < 8)
      goto fail;

    id3_parse_immediate(ptr, 8, field->immediate.value);
    break;

  case ID3_FIELD_TYPE_LATIN1:
  case ID3_FIELD_TYPE_LATIN1FULL:
    {
      id3_latin1_t *latin1;

      latin1 = id3_parse_latin1(ptr, length,
				field->type == ID3_FIELD_TYPE_LATIN1FULL);
      if (latin1 == 0)
	goto fail;

      field->latin1.ptr = latin1;
    }
    break;

  case ID3_FIELD_TYPE_LATIN1LIST:
    {
      id3_byte_t const *end;
      id3_latin1_t *latin1, **strings;

      end = *ptr + length;

      while (end - *ptr > 0) {
	latin1 = id3_parse_latin1(ptr, end - *ptr, 0);
	if (latin1 == 0)
	  goto fail;

	strings = (id3_latin1_t **)realloc(field->latin1list.strings,
			  (field->latin1list.nstrings + 1) * sizeof(*strings));
	if (strings == 0) {
	  mfree(latin1);
	  goto fail;
	}

	field->latin1list.strings = strings;
	field->latin1list.strings[field->latin1list.nstrings++] = latin1;
      }
    }
    break;

  case ID3_FIELD_TYPE_STRING:
  case ID3_FIELD_TYPE_STRINGFULL:
    {
      id3_ucs4_t *ucs4;

      ucs4 = id3_parse_string(ptr, length, *encoding,
			      field->type == ID3_FIELD_TYPE_STRINGFULL);
      if (ucs4 == 0)
	goto fail;

      field->string.ptr = ucs4;
    }
    break;

  case ID3_FIELD_TYPE_STRINGLIST:
    {
      id3_byte_t const *end;
      id3_ucs4_t *ucs4, **strings;

      end = *ptr + length;

      while (end - *ptr > 0) {
	ucs4 = id3_parse_string(ptr, end - *ptr, *encoding, 0);
	if (ucs4 == 0)
	  goto fail;

	strings = (id3_ucs4_t **)realloc(field->stringlist.strings,
			  (field->stringlist.nstrings + 1) * sizeof(*strings));
	if (strings == 0) {
	  mfree(ucs4);
	  goto fail;
	}

	field->stringlist.strings = strings;
	field->stringlist.strings[field->stringlist.nstrings++] = ucs4;
      }
    }
    break;

  case ID3_FIELD_TYPE_INT32PLUS:
  case ID3_FIELD_TYPE_BINARYDATA:
    {
      id3_byte_t *data;

      data = id3_parse_binary(ptr, length);
      if (data == 0)
	goto fail;

      field->binary.data   = data;
      field->binary.length = length;
    }
    break;
  }

  return 0;

 fail:
  return -1;
}

/*
 * NAME:	field->render()
 * DESCRIPTION:	render a field value
 */
id3_length_t id3_field_render(union id3_field const *field, id3_byte_t **ptr,
			      enum id3_field_textencoding *encoding,
			      int terminate)
{
  id3_length_t size;
  unsigned int i;

  if((!field) || (!encoding)) return 0;

  switch (field->type) {
  case ID3_FIELD_TYPE_INT32:
    return id3_render_int(ptr, field->number.value, 4);

  case ID3_FIELD_TYPE_INT24:
    return id3_render_int(ptr, field->number.value, 3);

  case ID3_FIELD_TYPE_INT16:
    return id3_render_int(ptr, field->number.value, 2);

  case ID3_FIELD_TYPE_TEXTENCODING:
    *encoding = (enum id3_field_textencoding)field->number.value;
  case ID3_FIELD_TYPE_INT8:
    return id3_render_int(ptr, field->number.value, 1);

  case ID3_FIELD_TYPE_LATIN1:
  case ID3_FIELD_TYPE_LATIN1FULL:
    return id3_render_latin1(ptr, field->latin1.ptr, terminate);

  case ID3_FIELD_TYPE_LATIN1LIST:
    size = 0;
    for (i = 0; i < field->latin1list.nstrings; ++i) {
      size += id3_render_latin1(ptr, field->latin1list.strings[i],
				(i < field->latin1list.nstrings - 1) ||
				terminate);
    }
    return size;

  case ID3_FIELD_TYPE_STRING:
  case ID3_FIELD_TYPE_STRINGFULL:
    return id3_render_string(ptr, field->string.ptr, *encoding, terminate);

  case ID3_FIELD_TYPE_STRINGLIST:
    size = 0;
    for (i = 0; i < field->stringlist.nstrings; ++i) {
      size += id3_render_string(ptr, field->stringlist.strings[i], *encoding,
				(i < field->stringlist.nstrings - 1) ||
				terminate);
    }
    return size;

  case ID3_FIELD_TYPE_LANGUAGE:
    return id3_render_immediate(ptr, field->immediate.value, 3);

  case ID3_FIELD_TYPE_FRAMEID:
    return id3_render_immediate(ptr, field->immediate.value, 4);

  case ID3_FIELD_TYPE_DATE:
    return id3_render_immediate(ptr, field->immediate.value, 8);

  case ID3_FIELD_TYPE_INT32PLUS:
  case ID3_FIELD_TYPE_BINARYDATA:
    return id3_render_binary(ptr, field->binary.data, field->binary.length);
  }

  return 0;
}

/*
 * NAME:	field->setint()
 * DESCRIPTION:	set the value of an int field
 */
int id3_field_setint(union id3_field *field, signed long number)
{
  if(!field) return -1;

  switch (field->type) {
  case ID3_FIELD_TYPE_INT8:
    if (number > 0x7f || number < -0x80)
      return -1;
    break;

  case ID3_FIELD_TYPE_INT16:
    if (number > 0x7fff || number < -0x8000)
      return -1;
    break;

  case ID3_FIELD_TYPE_INT24:
    if (number > 0x7fffffL || number < -0x800000L)
      return -1;
    break;

  case ID3_FIELD_TYPE_INT32:
    if (number > 0x7fffffffL || number < -0x80000000L)
      return -1;
    break;

  default:
    return -1;
  }

  id3_field_finish(field);

  field->number.value = number;

  return 0;
}

/*
 * NAME:	field->settextencoding()
 * DESCRIPTION:	set the value of a textencoding field
 */
int id3_field_settextencoding(union id3_field *field,
			      enum id3_field_textencoding encoding)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_TEXTENCODING)
    return -1;

  id3_field_finish(field);

  field->number.value = encoding;

  return 0;
}

static
int set_latin1(union id3_field *field, id3_latin1_t const *latin1)
{
  id3_latin1_t *data;

  if (latin1 == 0 || *latin1 == 0)
    data = 0;
  else {
    data = id3_latin1_duplicate(latin1);
    if (data == 0)
      return -1;
  }

  field->latin1.ptr = data;

  return 0;
}

/*
 * NAME:	field->setlatin1()
 * DESCRIPTION:	set the value of a latin1 field
 */
int id3_field_setlatin1(union id3_field *field, id3_latin1_t const *latin1)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_LATIN1)
    return -1;

  id3_field_finish(field);

  if (latin1) {
    id3_latin1_t const *ptr;

    for (ptr = latin1; *ptr; ++ptr) {
      if (*ptr == '\n')
	return -1;
    }
  }

  return set_latin1(field, latin1);
}

/*
 * NAME:	field->setfulllatin1()
 * DESCRIPTION:	set the value of a full latin1 field
 */
int id3_field_setfulllatin1(union id3_field *field, id3_latin1_t const *latin1)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_LATIN1FULL)
    return -1;

  id3_field_finish(field);

  return set_latin1(field, latin1);
}

static
int set_string(union id3_field *field, id3_ucs4_t const *string)
{
  id3_ucs4_t *data;

  if (string == 0 || *string == 0)
    data = 0;
  else {
    data = id3_ucs4_duplicate(string);
    if (data == 0)
      return -1;
  }

  field->string.ptr = data;

  return 0;
}

/*
 * NAME:	field->setstring()
 * DESCRIPTION:	set the value of a string field
 */
int id3_field_setstring(union id3_field *field, id3_ucs4_t const *string)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_STRING)
    return -1;

  id3_field_finish(field);

  if (string) {
    id3_ucs4_t const *ptr;

    for (ptr = string; *ptr; ++ptr) {
      if (*ptr == '\n')
	return -1;
    }
  }

  return set_string(field, string);
}

/*
 * NAME:	field->setfullstring()
 * DESCRIPTION:	set the value of a full string field
 */
int id3_field_setfullstring(union id3_field *field, id3_ucs4_t const *string)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_STRINGFULL)
    return -1;

  id3_field_finish(field);

  return set_string(field, string);
}

/*
 * NAME:	field->setstrings()
 * DESCRIPTION:	set the value of a stringlist field
 */
int id3_field_setstrings(union id3_field *field,
			 unsigned int length, id3_ucs4_t **ptrs)
{
  id3_ucs4_t **strings;
  unsigned int i;

  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_STRINGLIST)
    return -1;

  id3_field_finish(field);

  if (length == 0)
    return 0;

  strings = (id3_ucs4_t **)malloc(length * sizeof(*strings));
  if (strings == 0)
    return -1;

  for (i = 0; i < length; ++i) {
    strings[i] = id3_ucs4_duplicate(ptrs[i]);
    if (strings[i] == 0) {
      while (i--)
	mfree(strings[i]);

      mfree(strings);
      return -1;
    }
  }

  field->stringlist.strings  = strings;
  field->stringlist.nstrings = length;

  return 0;
}

/*
 * NAME:	field->addstring()
 * DESCRIPTION:	add a string to a stringlist field
 */
int id3_field_addstring(union id3_field *field, id3_ucs4_t const *string)
{
  id3_ucs4_t *_new, **strings;

  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_STRINGLIST)
    return -1;

  if (string == 0)
    string = id3_ucs4_empty;

  _new = id3_ucs4_duplicate(string);
  if (_new == 0)
    return -1;

  strings = (id3_ucs4_t **)realloc(field->stringlist.strings,
		    (field->stringlist.nstrings + 1) * sizeof(*strings));
  if (strings == 0) {
    mfree(_new);
    return -1;
  }

  field->stringlist.strings = strings;
  field->stringlist.strings[field->stringlist.nstrings++] = _new;

  return 0;
}

/*
 * NAME:	field->setlanguage()
 * DESCRIPTION:	set the value of a language field
 */
int id3_field_setlanguage(union id3_field *field, char const *language)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_LANGUAGE)
    return -1;

  id3_field_finish(field);

  if (language) {
    if (strlen(language) != 3)
      return -1;

    strcpy(field->immediate.value, (char *)language);
  }

  return 0;
}

/*
 * NAME:	field->setframeid()
 * DESCRIPTION:	set the value of a frameid field
 */
int id3_field_setframeid(union id3_field *field, char const *id)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_FRAMEID ||
      !id3_frame_validid(id))
    return -1;

  id3_field_finish(field);

  field->immediate.value[0] = id[0];
  field->immediate.value[1] = id[1];
  field->immediate.value[2] = id[2];
  field->immediate.value[3] = id[3];
  field->immediate.value[4] = 0;

  return 0;
}

/*
 * NAME:	field->setbinarydata()
 * DESCRIPTION:	set the value of a binarydata field
 */
int id3_field_setbinarydata(union id3_field *field,
			    id3_byte_t const *data, id3_length_t length)
{
  id3_byte_t *mem;

  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_BINARYDATA)
    return -1;

  id3_field_finish(field);

  if (length == 0)
    mem = 0;
  else {
    mem = (id3_byte_t *)malloc(length);
    if (mem == 0)
      return -1;

    if(!data) return -1;

    memcpy(mem, data, length);
  }

  field->binary.data   = mem;
  field->binary.length = length;

  return 0;
}

/*
 * NAME:	field->getint()
 * DESCRIPTION:	return the value of an integer field
 */
signed long id3_field_getint(union id3_field const *field)
{
  if(!field) return -1;

  if (field->type != ID3_FIELD_TYPE_INT8 &&
      field->type != ID3_FIELD_TYPE_INT16 &&
      field->type != ID3_FIELD_TYPE_INT24 &&
      field->type != ID3_FIELD_TYPE_INT32)
    return -1;

  return field->number.value;
}

/*
 * NAME:	field->gettextencoding()
 * DESCRIPTION:	return the value of a text encoding field
 */
enum id3_field_textencoding
id3_field_gettextencoding(union id3_field const *field)
{
  if(!field) return (enum id3_field_textencoding)-1;

  if (field->type != ID3_FIELD_TYPE_TEXTENCODING)
    return (enum id3_field_textencoding)-1;

  return (enum id3_field_textencoding)field->number.value;
}

/*
 * NAME:	field->getlatin1()
 * DESCRIPTION:	return the value of a latin1 field
 */
id3_latin1_t const *id3_field_getlatin1(union id3_field const *field)
{
  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_LATIN1)
    return 0;

  return field->latin1.ptr ? field->latin1.ptr : (id3_latin1_t const *) "";
}

/*
 * NAME:	field->getfulllatin1()
 * DESCRIPTION:	return the value of a full latin1 field
 */
id3_latin1_t const *id3_field_getfulllatin1(union id3_field const *field)
{
  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_LATIN1FULL)
    return 0;

  return field->latin1.ptr ? field->latin1.ptr : (id3_latin1_t const *) "";
}

/*
 * NAME:	field->getstring()
 * DESCRIPTION:	return the value of a string field
 */
id3_ucs4_t const *id3_field_getstring(union id3_field const *field)
{
  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_STRING)
    return 0;

  return field->string.ptr ? field->string.ptr : id3_ucs4_empty;
}

/*
 * NAME:	field->getfullstring()
 * DESCRIPTION:	return the value of a fullstring field
 */
id3_ucs4_t const *id3_field_getfullstring(union id3_field const *field)
{
  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_STRINGFULL)
    return 0;

  return field->string.ptr ? field->string.ptr : id3_ucs4_empty;
}

/*
 * NAME:	field->getnstrings()
 * DESCRIPTION:	return the number of strings in a stringlist field
 */
unsigned int id3_field_getnstrings(union id3_field const *field)
{
  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_STRINGLIST)
    return 0;

  return field->stringlist.nstrings;
}

/*
 * NAME:	field->getstrings()
 * DESCRIPTION:	return one value of a stringlist field
 */
id3_ucs4_t const *id3_field_getstrings(union id3_field const *field,
				       unsigned int index)
{
  id3_ucs4_t const *string;

  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_STRINGLIST ||
      index >= field->stringlist.nstrings)
    return 0;

  string = field->stringlist.strings[index];

  return string ? string : id3_ucs4_empty;
}

/*
 * NAME:	field->getframeid()
 * DESCRIPTION:	return the value of a frameid field
 */
char const *id3_field_getframeid(union id3_field const *field)
{
  if(!field) return 0;

  if (field->type != ID3_FIELD_TYPE_FRAMEID)
    return 0;

  return field->immediate.value;
}

/*
 * NAME:	field->getbinarydata()
 * DESCRIPTION:	return the value of a binarydata field
 */
id3_byte_t const *id3_field_getbinarydata(union id3_field const *field,
					  id3_length_t *length)
{
  static id3_byte_t const empty = 0;

  if((!field) || (!length)) return 0;

  if (field->type != ID3_FIELD_TYPE_BINARYDATA)
    return 0;

  if((field->binary.length == 0) || !(field->binary.data)) return &empty;

  *length = field->binary.length;

  return field->binary.data ? field->binary.data : &empty;
}
