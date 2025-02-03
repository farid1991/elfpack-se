# ifndef LIBID3TAG_COMPAT_H
# define LIBID3TAG_COMPAT_H

# include "id3tag.h"

typedef int id3_compat_func_t(struct id3_frame *, char const *,
			      id3_byte_t const *, id3_length_t);

struct id3_compat {
  char const *id;
  char const *equiv;
  id3_compat_func_t *translate;
};

struct id3_compat const *id3_compat_lookup(register char const *,
					   register unsigned int);

int id3_compat_fixup(struct id3_tag *);

# endif
