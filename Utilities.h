//
// Created by sabra on 28/03/17.
//

#include <bits/stdc++.h>

using namespace std;

#ifndef COMPILER_UTILITIES_H
#define COMPILER_UTILITIES_H

class Utilities {
public:
    // concatenation
    static string add_concatenation_to_string(string);

    static void add_concatenation_def(map<string, string> &);

    static void add_concatenation_expression(map<string, string> &);

    static void add_concatenation_keywords(vector<string> &);

    static int strstr(string, string);

    static string remove_spaces(string);

    static vector<string> split(char *, char *);

    static unordered_set<string> add_sets(unordered_set<string>, unordered_set<string>);

    static string add_redundant_spaces(string);

    static vector<string> read_file(string);

    static unordered_set<string> add_sets_except_epsilon(
            unordered_set<string> , unordered_set<string> ) ;

    };

#endif //COMPILER_UTILITIES_H
