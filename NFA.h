//
// Created by sabra on 26/03/17.
//

#include <bits/stdc++.h>
#include "Node.h"

#ifndef COMPILER_NFA_H
#define COMPILER_NFA_H

using namespace std;

class NFA {
public:
    NFA(Node *, Node *);

    Node *start = NULL;
    Node *end = NULL;

    NFA *NFA_or(NFA *, NFA *, int);

    NFA *NFA_concatenate(NFA *, NFA *, int);

    NFA *NFA_star(NFA *, int);

    NFA *NFA_plus(NFA *, int);

    void operation(stack<NFA *> &, char, int);

    NFA *evaluate_expression(string, int);

    NFA *NFA_or_final(NFA *, NFA *, int);

private:


};

#endif //COMPILER_NFA_H
