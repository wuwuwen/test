#pragma
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include <iostream>
#include <map>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;
int string_to_ascii(string& ptr);
class Word
{
    friend class Dictionary;
    friend class New_words_notebook;
    friend class My_program;
private:
    int key;
    string English;
    string Chinese;
public:
    Word() {}
    Word(string str);
    void show();
};
class Dictionary
{
    friend class New_words_notebook;
private:
    const char* dir;
    multimap<int, Word> first_char_map_52;
    multimap<int, pair<multimap<int, Word>::iterator, multimap<int, Word>::iterator>> first_char_map_1;
    multimap<int, Word> word_map;
public:
    Dictionary() {}
    Dictionary(const char* dir);
    void find_word(string find_word);
    Word& find_word_class(string find_word);
    void show_first_char_map_all_or_one(int first_char = 0);
};
class New_words_notebook
{
private:
    const char* dir;
    multimap<int, Word> new_word_map;
    Dictionary dictionary;
public:
    New_words_notebook() {}
    New_words_notebook(Dictionary& dictionary, const char* dir);
    void add_new_word(string new_word_English);
    void del_new_word(string del_word_English);  
    void show();
    ~New_words_notebook();
};

class My_program
{
private:
    Dictionary dictionary;
    New_words_notebook new_words_notebook;
public:
    My_program(const char* dictionary_dir, const char* new_words_notebook_dir);
    void run();
};

#endif