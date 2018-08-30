CC=gcc
CFLAGS=-Wall -std=c11

all: binsearch datagen

datagen: datagen.c 
	$(CC) -o datagen datagen.c -lm $(CFLAGS)

binsearch: binsearch.c util.o
	$(CC) -o binsearch binsearch.c util.o -pthread -lm $(CFLAGS)

util.o:
	$(CC) -c -o util.o util.c -lm $(CFLAGS) 

clean:
	rm datagen binsearch *.o
 
