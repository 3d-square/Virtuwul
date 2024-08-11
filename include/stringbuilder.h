#ifndef _STRING_BUILDER_
#define _STRING_BUILDER_

#include <stdlib.h>


#define WHITESPACE 1

typedef struct _string{
   char *string;
   size_t size;
   size_t capacity;
} sb;


void sb_append_c(sb *, char);

void sb_append_s(sb *, char *);

sb sb_from_str(char *);

void sb_clear(sb *);

void sb_free(sb *);

void sb_free_list(sb *, size_t);

void sb_print(sb *);

sb sb_get_word(const sb *, size_t, char, size_t *);

sb sb_substring(sb *, size_t, size_t);

long sb_indexof(const sb *, char, size_t);

sb *sb_split(const sb *, char, size_t *);

int sb_equal(sb *, sb *);

int sb_strequal(sb *, char *, size_t);

char *sb_to_cstr(sb *);

sb sb_read_file(const char *);

sb *sb_read_lines(const char *, size_t *);

sb *sb_read_tokens(const char *, size_t *);


#endif
