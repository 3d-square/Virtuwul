#include <stdlib.h>
#include <stdio.h>
#include "typecheck.h"
#include "stringbuilder.h"
#include "token.h"

char *type_to_str(TYPE type){
   switch(type){
      case INT   : return "INT";
      case DOUBLE: return "DOUBLE";
      case LONG  : return "LONG";
      case STRING: return "STRING";
      case CHAR  : return "CHAR";
      case PLUS  : return "PLUS";
      case MINUS : return "MINUS";
      case DIV   : return "DIV";
      case MULT  : return "MULT";
      case NONE  : return "NONE";
   }

   return "";
}

token sb_to_token(sb *str){
   TYPE type = NONE;
   if(strequal(str, "+", 1)) type = PLUS;
   else if(is_int(str)) type = INT;

   if(type == NONE){
      printf("Unable to implement the token '%.*s'\n", (int)(str->size), str->string);
      exit(1);
   }

   token new_token;
   new_token.type = type;
   
   if(type == INT){
      char *c_str = cstr(str);
      new_token.number = atoi(c_str);
      free(c_str);
   }

   return new_token;
}
