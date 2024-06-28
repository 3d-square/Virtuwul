UTILS=stringbuilder.o token.o typecheck.o

all: interpreter 

interpreter: main.c stringbuilder.o token.o typecheck.o
	gcc -Wall -Wextra -Werror -Wfatal-errors $(UTILS) -o $@ $<


stringbuilder.o: stringbuilder.c
	gcc -Wall -Wextra -Werror -Wfatal-errors -c -o $@ $<


token.o: token.c
	gcc -Wall -Wextra -Werror -Wfatal-errors -c -o $@ $<

typecheck.o: typecheck.c
	gcc -Wall -Wextra -Werror -Wfatal-errors -c -o $@ $<
