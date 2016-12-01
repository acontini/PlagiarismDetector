
#include "PlagiarismDetector.hpp"
#include <string>
#include <tuple>
#include <set>
#include "NgramCollection.hpp"
#include "LanguageModel.hpp"

/* Takes name of a file that contains paths to the documents. */
void PlagiarismDetector :: readFileList(std::string &fname) {

}

/* Read a list of paths from a file. */
std::vector<std::string> PlagiarismDetector :: readNames(std::string &listFile) {

}

/* Add ngrams to the model from a vector of words. */
void PlagiarismDetector :: buildNgrams(const std::vector<std::string> &text) {

}

/* Detects any documents listed in the file list that may be plagiarized, to the initialized sensitivity. */
void PlagiarismDetector :: detect() {

}

/* Prints the possible matches detected. */
std::set<std::tuple<std::string, std::string>> PlagiarismDetector :: getPossibleMatches() {
  return matches;
}

/* Overloads << for a set of string tuples. */
std::ostream& operator<<(std::ostream& out, const std::set<std::tuple<std::string, std::string>>& strTuples) {

  return out;

}

