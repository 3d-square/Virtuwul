#include "stringbuilder.h"
#include "typecheck.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

int is_int(sb *string){
   assert((string && string->size > 0) && "String is undefined");

   char *end;
   char *c_str = cstr(string);
   long _long = strtol(c_str, &end, 10);
   int ret = end[0] == '\0' && _long <= INT_MAX;
   free(c_str);

   return ret;
}

int is_long(sb *string){
   assert((string && string->size > 0) && "String is undefined");

   char *end;
   char *c_str = cstr(string);
   strtol(c_str, &end, 10);
   int ret = end[0] == '\0';
   free(c_str);

   return ret;
}

int is_double(sb *string){
   assert((string && string->size > 0) && "String is undefined");

   char *end;
   char *c_str = cstr(string);
   strtod(c_str, &end);
   int ret = end[0] == '\0';
   free(c_str);

   return ret;
}
