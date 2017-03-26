//
// Created by sabra on 26/03/17.
//
#include "bits/stdc++.h"
#include "Parser.h"
#include "Node.h"
#include "NFA.h"

using namespace std;

int nodes = 0;
vector<string> lexical_rules;
map<string, string> def_map;
vector<string> keywords, punctuations;
map<string, string> exp_map; // change in all

string add_concatination_to_string(string a) {
    string ans;
    ans =+ a[0];
    for(int i = 1; i < a.size(); i++) {
        if(a[i] == '|' || a[i-1] == '|'){
            ans += a[i];
            continue;
        }
        if(a[i-1] == '('){
            ans += a[i];
            continue;
        }
        if(a[i] == ')' || a[i] == '@'){
            ans += a[i];
            continue;
        }
        if(a[i] == '-'|| a[i] == '+' || a[i] == '*'){
            ans += a[i];
            continue;
        }
        ans += '@';
        ans += a[i];
    }
    return ans;
}
void add_concatination_def(){
    map<string, string>substituted;
    for(auto x : def_map) {
        substituted[x.first] = add_concatination_to_string(x.second);
    }
    def_map = substituted;
}

void add_concatination_expression(){
    map<string, string>substituted;
    for(auto x : exp_map) {
        substituted[x.first] = add_concatination_to_string(x.second);
    }
    exp_map = substituted;
}

NFA* NFA_or(NFA* a, NFA* b, int type) {
    NFA* result = new NFA(new Node(nodes, type), new Node(nodes+1, type));
    nodes += 2;
    result->start->adjacent['\0'].push_back(a->start);
    result->start->adjacent['\0'].push_back(b->start);
    a->end->adjacent['\0'].push_back(result->end);
    b->end->adjacent['\0'].push_back(result->end);
    a->end->isAccepting = false;
    b->end->isAccepting = false;
    result->end->isAccepting = true;
    return result;
}

NFA* NFA_concatenate(NFA* a, NFA* b) {
    a->end->adjacent['\0'].push_back(b->start);
    a->end->isAccepting = false;
    return a;
}

NFA* NFA_star(NFA* a , int type){
    NFA* result = new NFA(new Node(nodes, type), new Node(nodes+1, type));
    nodes += 2;
    a->end->isAccepting = false;
    a->end->adjacent['\0'].push_back(a->start);
    a->end->adjacent['\0'].push_back(result->end);
    result->start->adjacent['\0'].push_back(a->start);
    result->start->adjacent['\0'].push_back(result->end);
    result->end->isAccepting = true;
    return result;
}

NFA* NFA_plus(NFA* a) {
    a->end->adjacent['\0'].push_back(a->start);
    return a;
}

void operation(stack<NFA*>& nfa, char t, int type){
    if(t == '*') {
        NFA *a = nfa.top();
        nfa.pop();
        a = NFA_star(a, type);
        nfa.push(a);
    } else if(t == '+') {
        NFA* a = nfa.top();
        nfa.pop();
        a = NFA_plus(a);
        nfa.push(a);
    } else if(t == '|') {
        NFA* a = nfa.top();
        nfa.pop();
        NFA* b = nfa.top();
        nfa.pop();
        nfa.push(NFA_or(a, b, type));
    } else if(t == '@') {
        NFA* a = nfa.top();
        nfa.pop();
        NFA* b = nfa.top();
        nfa.pop();
        nfa.push(NFA_concatenate(a, b));
    } else {
        cout << "error";
    }
}
NFA* evaluate_expression(string s, int type) {
    stack<NFA*> nfa;
    stack<char> operations;
    for(int i = 0 ; i < s.size() ; i++) {
        if(s[i] == '\\') {
            NFA* aux = new NFA(new Node(nodes, type), new Node(nodes+1, type));
            nodes += 2;
            aux->start->adjacent[s[i+1]].push_back(aux->end);
            aux->end->isAccepting = true;
            nfa.push(aux);
            i++;
        } else if(s[i] == '(') {
            operations.push(s[i]);
        } else if(s[i] == ')') {
            while(operations.top() != '(') {
                char t = operations.top();
                operations.pop();
                operation(nfa, t, type);
            }
            operations.pop();
        } else if(s[i] == '|') {
            while(operations.top() != '(') {
                char t = operations.top();
                operations.pop();
                if(t == '@' || t == '*' || t == '+') {
                    operation(nfa, t, type);
                } else {
                    break;
                }
            }
            operations.push('|');
        } else if(s[i] == '@') {
            while(operations.top() != '(') {
                char t = operations.top();
                operations.pop();
                if(t == '*' || t == '+') {
                    operation(nfa, t, type);
                } else {
                    break;
                }
            }
            operations.push('@');
        } else if(s[i] == '*' || s[i] == '+') {
            operation(nfa, s[i], type);
        } else {
            NFA* aux = new NFA(new Node(nodes, type), new Node(nodes+1, type));
            nodes += 2;
            aux->start->adjacent[s[i]].push_back(aux->end);
            aux->end->isAccepting = true;
            nfa.push(aux);
        }
    }
    while(!operations.empty()) {
        char t = operations.top();
        operations.pop();
        operation(nfa, t, type);
    }
    if(nfa.size() != 1) {
        cout << "5555555555555555555";
    }
    return nfa.top();
}

