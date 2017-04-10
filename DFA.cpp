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
    if(node->isAccepting)type = min(type, node->type);
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

vector<string> DFA::match_dfa(DFA *dfa, vector<string> input, map<int,string> mp) {
    vector<string> matches;
    DFA *start = dfa;
    for(auto in: input) {
        int last_match = -1;
        int type = 20;
        int last_idx = 0;
        int i = 0;
        dfa = start;
        while(i < in.size()) {
            if(in[i] == ' ' || in[i] == '\t') {
                if(last_match == -1) {
                    matches.push_back("no match possible");
                    break;
                } else if(last_match == i-1) {
                    if(type <= 1) {
                        matches.push_back(in.substr(last_idx, i-last_idx));
                    } else{
                        matches.push_back(mp[type]);
                    }
                    while(i < in.size() && (in[i] == ' ' || in[i] == '\t')) {
                            i++;
                    }
                    last_idx = i;
                    last_match = -1;
                    type = 20;
                    dfa = start;
                } else {
                    if(type <= 1) {
                        matches.push_back(in.substr(last_idx, last_match-last_idx+1));
                    } else{
                        matches.push_back(mp[type]);
                    }
                    last_idx = last_match+1;
                    last_match = -1;
                    i = last_idx;
                    type = 20;
                    dfa = start;
                }
            } else {
                if(dfa->adjacent.find(in[i]) == dfa->adjacent.end()) {
                    if(last_match == -1) {
                        matches.push_back("no match possible");
                        break;
                    } else {
                        if(type <= 1) {
                            matches.push_back(in.substr(last_idx, last_match-last_idx+1));
                        } else {
                            matches.push_back(mp[type]);
                        }
                        last_idx = last_match+1;
                        i = last_idx;
                        last_match = -1;
                        dfa = start;
                        type = 20;
                    }
                } else {
                    dfa = dfa->adjacent[in[i]];
                    if(dfa->isAccepting) {
                        type = dfa->type;
                        last_match = i;
                    }
                    i++;
                }
            }
            if(i == in.size()) {
                if(last_match == -1 && last_idx < in.size()) {
                    matches.push_back("no match possible");
                    break;
                } else {
                    if(type <= 1) {
                            matches.push_back(in.substr(last_idx, last_match-last_idx+1));
                    } else {
                        matches.push_back(mp[type]);
                    }
                    cout << dfa->type << " " << dfa->id << endl;
                    last_idx = last_match+1;
                    i = last_idx;
                    last_match = -1;
                    dfa = start;
                    type = 20;
                }
            }
        }
    }
    return matches;
}

void DFA::printDFA(DFA *dfa, int extra) {
    stack<DFA *> s;
    s.push(dfa);
    set<int> vis;
    vector<int> accepting_nodes;
    cout << "Edges in form: from to edge_char\n";
    while (!s.empty()) {
        DFA *aux = s.top();
        s.pop();
        if (vis.find(aux->id) != vis.end())continue;
        vis.insert(aux->id);
        if(aux->isAccepting) {
            accepting_nodes.push_back(aux->id);
        }
        for (auto x : aux->adjacent) {
            cout << aux->id - extra << " " << x.second->id - extra << " " << x.first << endl;
            s.push(x.second);
        }
    }
    cout << "Accepting Nodes:\n";
    for(auto x: accepting_nodes) {
        cout << x << endl;
    }
}

