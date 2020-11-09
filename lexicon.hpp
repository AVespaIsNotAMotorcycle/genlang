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
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
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

    bool PopulateFromFile(string fname) {
        std::fstream file;
        file.open(fname);

        if (file.is_open()){   //checking whether the file is open
            string tp;
            while(getline(file, tp)){  //read data from file object and put it into string.
                if (tp.length() > 2) {
                    if (tp.substr(0,2) != "//") {
                        //cout << "Building word off of line: " << tp << "\n";   //print the data of the string

                        Word new_word = Word(tp);
                        bool succ = word_uses.Insert(new_word);

                        // Add to word_keys
                        StringKeys tmp = StringKeys(new_word.getName());
                        StringKeys x = word_keys.GetEntry(tmp);
                        if (x.getWords() == tmp.getWords()) { // If there are no Words associated with a name
                            //cout << "Created key for " << new_word.getName() << endl;
                            x.addWord(new_word);
                            word_keys.Insert(x);
                        }
                        else {
                            //cout << "key already exists for " << new_word.getName() << endl;
                            x.addWord(new_word);
                            word_keys.Insert(x);
                        }

                        
                        if (succ) {
                            //cout << "SUCCESS" << endl;
                        }
                        else {
                            //cout << "FAIL" << endl;
                        }
                    }
                }
            }
            file.close();   //close the file object.
        }
        else {
            //cout << "Could not open " << fname << endl;
        }

        //cout << endl << "------------------------------------------" << endl << endl;
    }

public:

    // Constructors
    
    // Base constructor
    Lexicon() {
        word_uses = HashTableWord();
        word_keys = HashTableStringKeys();
    }

    // Constructor that takes a text file containing words
    Lexicon(string fname, string mode) {
        //cout << "------------------------------------------" << endl;
        //cout << "Building lexicon from " << fname << endl;
        //cout << "------------------------------------------" << endl << endl;

        word_uses = HashTableWord();
        word_keys = HashTableStringKeys();

        if (mode == "single_file") {
            PopulateFromFile(fname);
        }
        else if (mode == "folder") {
                const char * path = fname.c_str();
                struct dirent *entry;
                DIR *dir = opendir(path);
                
                if (dir == NULL) {
                    return;
                }
                while ((entry = readdir(dir)) != NULL) {
                    //cout << entry->d_name << endl;
                    PopulateFromFile(fname + entry->d_name);
                }
                closedir(dir);
        }

    }

    // Prints every Word in word_uses
    void PrintWordByMeaning() {
        word_uses.PrintAll();
    }

    vector<Word> GetWordByText(string word_name) {
        StringKeys tmp = StringKeys(word_name);
        StringKeys x = word_keys.GetEntry(tmp);
        if (x.getWords() == tmp.getWords()) {
            cout << "Could not find " << word_name << endl;
            vector<Word> null_v;
            return null_v;
        }
        vector<Word> homophones = x.getWords();
        return homophones;
    }

    Word GetWordByMeaning(Word w) {
        return word_uses.GetEntry(w);
    }

    vector<Word> FindAllWordsWithTags (vector<string> tags) {
        vector<Word> words_with_tags = word_uses.GetAllWithTag(tags[0]);

        for (int i = 0; i < tags.size(); i++) {
            for (int j = 0; j < words_with_tags.size(); j++) {
                if (!(words_with_tags[j].hasTag(tags[i]))) {
                    vector<Word> nv;
                    for (int k = 0; k < words_with_tags.size(); k++) {
                        if (k != j) {
                            nv.push_back(words_with_tags[k]);
                        }
                    }
                    words_with_tags = nv;
                }
            }
        }

        return words_with_tags;
    } 

};

#endif