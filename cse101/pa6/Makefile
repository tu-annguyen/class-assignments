#------------------------------------------------------------------------------
#  Tu-An Nguyen, tunhnguy
#  2021 Spring CSE 101 PA6
#  Makefile for CSE 101 Programming Assignment 6
#
#  make                     makes Lex
#  make DictionaryTest      makes DictionaryTest
#  make clean               removes all binaries
#  make check1              checks DictionaryTest for memory leaks
#  make check2              checks Lex for memory leaks on in4
#------------------------------------------------------------------------------

TOP              = Lex
TOP_OBJ          = $(TOP).o
TOP_SRC          = $(TOP).c
ADT              = Dictionary
TEST             = $(ADT)Test
TEST_OBJ         = $(TEST).o
TEST_SRC         = $(TEST).c
ADT_OBJ          = $(ADT).o
ADT_SRC          = $(ADT).c
ADT_HDR          = $(ADT).h
COMPILE          = gcc -std=c11 -Wall -c 
LINK             = gcc -std=c11 -Wall -o
REMOVE           = rm -f
MEMCHECK         = valgrind --leak-check=full
DEBUG            = -g

$(TOP) : $(TOP_OBJ) $(ADT_OBJ)
	$(LINK) $(TOP) $(TOP_OBJ) $(ADT_OBJ)

$(TEST) : $(TEST_OBJ) $(ADT_OBJ)
	$(LINK) $(TEST) $(TEST_OBJ) $(ADT_OBJ)

$(TOP_OBJ) : $(TOP_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(TOP_SRC)

$(TEST_OBJ) : $(TEST_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(TEST_SRC)

$(ADT_OBJ) : $(ADT_SRC) $(ADT_HDR)
	$(COMPILE) $(DEBUG) $(ADT_SRC)

clean :
	$(REMOVE) $(TOP) $(TOP_OBJ) $(TEST) $(TEST_OBJ) $(ADT_OBJ)

check1 : $(TEST)
	$(MEMCHECK) ./$(TEST)

check2 : $(TOP)
	$(MEMCHECK) ./$(TOP) in4 junk4