/*
string_keys.hpp
Sam Ebersole
Defines the StringKeys class. 
Holds a string (how a word is written) and words (meanings that correspond)
November 5, 2020
*/

#ifndef STRING_KEYS_
#define STRING_KEYS_

#include <iostream>
#include <vector>
#include <typeinfo>
#include "word.hpp"
#include "string_keys_hashing.hpp"

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

class StringKeys {

private:
    string name; // The written word
    vector<Word> homophones; // All words corresponding to a given string


public:

    // CONSTRUCTORS
    StringKeys();

    StringKeys(string n) {
        name = n;
    }

    // ACCESSORS
    string getName() const {
        return name;
    }

    vector<Word> getWords() const {
        return homophones;
    }

    // OPERATOR OVERLOADS
    bool operator==(const StringKeys& b) const {
        return this->name == b.name;
    }

    bool operator!=(const StringKeys& b) const {
        return this->name != b.name;
    }

};

#endif