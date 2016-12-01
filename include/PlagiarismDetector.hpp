#ifndef CS120_PLAGIARISM_DETECTOR_HPP
#define CS120_PLAGIARISM_DETECTOR_HPP

#include <string>
#include <vector>
#include <tuple>
#include <set>
#include "LanguageModel.hpp"


enum Sensitivity { low, medium, high, invalid };


class NgramDocument {

public:

  /* Constructor: need to specify n. */
  NgramDocument(int n) : ngrams(n) { assert(n > 1); }

  /* The name of the file. */
  std::string name;

  /* The ngrams model of the file. */
  NgramCollection ngrams;

};


class PlagiarismDetector {

public:

  /* Constructor; need to specify sensitivity. */
  PlagiarismDetector(Sensitivity s) : sen(s)  { }

  /* Takes name of a file that contains paths to the documents. */
  void readFileList(std::string &fname);

  /* Detects any documents listed in the file list that may be plagiarized, to the initialized sensitivity. */
  void detect();

  /* Returns a set of tuples with the possible detected matches. */
  std::set<std::tuple<std::string, std::string>> getPossibleMatches();

private:

  /* The sensitivity to which the detector is calibrated. */
  Sensitivity sen;

  /* Read a list of paths from a file. */
  std::vector<std::string> readNames(std::string &listFile);

  /* Add ngrams to the model from a vector of words. */
  void buildNgrams(const std::vector<std::string> &text);

  /* Holds the ngram collection of each file. */
  std::vector<NgramDocument> models;

  /* The set of possible matches. */
  std::set<std::tuple<std::string, std::string>> matches;

};

/* Overloads << for a set of string tuples. */
std::ostream& operator<<(std::ostream& out, const std::set<std::tuple<std::string, std::string>>& strTuples);

#endif

