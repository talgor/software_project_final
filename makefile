CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

symnmf: symnmf.o symnmf_algo.o utils.o
	gcc -o symnmf symnmf.o symnmf_algo.o utils.o -lm

symnmf.o: symnmf.c symnmf_algo.h
	gcc $(CFLAGS) -c symnmf.c

symnmf_algo.o: symnmf_algo.c symnmf_algo.h utils.h
	gcc $(CFLAGS) -c symnmf_algo.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

clean:
	rm -f *.o symnmf