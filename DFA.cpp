#include "Node.h"
#include "DFA.h"

using namespace std;

static int nodes = 0;

DFA::DFA() {
    this->id = nodes;
    nodes++;
    this->isAccepting = false;
    this->type = 20;
}
