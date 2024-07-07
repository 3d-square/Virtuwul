#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "typecheck.h"
#include "stringbuilder.h"
#include "token.h"

struct value{
   sb *key;
   int value;
};

char *type_to_str(TYPE type){
   switch(type){
      case INT   : return "INT";
      case DOUBLE: return "DOUBLE";
      case STRING: return "STRING";
      case CHAR  : return "CHAR";
      case PLUS  : return "PLUS";
      case MINUS : return "MINUS";
      case DIV   : return "DIV";
      case MULT  : return "MULT";
      case EQUAL : return "EQUAL";
      case BOOLEAN : return "BOOLEAN";
      case PRINT : return "PRINT";
      case NONE  : return "NONE";
      case IF    : return "IF";
      case DO    : return "DO";
      case END   : return "END";
   }

   return "";
}

token sb_to_token(sb *str){
   // sb_print(str);
   token new_token;
   TYPE type = NONE;
   
   if(sb_strequal(str, "+", 1)){
      type = PLUS;
   }else if(sb_strequal(str, "-", 1)){
      type = MINUS;
   }else if(sb_strequal(str, "/", 1)){
      type = DIV;
   }else if(sb_strequal(str, "*", 1)){
      type = MULT;
   }else if(sb_strequal(str, "==", 2)){
      type = EQUAL;
   }else if(sb_strequal(str, "if", 2)){
      type = IF;
   }else if(sb_strequal(str, "do", 2)){
      type = DO;
   }else if(sb_strequal(str, "end", 3)){
      type = END;
   }else if(sb_strequal(str, "print", 5)){
      type = PRINT;
   }else if(sb_strequal(str, "true", 4)){
      type = BOOLEAN;
      new_token.number = 1;
   }else if(sb_strequal(str, "false", 5)){
      type = BOOLEAN;
      new_token.number = 0;
   }else if(is_int(str)){
      type = INT;
      char *c_str = sb_to_cstr(str);
      new_token.number = atol(c_str);
      free(c_str);
   }else if(is_double(str)){
      type = DOUBLE;
      char *c_str = sb_to_cstr(str);
      new_token.floating = atof(c_str);
      free(c_str);
   }else if(str->string[0] == '"' && str->string[str->size - 1] == '"'){
      type = STRING;
      new_token.string = sb_to_cstr(str);
   }else{
      new_token.string = sb_to_cstr(str);
   }


   new_token.type = type;
   new_token.freeable = 0;
   
   return new_token;
}

void free_tokens(token *tokens, size_t length){
   for(size_t i = 0; i < length; ++i){
      if(tokens[i].type == STRING){
         free(tokens[i].string);
      }
   }
   free(tokens);
}

void print_token(token *t){
   if(t->type == INT) printf("%s(%ld)\n", type_to_str(t->type), t->number);
   else if(t->type == DOUBLE) printf("%s(%f)\n", type_to_str(t->type), t->floating);
   else printf("%s\n", type_to_str(t->type));
}

void print_tokens(token *tokens, size_t list_length){
   for(size_t i = 0; i < list_length; ++i){
      print_token(&tokens[i]);
   }
}

token *split_file_contents(sb *file_contents, size_t *size){
   size_t start = 0;
   size_t end = 0;
   size_t file_length = file_contents->size;
   char *raw_file = sb_to_cstr(file_contents);
   size_t list_size = 0;
   size_t list_capacity = 16;

   token *tokens = malloc(sizeof(token) * list_capacity);
   // printf("list in bytes: %lu\n", sizeof(token) * list_capacity);
   assert(tokens);

   for(end = 0; end < file_length; ++end){
      // ignore all leading whitespace
      while(isspace(raw_file[start]) && end + 1 < file_length){
         ++end;
         ++start;
      }
      if(isspace(raw_file[end])){
         sb new_word = sb_substring(file_contents, start, end - 1);
         if(list_size >= list_capacity){
            list_capacity *= 2;
            tokens = realloc(tokens, list_capacity * sizeof(token));
            assert(tokens);
         }

         tokens[list_size++] = sb_to_token(&new_word);
         // sb_print(&new_word);
         sb_free(&new_word);
         start = end + 1;
      }else if(raw_file[end] == '"'){
         ++end;
         while(end < file_length){
            if(raw_file[end] == '"'){
               sb new_word = sb_substring(file_contents, start + 1, end - 1);
               if(list_size >= list_capacity){
                  list_capacity *= 2;
                  tokens = realloc(tokens, list_capacity * sizeof(token));
                  assert(tokens);
               }
      
               tokens[list_size++] = sb_to_token(&new_word);
               // sb_print(&new_word);
               sb_free(&new_word);
            }
            ++end;
         }
         start = end + 2;
      
      }
   }
   free(raw_file);

   *size = list_size;
   return tokens;
}

/*
token *tokenizer(sb *strings, size_t length){

   token *tokens = malloc(sizeof(token) * length);

   for(size_t i = 0; i < length; ++i){
      tokens[i] = sb_to_token(&(strings[i]));
   }

   return tokens;
} 
*/

void tokens_to_bytes(token *tokens, size_t list_length, const char *file_name){
   char bytecode[strlen(file_name) + 4];
   strcpy(bytecode, file_name);
   strcat(bytecode, ".bc");
   bytecode[strlen(file_name) + 3] = '\0';
   
   FILE *output_file = fopen(bytecode, "wb");
   assert(output_file);

   fwrite((char *)(&list_length), sizeof(list_length), 1, output_file);

   fwrite((char *)tokens, TOKEN_SIZE, list_length, output_file);

   fclose(output_file);
}

token *tokens_from_bytes(const char *file_name, size_t *list_length){
   FILE *input_file = fopen(file_name, "rb");
   assert(input_file);

   token *tokens;

   // Read in the number of tokens
   fread(list_length, sizeof(size_t), 1, input_file);

   tokens = malloc(TOKEN_SIZE * (*list_length));
   assert(tokens);

   // Read the tokens from the file
   fread((char *)tokens, TOKEN_SIZE, *list_length, input_file);

   fclose(input_file);


   return tokens;
}


