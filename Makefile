CC=gcc
CFLAGS=-Wall  

all: binsearch datagen

datagen: datagen.c 
	$(CC) -o  datagen datagen.c -g -lm $(CFLAGS)

binsearch: binsearch.c util.o
	$(CC) -o  binsearch binsearch.c util.o -g -pthread -lm $(CFLAGS)

util.o:
	$(CC) -c -o util.o util.c -lm $(CFLAGS) 

clean:
	rm datagen binsearch *.o
 
