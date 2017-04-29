//
// Created by M on 4/24/2017.
//


#include "Utilities.h"
#include "ParsingTable.h"

ParsingTable::ParsingTable(unordered_map<string, unordered_set<string>> &first_sets,
                           unordered_map<string, unordered_set<string>> &follow_sets,
                           unordered_map<string, vector<vector<string>>> &productions,
                           vector<string> &input_maches,
                           unordered_set<string> &terminals,
                           string &start_symbol
) {
    this->first_sets = first_sets;
    this->follow_sets = follow_sets;
    this->productions = productions;
    this->matches = input_maches;
    this->terminals = terminals;
    this->start_symbol = start_symbol;
}

void ParsingTable::build_the_table(void) {
    build_step1();
    build_step2();
    build_step3();
    match();
    // print_parsing_table();
}

void ParsingTable::build_step1(void) {
//for each terminal a in FIRST(x) , add A -> x to table[A,a]
    for (auto non_terminal : productions) {
        vector<vector<string>> pros = non_terminal.second;
        for (auto terminal : first_sets[non_terminal.first]) {
            if (terminal == "\\L") continue;
            for (int i = 0; i < pros.size(); i++) {
                if (first_sets[pros[i][0]].find(terminal) != first_sets[pros[i][0]].end()) {
                    pair<string, string> pair = make_pair(non_terminal.first, terminal);
                    table[pair] = pros[i];
                    break;
                }
            }
        }
    }
}

void ParsingTable::build_step2(void) {
//for each epislon in FIRST(x) for each terminal a in follow(A), add A -> e to table[A,a]
    for (auto non_terminal : productions) {
        vector<vector<string>> pros = non_terminal.second;
        for (int i = 0; i < pros.size(); i++) {
            if (first_sets[pros[i][0]].find("\\L") != first_sets[pros[i][0]].end()) {
                for (auto terminal : follow_sets[non_terminal.first]) {
                    pair<string, string> pair = make_pair(non_terminal.first, terminal);
                    if (table.find(pair) == table.end()) {
                        table[pair] = pros[i];
                    }
                }

            }
        }
    }
}


void ParsingTable::build_step3(void) {
//synch for every follow in (A) that doesn`t not have a production to go for , it is synchronizing token
    for (auto non_terminal : productions) {
        vector<vector<string>> pros = non_terminal.second;
        for (auto terminal : follow_sets[non_terminal.first]) {
            pair<string, string> pair = make_pair(non_terminal.first, terminal);
            if (table.find(pair) == table.end()) {
                vector<string> tmp;
                tmp.push_back("synch");
                table[pair] = tmp;
            }
        }
    }
}

void ParsingTable::print_parsing_table(void) {
    for (auto entry : table) {
        cout << "NonTerminal: " << entry.first.first << ", Terminal: " << entry.first.second;
        cout << "  Productions: ";
        for (int i = 0; i < entry.second.size(); i++) {
            cout << " " << entry.second[i];
        }
        cout << endl;
    }
}

void ParsingTable::match() {

    vector<string> stack;
    int i = 0;
    stack.push_back(start_symbol);
    ofstream parser_file;
    parser_file.open("Output.txt");
    while (!stack.empty() && i < matches.size()) {
        for (int j = stack.size() - 1; j >= 0; j--) {
            string x = stack[j];
            parser_file << x << " ";
        }
        parser_file << endl;
        string top = stack[stack.size() - 1];
        if (terminals.find(top) != terminals.end()) {
            // top is terminal
            stack.pop_back();
            if (top == matches[i]) {
                i++;
            } else {
                parser_file << "Missing terminal stack token is  " << top << " while token is " << matches[i] << endl;
            }
        } else {
            pair<string, string> pair = make_pair(top, matches[i]);
            if (table.find(pair) != table.end()) {
                vector<string> pro = table[pair];
                if (pro[0] == "synch" ) {
                    parser_file << "Synch token pop the stack " << endl;
                    stack.pop_back();
                } else if (pro[0] == "\\L") {
                    stack.pop_back();
                } else {
                    stack.pop_back();
                    for (int j = pro.size() - 1; j >= 0; j--) {
                        stack.push_back(pro[j]);
                    }

                }

            } else {
                parser_file << "Discarded token " << matches[i] << endl;

                //empty entry in the table so discard the input;
                i++;
            }
        }
    }
    for (int j = stack.size() - 1; j >= 0; j--) {
        string x = stack[j];
        parser_file << x << " ";
    }
    parser_file << endl;
    int n = stack.size();
    while (n > 0) {
        n--;
        pair<string, string> pair = make_pair(stack[n],"$");

        if (table.find(pair) != table.end()) {
            vector<string> pro = table[pair];
            if (pro[0] == "synch") {
                parser_file << "Synch token pop the stack " << endl;
                stack.pop_back();
            } else if (pro[0] == "\\L") {
                stack.pop_back();
            }
        }else {
            parser_file << "error missing " << stack[n] << ", inserted" << endl;
            stack.pop_back();
        }
    }
    cout << "Output.txt file generated" << endl;
    parser_file.close();
}


