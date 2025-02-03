# ifndef LIBID3TAG_LATIN1_H
# define LIBID3TAG_LATIN1_H

# include "id3tag.h"

id3_length_t id3_latin1_length(id3_latin1_t const *);
id3_length_t id3_latin1_size(id3_latin1_t const *);

void id3_latin1_copy(id3_latin1_t *, id3_latin1_t const *);
id3_latin1_t *id3_latin1_duplicate(id3_latin1_t const *);

id3_length_t id3_latin1_decodechar(id3_latin1_t const *, id3_ucs4_t *);
id3_length_t id3_latin1_encodechar(id3_latin1_t *, id3_ucs4_t);

void id3_latin1_decode(id3_latin1_t const *, id3_ucs4_t *);
void id3_latin1_encode(id3_latin1_t *, id3_ucs4_t const *);

id3_length_t id3_latin1_put(id3_byte_t **, id3_latin1_t);
id3_latin1_t id3_latin1_get(id3_byte_t const **);

id3_length_t id3_latin1_serialize(id3_byte_t **, id3_ucs4_t const *, int);
id3_ucs4_t *id3_latin1_deserialize(id3_byte_t const **, id3_length_t);

# endif
