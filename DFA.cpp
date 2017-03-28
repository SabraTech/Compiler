#include <bits/stdc++.h>
#include "Node.h"
#include "DFA.h"
#include "NFA.h"

using namespace std;

static int nodes = 0;

DFA::DFA() {
    this->id = nodes;
    nodes++;
    this->isAccepting = false;
    this->type = 20;
}

void DFA::epsilon_closure(Node* node, set<int> &vis, vector<Node*> &inner_nodes, bool &isAccepting, int &type) {
    if(vis.find(node->id) != vis.end()) {
        return;
    }
    vis.insert(node->id);
    inner_nodes.push_back(node);
    isAccepting |= node->isAccepting;
    type = min(type, node->type);
    vector<Node*> adj = node->adjacent['\0'];
    for(int i = 0; i < adj.size(); i++) {
        epsilon_closure(adj[i], vis, inner_nodes, isAccepting, type);
    }
}

DFA* DFA::convert_NFA_to_DFA(NFA* nfa) {
    map<set<int>, DFA*> visited_dfa;
    //set<int>types;
    set<int> vis;
    vector<Node*> inner_nodes;
    int type = 20;
    bool isAccepting = false;
    epsilon_closure(nfa->start, vis, inner_nodes, isAccepting, type);

    DFA* start_dfa = new DFA();
    start_dfa->inner_nodes = inner_nodes;
    start_dfa->type = type;
    start_dfa->isAccepting = isAccepting;
    visited_dfa[vis] = start_dfa;

    stack<DFA*> dfa_stack;
    dfa_stack.push(start_dfa);

    while(!dfa_stack.empty()) {
        DFA* dfa = dfa_stack.top();
        dfa_stack.pop();
        //if(dfa->isAccepting)types.insert(dfa->type);
        unordered_set<char> valid_char;
        for(int i = 0; i < dfa->inner_nodes.size(); i++) {
            for(auto x : inner_nodes[i]->adjacent) {
                if(x.first != '\0') {
                    valid_char.insert(x.first);
                }
            }
        }
        for(auto x : valid_char) {
            vis.clear();
            inner_nodes.clear();
            type = 20;
            isAccepting = false;
            for(int i = 0; i < dfa->inner_nodes.size(); i++) {
                if(dfa->inner_nodes[i]->adjacent.find(x) != dfa->inner_nodes[i]->adjacent.end()) {
                    for(auto y : dfa->inner_nodes[i]->adjacent[x]) {
                        epsilon_closure(y, vis, inner_nodes, isAccepting, type);
                    }
                }
            }
            if(visited_dfa.find(vis) != visited_dfa.end()) {
                dfa->adjacent[x] = visited_dfa[vis];
            } else {
                DFA* new_dfa = new DFA();
                new_dfa->inner_nodes = inner_nodes;
                new_dfa->isAccepting = isAccepting;
                new_dfa->type = type;
                dfa->adjacent[x] = new_dfa;
                visited_dfa[vis] = new_dfa;
                dfa_stack.push(new_dfa);
            }
        }
    }
    //cout << types.size() << endl;
    return start_dfa;
}

