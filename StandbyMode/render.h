# ifndef LIBID3TAG_RENDER_H
# define LIBID3TAG_RENDER_H

# include "id3tag.h"

id3_length_t id3_render_immediate(id3_byte_t **, char const *, unsigned int);
id3_length_t id3_render_syncsafe(id3_byte_t **, unsigned long, unsigned int);
id3_length_t id3_render_int(id3_byte_t **, signed long, unsigned int);
id3_length_t id3_render_binary(id3_byte_t **,
			       id3_byte_t const *, id3_length_t);
id3_length_t id3_render_latin1(id3_byte_t **, id3_latin1_t const *, int);
id3_length_t id3_render_string(id3_byte_t **, id3_ucs4_t const *,
			       enum id3_field_textencoding, int);
id3_length_t id3_render_padding(id3_byte_t **, id3_byte_t, id3_length_t);

id3_length_t id3_render_paddedstring(id3_byte_t **, id3_ucs4_t const *,
				     id3_length_t);

# endif
