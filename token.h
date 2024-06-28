#ifndef _TOKEN_
#define _TOKEN_

#include "token.h"

typedef enum _ttype{
   INT,
   DOUBLE,
   LONG,
   STRING,
   CHAR,
   PLUS,
   MINUS,
   DIV,
   MULT,
   NONE
} TYPE;

typedef struct _token{
   TYPE type;
   union {
      int number;
      double floating;
      long lint;
      unsigned uint;
      char c;
      char *string;
      void *data;
   };
} token;

token sb_to_token(sb *str);

char *type_to_str(TYPE type);

#endif
