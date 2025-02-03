# ifndef LIBID3TAG_FIELD_H
# define LIBID3TAG_FIELD_H

# include "id3tag.h"

void id3_field_init(union id3_field *, enum id3_field_type);
void id3_field_finish(union id3_field *);

int id3_field_parse(union id3_field *, id3_byte_t const **,
		    id3_length_t, enum id3_field_textencoding *);

id3_length_t id3_field_render(union id3_field const *, id3_byte_t **,
			      enum id3_field_textencoding *, int);

# endif
