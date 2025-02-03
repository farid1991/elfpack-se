# ifndef LIBID3TAG_UTF8_H
# define LIBID3TAG_UTF8_H

# include "id3tag.h"

id3_length_t id3_utf8_length(id3_utf8_t const *);
id3_length_t id3_utf8_size(id3_utf8_t const *);

id3_length_t id3_utf8_decodechar(id3_utf8_t const *, id3_ucs4_t *);
id3_length_t id3_utf8_encodechar(id3_utf8_t *, id3_ucs4_t);

void id3_utf8_decode(id3_utf8_t const *, id3_ucs4_t *);
void id3_utf8_encode(id3_utf8_t *, id3_ucs4_t const *);

id3_length_t id3_utf8_put(id3_byte_t **, id3_utf8_t);
id3_utf8_t id3_utf8_get(id3_byte_t const **);

id3_length_t id3_utf8_serialize(id3_byte_t **, id3_ucs4_t const *, int);
id3_ucs4_t *id3_utf8_deserialize(id3_byte_t const **, id3_length_t);

# endif
