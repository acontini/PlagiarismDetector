###################################################
# 600.120.01 CS120 Intermediate Programming
# Final Project - Plagiarism Detector
# Makefile - Makefile for final project
# 12/1/2016
# 
# Ryan Demo
# rdemo1
# rdemo1@jhu.edu
# 
# Mengying Pan
# mpan4
# mpan4@jhu.edu
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
# Targets 
###############################

all: $(BIN_DIR)/check_plagiarism $(BIN_DIR)/unitTest

test: $(BIN_DIR)/unitTest
	$(BIN_DIR)/unitTest

$(BIN_DIR)/check_plagiarism: $(OBJ_DIR)/check_plagiarism.o $(OBJ_DIR)/NgramCollection.o $(OBJ_DIR)/PlagiarismDetector.o
	$(GCC) $(CPPFLAGS) $(OBJ_DIR)/check_plagiarism.o $(OBJ_DIR)/NgramCollection.o $(OBJ_DIR)/PlagiarismDetector.o -o $(BIN_DIR)/check_plagiarism

$(BIN_DIR)/unitTest: $(OBJ_DIR)/unitTests-NgramCollection.o $(OBJ_DIR)/unitTestDriver.o $(OBJ_DIR)/NgramCollection.o
	$(GCC) $(CPPFLAGS) $(OBJ_DIR)/unitTestDriver.o $(OBJ_DIR)/unitTests-NgramCollection.o $(OBJ_DIR)/NgramCollection.o -o $(BIN_DIR)/unitTest

$(OBJ_DIR)/check_plagiarism.o: check_plagiarism.cpp $(INC_DIR)/PlagiarismDetector.hpp
	$(GCC) $(CPPFLAGS) -c check_plagiarism.cpp -o $(OBJ_DIR)/check_plagiarism.o

$(OBJ_DIR)/unitTestDriver.o: unitTestDriver.cpp 
	$(GCC) $(CPPFLAGS) -c unitTestDriver.cpp -o $(OBJ_DIR)/unitTestDriver.o

$(OBJ_DIR)/unitTests-NgramCollection.o: unitTests-NgramCollection.cpp 
	$(GCC) $(CPPFLAGS) -c unitTests-NgramCollection.cpp -o $(OBJ_DIR)/unitTests-NgramCollection.o

$(OBJ_DIR)/NgramCollection.o: NgramCollection.cpp $(INC_DIR)/NgramCollection.hpp 
	$(GCC) $(CPPFLAGS) -c NgramCollection.cpp -o $(OBJ_DIR)/NgramCollection.o

$(OBJ_DIR)/PlagiarismDetector.o: PlagiarismDetector.cpp $(INC_DIR)/PlagiarismDetector.hpp $(INC_DIR)/NgramCollection.hpp
	$(GCC) $(CPPFLAGS) -c PlagiarismDetector.cpp -o $(OBJ_DIR)/PlagiarismDetector.o


###############################
# "Utility" Rules
###############################
# the .PHONY line says these targets aren't expected to generate files named after the target
.PHONY: clean test

# clean up generated files
clean:
	@echo Removing generated files...
	@rm -f $(OBJ_DIR)/*.o 
