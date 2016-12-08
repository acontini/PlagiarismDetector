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
std::vector<std::string> PlagiarismDetector :: readFileList(std::string &fname) {
  detectionPerformed = false;
  std::vector<std::string> nameList = readNames(fname);
  for (std::string &name : nameList) {
    addTextFromFile(name);
  }
  return nameList;
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

  std::vector<NgramCollection> sentences;
  static std::set<char> punctSet = {'!','?','.'};
  static std::set<char> quoteSet = {'\"', '\''};

  std::string word;
  std::vector<std::string> text; // the document will be stored here
  std::vector<std::string> sentenceText; // the current sentence will be stored here

  for (unsigned i=1; i<n; i++) {
    text.push_back("<START_" + std::to_string(i) + ">"); // add n-1 start tags
  }

  while (fin >> word) {
    bool end = false;
    if (punctSet.find(word.back()) != punctSet.end()) {  // Remove sentence-ending punctuation
      word = word.substr(0, word.size()-1);
      end = true;
    } else if ((word.length() > 1 && quoteSet.find(word.back()) != quoteSet.end() && punctSet.find(*(word.end()-2)) != punctSet.end())) {
      // Also remove sentence-ending punctuation if there's a quotation mark at the end
      word = word.substr(0, word.size()-2);
      end = true;
    }
    text.push_back(word); // read words from the file and add them to the document 
    sentenceText.push_back(word);  // add words to the current sentence

    if (end) {  // If the end of the sentence has been reached
      if (n > sentenceText.size()) {  // Discard sentence if less than n in length
        sentenceText.clear();
      } else {
        NgramCollection sentence(n);  // Make a new NgramCollection for the sentence
        auto first = sentenceText.begin(); // first element
        auto last = sentenceText.begin() + n; // n-1th element
        int i = 0;

        while (i+n <= sentenceText.size()) {
          sentence.increment(first, last); // add ngrams until we run out
          ++first;
          ++last;
          ++i;
        }
        sentences.push_back(sentence);  // Add the ngram representation of the sentence to the document's sentences
        sentenceText.clear();  // Clear sentence text array for next sentence
      }
    }
  }

  for (unsigned i=1; i<n; i++) {
    text.push_back("<END_" + std::to_string(i) + ">"); // add n-1 end tags
  }
  fin.close(); // close the file

  buildNgramDoc(fname, text, sentences); // add ngrams from the document to the model
}

/* Add ngrams to the model from a vector of words. */
void PlagiarismDetector :: buildNgramDoc(const std::string &fname, const std::vector<std::string> &text, const std::vector<NgramCollection> &sentences) {

  NgramDocument ngramDoc(fname, n);

  auto first = text.begin(); // first element
  auto last = text.begin() + n; // n-1th element

  while (last-1 != text.end()) {
    ngramDoc.ngrams.increment(first, last); // add ngrams until we run out
    ++first;
    ++last;
  }

  ngramDoc.sentenceNgrams = sentences;
  documents.push_back(ngramDoc);
}


/* Detects any documents listed in the file list that may be plagiarized, to the initialized sensitivity.
 * Results stored in matches variable. */
void PlagiarismDetector :: detect() {

  auto dStart = documents.begin();
  auto dEnd = documents.end();
  auto mEnd = matches.end();

  for (auto iter1 = dStart; iter1 != dEnd; iter1++) {  // Loop over documents
    for (auto iter2 = dStart; iter2 != dEnd; iter2++) {  // Loop over all docs for each document

      unsigned int pos1 = iter1 - dStart;
      unsigned int pos2 = iter2 - dStart;
      // Make this document pair a tuple sorted by document index
      std::tuple<unsigned int, unsigned int> pair = pos1 < pos2 ? std::make_tuple(pos1, pos2) : std::make_tuple(pos2, pos1);

      if (pos1 != pos2 && matches.find(pair) == mEnd) {  // If documents aren't the same, and haven't been flagged for plagiarism yet
        if (iter1->isPlagiarismSuspect(*iter2)) {  // See if the two have suspicious similarities
          matches.insert(pair);  // If they do, add the pair to the matches
        }
      }

    }
  }
  detectionPerformed = true;  // This flag is to prevent a potential user from calling detect twice on the same dataset with the same sensitivity
}

/* Check if another NgramDocument is suspect of plagiarizing the current NgramDocument. */
bool PlagiarismDetector :: NgramDocument :: isPlagiarismSuspect(NgramDocument &other) { 
  static const double containmentThreshold = 0.4;
 
  for (auto &sentence : sentenceNgrams) {  // For each sentence in the current document, check the sentence intersection ratio with the other document
    if (other.ngrams.intersectionRatioWithSentence(sentence) > containmentThreshold) {  // If above suspicion level, return true
      return true;
    }
  }
  return false;
  
}

/* Prints the possible matches detected. */
std::set<std::tuple<std::string, std::string>> PlagiarismDetector :: getPossibleMatches() {

  if (matches.empty()) {
    std::string msg = detectionPerformed ? "There are no matches.\n" : "You must run detect() after importing documents to check.\n";
    std::cerr << msg;
  }

  std::set<std::tuple<std::string, std::string>> matchesStringTuples;  // Create the return type, we're now going to map from int to string based on the name at the int index

  for (auto &match : matches) {  // Loop over the matches and add the name tuples to the return object
    std::string first = documents[std::get<0>(match)].name;
    std::string second = documents[std::get<1>(match)].name;
    matchesStringTuples.insert(std::make_tuple(first, second));
  }

  return matchesStringTuples;

}

/* Overloads << for a set of string tuples. */
std::ostream& operator<<(std::ostream& out, const std::set<std::tuple<std::string, std::string>>& strTuples) {
  for (auto &tup : strTuples) {
    out << std::get<0>(tup) << ", " << std::get<1>(tup) << "\n";  // Print a set of string tuples, comma separated
  }
  return out;
}

/* Prints all the ngrams in the document set. For testing purposes. */
std::string PlagiarismDetector :: toString() {
  std::string s{};
  for (NgramDocument& nd : documents) {
    s = s + nd.ngrams.toString('a');
  }
  return s;
}

/* Prints all sentence ngrams in all documents in the document set. For testing purposes. */
std::string PlagiarismDetector :: sentenceToString() {
  std::string s{};
  for (NgramDocument& nd : documents) {
    s += nd.name + "\n";
    for (NgramCollection& nc : nd.sentenceNgrams) {
      s += nc.toString('a');
    }
  }
  return s;
}
