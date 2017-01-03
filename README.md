# Ngram-Based Plagiarism Detector
600.120.01 | CS120 | Intermediate Programming
Final Project - Plagiarism Detector
12/7/2016

Ryan Demo
rdemo1@jhu.edu

Mengying Pan
mpan4@jhu.edu

Score: 200/200

TA Feedback: "Code has great style and it is obvious that a lot of effort was put into the design of the class layout as well as the detection algorithm. Efficiency was also considered a lot more than in any other project which is obvious from the runtime. No flaws!"

--------------------------------------------------------------------------------------------
### Running
Compile from `src/` with:
```bash
$ make
```

Run from `src/` with:
```bash
$ ../bin/check_plagiarism [file with list of document paths] [sensitivity]
```

##### Arguments:
- `[file with list of document paths]`	string:	name of file containing a list of document paths, line-delimited
- `[sensitivity]`							char:	'h' for high, 'm' for medium, 'l' for low

Valgrind reports no memory issues whatsoever.

--------------------------------------------------------------------------------------------
### Testing
Run from src/ with:
```bash
$ make test
```

We've included tests for NgramCollection, NgramDocument, and PlagiarismDetector. NgramDocument is nested in PlagiarismDetector, so its unit tests are in the PlagiarismDetector unit tests file.

