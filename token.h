#ifndef _TOKEN_
#define _TOKEN_

#include "token.h"

#define get_value(v) (v->type == INT) ? v->number : v->floating
#define is_numeric(t) (t == INT || t == DOUBLE)
#define TOKEN_SIZE sizeof(token)

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
   SET,
   LET,
   IDENTIFIER,
   TYPE_INT,
   TYPE_DOUBLE,
   TYPE_STRING,
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

void print_tokens(token *, size_t);

token *split_file_contents(sb *, size_t *);

void tokens_to_bytes(token *, size_t, const char *);

token *tokens_from_bytes(const char *, size_t *);

/* token *tokenizer(sb *, size_t) */

#endif
