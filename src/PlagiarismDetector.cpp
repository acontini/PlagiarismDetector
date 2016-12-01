
#include <string>
#include <tuple>
#include <set>
#include <fstream>
#include <iostream>
#include "NgramCollection.hpp"
#include "LanguageModel.hpp"
#include "PlagiarismDetector.hpp"

/* Takes name of a file that contains paths to the documents. */
void PlagiarismDetector :: readFileList(std::string &fname) {
  std::vector<std::string> nameList = readNames(fname);
  for (std::string &name : nameList) {
    addTextFromFile(name);
  }
}

/* Read a list of paths from a file. */
std::vector<std::string> PlagiarismDetector :: readNames(std::string &listFile) {
  std::vector<std::string> names;
  std::ifstream fin(listFile); // try to open file
  if (!fin.is_open()) { // see if it worked
    std::cerr << "Error: failed to open listfile '" << listFile << "'\n";
    exit(-1);
  }

  std::string name;
  while (fin >> name) {
    names.push_back(name); // read names until we run out of file
  }
  fin.close(); // close the file
  return names;
}

/* Add text from the given document file to the model. */
void PlagiarismDetector :: addTextFromFile(std::string &fname) {
  std::ifstream fin(fname);
  if (!fin.is_open()) { // make sure it opened
    std::cerr << "Error: could not open file '" << fname << "', file will be skipped...\n";
    return;
  }

  std::string word;
  std::vector<std::string> text; // the document will be stored here
  for (unsigned i=1; i<n; i++) {
    text.push_back("<START_" + std::to_string(i) + ">"); // add n-1 start tags
  }
  while (fin >> word) {
    text.push_back(word); // read words from the file and add them to the document 
  }
  for (unsigned i=1; i<n; i++) {
    text.push_back("<END_" + std::to_string(i) + ">"); // add n-1 end tags
  }
  fin.close(); // close the file

  buildNgramDoc(fname, text); // add ngrams from the document to the model
}

/* Add ngrams to the model from a vector of words. */
void PlagiarismDetector :: buildNgramDoc(const std::string &fname, const std::vector<std::string> &text) {

  NgramDocument ngramDoc(fname, n);

  auto first = text.begin(); // first element
  auto last = text.begin() + n; // n-1th element

  while (last-1 != text.end()) {
    ngramDoc.ngrams.increment(first, last); // add ngrams until we run out
    ++first;
    ++last;
  }

  documents.push_back(ngramDoc);
}


/* Detects any documents listed in the file list that may be plagiarized, to the initialized sensitivity.
 * Results stored in matches variable. */
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

