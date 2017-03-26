//
// Created by sabra on 26/03/17.
//

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H


class Node {
public:
    unordered_map<char, vector<Node*>> adjacent;
    int type;
    int id;
    bool isAccepting;
    Node(int, int);
};


#endif //COMPILER_NODE_H
