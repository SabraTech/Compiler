//
// Created by sabra on 26/03/17.
//

#include "bits/stdc++.h"
#include "Parser.h"
#include "Node.h"
#include "NFA.h"

using namespace std;


vector<string> lexical_rules;
map<string, string> def_map;
vector<string> keywords, punctuations;
map<string, string> exp_map;

string add_concatination_to_string(string a) {
    string ans;
    ans =+ a[0];
    for(int i = 1; i < a.size(); i++) {
        if(a[i] == '|' || a[i-1] == '|' || a[i-1] == '\\'){
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
    for(auto const& x : def_map) {
        substituted[x.first] = add_concatination_to_string(x.second);
    }
    def_map = substituted;
}

void add_concatination_expression(){
    map<string, string>substituted;
    for(auto const& x : exp_map) {
        substituted[x.first] = add_concatination_to_string(x.second);
    }
    exp_map = substituted;
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
            int found = strstr(value, key);
            while(found != -1){
                string str;
                str += value.substr(0,found);
                str += def_map[key];
                str += value.substr(found + key.size(),value.size());
                def_map[y.first] = str;
                found = strstr(str,key);
                value = y.second;
            }
        }
    }

    for(auto const& x : list) {
        string key = x.second;
        for (auto const& y : exp_map) {
            string value = y.second;
            int found = strstr(value, key);
            while (found != -1) {
                string str;
                str += value.substr(0, found);
                str += def_map[key];
                str += value.substr(found + key.size(), value.size());
                exp_map[y.first] = str;
                found = strstr(str, key);
                value = y.second;
            }
        }
    }
    //////////////////////

    // remove any space after substitute
    for(auto const& entry : exp_map){
        exp_map[entry.first] = remove_spaces(entry.second);
    }

    add_concatination_def();
    add_concatination_expression();
    for(int i = 0; i < keywords.size(); i++) {
        keywords[i] = add_concatination_to_string(remove_spaces(keywords[i]));
        if(keywords[i][0] == '\0') {
            continue;
        }
        cout << keywords[i] << endl;
    }

    // print to test here
    cout << "def map" << endl;
    for (auto const& x : def_map)
    {
        cout << x.first << " : " << x.second << endl ;
    }

    cout << "exp map" << endl;
    for (auto const& x : exp_map)
    {
        cout << x.first << " : " << x.second << endl ;
    }

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
    // minimize DFA
    // enter code

    return 0;
}
