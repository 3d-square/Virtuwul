#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "stringbuilder.h"



void appendc(sb *builder, char c){
   assert(builder);
   assert(builder->string);

   if(builder->size + 1 > builder->capacity){
      builder->string = realloc(builder->string, builder->capacity * 2);

      assert(builder->string);
      builder->capacity *= 2;
   }

   builder->string[(builder->size)++] = c;
}

void appends(sb *builder, char *s){
   size_t s_len = strlen(s);
   if(builder->size + s_len > builder->capacity){
      builder->string = realloc(builder->string, builder->capacity + s_len);

      assert(builder->string);
      builder->capacity += s_len;
   }

   strncat(builder->string, s, s_len);
   builder->size += s_len;
}

void initsb(sb *builder, char *s){
   assert(builder);
   size_t s_len = strlen(s);
   size_t init_len = s_len > 8 ? s_len : 8;

   builder->string = malloc(init_len * sizeof(char));

   assert(builder->string);

   strncpy(builder->string, s, s_len);
   builder->capacity = init_len;
   builder->size = s_len;
}

void clear(sb *builder){
   assert(builder);
   builder->size = 0;
}

void freesb(sb *builder){
   assert(builder);
   free(builder->string);
}

void printsb(sb *builder){
   assert(builder);
   printf("'%.*s'\n", (int)builder->size, builder->string);
}

sb get_word(const sb *builder, size_t start, char delim, size_t *spaceidx){
   
   assert(builder);
   assert(builder->string);
   assert(start < builder->size);
   size_t word_len = 0;

   if(delim == WHITESPACE){
      for(long unsigned  i = start; i < builder->size && !isspace(builder->string[i]); ++word_len, ++i);
   }else{
      for(long unsigned  i = start; i < builder->size && builder->string[i] != delim; ++word_len, ++i);
   }

   *spaceidx = start + word_len;

   sb word = {0};

   char *string = malloc(word_len);
   strncpy(string, builder->string + start, word_len);

   word.string = string;
   word.capacity = word_len;
   word.size = word_len;


   return word;
}

sb substring(sb *builder, size_t start, size_t end){
   assert(builder);
   assert(builder->string);
   assert(start < builder->size);

   end = builder->size < end ? builder->size : end;
   size_t substr_len = end - start + 1;

   sb substr;

   substr.size = substr_len;
   substr.capacity = substr_len;

   char *string = malloc(substr_len * sizeof(char));

   strncpy(string, builder->string + start, substr_len);
   substr.string = string;

   return substr;
}

sb *sbsplit(const sb *builder, char delim, size_t *length){
   assert(builder);
   assert(builder->string);

   size_t array_size = 0;

   for(size_t i = 0; i < builder->size; ++i){
      if(delim != WHITESPACE){
         if(builder->string[i] == delim) ++array_size;
      }else{
         if(isspace(builder->string[i])) ++array_size;
      }
   }

   sb *splitted = malloc((array_size + 1) * sizeof(sb));

   size_t next = 0;
   size_t index = 0;

   while(next < builder->size){
      splitted[index++] = get_word(builder, next, delim, &next);
      ++next;
   }

   
   *length = index;

   return splitted;
   
}

void freesblist(sb *list, size_t length){
   assert(list);

   for(size_t i = 0; i < length; ++i){
      freesb(&(list[i]));
   }

   free(list);

}

int sbequal(sb *lhs, sb *rhs){
   if(lhs == rhs)
      return 1;

   if(lhs == NULL || rhs == NULL)
      return 0;

   if(lhs->string == rhs->string)
      return 1;

   if(lhs->size != rhs->size)
      return 0;

   return strncmp(lhs->string, rhs->string, lhs->size) == 0;
}


int strequal(sb *builder, char *string, size_t size){
   if(builder == NULL)
      return 0;

   if(string == NULL)
      return 0;

   if(builder->size != size)
      return 0;

   return strncmp(builder->string, string, size) == 0;
}

char *cstr(sb *builder){
   assert(builder);

   char *sbascstr = malloc((builder->size + 1) * sizeof(char));
   strncpy(sbascstr, builder->string, builder->size);
   sbascstr[builder->size] = '\0';

   return sbascstr;
}
