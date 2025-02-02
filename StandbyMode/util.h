# ifndef LIBID3TAG_UTIL_H
# define LIBID3TAG_UTIL_H

# include "id3tag.h"

id3_length_t id3_util_unsynchronise(id3_byte_t *, id3_length_t);
id3_length_t id3_util_deunsynchronise(id3_byte_t *, id3_length_t);

id3_byte_t *id3_util_compress(id3_byte_t const *, id3_length_t,
			      id3_length_t *);
id3_byte_t *id3_util_decompress(id3_byte_t const *, id3_length_t,
				id3_length_t);

# endif
