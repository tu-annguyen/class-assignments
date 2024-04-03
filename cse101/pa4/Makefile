#------------------------------------------------------------------------------
#  Tu-An Nguyen, tunhnguy
#  2021 Spring CSE 101 PA4
#  Makefile for List ADT
#
#  make                makes Shuffle
#  make ListClient     make ListClient
#  make clean          removes binary files
#  make check1         runs valgrind on ListClient
#  make check2         runs valgrind on Shuffle with command line argument 35
#------------------------------------------------------------------------------

COMPILE = g++ -std=c++11 -Wall -c
LINK = g++ -std=c++11 -Wall -o
MEMCHECK = valgrind --leak-check=full
DEBUG = -g

Shuffle : Shuffle.o List.o
	$(LINK) Shuffle Shuffle.o List.o 

Shuffle.o : List.h Shuffle.cpp
	$(COMPILE) $(DEBUG) Shuffle.cpp

ListTest: ListTest.o List.o
	$(LINK) ListTest ListTest.o List.o 

ListTest.o : List.h ListTest.cpp
	$(COMPILE) $(DEBUG) ListTest.cpp

List.o : List.h List.cpp
	$(COMPILE) $(DEBUG) List.cpp

clean :
	rm -f Shuffle Shuffle.o ListTest ListTest.o List.o

check1 : ListTest
	$(MEMCHECK) ./ListTest

check2 : Shuffle
	$(MEMCHECK) ./Shuffle 35