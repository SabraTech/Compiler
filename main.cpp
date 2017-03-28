//
// Created by sabra on 26/03/17.
//

#include "bits/stdc++.h"
#include "Parser.h"
#include "Node.h"
#include "NFA.h"
#include "DFA.h"
#include "Utilities.h"

using namespace std;

vector<string> lexical_rules;
map<string, string> def_map;
vector<string> keywords, punctuations;
map<string, string> exp_map;

int main() {
    /*const char * filename = "home/sabra/ClionProjects/Compiler/Rules.txt";
    ifstream myFile;
    myFile.open(filename, ios::in);
    string line;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            lexical_rules.push_back(line);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    myFile.close();*/

    lexical_rules = {"letter = a-z | A-Z",
                     "digit = 0-9",
                     "id: letter (letter | digit)*",
                     "digits = digit+",
                     "{boolean int float}",
                     "num: digit+ | digit+ . digits ( \\L | E digits)",
                     "relop: \\=\\= | !\\= | > | >\\= | < | <\\=",
                     "assign: =",
                     "{ if else while }",
                     "[; , \\( \\) { }]",
                     "addop: \\+ | -",
                     "mulop: \\* | /"

    };

    // parse
    def_map = Parser::parse_out_definitions(lexical_rules);
    exp_map = Parser::parse_out_expressions(lexical_rules);
    keywords = Parser::parse_out_keywords(lexical_rules);
    punctuations = Parser::parse_out_punctuations(lexical_rules);


    // substitute
    vector<pair<int, string>> list;
    for(auto const& entry : def_map) {
        string key = entry.first;
        list.push_back(make_pair(key.size(), key));
    }
    sort(list.begin(), list.end(), greater<pair<int, string>>());

    for(auto const& x : list){
        string key = x.second;
        for(auto const& y : def_map){
            string value = y.second;
            int found = Utilities::strstr(value, key);
            while(found != -1){
                string str;
                str += value.substr(0,found);
                str += def_map[key];
                str += value.substr(found + key.size(),value.size());
                def_map[y.first] = str;
                found = Utilities::strstr(str,key);
                value = y.second;
            }
        }
    }

    for(auto const& x : list) {
        string key = x.second;
        for (auto const& y : exp_map) {
            string value = y.second;
            int found = Utilities::strstr(value, key);
            while (found != -1) {
                string str;
                str += value.substr(0, found);
                str += def_map[key];
                str += value.substr(found + key.size(), value.size());
                exp_map[y.first] = str;
                found = Utilities::strstr(str, key);
                value = y.second;
            }
        }
    }

    //////////////////////

    // remove any space after substitute
    for(auto const& entry : exp_map){
        exp_map[entry.first] = Utilities::remove_spaces(entry.second);
    }

    // concatenation Added
    Utilities::add_concatenation_def(def_map);
    Utilities::add_concatenation_expression(exp_map);
    Utilities::add_concatenation_keywords(keywords);
    /*for(int i = 0; i < keywords.size(); i++) {
        keywords[i] = add_concatination_to_string(remove_spaces(keywords[i]));
        if(keywords[i][0] == '\0') {
            continue;
        }
        cout << keywords[i] << endl;
    }*/

    // print to test here
    cout << "def map of the grammer" << endl;
    for (auto const& x : def_map)
    {
        cout << x.first << " : " << x.second << endl ;
    }
    cout << "---------------------------" << endl;
    cout << "exp map of the grammer" << endl;
    for (auto const& x : exp_map)
    {
        cout << x.first << " : " << x.second << endl ;
    }
    cout << "---------------------------" << endl;
    cout << "Now begin creating the NFA graph and convert it to DFA" << endl;
    //Testing evaluating expression
    NFA* final_nfa = NULL;
    NFA* dummy = new NFA(new Node(0,0), new Node(0,0));
    int expression_num = 2;

    // maps between expression_num and the corresponding label
    unordered_map<int, string> mp;
    for(auto x : exp_map) {
        mp[expression_num] = x.first;
        NFA* aux = dummy->evaluate_expression(x.second, expression_num++);
        if(final_nfa == NULL) {
            final_nfa = aux;
        } else {
            final_nfa = dummy->NFA_or(final_nfa, aux, 20);
        }
    }

    for(auto x : punctuations) {
        NFA* aux = dummy->evaluate_expression(x, 1);
        final_nfa = dummy->NFA_or(final_nfa, aux, 20);
    }
    for(auto x : keywords) {
        if(x[0] == '\0') {
            continue;
        }
        NFA* aux = dummy->evaluate_expression(x, 0);
        final_nfa = dummy->NFA_or(final_nfa, aux, 20);
    }

    // transfer NFA to DFA

    DFA* dfa_not_minimized = DFA::convert_NFA_to_DFA(final_nfa);
    cout << "Printing the final DFA" << endl;
    cout << "ID Type Accepting size_of_adjacent" << endl;
    for(auto x : dfa_not_minimized->inner_nodes){

        cout << x->id << " " << x->type << " " << x->isAccepting << " " << x->adjacent.size() << endl;
    }
    // minimize DFA
    // enter code

    return 0;
}
