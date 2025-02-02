# ifndef LIBID3TAG_UCS4_H
# define LIBID3TAG_UCS4_H

# include "id3tag.h"

# define ID3_UCS4_REPLACEMENTCHAR  0x000000b7L  /* middle dot */

extern id3_ucs4_t const id3_ucs4_empty[];

id3_length_t id3_ucs4_length(id3_ucs4_t const *);
id3_length_t id3_ucs4_size(id3_ucs4_t const *);

id3_length_t id3_ucs4_latin1size(id3_ucs4_t const *);
id3_length_t id3_ucs4_utf16size(id3_ucs4_t const *);
id3_length_t id3_ucs4_utf8size(id3_ucs4_t const *);

void id3_ucs4_copy(id3_ucs4_t *, id3_ucs4_t const *);
id3_ucs4_t *id3_ucs4_duplicate(id3_ucs4_t const *);

# endif
