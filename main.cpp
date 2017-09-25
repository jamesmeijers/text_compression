//
//  main.cpp
//  text_compressor
//
//  Created by James Meijers on 2017-07-13.
//  Copyright © 2017 James Meijers. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void compress(string file);

void decompress(string file);

void make_translation(string names);


string* translations_one_letter;
//string* translations_two_letter;

int main(int argc, const char * argv[]) {
    
    //These variables must be set by the user
    //word list is a list of most common English words, in order, one word per line
    string word_list, original_location, compress_location;
    
    make_translation(word_list);
    compress(original_location);
    decompress(compress_location);
    
    
    
    return 0;
}


void compress(string file){
    ifstream file_in;
    file_in.open(file);
    ofstream file_out;
    file_out.open(file + ".comp");
    
    string word, word_original;
    char a;
    a = file_in.get();
    while (a != '\n' && a != '\t' && a != ' ' && a != ',' && a != '.' && a != '!' && a != '?' && a != '"'){
        word = word + a;
        a = file_in.get();
    }
    
    
    bool capital;
    while (!file_in.eof()){
        word_original = word;
        if (word[0] >= 'A' && word[0] <= 'Z') {
            word[0] = word[0] - 'A';
            capital = true;
        }
        else capital = false;
        
        bool replaced = false;
        for (unsigned i = 0; i < 255; i++){
            if (translations_one_letter[i] == word){
                char tmp = i;
                file_out << '\\' << tmp;
                replaced = true;
                break;
            }
        }
        if (!replaced){
            file_out << word_original;
        }
        
        while (!(a != '\n' && a != '\t' && a != ' ' && a != ',' && a != '.' && a != '!' && a != '?' && a != '"') && !file_in.eof()){
            file_out << a;
            a = file_in.get();
        }
        word = a;
        a = file_in.get();
        while (a != '\n' && a != '\t' && a != ' ' && a != ',' && a != '.' && a != '!' && a != '?' && a != '"' && !file_in.eof()){
            word = word + a;
            a = file_in.get();
        }
        
        
    }
}

void decompress(string file){
    ifstream compressed;
    ofstream decompressed;
    
    compressed.open(file);
    decompressed.open(file + ".decomp.txt");
    
    
    unsigned char a;
    a = compressed.get();
    while (!compressed.eof()){
        if (a != '\\') decompressed.put(a);
        else {
            a = compressed.get();
            int b = a;
            decompressed << translations_one_letter[b];
        }
        a = compressed.get();
    }
}

void make_translation(string file){ //file of words in order of commonality
    ifstream words;
    words.open(file);
    translations_one_letter = new string[255];
    
    string word;
    getline(words, word);
    int i = 0;
    while (!words.eof() && i < 255){
        if (i == 92) { //protecting the "\\" character
            translations_one_letter[i] = "\\";
            i++;
            continue;
        }
        if (word.length() > 2) {
            translations_one_letter[i] = word;
            i++;
        }
        getline(words, word);

    }
}