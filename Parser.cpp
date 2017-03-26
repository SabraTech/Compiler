//
// Created by sabra on 26/03/17.
//

#include "bits/stdc++.h"
#include "Parser.h"

using namespace std;

static inline string Parser::left_trim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

static inline string Parser::right_trim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

static inline string Parser::trim(string &s) {
    return left_trim(right_trim(s));
}

string Parser::extract (string RHS){
    string RHS_without_space;
    size_t pos = 0, found;
    while((found = RHS.find_first_of(' ', pos)) != npos){
        RHS_without_space += RHS.substr(pos, found - pos);
        pos = found + 1;
    }
    RHS_without_space += RHS.substr(pos);
    string RHS_with_or;
    for (int i = 0 ; i < RHS_without_space.size() ; i++){
        if(RHS_without_space[i] == '-'){
            for(char x = RHS_with_or.back()+1; x <= RHS_without_space[i+1] ; x++){
                RHS_with_or += '|';
                RHS_with_or += x;
            }
            i++;
        } else {
            RHS_with_or += RHS_without_space[i];
        }
    }
    return "(" + RHS_with_or + ")";
}

map<string, string> Parser::parse_out_definitions(vector<string> &rules) {
    map<string, string> data;
    for(int i = 0; i < rules.size(); i++){
        string line = rules[i];
        size_t index_of_equal = line.find_first_of("=");
        size_t index_of_colon = line.find_first_of(":");
        if(index_of_equal != npos && index_of_colon == npos){
            string LHS = line.substr(0,found - 1);
            LHS = trim(LHS);
            string RHS = line.substr(found + 1);
            RHS = trim(RHS);
            RHS = extract(RHS); // add in .h file
            data.insert({LHS, RHS});
            rules.erase(rules.begin() + i);
        }
    }
    return data;
}

map<string, string> Parser::parse_out_expressions(vector<string> &rules) {
    map<string, string> data;
    for(int i = 0; i < rules.size(); i++){
        string line = rules[i];
        size_t index_of_colon = line.find(":");
        if(index_of_colon != npos){
            string LHS = line.substr(0,found - 1);
            LHS = trim(LHS);
            string RHS = line.substr(found + 1);
            RHS = trim(RHS);
            data.insert({LHS, RHS});
            rules.erase(rules.begin() + i);
        }
    }
    return data;
}

vector<string> Parser::parse_out_keywords(vector<string> &rules) {
    vector<string> keywords;
    for(int i = 0; i < rules.size(); i++){
        string line = rules[i];
        size_t index_of_open_braces = line.find("{");
        if(index_of_open_braces != npos){
            size_t index_of_close_braces = line.find("}");
            string words = line.substr(index_of_open_braces + 1, index_of_close_braces - 1);
            size_t pos = 0, found;
            while((found = words.find_first_of(' ', pos)) != npos){
                keywords.push_back(words.substr(pos, found - pos));
                pos = found + 1;
            }
            keywords.push_back(words.substr(pos));
            rules.erase(rules.begin() + i);
        }
    }
    return keywords;
}

vector<string> Parser::parse_out_punctuations(vector<string> &rules) {
    vector<string> punctuations;
    for(int i = 0; i < rules.size(); i++){
        string line = rules[i];
        size_t index_of_open_sqbrack = line.find("[");
        if(index_of_open_braces != npos){
            size_t index_of_close_sqbrack = line.find("]");
            string words = line.substr(index_of_open_sqbrack + 1, index_of_close_sqbrack - 1);
            size_t pos = 0, found;
            while((found = words.find_first_of(' ', pos)) != npos){
                punctuations.push_back(words.substr(pos, found - pos));
                pos = found + 1;
            }
            punctuations.push_back(words.substr(pos));
            rules.erase(rules.begin() + i);
        }
    }
    return  punctuations;
}
