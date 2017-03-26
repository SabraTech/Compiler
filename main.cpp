//
// Created by sabra on 26/03/17.
//
#include "bits/stdc++.h"
#include "Parser.h"

using namespace std;

vector<string> lexical_rules;
map<string, string> def_map;
vector<string> keywords, punctuations;
vector<pair<string, string>> exp_map; // change in all

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
    for(int i = 0 ; i < exp_map.size();  i++){
        exp_map[i].second = add_concatination_to_string(exp_map[i].second);
    }
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
    map<string, string> exp_map = Parser::parse_out_expressions(lexical_rules);
    keywords = Parser::parse_out_keywords(lexical_rules);
    punctuations = Parser::parse_out_punctuations(lexical_rules);

    add_concatination_def();
    // now substitute definitions into expressions and def in def
    // remove any space after substitute
    add_concatination_expression();


    // create NFA for expressions, keywords and punctuations
    // or all NFAs to one big NFA
    // transfer NFA to DFA
    // minimize DFA
    // enter code

    return 0;
}
