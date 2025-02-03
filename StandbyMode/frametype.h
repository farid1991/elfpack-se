
# ifndef LIBID3TAG_FRAMETYPE_H
# define LIBID3TAG_FRAMETYPE_H

struct id3_frametype {
  char const *id;
  unsigned int nfields;
  enum id3_field_type const *fields;
  int defaultflags;
  char const *description;
};

extern struct id3_frametype const id3_frametype_text;
extern struct id3_frametype const id3_frametype_url;
extern struct id3_frametype const id3_frametype_experimental;
extern struct id3_frametype const id3_frametype_unknown;
extern struct id3_frametype const id3_frametype_obsolete;

struct id3_frametype const *id3_frametype_lookup(register char const *,
						 register unsigned int);

# endif
