#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "stringbuilder.h"
#include "token.h"

#define STACK_SIZE 1000

#define CASE(m, b) case m: b break
#define CASEPASS(m) case m:

double absolute(double x){
   return x < 0.0 ? -1 * x : x;
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
   if(argc != 3){
      printf("usage: ./%s [compile|run] [file]\n", __FILE__);
      exit(1);
   }

   const char *file_name = argv[2];

   if(strcmp(argv[1], "compile") == 0){
      size_t list_length;
      sb file_contents = sb_read_file(file_name);
      token *tokens = split_file_contents(&file_contents, &list_length);
      sb_free(&file_contents);

      tokens_to_bytes(tokens, list_length, file_name);
      
      free_tokens(tokens, list_length);
   }else if(strcmp(argv[1], "run") == 0){
      size_t list_length;
      token *tokens = tokens_from_bytes(file_name, &list_length);

      interpreter(tokens, list_length);

      free_tokens(tokens, list_length);
   }else{
      fprintf(stderr, "'%s' is an unrecognized command\n", argv[1]);
   }

   return 0;
}
