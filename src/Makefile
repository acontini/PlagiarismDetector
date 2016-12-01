###################################################
# Sample Makefile for HW8
# Ben Mitchell
# 2016-11-14
#
###################################################

###############################
# Compiler commands
###############################
GCC = g++
CPPFLAGS = -std=c++11 -pedantic -Wall -Wextra -g -I../include

###############################
# pa.hpp variables
###############################
# where to look for .hpp files
INC_DIR = ../include
# where to put .o files
OBJ_DIR = ./obj
# where to put executables
BIN_DIR = ../bin
 
# make sure the output directories exists...
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

###############################
# targets 
###############################

all: $(BIN_DIR)/a8 $(BIN_DIR)/unitTest

test: $(BIN_DIR)/unitTest
	$(BIN_DIR)/unitTest

$(BIN_DIR)/a8: $(OBJ_DIR)/a8.o $(OBJ_DIR)/NgramCollection.o $(OBJ_DIR)/LanguageModel.o 
	$(GCC) $(CPPFLAGS) $(OBJ_DIR)/a8.o $(OBJ_DIR)/NgramCollection.o $(OBJ_DIR)/LanguageModel.o -o $(BIN_DIR)/a8

$(BIN_DIR)/unitTest: $(OBJ_DIR)/unitTests-NgramCollection.o $(OBJ_DIR)/unitTestDriver.o $(OBJ_DIR)/NgramCollection.o
	$(GCC) $(CPPFLAGS) $(OBJ_DIR)/unitTestDriver.o $(OBJ_DIR)/unitTests-NgramCollection.o $(OBJ_DIR)/NgramCollection.o -o $(BIN_DIR)/unitTest

$(OBJ_DIR)/a8.o: a8.cpp 
	$(GCC) $(CPPFLAGS) -c a8.cpp -o $(OBJ_DIR)/a8.o

$(OBJ_DIR)/unitTestDriver.o: unitTestDriver.cpp 
	$(GCC) $(CPPFLAGS) -c unitTestDriver.cpp -o $(OBJ_DIR)/unitTestDriver.o

$(OBJ_DIR)/unitTests-NgramCollection.o: unitTests-NgramCollection.cpp 
	$(GCC) $(CPPFLAGS) -c unitTests-NgramCollection.cpp -o $(OBJ_DIR)/unitTests-NgramCollection.o

$(OBJ_DIR)/NgramCollection.o: NgramCollection.cpp $(INC_DIR)/NgramCollection.hpp 
	 $(GCC) $(CPPFLAGS) -c NgramCollection.cpp -o $(OBJ_DIR)/NgramCollection.o

$(OBJ_DIR)/LanguageModel.o: LanguageModel.cpp $(INC_DIR)/LanguageModel.hpp $(INC_DIR)/NgramCollection.hpp
	 $(GCC) $(CPPFLAGS) -c LanguageModel.cpp -o $(OBJ_DIR)/LanguageModel.o


###############################
# "utility" rules
###############################
# the .PHONY line says these targets aren't expected to generate files named after the target
.PHONY: clean test

# clean up generated files
clean:
	@echo removing generated files...
	@rm -f $(OBJ_DIR)/*.o 
