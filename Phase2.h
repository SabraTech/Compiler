#include <bits/stdc++.h>

#ifndef COMPILER_PHASE2_H
#define COMPILER_PHASE2_H

using namespace std;

class Phase2 {
public:

    Phase2(vector<string> &);
    void startPhase2();
    void print_first_sets(void) ;

private:
    void initialize_cfg_list();
    void build_the_map();
    void get_first_sets();
    unordered_set<string> get_first_sets(string);

    vector<string> cfg_list;
    unordered_set<string> terminals;
    unordered_map<string, vector<vector<string>>> productions;
    unordered_map<string, unordered_set<string>> first;
    unordered_set<string> visited;


};

#endif //COMPILER_PHASE2_H
