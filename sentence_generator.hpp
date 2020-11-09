/*
sentence_generator.hpp
Sam Ebersole
Defines the SentenceGenerator class. 
Uses a CFG to generate sentences.
November 7, 2020
*/

#ifndef SENTENCE_GENERATOR
#define SENTENCE_GENERATOR

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

class SentenceGenerator {

private:
    vector<vector<string>> grammar; // rules for constructing sentences
    Lexicon lex; // available vocabulary

    void BuildSentence(vector<string> sentence) {

        string sentence_string;
        for (int i = 0; i < sentence.size(); i++) {
            sentence_string += sentence[i] + " ";
        }
        cout << sentence_string << endl;

        for (int i = 0; i < sentence.size(); i++) {
            // if sentence[i] is a variable
            if (sentence[i][0] == '$') {
                // find the variable in grammar
                int id = -1;
                for (int j = 0; j < grammar.size(); j++) {
                    if (grammar[j][0] == sentence[i]) {
                        id = j;
                        break;
                    }
                }
                if (id == -1) {
                    cout << sentence[i] << " not found in grammar." << endl;
                    break;
                }


                // Convert variable into all possible options and perform recursive call
                for (int j = 1; j < grammar[id].size(); j++) {

                    // break up gramamr[id][j] into a vector<string>
                    string var_conv = grammar[id][j];
                    vector<string> var_conv_vector;
                    string var_s = "";
                    for (int k = 0; k < var_conv.length(); k++) {
                        if (var_conv[k] == ' ') {
                            var_conv_vector.push_back(var_s);
                            var_s = "";
                        }
                        else {
                            var_s += var_conv[k];
                        }
                    }
                    if (var_s.length() > 0) {
                        var_conv_vector.push_back(var_s);
                        var_s = "";
                    }

                    // insert this vector in place of the previous item
                    vector<string> n_s;
                    for (int k = 0; k < sentence.size(); k++) {
                        if (k == i) {
                            for (int z = 0; z < var_conv_vector.size(); z++) {
                                if (var_conv_vector[z] != "E") {
                                    n_s.push_back(var_conv_vector[z]);   
                                }
                            }
                        }
                        else {
                            n_s.push_back(sentence[k]);
                        }
                    }
                    BuildSentence(n_s);
                    /*
                    vector<string> n_s = sentence;
                    n_s[i] = grammar[id][j];
                    BuildSentence(n_s);
                    */
                }
            }
            else {  // if sentence[i] is not a variable

                // record all tags listed
                vector<string> tags;
                string tag = "";
                bool recording_tag = false;
                for (int j = 0; j < sentence[i].length(); j++) {
                    if (recording_tag) {
                        if (sentence[i][j] == '}') {
                            tags.push_back(tag);
                            recording_tag = false;
                        }
                        else {
                            tag += sentence[i][j];
                        }
                    }
                    else {
                        if (sentence[i][j] == '{') {
                            recording_tag = true;
                        }
                    }
                }

                // if any tags are listed, find all words in the lexicon with these tags and recursively call method
                if (tags.size() > 0) {
                    vector<Word> wwt = lex.FindAllWordsWithTags(tags);
                    if (wwt.size() == 0) {
                        vector<string> n_s = sentence;
                        n_s[i] = "[NONE]";
                        BuildSentence(n_s);
                    }
                    for (int j = 0; j < wwt.size(); j++) {
                        vector<string> n_s = sentence;
                        n_s[i] = wwt[j].getName();
                        BuildSentence(n_s);
                    }
                }

            }
        }
    }

public:

    SentenceGenerator() {

        //grammar = vector<vect

    }

    void SetLex(Lexicon l) {
        lex = l;
    }

    void ConstructGrammar(string fname) {
        std::fstream file;
        file.open(fname);
        
        if (file.is_open()){   //checking whether the file is open
            string tp;
            bool reading_variable = false;

            while(getline(file, tp)){  //read data from file object and put it into string.

                if (tp.length() > 0) { //  skip empty lines

                    if (!reading_variable) { // if not currently reading a vairable's info
                        if (tp[0] == '$') {
                            string var_name = "";
                            for (int i = 0; tp[i] != ' '; i++) { // get name of variable
                                var_name += tp[i];
                            }
                            vector<string> n_l;
                            n_l.push_back(var_name);
                            grammar.push_back(n_l);
                            reading_variable = true; 
                        }
                    }
                    else { // if currently reading a variable's info
                        if (tp[0] == '}') { // break on end of bracketed section
                            reading_variable = false;
                        }
                        else {
                            if (tp.length() > 0) { // skip empty lines
                                string connection = "";
                                for (int i = 0; i < tp.length(); i++) {
                                    if (tp[i] == '/') { // there's no division, so / is only used to mark comments
                                        break;
                                    }
                                    else {
                                        connection += tp[i];
                                    }
                                }
                                // after iterating through line, append to relevant vector in grammar
                                if (connection.length() > 0) {
                                    grammar[grammar.size() - 1].push_back(connection);
                                }
                            }
                        }
                    }

                }

            }
            file.close();   //close the file object.
        }
        else {
            cout << "Could not open " << fname << endl;
        }

        cout << endl << "------------------------------------------" << endl << endl;
    }

    void PrintGrammar() {
        for (int i = 0; i < grammar.size(); i++) {
            cout << grammar[i][0] << " = {" << endl;

            for (int j = 1; j < grammar[i].size(); j++) {
                cout << '\t' << grammar[i][j] << endl;
            }

            cout << "}" << endl << endl;
        }
    }

    void BuildSentenceDriver() {
        vector<string> s;
        s.push_back("$Sentence");
        BuildSentence(s);
    }

};

#endif