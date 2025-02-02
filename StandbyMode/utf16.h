# ifndef LIBID3TAG_UTF16_H
# define LIBID3TAG_UTF16_H

# include "id3tag.h"

enum id3_utf16_byteorder {
  ID3_UTF16_BYTEORDER_ANY,
  ID3_UTF16_BYTEORDER_BE,
  ID3_UTF16_BYTEORDER_LE
};

id3_length_t id3_utf16_length(id3_utf16_t const *);
id3_length_t id3_utf16_size(id3_utf16_t const *);

id3_length_t id3_utf16_decodechar(id3_utf16_t const *, id3_ucs4_t *);
id3_length_t id3_utf16_encodechar(id3_utf16_t *, id3_ucs4_t);

void id3_utf16_decode(id3_utf16_t const *, id3_ucs4_t *);
void id3_utf16_encode(id3_utf16_t *, id3_ucs4_t const *);

id3_length_t id3_utf16_put(id3_byte_t **, id3_utf16_t,
			   enum id3_utf16_byteorder);
id3_utf16_t id3_utf16_get(id3_byte_t const **, enum id3_utf16_byteorder);

id3_length_t id3_utf16_serialize(id3_byte_t **, id3_ucs4_t const *,
				 enum id3_utf16_byteorder, int);
id3_ucs4_t *id3_utf16_deserialize(id3_byte_t const **, id3_length_t,
				  enum id3_utf16_byteorder);

# endif
