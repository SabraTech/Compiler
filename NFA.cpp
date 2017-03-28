//
// Created by sabra on 26/03/17.
//

#include "NFA.h"
#include "Node.h"
#include <bits/stdc++.h>

using namespace std;

NFA::NFA(Node *start, Node *end) {
    this->start = start;
    this->end = end;
}

NFA *NFA::NFA_or(NFA *a, NFA *b, int type) {
    NFA *result = new NFA(new Node(type), new Node(type));
    result->start->adjacent['\0'].push_back(a->start);
    result->start->adjacent['\0'].push_back(b->start);
    a->end->adjacent['\0'].push_back(result->end);
    b->end->adjacent['\0'].push_back(result->end);
    a->end->isAccepting = false;
    b->end->isAccepting = false;
    result->end->isAccepting = true;
    return result;
}

NFA *NFA::NFA_concatenate(NFA *a, NFA *b, int type) {
    NFA *result = new NFA(new Node(type), new Node(type));
    result->start->adjacent['\0'].push_back(a->start);
    b->end->adjacent['\0'].push_back(result->end);
    a->end->adjacent['\0'].push_back(b->start);
    a->end->isAccepting = false;
    b->end->isAccepting = false;
    result->end->isAccepting = true;
    return result;
}

NFA *NFA::NFA_star(NFA *a, int type) {
    NFA *result = new NFA(new Node(type), new Node(type));
    a->end->isAccepting = false;
    a->end->adjacent['\0'].push_back(a->start);
    a->end->adjacent['\0'].push_back(result->end);
    result->start->adjacent['\0'].push_back(a->start);
    result->start->adjacent['\0'].push_back(result->end);
    result->end->isAccepting = true;
    return result;
}

NFA *NFA::NFA_plus(NFA *a, int type) {
    NFA *result = new NFA(new Node(type), new Node(type));
    a->end->isAccepting = false;
    a->end->adjacent['\0'].push_back(a->start);
    a->end->adjacent['\0'].push_back(result->end);
    result->start->adjacent['\0'].push_back(a->start);
    result->end->isAccepting = true;
    return result;
}

void NFA::operation(stack<NFA *> &nfa, char t, int type) {
    if (t == '*') {
        NFA *a = nfa.top();
        nfa.pop();
        nfa.push(NFA_star(a, type));
    } else if (t == '+') {
        NFA *a = nfa.top();
        nfa.pop();
        nfa.push(NFA_plus(a, type));
    } else if (t == '|') {
        NFA *a = nfa.top();
        nfa.pop();
        NFA *b = nfa.top();
        nfa.pop();
        nfa.push(NFA_or(b, a, type));
    } else if (t == '@') {
        NFA *a = nfa.top();
        nfa.pop();
        NFA *b = nfa.top();
        nfa.pop();
        nfa.push(NFA_concatenate(b, a, type));
    } else {
        cout << "error no valid NFA operation";
    }
}

NFA *NFA::evaluate_expression(string s, int type) {
    stack<NFA *> nfa;
    stack<char> operations;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '\\') {
            NFA *aux = new NFA(new Node(type), new Node(type));
            if(s[i+1] != 'L') {
                aux->start->adjacent[s[i + 1]].push_back(aux->end);
            } else {
                aux->start->adjacent['\0'].push_back(aux->end);
            }
            aux->end->isAccepting = true;
            nfa.push(aux);
            i++;
        } else if (s[i] == '(') {
            operations.push(s[i]);
        } else if (s[i] == ')') {
            while (operations.top() != '(') {
                char t = operations.top();
                operations.pop();
                operation(nfa, t, type);
            }
            operations.pop();
        } else if (s[i] == '|') {
            while (!operations.empty() && operations.top() != '(') {
                char t = operations.top();
                if (t == '@' || t == '*' || t == '+') {
                    operation(nfa, t, type);
                    operations.pop();
                } else {
                    break;
                }
            }
            operations.push('|');
        } else if (s[i] == '@') {
            while (!operations.empty() && operations.top() != '(') {
                char t = operations.top();
                if (t == '*' || t == '+') {
                    operation(nfa, t, type);
                    operations.pop();
                } else {
                    break;
                }
            }
            operations.push('@');
        } else if (s[i] == '*' || s[i] == '+') {
            operation(nfa, s[i], type);
        } else {
            NFA *aux = new NFA(new Node(type), new Node(type));
            aux->start->adjacent[s[i]].push_back(aux->end);
            aux->end->isAccepting = true;
            nfa.push(aux);
        }
    }
    while (!operations.empty()) {
        char t = operations.top();
        operations.pop();
        operation(nfa, t, type);
    }
    if (nfa.size() != 1) {
        cout << nfa.size() << " error not empty stack\n";
    }
    return nfa.top();
}

