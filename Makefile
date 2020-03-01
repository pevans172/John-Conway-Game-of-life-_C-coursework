all: gameoflife.o libgol.so
		gcc -Wall -Wextra -pedantic -std=c11  gameoflife.o -L $(shell pwd) -l gol -o gameoflife -lm

libgol.o: gol.c 
		gcc -Wall -Wextra -pedantic -std=c11 -fPIC -c gol.c -o libgol.o -lm		

libgol.so: libgol.o 
		gcc -Wall -Wextra -pedantic -std=c11 -shared libgol.o -o libgol.so -lm

gameoflife.o: gameoflife.c
		gcc -Wall -Wextra -pedantic -std=c11 -c gameoflife.c -o gameoflife.o -lm

clean:
		rm -rf libgol.o libgol.so gameoflife.o gameoflife
