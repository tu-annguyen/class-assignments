#------------------------------------------------------------------------------
#  Tu-An Nguyen, tunhnguy
#  2021 Spring CSE101 PA3
#  Makefile for CSE 101 Programming Assignment 3
#
#  make                     makes FindComponents
#  make all                 makes FindComponents, GraphTest, and GraphClient
#  make FindComponents      makes FindComponents
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

FindComponents : FindComponents.o $(BASE_OBJECTS)
	$(LINK) FindComponents FindComponents.o $(BASE_OBJECTS)

all : FindComponents GraphTest GraphClient

GraphClient : GraphClient.o $(BASE_OBJECTS)
	$(LINK) GraphClient GraphClient.o $(BASE_OBJECTS)

GraphTest : GraphTest.o $(BASE_OBJECTS)
	$(LINK) GraphTest GraphTest.o $(BASE_OBJECTS)

FindComponents.o : FindComponents.c $(HEADERS)
	$(COMPILE) $(DEBUG) FindComponents.c
 
GraphClient.o : GraphClient.c $(HEADERS)
	$(COMPILE) $(DEBUG) GraphClient.c

GraphTest.o : GraphTest.c $(HEADERS)
	$(COMPILE) $(DEBUG) GraphTest.c

$(BASE_OBJECTS) : $(BASE_SOURCES) $(HEADERS)
	$(COMPILE) $(DEBUG) $(BASE_SOURCES)

clean :
	$(REMOVE) FindComponents GraphClient GraphTest FindComponents.o GraphClient.o GraphTest.o $(BASE_OBJECTS)

checkFind : FindComponents
	$(MEMCHECK) ./FindComponents in3 junk3

checkClient : GraphClient
	$(MEMCHECK) ./GraphClient