CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic
DEBUG = -g

all: banhammer

banhammer: banhammer.o bf.o bv.o hash.o ll.o node.o parser.o speck.o
	$(CC) $(CFLAGS) $(DEBUG) -o banhammer banhammer.o bf.o bv.o hash.o ll.o node.o parser.o speck.o

banhammer.o: banhammer.c
	$(CC) $(DEBUG) -c banhammer.c 

bf.o: bf.c
	$(CC) $(DEBUG) -c bf.c

bv.o: bv.c
	$(CC) $(DEBUG) -c bv.c 

hash.o: hash.c
	$(CC) $(DEBUG) -c hash.c 

ll.o: ll.c
	$(CC) $(DEBUG) -c ll.c 

node.o: node.c
	$(CC) $(DEBUG) -c node.c 

parser.o: parser.c
	$(CC) $(DEBUG) -c parser.c 

speck.o: speck.c
	$(CC) $(DEBUG) -c speck.c

clean:
	rm -f banhammer banhammer.o bf.o bv.o hash.o ll.o node.o parser.o speck.o

format:
	clang-format -i -style=file banhammer.c bf.c bf.h bv.c bv.h hash.c hash.h ll.c ll.h node.c node.h parser.c parser.h speck.c speck.h
