# ifndef LIBID3TAG_PARSE_H
# define LIBID3TAG_PARSE_H

signed long id3_parse_int(id3_byte_t const **, unsigned int);
unsigned long id3_parse_uint(id3_byte_t const **, unsigned int);
unsigned long id3_parse_syncsafe(id3_byte_t const **, unsigned int);
void id3_parse_immediate(id3_byte_t const **, unsigned int, char *);
id3_latin1_t *id3_parse_latin1(id3_byte_t const **, id3_length_t, int);
id3_ucs4_t *id3_parse_string(id3_byte_t const **, id3_length_t,
			     enum id3_field_textencoding, int);
id3_byte_t *id3_parse_binary(id3_byte_t const **, id3_length_t);

# endif
