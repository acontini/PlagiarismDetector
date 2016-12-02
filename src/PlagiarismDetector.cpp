/*
600.120.01 CS120 Intermediate Programming
Final Project - Plagiarism Detector
PlagiarismDetector.cpp - implementation for a plagiarism detection model
12/1/2016

Ryan Demo
rdemo1
rdemo1@jhu.edu

Mengying Pan
mpan4
mpan4@jhu.edu
*/

#include <string>
#include <tuple>
#include <set>
#include <fstream>
#include <iostream>
#include "NgramCollection.hpp"
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

  auto dStart = documents.begin();
  auto dEnd = documents.end();
  auto mEnd = matches.end();

  for (auto iter1 = dStart; iter1 != dEnd; iter1++) {
    for (auto iter2 = dStart; iter2 != dEnd; iter2++) {

      unsigned int pos1 = iter1 - dStart;
      unsigned int pos2 = iter2 - dStart;
      std::tuple<unsigned int, unsigned int> pair = pos1 < pos2 ? std::make_tuple(pos1, pos2) : std::make_tuple(pos2, pos1);

      if (pos1 != pos2 && matches.find(pair) == mEnd) {
        if (iter1->isPlagiarismSuspect(*iter2)) {
          matches.insert(pair);
        }
      }

    }
  }
}

/* Check if another NgramDocument is suspect of plagiarizing the current NgramDocument. */
bool NgramDocument :: isPlagiarismSuspect(NgramDocument &other) { 
  return intersectionCount(ngrams, other.ngrams) > 0.4 * ngrams.total;
}

/* Prints the possible matches detected. */
std::set<std::tuple<std::string, std::string>> PlagiarismDetector :: getPossibleMatches() {

  if (matches.empty()) {
    std::cerr << "You must first run .detect() on a PlagiarismDetector before seeing matching results.\n";
  }

  std::set<std::tuple<std::string, std::string>> matchesStringTuples;

  for (auto &match : matches) {
    std::string first = documents[std::get<0>(match)].name;
    std::string second = documents[std::get<1>(match)].name;
    matchesStringTuples.insert(std::make_tuple(first, second));
  }

  return matchesStringTuples;

}

/* Overloads << for a set of string tuples. */
std::ostream& operator<<(std::ostream& out, const std::set<std::tuple<std::string, std::string>>& strTuples) {
  for (auto &tup : strTuples) {
    out << std::get<0>(tup) << ", " << std::get<1>(tup) << "\n";
  }
  return out;
}

/* Return the vector of sentence Ngrams for the file of the name fname. */
std::vector<NgramCollection> NgramDocument::sentenceNgrams(std::string fname) {
  std::vector<NgramCollection> sentences;

  std::ifstream fin(fname);
  if (!fin.is_open()) { // make sure it opened
    std::cerr << "Error: could not open file '" << fname << "', file will be skipped...\n";
    return;
  }
  std::string word;
  std::vector<std::string> text; // the document will be stored here
  while (fin >> word) {
    text.push_back(word);
    if (n <= text.size()
	&&(punct.find(word.back()) != punct.end() 
	||(quote.find(word.back()) != quote.end() && punct.find(*(word.end()-2)) != punct.end()))) {
      NgramCollection sentence(n);
      auto first = text.begin(); // first element
      auto last = text.begin() + n; // n-1th element
      int i = 0;

      while (i+n <= text.size()) {
	sentence.increment(first, last); // add ngrams until we run out
	++first;
	++last;
	++i;
      }
      sentences.push_back(sentence);
    }
    text.clear();
  }
  fin.close(); // close the file
  return sentences;  
}


