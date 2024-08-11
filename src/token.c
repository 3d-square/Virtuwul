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
      case LESS  : return "LESS";
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
      case FOR   : return "FOR";
      case SET   : return "SET";
      case TO    : return "TO";
      case VAR_INT : return "VAR_INT";
   }

   return "";
}

token sb_to_token(sb *str){
   // sb_print(str);
   token new_token = {0};
   TYPE type = NONE;
   
   if(sb_strequal(str, "+", 1)){
      type = PLUS;
   }else if(sb_strequal(str, "-", 1)){
      type = MINUS;
   }else if(sb_strequal(str, "/", 1)){
      type = DIV;
   }else if(sb_strequal(str, "*", 1)){
      type = MULT;
   }else if(sb_strequal(str, "<", 1)){
      type = LESS;
   }else if(sb_strequal(str, "=", 1)){
      type = SET;
   }else if(sb_strequal(str, "==", 2)){
      type = EQUAL;
   }else if(sb_strequal(str, "if", 2)){
      type = IF;
   }else if(sb_strequal(str, "do", 2)){
      type = DO;
   }else if(sb_strequal(str, "->", 2)){
      type = TO;
   }else if(sb_strequal(str, "end", 3)){
      type = END;
   }else if(sb_strequal(str, "for", 3)){
      type = FOR;
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
   else if(t->type == DO) printf("DO -> %ld\n", t->number);
   else printf("%s\n", type_to_str(t->type));
}

void print_tokens(token *tokens, size_t list_length){
   for(size_t i = 0; i < list_length; ++i){
      print_token(&tokens[i]);
   }
}

token *tokens_from_bytes(const char *file_name, size_t *list_length){
   FILE *input_file = fopen(file_name, "rb");
   assert(input_file);

   token *tokens;
   // Read in the number of tokens
   fread(list_length, sizeof(size_t), 1, input_file);

   printf("%lu\n", *list_length);
   tokens = malloc(TOKEN_SIZE * (*list_length));
   assert(tokens);

   // Read the tokens from the file
   fread((char *)tokens, TOKEN_SIZE, *list_length, input_file);

   fclose(input_file);


   return tokens;
}


