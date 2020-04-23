CC=gcc
CFLAGS=-g -Wall -O0
LDFLAGS=

all:	simplexe

simplexe: simplexe.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm simplexe
