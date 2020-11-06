/*
phrase.hpp
Sam Ebersole
Defines the Phrase class. 
This class is a generalized class to describe a phrase. 
There are many derived classes for specific types of phrases (e.g. Noun Phrase).
November 5, 2020
*/

#ifndef PHRASE_
#define PHRASE_

#include <iostream>
#include <vector>
#include "word.hpp"

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

class Phrase {

private:
    vector<Word> words;

public:

    // Constructors
    Phrase();


};

#endif