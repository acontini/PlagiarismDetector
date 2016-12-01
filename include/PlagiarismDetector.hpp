#ifndef CS120_PLAGIARISM_DETECTOR_HPP
#define CS120_PLAGIARISM_DETECTOR_HPP
#include <vector>
#include <string>
#include <cassert>
#include "NgramCollection.hpp"
#include "LanguageModel.hpp"

enum Sensitivity { low, medium, high, invalid };

class PlagiarismDetector {

public:

  // constructor; need to specify sensitivity
  PlagiarismDetector(Sensitivity s) : sen(s)  { }

  // takes name of a file that contains paths to the documents
  void readFileList(std::string fname);

  // detects any documents listed in the file list that may be plagiarized, to the initialized sensitivity
  void detect();

  // prints the possible matches detected
  void printPossibleMatches();

private:

  // the sensitivity to which the detector is calibrated
  Sensitivity sen;

  // holds the language model of each file
  std::vector<LanguageModel> models;

};

#endif

