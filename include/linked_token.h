#ifndef __LINKED_TOKEN__
#define __LINKED_TOKEN__

#include "stringbuilder.h"
#include "token.h"

typedef struct _linked_node{
   struct _linked_node *next, *prev;
   token value;
} linked_node;

typedef struct _linked_token{
   linked_node *head, *tail;
   size_t size;
} linked_token;


// ALWAYS RETURN THE HEAD OF THE LIST
linked_token *linked_add(linked_token *list, token value);

linked_token *linked_remove(linked_token *list, int index);

linked_token *init_linked();

void linked_free(linked_token *list);

void linked_print(linked_token *list);

void tokens_to_bytes(linked_token *, const char *);

linked_token *split_file_contents(sb *);

token *linked_at(linked_token *, int);

#endif
