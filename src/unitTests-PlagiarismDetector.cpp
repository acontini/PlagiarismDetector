/*
600.120.01 CS120 Intermediate Programming
Final Project - Plagiarism Detector
unitTests-PlagiarismDetector.cpp - unit tests for the PlagiarismDetector class
12/6/2016

Ryan Demo
rdemo1
rdemo1@jhu.edu

Mengying Pan
mpan4
mpan4@jhu.edu
*/

/*
 * Unit test cases for PlagiarismDetector class.
 *
 * TODO: some test cases have been provided, but you will need
 * to write your own for the remaining functions
 */

#include "catch.hpp" // simple unit-testing framework
//#include "../include/PlagiarismDetector.hpp" // header declaring the functions we want to test
#include "PlagiarismDetector.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

using std::string;
using std::vector;
using std::list;


TEST_CASE("readNames","[readNames]") {
  Sensitivity s = medium;
  std::string n = "../data/filenameList1.txt";
  PlagiarismDetector pd(s);
  std::vector<std::string> nv = pd.readFileList(n);
  REQUIRE(nv.at(0) == "test1/file1.txt");
  REQUIRE(nv.at(1) == "test1/file2.txt");
}
TEST_CASE("readNames document check","[readNames],[toString]") {
  Sensitivity s = medium;
  std::string n = "../data/filenameList1.txt";
  PlagiarismDetector pd(s);
  pd.readFileList(n);
  REQUIRE("I like apples 1\nlike apples and 1\napples and apples 1\nI like apples 1\nlike apples and 1\n" == pd.toString());
}
