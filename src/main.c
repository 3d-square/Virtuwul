#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "stringbuilder.h"
#include "token.h"
#include "interpreter.h"
#include "parser.h"
#include <linked_token.h>

void compile(const char *file_name){
   sb file_contents = sb_read_file(file_name);
   printf("Reading tokens...\n");
   linked_token *tokens = split_file_contents(&file_contents);
   sb_free(&file_contents);

   printf("Parsing %lu tokens....\n", tokens->size);

   token_parser(tokens);

   tokens_to_bytes(tokens, file_name);
   printf("Byte code created.\n");
   linked_print(tokens);
   
   linked_free(tokens);
}

void run(const char *file_name){
   size_t list_length;
   token *tokens = tokens_from_bytes(file_name, &list_length);

   interpreter(tokens, list_length);

   free_tokens(tokens, list_length);
}

int main(int argc, char *argv[]){
   if(argc != 3){
      printf("usage: ./%s [compile|run] [file]\n", __FILE__);
      exit(1);
   }

   const char *file_name = argv[2];

   if(strcmp(argv[1], "compile") == 0){
      compile(file_name);
   }else if(strcmp(argv[1], "run") == 0){
      run(file_name);
   }else if(strcmp(argv[1], "compile-run") == 0){
      compile(file_name);
      char bytecode[strlen(file_name) + 4];
      int bc_l = 0;
      for(int i = 0; file_name[i] && file_name[i] != '.'; ++i){
         bytecode[bc_l++] = file_name[i];
      }
      bytecode[bc_l] = '.';
      bytecode[bc_l + 1] = 'b';
      bytecode[bc_l + 2] = 'c';
      bytecode[bc_l + 3] = '\0';


      run(bytecode);
   }else{
      fprintf(stderr, "'%s' is an unrecognized command\n", argv[1]);
   }

   return 0;
}
