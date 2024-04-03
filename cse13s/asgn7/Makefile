CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic
DEBUG = -g

all: encode decode

encode: encode.o io.o trie.o word.o
	$(CC) $(CFLAGS) $(DEBUG) -o encode encode.o io.o trie.o word.o

decode: decode.o io.o trie.o word.o
	$(CC) $(CFLAGS) $(DEBUG) -o decode decode.o io.o trie.o word.o

encode.o: encode.c
	$(CC) $(DEBUG) -c encode.c

decode.o: decode.c
	$(CC) $(DEBUG) -c decode.c

io.o: io.c
	$(CC) $(DEBUG) -c io.c

trie.o: trie.c
	$(CC) $(DEBUG) -c trie.c

word.o: word.c
	$(CC) $(DEBUG) -c word.c

clean:
	rm -f encode decode encode.o decode.o io.o trie.o word.o

format:
	clang-format -i -style=file encode.c decode.c io.c trie.c word.c code.h endian.h io.h word.h
