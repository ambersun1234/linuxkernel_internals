all: bbp

bbp: main.c bbp.c bbp.h
	gcc -D _GNU_SOURCE bbp.c main.c -o bbp -std=c11 -lm -lpthread

.PHONY: clean

clean:
	rm -rf bbp a.out
