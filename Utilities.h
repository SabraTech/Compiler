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

};

#endif //COMPILER_UTILITIES_H
