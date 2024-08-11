#include "stringbuilder.h"
#include "typecheck.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

int is_int(sb *string){
   if(!(string && string->size > 0)){
      fprintf(stderr, "String is undefined\n");
      exit(1);
   }

   char *end;
   char *c_str = sb_to_cstr(string);
   strtol(c_str, &end, 10);
   int ret = end[0] == '\0';
   free(c_str);

   return ret;
}

int is_double(sb *string){
   assert((string && string->size > 0) && "String is undefined");

   char *end;
   char *c_str = sb_to_cstr(string);
   strtod(c_str, &end);
   int ret = end[0] == '\0';
   free(c_str);

   return ret;
}
