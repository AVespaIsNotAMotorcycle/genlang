/*
word.hpp
Sam Ebersole
Defines the Word class. 
This class is a generalized class to describe a word. 
There are many derived classes for specific types of words (e.g. Noun, Verb, Adposition).
November 5, 2020
*/

#ifndef WORD_
#define WORD_

#include <iostream>
#include <vector>

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

enum lexical_category {};

class Word {

private:
    string name; // The written word
    int def_code; // An id number based on the meaning of the word

public:

    // Constructors
    Word();

    // ACCESSORS
    int getDefCode() const {
        return def_code;
    }

    string getName() const {
        return name;
    }

    // OPERATOR OVERLOADS
    bool operator==(const Word& b) const {
        return this->def_code == b.def_code;
    }

    bool operator!=(const Word& b) const {
        return this->def_code != b.def_code;
    }

};

#endif