--------------------------------------------------------------------------------------------
### Plan of Attack
We began looking for a good design by browsing the CS literature on the subject. We read the paper [On Automatic Plagiarism Detection Based on n-Grams Comparison](https://pdfs.semanticscholar.org/9520/a7bf0ed28554de7fdabd170446485e5bf621.pdf), a title that matched our plan of attack well. It gave us some pointers on how best to go about this, notably the idea to check individual sentence ngrams against all documents.

Our design modifies LanguageModel from the previous homework assignment to be `PlagiarismDetector`. It reads in a list of documents, and stores each document in an array of `NgramDocument` objects. You can run detect() on it, where it'll execute the matching algorithm, and then we've added a method to return the potential plagiarism matches. The matches are returned as a set of string tuples, and we've overloaded `<<` to print this data structure in the format we'd like. Each `NgramDocument` contains a `NgramCollection` for the language model of the document, a vector of `NgramCollection` objects that hold all of the document's sentences, and the file's name. We keep the name around for easy printing of potential plagiarism pairs. Though more memory intensive, we store all of the sentence ngram models AND the document's ngram makeup as a whole because we need both for our detection algorithm. To keep it as fast as possible, we avoid reopening the file for reading to get sentences, instead creating all of the sentence ngrams when they are read into the document's ngram model during the first pass. `NgramDocument` also has a method to compare it to another document, and determine if there are questionable similarities between the two documents.

The core of our algorithm is the ngram intersection ratio. When we compare a sentence's ngram makeup to a document's ngrams, we take the number of intersections between the two ngrams (counting repeated ngrams as well, those with more than 1 as the map value's value). We divide the number of intersections by the number of ngrams in the sentence. This gives us a ratio of how much of the sentence exists within a document. When this ratio is above a threshold, we mark the pair of documents as plagiarism suspects. We determined 0.4 is a good threshold ratio, as it filters out a lot of false positivies but isn't terribly restrictive.

This intersection ratio calculation occurs between every sentence on every document, with every document. The algorithm intelligently skips any documents already flagged as plagiarism suspects, so that previously matched documents do not repeat the comparison procedure. This is done by indexing matches by their index in the documents array. We always store the lower of the two document indicies first in the tuple, to make searching quick. We use the index (an unsigned int) for efficiency as well, as it eliminates the need for string comparison (i.e. comparing document names to see if they've already been matched).

--------------------------------------------------------------------------------------------
### Design
The program is designed as follows, to keep it somewhat modular:

- `check_plagiarism` is a driver that creates a `PlagiarismDetector` with a specific sensitivity.
- `PlagiarismDetector` has-a vector of `NgramDocument` objects, that it runs detect on. It also has-a vector of string tuples that the matches are stored in.
- `NgramDocument` has-a vector of `NgramCollection` objects that are the ngram representations of each sentence in the document, and has-a `NgramCollection` that is the overall ngram representation of the document. It has-a name (a string) as well.

##### Source Files:
- check_plagiarism.cpp: driver to detect plagiarism within documents listed in a file, with a sensitivity
- Makefile: Makefile for final project
- NgramCollection.cpp: implementation of an ngram collection
- PlagiarismDetector.cpp: implementation for a plagiarism detection model
- unitTestDriver.cpp: drives unit tests
- unitTests-NgramCollection.cpp: unit tests for the `NgramCollection` class
- unitTests-PlagiarismDetector.cpp: unit tests for the `PlagiarismDetector` and `NgramDocument` classes

##### Header Files:
- catch.hpp: catch unit testing header file
- NgramCollection.hpp: header for an ngram collection
- PlagiarismDetector.hpp: header for a plagiarism detector model, contains class definitions for both `PlagiarismDetector` and `NgramDocument` nested class

--------------------------------------------------------------------------------------------
### Sensitivity Flags
In the program, the sensitivity flags correspond to the n in n-grams that we generate for each document. They are defined in the Sensitivity enum in PlagiarismDetector.hpp. Low sensitivity corresponds to 4-grams, medium to trigrams, and high to bigrams.

Using bigrams for high sensitivity is useful, since many documents are likely to share bigrams. This eliminates most of the error of using higher n-grams, since possible results won't be missed. However, high sensitivity may turn up significantly more false positives.

We found that trigrams are good for medium sensitivity because they offer a balance of precision and accuracy. We get fewer matches than we do with bigrams, but they are more likely to be actual instances of plagiarism.

4-grams for low sensitivity begin to only detect large blocks of plagiarism, such as wholly copied sentences.

We do not need to change our intersection ratio threshold as described in the Design section; sensitivity depends only on the ngrams.

--------------------------------------------------------------------------------------------
### Timing
Anticipated times on JHU document sets:
Small document set 		~ 0.6 seconds
Medium document set 	~ 3.6 seconds
Big document set 		~ 17 minutes
Large document set 		~ 201 minutes (projected)
Huge document set 		~ 278 minutes (projected)

We were able to bring our times down by an order of magnitude by using the compiler flag `-O3` and eliminating all instances of pass by value in `for` loops.

--------------------------------------------------------------------------------------------
### Challenges
The biggest two challenges were devising an algorithm then making it more efficient. We knew we wanted to compare the ngram representations of documents, but we had trouble figuring out how to best go about that. One one hand, comparing two whole documents doesn't offer the level of granularity we necessarly need, and leaves room for false positivies. Comparing sentences or parts of sentences for direct matches is hugely inefficient, so we wanted to compare ngrams. We read up on different methods for detecting plagiarism, then began to implement our own version of a sentence-document ngram comparsion.

We had a brute force solution that was O(n^2) to read each file and then its sentences, multiplied by O(s*d), where s is the number of sentences in the corpus and d is the number of documents in the corpus. The latter did not change, but we got read to be O(n) by creating sentence ngrams as we read words into document ngram collections. That way, we only had to read each file once, though this doubled the amount of memory we had to allocate. However, speed is more important here.

We also implemented other little efficiencies throughout our algorithm. If two documents have already been compared, we don't compare them again. We use ints instead of strings where possible for comparison. We use early exit with our if statements and methods where possible.

One other challenge was making our unit tests, since we didn't have too many public methods. The private methods are well broken down into steps, but the higher ones all depend on lower ones. Therefore we had to test the lowest level methods first, then went up through the nestings, so to speak, and tested each level.

--------------------------------------------------------------------------------------------
### Future
If we had more time, we would like to add a few features:

- Printing the match that triggered a plagiarism warning (this wouldn't be terribly difficult given our current implementation; we could just print the sentence that set off the warning - however, we early exit on the first instance of plagiarism, so we'd have to loop through the whole document to print out every possible instance of one document plagiarising from another - not good for efficiency)

- Filtering out quotations: if a string is wrapped in quotation marks, we'd ignore it in the plagiarism detection as it is a cited source (hopefully)

- Improving efficiency by removing common words before running our algorithm, or better yet, experiment with using an algorithm entirely based on common words because of the tendency for plagiarists to use synonyms but keep sentence structure (articles, verbs, etc.) the same
