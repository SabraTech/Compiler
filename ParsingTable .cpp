//
// Created by M on 4/24/2017.
//


#include "Utilities.h"
#include "ParsingTable.h"

/*!
 *
 * @param first_sets
 * @param follow_sets
 * @param productions
 * @return
 */
ParsingTable::ParsingTable(unordered_map<string, unordered_set<string>> &first_sets,
                           unordered_map<string, unordered_set<string>> &follow_sets,
                           unordered_map<string, vector<vector<string>>> &productions) {
    this->first_sets = first_sets;
    this->follow_sets = follow_sets;
    this->productions = productions;
}

void ParsingTable::build_the_table(void) {
    build_step1();
    build_step2();
    print_parsing_table();
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
        for (auto terminal : first_sets[non_terminal.first]) {
            if (terminal == "\\L") continue;
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