#include <stdlib.h>
#include <stdio.h>
#include "typecheck.h"
#include "stringbuilder.h"
#include "token.h"

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
   }

   return "";
}

token sb_to_token(sb *str){
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
   }

   if(type == NONE){
      printf("Unable to implement the token '%.*s'\n", (int)(str->size), str->string);
      exit(1);
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
