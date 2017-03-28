//
// Created by Sherif on 28/03/17.
//

#include <bits/stdc++.h>
#include "Node.h"
#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

using namespace std;

class DFA{
public:
    DFA();
    int id;
    vector<Node*> inner_nodes;
    int type;
    map<char, DFA*> adjacent;
    bool isAccepting;
private:


};

#endif //COMPILER_NFA_H
