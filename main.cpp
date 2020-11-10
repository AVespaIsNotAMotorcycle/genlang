#include <iostream>
#include <vector>

#include "word.hpp"
#include "string_keys_hashing.hpp"
#include "lexicon.hpp"
#include "phrase.hpp"
#include "string_keys.hpp"
#include "parser.hpp"
#include "sentence_generator.hpp"

using std::string;
using std::cin; 
using std::cout; 
using std::endl;
using std::vector;

int main () {

    Lexicon english_lex = Lexicon("./english_lexicon/", "folder");
    //cout << "Printing English Lexicon:" << endl << endl;
    //english_lex.PrintWordByMeaning();

    SentenceGenerator english_sentence_generator = SentenceGenerator();
    english_sentence_generator.SetLex(english_lex);
    english_sentence_generator.ConstructGrammar("./english_grammar/english_cfg.txt");

    string sentence = "I eat food.";
    Parser engparse = Parser(english_lex);

    string command;
    cout << "Input command (parse, generate, lexicon, grammar): ";
    cin >> command;

    if (command == "parse") {
        cout << "Sentence: " << sentence << endl;
        engparse.parseString(sentence);
    }
    if (command == "generate") {
        english_sentence_generator.BuildSentenceDriver();
    }
    if (command == "lexicon") {
        english_lex.PrintWordByMeaning();
    }
    if (command == "grammar") {
        english_sentence_generator.PrintGrammar();
    }

    return 0;
}