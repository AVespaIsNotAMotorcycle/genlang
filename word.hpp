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
    vector<string> tags; // Information about the use of the word
    string def_code; // An id number based on the meaning of the word

    // MODIFIERS
    bool addTag(string n_tag) {
        for (int i = 0; i < tags.size(); i++) {
            if (tags[i] == n_tag) {
                return false;
            }
        }
        tags.push_back(n_tag);
        return true;
    }

public:

    // Constructors
    Word() {
        name = "";
        tags;
        def_code = "";
    }

    Word(string line) {
        name = "";
        string text = "";
        for (int i = 0; i < line.length(); i++) {
            // Set word name
            if (line[i] == ' ') {
                if (name == "") {
                    name = text;
                    text = "";
                }
            }
            // Set tags
            else if (line[i] == '}') {
                addTag(text);
                text = "";
            }
            // Break on comments
            else if (line[i] == '/' && text == "/") {
                break;
            }
            // Add characters to text
            else if (line[i] != '{') {
                text += line[i];
            }
        }

        // set def code
        string l = "";
        l += name + ": ";
        for (int i = 0; i < tags.size(); i++) {
            l += "{" + tags[i] + "}";
            if (i < tags.size() - 1) {
                l += ", ";
            }
        }
        def_code = l;
    }

    // ACCESSORS
    string getDefCode() const {
        return def_code;
    }

    string getName() const {
        return name;
    }

    vector<string> getTags() const {
        return tags;
    }

    bool hasTag(string tag) const {
        for (int i = 0; i < tags.size(); i++) {
            if (tags[i] == tag) {
                return true;
            }
        }
        return false;
    }

    void print() {
        cout << def_code << endl;
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