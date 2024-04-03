CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic
DEBUG = -g

all: sorting

sorting: sorting.o bubble.o shell.o quick.o stack.o heap.o set.o
	$(CC) $(CFLAGS) $(DEBUG) -o sorting sorting.o bubble.o shell.o quick.o stack.o heap.o set.o

sorting.o: sorting.c
	$(CC) $(DEBUG) -c sorting.c

bubble.o: bubble.c
	$(CC) $(DEBUG) -c bubble.c

shell.o: shell.c
	$(CC) $(DEBUG) -c shell.c

quick.o: quick.c
	$(CC) $(DEBUG) -c quick.c

stack.o: stack.c
	$(CC) $(DEBUG) -c stack.c

heap.o: heap.c
	$(CC) $(DEBUG) -c heap.c

set.o: set.c
	$(CC) $(DEBUG) -c set.c

clean:
	rm -f sorting sorting.o bubble.o shell.o quick.o stack.o heap.o set.o

format:
	clang-format -i -style=file sorting.c bubble.h bubble.c gaps.h shell.h shell.c quick.h quick.c stack.h stack.c heap.h heap.c set.h set.c
