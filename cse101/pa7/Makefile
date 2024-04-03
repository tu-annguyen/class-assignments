#------------------------------------------------------------------------------
#  Tu-An Nguyen, tunhnguy
#  2021 Spring CSE 101 PA7
#  Makefile for CSE 101 Programming Assignment 7
#
#  make                  makes Order
#  make WordFrequency    makes WordFrequency
#  make clean            removes all binaries
#  make check1           checks WordFrequency for memory leaks on Big.txt 
#  make check2           checks Order for memory leaks on English.txt
#------------------------------------------------------------------------------

TOP              = Order
TOP_OBJ          = $(TOP).o
TOP_SRC          = $(TOP).c
ADT              = Dictionary
TEST             = WordFrequency
ADT_TEST		 = DictionaryTest
TEST_OBJ         = $(TEST).o
TEST_SRC         = $(TEST).c
ADT_TEST_OBJ     = $(ADT_TEST).o
ADT_TEST_SRC	 = $(ADT_TEST).c
ADT_OBJ          = $(ADT).o
ADT_SRC          = $(ADT).c
ADT_HDR          = $(ADT).h
COMPILE          = gcc -std=c11 -Wall -c 
LINK             = gcc -std=c11 -Wall -o
REMOVE           = rm -f
MEMCHECK         = valgrind --leak-check=full
DEBUG			 = -g

$(TOP) : $(TOP_OBJ) $(ADT_OBJ)
	$(LINK) $(TOP) $(TOP_OBJ) $(ADT_OBJ)

$(TEST) : $(TEST_OBJ) $(ADT_OBJ)
	$(LINK) $(TEST) $(TEST_OBJ) $(ADT_OBJ)

$(ADT_TEST) : $(ADT_TEST_OBJ) $(ADT_OBJ)
	$(LINK) $(ADT_TEST) $(ADT_TEST_OBJ) $(ADT_OBJ)

$(TOP_OBJ) : $(TOP_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(TOP_SRC)

$(TEST_OBJ) : $(TEST_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(TEST_SRC)

$(ADT_TEST_OBJ) : $(ADT_TEST_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(ADT_TEST_SRC)

$(ADT_OBJ) : $(ADT_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(ADT_SRC)

clean :
	$(REMOVE) $(TOP) $(TOP_OBJ) $(TEST) $(ADT_TEST) $(TEST_OBJ) $(ADT_OBJ) $(ADT_TEST_OBJ) junk

check1 : $(TEST)
	$(MEMCHECK) ./$(TEST) Big.txt junk

check2 : $(TOP)
	$(MEMCHECK) ./$(TOP) English.txt junk
