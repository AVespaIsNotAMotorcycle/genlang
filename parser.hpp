/*
parser.hpp
Sam Ebersole
Defines the Parser class. 
Iterates through a sentence and determines how words are used.
November 7, 2020
*/

#ifndef PARSER_
#define PARSER_

#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include "lexicon.hpp"
#include "word.hpp"
#include "string_keys.hpp"

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

class Parser {

private:
    Lexicon lex;

    vector<string> getSentenceWordVector(string sentence) {
        vector<string> words;
        string word = "";

        for (int i = 0; i < sentence.length(); i++) {
            if (sentence[i] == ' ') {
                words.push_back(word);
                word = "";
            }
            else if (sentence[i] == '.') {
                words.push_back(word);
                word = "";
                //word += sentence[i];
                //words.push_back(word);
                //word = "";
            }
            else {
                word += sentence[i];
            }
        }

        return words;
    }

    void parseSentenceVector(vector<string> sentence_vector, vector<Word> sentence_words, int index, Lexicon lex) {

        if (index == sentence_vector.size()) {
            cout << endl << "-------------------- parseSentenceVector() --------------------" << endl << endl;

            for (int i = 0; i < sentence_words.size(); i++) {
                sentence_words[i].print();
            }

            return;
        }

        vector<Word> possible_meanings = lex.GetWordByText(sentence_vector[index]);

        for (int i = 0; i < possible_meanings.size(); i++) {
            vector<Word> n_s_w = sentence_words;
            n_s_w.push_back(possible_meanings[i]);
            parseSentenceVector(sentence_vector, n_s_w, index + 1, lex);
        }

    }

public:

    // Constructors
    
    // Base constructor
    Parser() {

    }

    // Constructor that takes a text file containing words
    Parser(Lexicon l) {
        lex = l;
    }

    bool parseString(string sentence) {

        vector<string> sentence_words = getSentenceWordVector(sentence);

        parseSentenceVector(sentence_words, vector<Word>(), 0, lex);

        return true;
        
    }

};

#endif