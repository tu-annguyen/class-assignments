CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic
DEBUG = -g

all: gen dec err

gen: generator.o bm.o hamming.o
	$(CC) $(CFLAGS) $(DEBUG) -o gen generator.o bm.o hamming.o

dec: decoder.o bm.o hamming.o
	$(CC) $(CFLAGS) -o dec decoder.o bm.o hamming.o

err: error.o
	$(CC) $(CFLAGS) -o err error.o

bm.o: bm.c
	$(CC) $(DEBUG) -c bm.c

hamming.o: hamming.c
	$(CC) $(DEBUG) -c hamming.c

generator.o: generator.c
	$(CC) $(DEBUG) -c generator.c

decoder.o: decoder.c
	$(CC) $(DEBUG) -c decoder.c

error.o: error.c
	$(CC) -c error.c

clean:
	rm -f gen dec err generator.o decoder.o error.o bm.o hamming.o

format:
	clang-format -i -style=file generator.c decoder.c bm.c hamming.c error.c bm.h hamming.h
