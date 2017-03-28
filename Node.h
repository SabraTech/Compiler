//
// Created by sabra on 26/03/17.
//

#include <bits/stdc++.h>

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H

using namespace std;

class Node {
public:
    map<char, vector<Node*>> adjacent;
    int type;
    int id;
    bool isAccepting;
    Node(int);
};


#endif //COMPILER_NODE_H
