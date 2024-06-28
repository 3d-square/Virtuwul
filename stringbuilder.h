#ifndef _STRING_BUILDER_
#define _STRING_BUILDER_

#include <stdlib.h>


#define WHITESPACE 1

typedef struct _string{
   char *string;
   size_t size;
   size_t capacity;
} sb;


void appendc(sb *, char);

void appends(sb *, char *);

void initsb(sb *, char *);

void clear(sb *);

void freesb(sb *);

void freesblist(sb *, size_t);

void printsb(sb *);

sb get_word(const sb *, size_t, char, size_t *);

sb substring(sb *, size_t, size_t);

sb *sbsplit(const sb *, char, size_t *);

int sbequal(sb *, sb *);

int strequal(sb *, char *, size_t);

char *cstr(sb *);


#endif
