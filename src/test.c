#include <stdio.h>
#include <string.h>
#include <linked_token.h>
#include <token.h>

void move1(int array[], int length, int eleI, int toI){

   if(eleI >= length || toI >= length) return;

   int tmp = array[eleI];
   int currI;

   if(eleI < toI){
      printf("Hi\n");
      currI = eleI;

      for(;currI < toI; currI++){
         array[currI] = array[currI + 1];
      }
      array[toI] = tmp;
   }else if(eleI > toI){
      printf("Hi2\n");
      currI = eleI;

      for(;currI > toI; currI--){
         array[currI] = array[currI - 1];
      }
      array[toI] = tmp;  
   }

}

void test1(){
   int array[10] = {1,2,3,4,5,6,7,8,9,10};

   for(int i = 0; i < 10; ++i){
      printf("|%d|", array[i]);
   }
   puts("\n");

   move1(array, 10, 10, 4);

   for(int i = 0; i < 10; ++i){
      printf("|%d|", array[i]);
   }
   puts("\n");
}

void test2(){
   linked_token *list = init_linked();

   token t1 = {0, 0, {}};
   token t2 = {1, 0, {}};
   token t3 = {2, 0, {}};
   token t4 = {3, 0, {}};

   puts("Size 1");
   linked_add(list, t1);
   linked_print(list);

   puts("Size 0");
   linked_remove(list, 0);
   linked_print(list);

   puts("Size 1");
   linked_add(list, t2);
   linked_print(list);

   puts("Size 2");
   linked_add(list, t3);
   linked_print(list);

   puts("Size 3");
   linked_add(list, t4);
   linked_print(list);

   puts("Size 2");
   linked_remove(list, 2);
   linked_print(list);

   linked_free(list);
}

int main(){
   // test1();
   test2();
}
