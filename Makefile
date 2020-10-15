CC=gcc
CFLAGS=-O3
LDFLAGS=

all:	simplexe

simplexe: simplexe.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm simplexe