int strstr(string haystack, string needle) {
    int f[needle.size()];
    for(int i = 1 , k = 0 ; i < needle.size() ; i++){
        while(k > 0 && needle[i] != needle[k])
            k = f[k-1];
        if(needle[i] == needle[k])
            k++;
        f[i] = k;
    }
    for(int i = 0 , k = 0 ; i < haystack.length() ; i++){
        while(k>0 && haystack[i] != needle[k])
            k = f[k-1];
        if(haystack[i] == needle[k]){
            k++;
        }
        if(k == needle.length()){
            return i - k + 1;
        }
    }
    return -1;
}

string remove_spaces(string s){
    string s_without_space;
    size_t pos = 0, found;
    while((found = s.find_first_of(' ', pos)) != string::npos){
        s_without_space += s.substr(pos, found - pos);
        pos = found + 1;
    }
    s_without_space += s.substr(pos);
    return s_without_space;
}


void replaceToken(string &in, string &out){

}

int main() {
    ifstream iFile("Rules.txt");
    assert(iFile.is_open());

    for(string s; getline(iFile, s);){
        lexical_rules.push_back(s);
    }

    iFile.close();

    // parse
    def_map = Parser::parse_out_definitions(lexical_rules);
    exp_map = Parser::parse_out_expressions(lexical_rules);
    keywords = Parser::parse_out_keywords(lexical_rules);
    punctuations = Parser::parse_out_punctuations(lexical_rules);

    add_concatination_def();
    // substitute

    vector<pair<int, string>> list;
    for(auto entry : def_map) {
        string key = entry.first;
        list.push_back(make_pair(key.size(), key));
    }
    sort(list.begin(), list.end(), greater<pair<int, string>>());

    for(auto x : list){
        string key = x.second;
        for(auto y : def_map){
            string value = y.second;
            int found = strstr(value, key);
            while(found != -1){
                string str;
                str += value.substr(0,found);
                str += def_map[key];
                str += value.substr(found + key.size(),value.size());
                def_map[y.first] = str;
                found = strstr(str,key);
            }
        }
    }

    for(auto x : list) {
        string key = x.second;
        for (auto y : exp_map) {
            string value = y.second;
            int found = strstr(value, key);
            while (found != -1) {
                string str;
                str += value.substr(0, found);
                str += def_map[key];
                str += value.substr(found + key.size(), value.size());
                exp_map[y.first] = str;
                found = strstr(str, key);
            }
        }
    }
    //////////////////////
    for(auto entry : exp_map){
        exp_map[entry.first] = remove_spaces(entry.second);
    }
    // remove any space after substitute
    add_concatination_expression();


    // create NFA for expressions, keywords and punctuations
    // or all NFAs to one big NFA
    // transfer NFA to DFA
    // minimize DFA
    // enter code

    return 0;
}
