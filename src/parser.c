#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "stringbuilder.h"
#include "token.h"
#include <linked_token.h>

#define CASE(m, b) case m: b break
#define CASEPASS(m) case m:
#define print_control_stack() print_list(stack_top, control_stack)

void print_list(size_t length, token **list){
   for(size_t i = 0; i < length; ++i){
      print_token(list[i]);
   }
}

int ptr_search(token *list[], size_t list_size, const token *act){
   const char *act_v = act->string;
   for(size_t i = 0; i < list_size; ++i){
      if(strcmp(list[i]->string,act_v) == 0){
         return i;
      }
   }
   return -1;
}

void token_parser(linked_token *tokens){
   linked_node *curr = tokens->head;
   size_t op_index;
   int stack_tracker = 0;
   token *control_stack[tokens->size];
   size_t stack_top = 0;
   token *variables[100];
   size_t var_top = 0;
   // linked_print(tokens);

   for(op_index = 0; op_index < tokens->size && curr; ++op_index, curr = curr->next){
      token *current = &(curr->value);
      // printf("%lu: ", op_index);
      // print_token(current);
      switch(current->type){
         CASEPASS(INT)
         CASEPASS(BOOLEAN)
         CASE(DOUBLE, {
            ++stack_tracker;
         });
         CASEPASS(MINUS)
         CASEPASS(DIV)
         CASEPASS(MULT)
         CASEPASS(EQUAL)
         CASEPASS(LESS)
         CASE(PLUS, {
            // Make sure there is a token on the stack
            assert(stack_tracker > 0 && "Print expects a operator to print");
            // Make sure there is a compatible token folowing this
            assert(op_index + 1 < tokens->size && "Operator expects a right hand operation");

            // Do nothing to the stack tracker because the operator will always add a item to the value
            
            ++op_index;
            curr = curr->next;
         });
         CASE(PRINT, {
            assert(stack_tracker > 0 && "Print expects a operator to print");

            --stack_tracker;
         });
         CASE(IF, {
            current->number = -1;
            control_stack[stack_top++] = current;
         });
         CASE(FOR, {
            current->number = op_index;
            control_stack[stack_top++] = current;
         });
         CASE(DO, {
            assert(stack_tracker > 0 && "DO statements require a boolean expression");
            assert(stack_top > 0 && "DO statements require a control statement");
            print_control_stack();

            TYPE control_op = control_stack[stack_top - 1]->type;

            if(control_op == IF){
               control_stack[stack_top++] = current;
               assert(op_index + 1 < tokens->size && "DO statements require an ending token");
            }else if(control_op == FOR){
               control_stack[stack_top - 1]->number = op_index;
               control_stack[stack_top++] = current;
               assert(op_index + 1 < tokens->size && "DO statements require an ending token");
            }else{
               fprintf(stderr, "%s is not a control_op\n", type_to_str(control_op));
               exit(1);
            }
         });
         CASE(END, {
            assert(stack_top > 0 && "END statements require a control and do statement");

            token *do_op = control_stack[stack_top - 1];

            if(do_op->type == DO){
               token *control_op = control_stack[stack_top - 2];
               do_op->number = op_index; // if do is false go to end
               current->number = control_op->number >= 0 ? control_op->number : (long)op_index;
            }else{
               fprintf(stderr, "END requires a DO statement not %s\n", type_to_str(do_op->type));
               exit(1);
            }

            stack_top -= 2;
         });
         CASE(NONE, {
            int index;
            if((index = ptr_search(variables, var_top, current)) != -1){
               // variables[var_top++] = current;
               token *initial = linked_at(tokens, index);
               current->type = initial->type;
               current->freeable = initial->freeable;
            }else if(op_index + 1 < tokens->size && curr->next->value.type == SET){
               variables[var_top++] = current;
               current->freeable = var_top;
            }else{
               fprintf(stderr, "Error: NONE type %s is undefined\n", current->string);
               exit(1);
            }
           
         });
         CASE(SET, {
            assert(var_top > 0 && "Set expects an identifier on the left hand side");
            token *identifier = variables[var_top - 1];

            if(identifier->type == NONE){
               identifier->type = VAR_INT;
            }else{
               fprintf(stderr, "SET requires a NONE statement not %s\n", type_to_str(identifier->type));
               exit(1);
            }
         });
         // CASE(TO, {
         //    assert(stack_top > 0 && "TO expectes FOR statement");
         //    if(control_stack[stack_top - 1]->type != FOR) assert(0 && "TO expectes FOR statement");
         // });
         default: {
            printf("%s is unimplemented\n", type_to_str(current->type));
            assert(0 && "A token has not been implemented yet");
         } break;
      }
   }

}

