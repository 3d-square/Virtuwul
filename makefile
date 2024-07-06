UTILS=stringbuilder.o token.o typecheck.o
HEADERS=token.h stringbuilder.h

all: interpreter 

interpreter: main.c stringbuilder.o token.o typecheck.o 
	gcc -Wall -Wextra -Werror -Wfatal-errors $(UTILS) -o $@ $<


stringbuilder.o: stringbuilder.c stringbuilder.h
	gcc -Wall -Wextra -Werror -Wfatal-errors -c -o $@ $<


token.o: token.c token.h
	gcc -Wall -Wextra -Werror -Wfatal-errors -c -o $@ $<

typecheck.o: typecheck.c typecheck.h
	gcc -Wall -Wextra -Werror -Wfatal-errors -c -o $@ $<
