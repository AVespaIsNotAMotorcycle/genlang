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
        cout << "-------------------------------------" << endl << endl;
        for (int i = 0; i < sentence_words.size(); i++) {
            sentence_words[i].print();
        }
        cout << endl << "-------------------------------------" << endl << endl;
        return;
    }

    vector<Word> possible_meanings = lex.GetWordByText(sentence_vector[index]);

    //cout << sentence_vector[index] << " meanings: " << endl;

    for (int i = 0; i < possible_meanings.size(); i++) {
        //cout << "\t";
        //possible_meanings[i].print();
        vector<Word> n_s_w = sentence_words;
        n_s_w.push_back(possible_meanings[i]);
        parseSentenceVector(sentence_vector, n_s_w, index + 1, lex);
    }

    cout << endl << endl;

}

bool parseString(string sentence, Lexicon lex) {

    vector<string> sentence_words = getSentenceWordVector(sentence);

    parseSentenceVector(sentence_words, vector<Word>(), 0, lex);

    return true;
}

int main () {

    Lexicon english_lex = Lexicon("./english_lexicon/", "folder");
    cout << "Printing English Lexicon:" << endl << endl;
    english_lex.PrintWordByMeaning();

    string sentence = "I eat food.";
    cout << endl << "------------------------------------------" << endl << endl;
    std::cout << "Input sentence: " << sentence << std::endl;
    cout << endl << "------------------------------------------" << endl << endl;

    cout << "------------------------------------------" << endl;
    parseString(sentence, english_lex);
    cout << "------------------------------------------" << endl << endl;

    return 0;
}