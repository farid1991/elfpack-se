# ifndef LIBID3TAG_FRAME_H
# define LIBID3TAG_FRAME_H

# include "id3tag.h"

int id3_frame_validid(char const *);

void id3_frame_addref(struct id3_frame *);
void id3_frame_delref(struct id3_frame *);

struct id3_frame *id3_frame_parse(id3_byte_t const **, id3_length_t,
				  unsigned int);
id3_length_t id3_frame_render(struct id3_frame const *, id3_byte_t **, int);

# endif
