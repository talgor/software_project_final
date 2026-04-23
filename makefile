CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

symnmf: symnmf.o utils.o
	gcc -o symnmf symnmf.o utils.o -lm

symnmf.o: symnmf.c symnmf.h utils.h
	gcc $(CFLAGS) -c symnmf.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

clean:
	rm -f *.o symnmf