HEADERS=token.h stringbuilder.h
OBJ=./obj/interpreter.o ./obj/parser.o ./obj/token.o ./obj/typecheck.o ./obj/stringbuilder.o ./obj/linked_token.o
CFLAGS=-Wall -Wextra -Werror # -Wfatal-errors 
INCLUDE_PATH=-I ./include/
all: virtuwul $(OBJ)

virtuwul: ./src/main.c $(OBJ)
	gcc $(CFLAGS) $(INCLUDE_PATH) -o $@ ./src/main.c $(OBJ)

./obj/%.o: ./src/%.c
	gcc $(CFLAGS) $(INCLUDE_PATH) -c -o $@ $<

clean:
	rm ./obj/* virtuwul

tests: ./src/test.c
	gcc $(CFLAGS) $(INCLUDE_PATH) -o tmptest ./src/test.c $(OBJ)
