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
  if (arg == "l") {
    return low;
  } else if (arg == "m") {
    return medium;
  } else if (arg == "h") {
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

  if (argc < 2 || argc > 3) {
    printUsage();
    return 1;
  }

  std::string inputName(argv[1]);
  Sensitivity s = medium;

  if (argc == 3) {
    std::string senString(argv[2]);
    s = charToSensitivity(senString);
  }

  if (s == invalid) {
    return 1;
  }

  PlagiarismDetector detector(s);
  detector.readFileList(inputName);
  detector.detect();
  std::cout << detector.getPossibleMatches();

  return 0;

}

