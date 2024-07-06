#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "stringbuilder.h"
#include "token.h"

#define STACK_SIZE 500

#define CASE(m, b) case m: b break
#define CASEPASS(m) case m:

#define get_value(v) (v->type == INT) ? v->number : v->floating
#define is_numeric(t) (t == INT || t == DOUBLE)

double absolute(double x){
   return x < 0.0 ? -1 * x : x;
}

void print_tokens(token *tokens, size_t list_length){
   for(size_t i = 0; i < list_length; ++i){
      if(tokens[i].type == INT) printf("%s(%ld)\n", type_to_str(tokens[i].type), tokens[i].number);
      else if(tokens[i].type == DOUBLE) printf("%s(%f)\n", type_to_str(tokens[i].type), tokens[i].floating);
      else printf("%s\n", type_to_str(tokens[i].type));
   }
}

token *split_file_contents(sb *file_contents, size_t *size){
   size_t start = 0;
   size_t end = 0;
   size_t file_length = file_contents->size;
   char *raw_file = file_contents->string;
   size_t list_size = 0;
   size_t list_capacity = 16;

   token *tokens = malloc(sizeof(token) * list_capacity);
   // printf("list in bytes: %lu\n", sizeof(token) * list_capacity);
   assert(tokens);

   for(end = 0; end < file_length; ++end){
      if(isspace(raw_file[end])){
         sb new_word = sb_substring(file_contents, start, end - 1);
         if(list_size >= list_capacity){
            list_capacity *= 2;
            tokens = realloc(tokens, list_capacity * sizeof(token));
            assert(tokens);
         }

         tokens[list_size++] = sb_to_token(&new_word);
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
               sb_free(&new_word);
            }
            ++end;
         }
         start = end + 2;
      
      }
   }

   *size = list_size;
   return tokens;
}

token *tokenizer(sb *strings, size_t length){

   token *tokens = malloc(sizeof(token) * length);

   for(size_t i = 0; i < length; ++i){
      tokens[i] = sb_to_token(&(strings[i]));
   }

   return tokens;
}


token *do_equal(token *lhs, token *rhs){
   int equal_type;

   token *output = (lhs->freeable == 1) ? lhs : malloc(sizeof(token));

   if(lhs->type == EQUAL && rhs->type == EQUAL){
      equal_type = 0;
   }else if(lhs->type != EQUAL && rhs->type != EQUAL){
      equal_type = 1;
   }else{
      assert(0 && "Unable to compare values of different types");
   }

   if(equal_type == 0){
      output->number = lhs->number == rhs->number;
   }else if(equal_type == 1){
      output->number = absolute(((double)get_value(lhs)) - ((double)get_value(rhs))) < .000000001;
   }

   output->type = BOOLEAN;
   output->freeable = 1;

   return output;
}

token *do_operation(TYPE type, token *lhs, token *rhs){
   assert((is_numeric(rhs->type) ||
          is_numeric(lhs->type)) && "Operations are only supported for numerical types");

   token *output = (lhs->freeable == 1) ? lhs : malloc(sizeof(token));
   assert(output && "Could not create the new token");

   // Chose the type of the output
   // long + long = long
   // long + int = long
   // int + double = double
   // long + double = double
   //

   TYPE output_type = (lhs->type == DOUBLE || rhs->type == DOUBLE) ? DOUBLE : INT;
   if(output_type == DOUBLE){
      output->floating = (double)(get_value(lhs));
      if(type == PLUS){
         output->floating += get_value(rhs);
      }else if(type == MULT){
         output->floating *= get_value(rhs);
      }else if(type == MINUS){
         output->floating -= get_value(rhs);
      }else if(type == DIV){
         output->floating /= get_value(rhs);
      }else{
         assert(0 && "This operation has not been implemented");
      }
      }else if(output_type == INT){
         output->number = get_value(lhs);
      if(type == PLUS){
         output->number += get_value(rhs);
      }else if(type == MINUS){
         output->number -= get_value(rhs);
      }else if(type == MULT){
         output->number *= get_value(rhs);
      }else if(type == DIV){
         output->number /= get_value(rhs);
      }else{
         assert(0 && "This operation has not been implemented");
      }
   }

   output->freeable = 1;
   output->type = output_type;
   return output;
}

int interpreter(token *program, size_t program_size){
   size_t op_index = 0;
   size_t stack_top = 0;
   token *stack[STACK_SIZE];
   token *current;

   while(op_index < program_size){
      current = &(program[op_index]);
      switch(current->type){
         CASEPASS(MINUS)
         CASEPASS(DIV)
         CASEPASS(MULT)
         CASE(PLUS, {
            assert(op_index + 1 < program_size && "Operation requires left hand side");
            stack[stack_top - 1] = do_operation(current->type, stack[stack_top - 1], &(program[++op_index]));
         });
         CASE(EQUAL, {
            assert(op_index + 1 < program_size && "Operation requires left hand side");
            stack[stack_top - 1] = do_equal(stack[stack_top - 1], &(program[++op_index]));
         });
         CASE(DOUBLE, {
            stack[stack_top++] = current;      
         });
         CASEPASS(BOOLEAN)
         CASE(INT, {
            stack[stack_top++] = current;
         });
         CASE(PRINT, {
            token *token_to_print = stack[--stack_top];
            if(token_to_print->type == INT){
               printf("%ld\n", token_to_print->number);
            }else if(token_to_print->type == DOUBLE){
               printf("%f\n", token_to_print->floating);
            }else if(token_to_print->type == BOOLEAN){
               printf("%s\n", token_to_print->number ? "true" : "false");
            }else{
               printf("%s in print has not been implemented yet\n", type_to_str(token_to_print->type));
            }
            if(token_to_print->freeable == 1){
               free(token_to_print);
            }
         });
         default:
            printf("%s in interpreter has not been implemented yet\n", type_to_str(current->type));
            assert(0);
      }

      ++op_index;
   }

   return 0;
}


int main(int argc, char *argv[]){
   if(argc != 2){
      printf("usage: ./%s [file]\n", __FILE__);
      exit(1);
   }

   const char *file_name = argv[1];

   size_t list_length;
   // sb *list = sb_read_tokens(file_name, &list_length);
   sb file_contents = sb_read_file(file_name);
   token *tokens = split_file_contents(&file_contents, &list_length);
   sb_free(&file_contents);

   // token *tokens = tokenizer(list, list_length);


   interpreter(tokens, list_length);
   
   free_tokens(tokens, list_length);
   // sb_free_list(list, list_length);

   return 0;
}
