# genlang

## English Parsing

The first step was to create a program to parse through English sentences into discrete particles, which can then be translated into the generated language. I did this before making the language generation program, because I wanted to get a good feeling for how English sentences work.

### TO DO:

_ - Implement HashTable

_ - Implement the Lexicon class, containing two HashTables: one HashTable of word objects, with each use of a word having its own entry, and and HashTable of HashKey arrays, allowing the program to acquire all uses for a given string. Ex: word string is entered into the table of keys, which returns the array of keys that correspond to each entry in the table of uses.

_ - Add function, called by getSentenceWordVector, which creates arrays of Word objects by searching a lexicon for each word string in a sentence. These arrays of word objects will then be used to parse the meaning of the word, rather than via the strings, since these will contain more information of what kind of information each word carries. (e.g., there may be too arrays which differ in that one contains the Word sail, labelled as a verb (i.e. "We sail away"), and one containing the Word sail, labelled as a noun (i.e. "We patched the sail").
