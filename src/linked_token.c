#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <token.h>
#include <linked_token.h>
#include <stringbuilder.h>
#include <assert.h>
#include <stdlib.h>

#define TODO() fprintf(stderr, "%s:%s:%d: TODO\n", __FILE__, __FUNCTION__, __LINE__); exit(1)
#define LOG_STATEMENT(st) /*printf("Begin %s\n", #st);*/ st 


linked_token *linked_add(linked_token *list, token value){
   if(list->head == NULL){

      list->head = malloc(sizeof(linked_node));

      list->head->prev = NULL;
      list->head->next = NULL;
      list->head->value = value;

      list->tail = list->head;
   }else{
      linked_node *tail = list->tail;

      tail->next = malloc(sizeof(linked_node));

      tail->next->prev = tail;
      tail->next->next = NULL;
      tail->next->value = value;

      list->tail = list->tail->next;
   }
   list->size++;
   return list;
}

linked_token *linked_remove(linked_token *list, int index){
   linked_node *current = list->head;

   for(int i = 0; i < index && current; ++i){
      current = current->next;
   }

   if(current == NULL){
      fprintf(stderr, "Error: List index out of bounds\n");
      exit(1);
   }

   if(list->head == list->tail){
      list->head = NULL;
      list->tail = NULL;
      free(list->head);
   }else if(current == list->tail){
      list->tail = list->tail->prev;
      list->tail->next = NULL;
      
      free(current);
   }else{
      current->prev = current->next;
      free(current);
   }
   list->size--;
   return list;
}

linked_token *init_linked(){
   linked_token *new_list = malloc(sizeof(linked_token));

   new_list->head = NULL;
   new_list->tail = NULL;
   new_list->size = 0;

   return new_list;
}

void linked_free(linked_token *list){
   linked_node *curr = list->head;
   while(curr){
      linked_node *tmp = curr->next;
      free(curr);
      curr = tmp;
   }

   free(list);
}

void linked_print(linked_token *list){
   linked_node *current = list->head;
   size_t i = 0;
   while(i < list->size && current){
      LOG_STATEMENT(printf("%s ", type_to_str(current->value.type)));

      LOG_STATEMENT(current = current->next);
      i++;
   }
   printf("\n");
}

void tokens_to_bytes(linked_token *tokens, const char *file_name){
   char bytecode[strlen(file_name) + 4];
   int bc_l = 0;
   for(int i = 0; file_name[i] && file_name[i] != '.'; ++i){
      bytecode[bc_l++] = file_name[i];
   }
   bytecode[bc_l] = '.';
   bytecode[bc_l + 1] = 'b';
   bytecode[bc_l + 2] = 'c';
   bytecode[bc_l + 3] = '\0';
   
   FILE *output_file = fopen(bytecode, "wb");
   assert(output_file);

   fwrite((char *)(&tokens->size), sizeof(tokens->size), 1, output_file);
   
   linked_node *curr = tokens->head;
   while(curr){
      fwrite((char *)(&curr->value), sizeof(token), 1, output_file);
      curr = curr->next;
   }

   fclose(output_file);
}

linked_token *split_file_contents(sb *file_contents){
   size_t start = 0;
   size_t end = 0;
   size_t file_length = file_contents->size;
   char *raw_file = sb_to_cstr(file_contents);

   linked_token *tokens = init_linked();
   // printf("list in bytes: %lu\n", sizeof(token) * list_capacity);
   assert(tokens);

   for(end = 0; end < file_length; ++end){
      // ignore all leading whitespace
      while(isspace(raw_file[start]) && end + 1 < file_length){
         ++end;
         ++start;
      }
      if(isspace(raw_file[end])){
         sb new_word = sb_substring(file_contents, start, end - 1);

         linked_add(tokens, sb_to_token(&new_word));
         // sb_print(&new_word);
         sb_free(&new_word);
         start = end + 1;
      }else if(raw_file[end] == '"'){
         ++end;
         while(end < file_length){
            if(raw_file[end] == '"'){
               sb new_word = sb_substring(file_contents, start + 1, end - 1);
      
               linked_add(tokens, sb_to_token(&new_word));

               sb_free(&new_word);
            }
            ++end;
         }
         start = end + 2;
      
      }
   }
   free(raw_file);

   return tokens;
}

token *linked_at(linked_token *list, int index){
   if(list == NULL){
      fprintf(stderr, "List has not been initialized\n");
      exit(1);
   }else if((size_t)index >= list->size){
      fprintf(stderr, "Index Out of Bounds\n");
      exit(1);
   }

   linked_node *current = list->head;

   for(int i = 0; i < index; i++, current = current->next);

   return &current->value;
}
