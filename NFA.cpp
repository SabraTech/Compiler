//
// Created by sabra on 26/03/17.
//

#include "NFA.h"
#include "Node.h"

NFA::NFA(Node* start, Node* end){
    this->start = start;
    this->end = end;
}

