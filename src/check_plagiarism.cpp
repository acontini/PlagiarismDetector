/*
600.120.01 CS120 Intermediate Programming
Final Project - Plagiarism Detector
check_plagiarism.cpp - driver to detect plagiarism within documents listed in a file, with a sensitivity
12/1/2016

Ryan Demo
rdemo1
rdemo1@jhu.edu

Mengying Pan
mpan4
mpan4@jhu.edu
*/

#include <iostream>
#include <string>
#include <tuple>
#include <set>
#include "PlagiarismDetector.hpp"

Sensitivity charToSensitivity(std::string arg) {
  if (arg == "l" || arg == "L") {  // Handles both upper and lower case
    return low;
  } else if (arg == "m" || arg == "M") {
    return medium;
  } else if (arg == "h" || arg == "H") {
    return high;
  } else {
    return invalid;
  }
}

/* Function to print how to call this program. */
void printUsage() {
    std::cout << "Usage: check_plagiarism [file with list of document paths] [sensitivity]\n";
}

/* Main: parse arguments, call appropriate functions. */
int main(int argc, char** argv) {

  if (argc < 2 || argc > 3) {  // Given an incorrect number of args
    printUsage();
    return 1;
  }

  std::string inputName(argv[1]);  // Get the file name with the list of document paths
  Sensitivity s = medium;  // Initialize the sensitivity

  if (argc == 3) {  // If given a sensitivity argument, get the enum case for it
    std::string senString(argv[2]);
    s = charToSensitivity(senString);
  }

  if (s == invalid) {  // Quit if given invalid sensitivity
    std::cerr << "Invalid sensitivity.\n";
    return 1;
  }

  PlagiarismDetector detector(s);  // Create a detector with a sensitivity
  detector.readFileList(inputName);  // Read the file list
  detector.detect();  // Run the detection algorithm
  std::cout << detector.getPossibleMatches();  // Get the matched documents and print them

  return 0;

}

