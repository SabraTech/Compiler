//
// Created by sabra on 26/03/17.
//

#include "Node.h"

Node::Node(int id, int type) {
    this->id = id;
    this->type = type;
    this->isAccepting = false;
}