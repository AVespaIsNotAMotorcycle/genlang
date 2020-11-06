/*
lexicon.hpp
Sam Ebersole
Defines the Lexicon class. 
Stores all words of a language.
November 5, 2020
*/

#ifndef LEXICON_
#define LEXICON_

#include <iostream>
#include <vector>
#include "word_hashing.hpp"
#include "word.hpp"
#include "string_keys_hashing.hpp"
#include "string_keys.hpp"

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

class Lexicon {

private:

    HashTableStringKeys word_keys; // Allows the program to obtain all words based on a string input
    HashTableWord word_uses; // Contains words separated by use

public:

    // Constructors
    
    // Base constructor
    Lexicon();

    // Constructor that takes a text file containing words
    Lexicon(string fname) {
        
    }

    vector<Word> GetWordByText(string word_name) {
        StringKeys tmp = StringKeys(word_name);
        StringKeys x = word_keys.GetEntry(tmp);
        if (x == tmp) {
            cout << "Could not find " << word_name << endl;
            vector<Word> null_v;
            return null_v;
        }
        vector<Word> homophones = x.getWords();
    }

    Word GetWordByMeaning(Word w) {
        return word_uses.GetEntry(w);
    }

};

#endif