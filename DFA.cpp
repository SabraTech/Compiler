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

void DFA::epsilon_closure(Node *node, set<int> &vis, vector<Node *> &inner_nodes, bool &isAccepting, int &type) {
    if (vis.find(node->id) != vis.end()) {
        return;
    }
    vis.insert(node->id);
    inner_nodes.push_back(node);
    isAccepting |= node->isAccepting;
    type = min(type, node->type);
    for (int i = 0; i < node->adjacent['\0'].size(); i++) {
        DFA::epsilon_closure(node->adjacent['\0'][i], vis, inner_nodes, isAccepting, type);
    }
}

DFA *DFA::convert_NFA_to_DFA(NFA *nfa) {
    map<set<int>, DFA *> visited_dfa;
    set<int> vis;
    vector<Node *> inner_nodes;
    int type = 20;
    bool isAccepting = false;
    DFA::epsilon_closure(nfa->start, vis, inner_nodes, isAccepting, type);

    DFA *start_dfa = new DFA();
    start_dfa->inner_nodes = inner_nodes;
    start_dfa->type = type;
    start_dfa->isAccepting = isAccepting;
    visited_dfa[vis] = start_dfa;

    stack<DFA *> dfa_stack;
    dfa_stack.push(start_dfa);

    vis.clear();
    inner_nodes.clear();
    while (!dfa_stack.empty()) {
        DFA *dfa = dfa_stack.top();
        dfa_stack.pop();
        set<char> valid_char;
        for (int i = 0; i < dfa->inner_nodes.size(); i++) {
            for (auto x : dfa->inner_nodes[i]->adjacent) {
                if (x.first != '\0') {
                    valid_char.insert(x.first);
                }
            }
        }
        for (auto x : valid_char) {
            vis.clear();
            inner_nodes.clear();
            type = 20;
            isAccepting = false;
            for (int i = 0; i < dfa->inner_nodes.size(); i++) {
                if (dfa->inner_nodes[i]->adjacent.find(x) != dfa->inner_nodes[i]->adjacent.end()) {
                    for (auto y : dfa->inner_nodes[i]->adjacent[x]) {
                        DFA::epsilon_closure(y, vis, inner_nodes, isAccepting, type);
                    }
                }
            }
            if (visited_dfa.find(vis) != visited_dfa.end()) {
                dfa->adjacent[x] = visited_dfa[vis];
            } else {
                DFA *new_dfa = new DFA();
                new_dfa->inner_nodes = inner_nodes;
                new_dfa->isAccepting = isAccepting;
                new_dfa->type = type;
                dfa->adjacent[x] = new_dfa;
                visited_dfa[vis] = new_dfa;
                dfa_stack.push(new_dfa);
            }
        }
    }
    return start_dfa;
}

int DFA::match_dfa(DFA *dfa, string input) {
    for (int i = 0; i < input.size(); i++) {
        if (dfa->adjacent.find(input[i]) != dfa->adjacent.end()) {
            dfa = dfa->adjacent[input[i]];
        } else {
            cout << "Error in DFA::match_dfa\n";
        }
    }
    if (dfa->isAccepting) {
        return dfa->type;
    }
    return -1;
}

void DFA::printDFA(DFA *dfa) {
    stack<DFA *> s;
    s.push(dfa);
    set<int> vis;
    ///if you want to print it in a file enter file_name in freopen(file_name, "w", stdout);
    int aaaaaa = 0;
    vector<char> hamada;
    while (!s.empty()) {
        DFA *aux = s.top();
        s.pop();
        if (vis.find(aux->id) != vis.end())continue;
        vis.insert(aux->id);
        for (auto x : aux->adjacent) {
            if (aux->id == x.second->id)aaaaaa++, hamada.push_back(x.first);
            cout << aux->id << " " << x.second->id << " " << x.first << endl;
            s.push(x.second);
        }
    }
    cout << aaaaaa << endl;
    for (auto x : hamada) {
        cout << x << endl;
    }
}

