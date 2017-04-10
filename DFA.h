//
// Created by Sherif on 28/03/17.
//

#include <bits/stdc++.h>
#include "Node.h"
#include "NFA.h"

#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

using namespace std;

class DFA {
public:
    DFA();

    int id;
    vector<Node *> inner_nodes;
    int type;
    map<char, DFA *> adjacent;
    bool isAccepting;

    static void epsilon_closure(Node *, set<int> &, vector<Node *> &, bool &, int &);

    static DFA *convert_NFA_to_DFA(NFA *);

    static vector<string> match_dfa(DFA *, vector<string>, map<int,string>);

    static void printDFA(DFA *, int);

private:


};

#endif //COMPILER_NFA_H
