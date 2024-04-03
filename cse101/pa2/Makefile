#------------------------------------------------------------------------------
#  Tu-An Nguyen, tunhnguy
#  2021 Spring CSE101 PA2
#  Makefile for CSE 101 Programming Assignment 2
#
#  make                     makes FindPath
#  make all                 makes FindPath, GraphTest, and GraphClient
#  make FindPath            makes FindPath
#  make GraphClient         makes GraphClient
#  make GraphTest           makes GraphTest
#  make clean               removes binaries
#  make checkFind           tests FindPath for memory leaks on in3
#  make checkClient         tests GraphClient for memory leaks
#------------------------------------------------------------------------------

BASE_SOURCES   = Graph.c List.c
BASE_OBJECTS   = Graph.o List.o
HEADERS        = Graph.h List.h
COMPILE        = gcc -c -std=c11 -Wall
LINK           = gcc -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full
DEBUG          = -g

FindPath : FindPath.o $(BASE_OBJECTS)
	$(LINK) FindPath FindPath.o $(BASE_OBJECTS)

all : FindPath GraphTest GraphClient

GraphClient : GraphClient.o $(BASE_OBJECTS)
	$(LINK) GraphClient GraphClient.o $(BASE_OBJECTS)

GraphTest : GraphTest.o $(BASE_OBJECTS)
	$(LINK) GraphTest GraphTest.o $(BASE_OBJECTS)

FindPath.o : FindPath.c $(HEADERS)
	$(COMPILE) $(DEBUG) FindPath.c

GraphClient.o : GraphClient.c $(HEADERS)
	$(COMPILE) $(DEBUG) GraphClient.c

GraphTest.o : GraphTest.c $(HEADERS)
	$(COMPILE) $(DEBUG) GraphTest.c

$(BASE_OBJECTS) : $(BASE_SOURCES) $(HEADERS)
	$(COMPILE) $(DEBUG) $(BASE_SOURCES)

clean :
	$(REMOVE) FindPath GraphClient GraphTest FindPath.o GraphClient.o GraphTest.o $(BASE_OBJECTS)

checkFind : FindPath
	$(MEMCHECK) FindPath in3 junk3

checkClient : GraphClient
	$(MEMCHECK) GraphClient