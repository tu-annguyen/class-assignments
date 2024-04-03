CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: life.o universe.o
	$(CC) $(CFLAGS) -o life life.o universe.o -lncurses

universe.o: universe.c
	$(CC) -c universe.c

life.o: life.c
	$(CC) -c life.c

clean:
	rm -f universe life universe.o life.o

format:
	clang-format -i -style=file universe.c
	clang-format -i -style=file life.c
	clang-format -i -style=file universe.h
