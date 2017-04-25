//
// Created by M on 4/24/2017.
//

#ifndef COMPILER_PARSINGTABLE_H
#define COMPILER_PARSINGTABLE_H

#include <bits/stdc++.h>

using namespace std;

class ParsingTable {
public:

    ParsingTable(unordered_map<string, unordered_set<string>> &, unordered_map<string, unordered_set<string>> &,
                 unordered_map<string, vector<vector<string>>> &, vector<string> &, unordered_set<string> &, string &);

    void build_the_table(void);

private:
    // This takes the non-terminal and terminal symbols as inputs then returns the production
    map<pair<string, string>, vector<string>> table;
    unordered_map<string, unordered_set<string>> first_sets;
    unordered_map<string, unordered_set<string>> follow_sets;
    unordered_set<string> terminals;
    string start_symbol;
    unordered_map<string, vector<vector<string>>> productions;
    vector<string> matches;

    void build_step1(void);

    void build_step2(void);

    void build_step3(void);

    void print_parsing_table(void);

    void match();

};

#endif //COMPILER_PARSINGTABLE_H
