CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: mathlib.o mathlib-test.o
	$(CC) $(CFLAGS) -o mathlib-test mathlib.o mathlib-test.o -lm

mathlib.o: mathlib.c
	$(CC) -c mathlib.c

mathlib-test.o: mathlib-test.c
	$(CC) -c mathlib-test.c

clean:
	rm -f mathlib mathlib-test mathlib.o mathlib-test.o
