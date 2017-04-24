//
// Created by sabra on 17/04/17.
//

#include "Phase1.h"
#include "Parser.h"
#include "Node.h"
#include "NFA.h"
#include "DFA.h"
#include "Utilities.h"
#include "MinimizationTable.h"

using namespace std;

vector<string> lexical_rules;
map<string, string> def_map;
vector<string> keywords, punctuations;
map<string, string> exp_map;
DFA *useless;
DFA *minimized;
map<int, string> mp;

Phase1::Phase1(vector<string> &list) {
    lexical_rules = list;
}

void Phase1::startPhase1() {

    def_map = Parser::parse_out_definitions(lexical_rules);
    exp_map = Parser::parse_out_expressions(lexical_rules);
    keywords = Parser::parse_out_keywords(lexical_rules);
    punctuations = Parser::parse_out_punctuations(lexical_rules);


    // substitute
    vector<pair<int, string>> list;
    for (auto const &entry : def_map) {
        string key = entry.first;
        list.push_back(make_pair(key.size(), key));
    }
    sort(list.begin(), list.end(), greater<pair<int, string>>());

    for (auto const &x : list) {
        string key = x.second;
        for (auto const &y : def_map) {
            string value = y.second;
            int found = Utilities::strstr(value, key);
            while (found != -1) {
                string str;
                str += value.substr(0, found);
                str += def_map[key];
                str += value.substr(found + key.size(), value.size());
                def_map[y.first] = str;
                found = Utilities::strstr(str, key);
                value = y.second;
            }
        }
    }

    for (auto const &x : list) {
        string key = x.second;
        for (auto const &y : exp_map) {
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
    for (auto const &entry : exp_map) {
        exp_map[entry.first] = Utilities::remove_spaces(entry.second);
    }

    Utilities::add_concatenation_def(def_map);
    Utilities::add_concatenation_expression(exp_map);
    Utilities::add_concatenation_keywords(keywords);

    //Testing evaluating expression
    NFA *final_nfa = NULL;
    NFA *dummy = new NFA(new Node(0), new Node(0));
    int expression_num = 2;

    //Add extra parameter for merging to keep original NFAs has an accepting state
    for (auto x : exp_map) {
        mp[expression_num] = x.first;
        NFA *aux = dummy->evaluate_expression(x.second, expression_num++);
        if (final_nfa == NULL) {
            final_nfa = aux;
        } else {
            final_nfa = dummy->NFA_or_final(final_nfa, aux, 20);
        }
    }

    for (auto x : punctuations) {
        NFA *aux = dummy->evaluate_expression(x, 1);
        final_nfa = dummy->NFA_or_final(final_nfa, aux, 20);
    }
    for (auto x : keywords) {
        if (x[0] == '\0') {
            continue;
        }
        NFA *aux = dummy->evaluate_expression(x, 0);
        final_nfa = dummy->NFA_or_final(final_nfa, aux, 20);
    }

    // transfer NFA to DFA

    DFA *dfa_not_minimized = DFA::convert_NFA_to_DFA(final_nfa);

    useless = new DFA();

    minimized = MinimizationTable::minimize_DFA(dfa_not_minimized);

    mp[0] = "Keyword";
    mp[1] = "Punctuation";

}

/*!
 *
 * @param code
 */
void Phase1::finishPhase1(vector<string> &code) {

    // to print matches
    ofstream tokenFile;
    tokenFile.open("Token.txt");
    vector<string> matches = DFA::match_dfa(minimized, code, mp);
    for (auto x : matches) {
        tokenFile << x << endl;
    }
    tokenFile.close();

    //to print dfa
    ofstream transitionFile;
    transitionFile.open("Transition.txt");
    // freopen("/home/sabra/ClionProjects/Compiler/Transition.txt", "w", stdout);
    DFA::printDFA(minimized, useless->id, transitionFile, mp);
    transitionFile.close();

    cout << "The Token.txt and Transition.txt Files Generated" << endl;
}


