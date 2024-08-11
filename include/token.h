#ifndef _TOKEN_
#define _TOKEN_

// #include <linked_token.h>
#include <stringbuilder.h>

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
   LESS,
   BOOLEAN,
   EQUAL,
   PRINT,
   NONE,
   IF,
   DO,
   END,
   FOR,
   VAR_INT,
   TO,
   SET
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
      int values[4];
   };
} token;

token sb_to_token(sb *str);

char *type_to_str(TYPE type);

void free_tokens(token *, size_t);

void print_tokens(token *, size_t);

void print_token(token *);

token *tokens_from_bytes(const char *, size_t *);

/* token *tokenizer(sb *, size_t) */

#endif
