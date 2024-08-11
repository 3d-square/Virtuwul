#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "stringbuilder.h"
#include "token.h"
#include <linked_token.h>

#define STACK_SIZE 1000
#define MAX_LENGTH 500

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
      }else if(type == DIV){
         output->floating /= get_value(rhs);
      }else if(type == MINUS){
         output->floating -= get_value(rhs);
      }else if(type == LESS){
         output->number = output->floating < (double)get_value(rhs);
      }else{
         assert(0 && "This operator has not been implemented");
      }
   }else{
      output->number = (get_value(lhs));
      if(type == PLUS){
         output->number += get_value(rhs);
      }else if(type == MULT){
         output->number *= get_value(rhs);
      }else if(type == DIV){
         output->number /= get_value(rhs);
      }else if(type == MINUS){
         output->number -= get_value(rhs);
      }else if(type == LESS){
         output->number = (output->number < (long)rhs->number);
      }else{
         assert(0 && "This operator has not been implemented");
      }
   }

   output->type = output_type;
   output->freeable = 1;

   return output;
}

int interpreter(token *program, size_t program_size){
   size_t op_index = 0;
   size_t stack_top = 0;
   token *stack[STACK_SIZE];
   token *current;
   int i = 0;

   while(op_index < program_size && i++ < MAX_LENGTH){
      current = &(program[op_index]);
      print_token(current);
      switch(current->type){
         CASEPASS(MINUS)
         CASEPASS(DIV)
         CASEPASS(MULT)
         CASEPASS(LESS)
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
               exit(1);
            }
            if(token_to_print->freeable == 1){
               free(token_to_print);
            }
         });
         CASE(IF, {
            // Do noting right now
            // printf("IF\n");
         });
         CASE(DO, {
            // printf("DO\n");
            token *truthy = stack[--stack_top];
            if(truthy->number){
               // do nothing
               // printf("Thing works\n");
            }else{
               op_index = current->number; // Go to end if the statement is false
               // printf("truthy->num = %ld\n", truthy->number);
               // printf("goto = %lu\n", op_index);
               // goto END_LOOP;
            }
            if(truthy->freeable == 1){
               free(truthy);
            }
         });
         CASE(END, {
            // printf("END\n");
            op_index = current->number;
            // printf("current->num = %ld\n", current->number);
         });
         CASE(VAR_INT, {
            printf("vari = %d\n", current->freeable);
            if(op_index + 2 < program_size && program[op_index + 1].type == SET){
               printf("Here\n");
               stack[stack_top++] = &program[op_index + 2];
               op_index += 2;
               printf("%lu\n",op_index);
            }else{
               fprintf(stderr, "VAR_INT IS NOT IMPLEddNTED FOR USAGE\n");
            }

         });
         default:
            printf("%s in interpreter has not been implemented yet\n", type_to_str(current->type));
            assert(0);
      }

      ++op_index;
      // print_token(current);
   }
// END_LOOP:

   return 0;
}
