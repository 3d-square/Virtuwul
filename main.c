#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "stringbuilder.h"
#include "token.h"

token *tokenizer(sb *strings, size_t length){

   token *tokens = malloc(sizeof(token) * length);

   for(size_t i = 0; i < length; ++i){
      tokens[i] = sb_to_token(&(strings[i]));
   }

   return tokens;
}

int main(int argc, char *argv[]){
   if(argc != 2){
      printf("usage: ./%s [file]\n", __FILE__);
      exit(1);
   }

   const char *file_name = argv[1];

   FILE *file = fopen(file_name, "r");
   assert(file && "Unable to open the file");

   sb file_contents;
   initsb(&file_contents, "");

   int c;

   while((c = fgetc(file)) != EOF){
      appendc(&file_contents, c);
   }

   fclose(file);
   size_t list_length;
   sb *list = sbsplit(&file_contents, WHITESPACE, &list_length);
   token *tokens = tokenizer(list, list_length);

   for(size_t i = 0; i < list_length; ++i){
      if(tokens[i].type == INT) printf("%s(%d)\n", type_to_str(tokens[i].type), tokens[i].number);
      else printf("%s\n", type_to_str(tokens[i].type));
   }
   
   free(tokens);
   freesblist(list, list_length);

   freesb(&file_contents);

   return 0;
}
