#include <iostream>
#include <sstream>
#include <string>
#include "PlagiarismDetector.hpp"

enum Sensitivity { low, medium, high, invalid };

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

  PlagiarismDetector detector();
  detector.readFileList(inputName);
  detector.detectWithSensitivity(s);
  detector.printPossibleMatches();

  return 0;

}

