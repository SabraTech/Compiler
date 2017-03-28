//
// Created by sabra on 26/03/17.
//

#include "Node.h"

int nodes = 0;

Node::Node(int type) {
    this->id = nodes++;
    this->type = type;
    this->isAccepting = false;
}
