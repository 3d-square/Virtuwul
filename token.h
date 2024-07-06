#ifndef _TOKEN_
#define _TOKEN_

#include "token.h"

typedef enum _ttype{
   INT,
   DOUBLE,
   STRING,
   CHAR,
   PLUS,
   MINUS,
   DIV,
   MULT,
   BOOLEAN,
   EQUAL,
   PRINT,
   NONE
} TYPE;

typedef struct _token{
   TYPE type;
   int freeable;
   union {
      double floating;
      long number;
      char c;
      char *string;
      void *data;
   };
} token;

token sb_to_token(sb *str);

char *type_to_str(TYPE type);

void free_tokens(token *, size_t);

#endif
