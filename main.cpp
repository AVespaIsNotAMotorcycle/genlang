#include <iostream>
#include <vector>

#include "word.hpp"
#include "string_keys_hashing.hpp"
#include "lexicon.hpp"
#include "phrase.hpp"
#include "string_keys.hpp"

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

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
            word += sentence[i];
            words.push_back(word);
            word = "";
        }
        else {
            word += sentence[i];
        }
    }

    return words;
}

bool parseString(string sentence) {

    vector<string> sentence_words = getSentenceWordVector(sentence);

    for (int i = 0; i < sentence_words.size(); i++) {
        cout << sentence_words[i] << endl;
    }

    return true;
}

int main () {

    string sentence = "I eat food.";
    std::cout << "Input sentence: " << sentence << std::endl;
    parseString(sentence);

    return 0;
}