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

    void BuildSentence(vector<string> sentence, vector<string> subject_tags, vector<string> verb_tags) {

        bool complete = true;

        string sentence_string;
        for (int i = 0; i < sentence.size(); i++) {
            sentence_string += sentence[i] + " ";
        }
        //cout << sentence_string << endl;

        for (int i = 0; i < sentence.size(); i++) {
            // if sentence[i] is a variable
            if (sentence[i][0] == '$') {

                complete = false;

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
                    BuildSentence(n_s, subject_tags, verb_tags);
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
                            tag = "";
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

                    complete = false;

                    bool is_subject = false;
                    bool is_verb = false;

                    // If the subject or verb has already been defined, ensure the other will agree
                    for (int j = 0; j < tags.size(); j++) {
                        if (tags[j] == "FLAG_VERB") {

                            is_verb = true;

                            if (subject_tags.size() != 0) {
                                for (int z = 0; z < subject_tags.size(); z++) {
                                    tags.push_back(subject_tags[z]);
                                }
                            }

                            vector<string> n_t;
                            for (int z = 0; z < tags.size(); z++) {
                                if (z != j) {
                                    n_t.push_back(tags[z]);
                                }
                            }
                            tags = n_t;
                        }
                        else if (tags[j] == "FLAG_SUBJECT") {

                            is_subject = true;

                            if (verb_tags.size() != 0) {

                                //cout << "--------------------------" << endl << "VERB TAGS ARE: ";
                                for (int z = 0; z < verb_tags.size(); z++) {
                                    tags.push_back(verb_tags[z]);
                                    //cout << verb_tags[z] << " ";
                                }
                                //cout << endl;
                            }
                            vector<string> n_t;
                            for (int z = 0; z < tags.size(); z++) {
                                if (z != j) {
                                    n_t.push_back(tags[z]);
                                }
                            }
                            tags = n_t;
                            //cout << "SUBJECT TAGS ARE: ";
                            //for (int z = 0; z < tags.size(); z++) {
                            //    cout << tags[z] << " ";
                            //}
                            //cout << endl << "--------------------------" << endl;
                        }
                    }

                    vector<Word> wwt = lex.FindAllWordsWithTags(tags);
                    if (wwt.size() == 0) {
                        //vector<string> n_s = sentence;
                        return;
                        //n_s[i] = "[NONE]";
                        //BuildSentence(n_s, subject_tags, verb_tags);
                    }
                    for (int j = 0; j < wwt.size(); j++) {

                        // if the subject is first defined
                        if (is_subject && verb_tags.size() == 0) {
                            vector<string> w_tags = wwt[j].getTags();
                            for (int k = 0; k < w_tags.size(); k++) {
                                // check for number
                                if (w_tags[k] == "singular" || w_tags[k] == "plural") {
                                    subject_tags.push_back(w_tags[k]);
                                    //cout << "Added " << w_tags[k] << " to subject_tags" << endl;
                                }
                                // check for person
                                if (w_tags[k] == "first_person" || w_tags[k] == "second_person" || w_tags[k] == "third_person") {
                                    subject_tags.push_back(w_tags[k]);
                                    //cout << "Added " << w_tags[k] << " to subject_tags" << endl;
                                }
                                else if (w_tags[k] == "noun") {
                                    subject_tags.push_back("third_person");
                                }
                            }
                        }
                        // if the verb is first defined
                        if (is_verb && subject_tags.size() == 0) {
                            vector<string> w_tags = wwt[j].getTags();
                            for (int k = 0; k < w_tags.size(); k++) {
                                if (w_tags[k] == "first_person" || w_tags[k] == "second_person" || w_tags[k] == "third_person" || w_tags[k] == "singular" || w_tags[k] == "plural") {
                                    verb_tags.push_back(w_tags[k]);
                                    //cout << "Added " << w_tags[k] << " to verb_tags" << endl;
                                }
                            }
                        }

                        vector<string> n_s = sentence;

                        n_s[i] = wwt[j].getName();
                        BuildSentence(n_s, subject_tags, verb_tags);
                    }
                }

            }
        }
        if (complete) {
            
            //cout << " --------------------------------------  " << endl;
            //cout << sentence_string << endl;
            /*
            cout << "SUBJECT_TAGS: ";
            for (int i = 0; i < subject_tags.size(); i++) {
                cout << subject_tags[i] << ", ";
            }
            cout << endl;
            cout << "VERB_TAGS: ";
            for (int i = 0; i < verb_tags.size(); i++) {
                cout << verb_tags[i] << ", ";
            }
            cout << endl;
            cout << " --------------------------------------  " << endl;
            */
           cout << sentence_string << endl;
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
        vector<string> t;
        s.push_back("$Sentence");
        BuildSentence(s, t, t);
    }

};

#endif