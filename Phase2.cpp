#include "Phase2.h"
#include "Parser.h"
#include "Utilities.h"

using namespace std;


Phase2::Phase2(vector<string> &input_file) {
    cfg_list= input_file;
}
void Phase2::startPhase2() {
    terminals.insert("\\L");
    initialize_cfg_list();
    build_the_map();
    get_first_sets();
}
void Phase2::initialize_cfg_list() {
    //Operates on cfg_list to every production on one line no line starts with or "|" and removes spaces at the begining and "#"
    int j = 0;
    for(int i = 0; i < cfg_list.size(); i++) {
        string str = Parser::left_trim(cfg_list[i]);
        if(str[0] == '#' ) {
            str = str.substr(1, str.size());
            i++;
            if(i < cfg_list.size()) {
                string str2 =  Parser::left_trim(cfg_list[i]);
                while(str2[0] != '#' && i < cfg_list.size() ) {
                    str += str2;
                    i++;
                    str2 =  Parser::left_trim(cfg_list[i]);
                }
            }
            i--;
        }
        cfg_list[j++] =  Parser::left_trim(str);
     }
     int size = cfg_list.size();
     for(int i = 0; i < size - j; i++){
         cfg_list.pop_back();
     }
}

void Phase2::build_the_map() {
    for(int i = 0; i < cfg_list.size(); i++) {
          string production = cfg_list[i];
          int index_of_equal = production.find_first_of("=");
          string LHS = production.substr(0, index_of_equal);
          LHS = Parser::trim(LHS);
          string RHS = production.substr(index_of_equal + 1);
          RHS = Parser::trim(RHS);

          char *cstr = new char[RHS.length() + 1];
          strcpy(cstr, RHS.c_str());
          vector<string> pro = Utilities::split(cstr, "|");

          vector<vector<string>> value;
          for(int j = 0; j < pro.size(); j++) {
              pro[j] = Parser::trim(pro[j]);
              pro[j] = Utilities::add_redundant_spaces(pro[j]);
              char *cstr2 = new char[pro[j].length() + 1];
              strcpy(cstr2, pro[j].c_str());
              vector<string> temp = Utilities::split(cstr2, " ");
              for(int k = 0; k < temp.size(); k++) {
                  if(temp[k][0] == '\'') {
                      temp[k] = temp[k].substr(1, temp[k].length() - 2);
                      terminals.insert(temp[k]);
                   }
              }

              value.push_back(temp);
          }
          productions[LHS] = value;
    }
}


void Phase2::get_first_sets() {
    for(auto key : productions) {
        if(visited.find(key) == visited.end()) {
            get_first_sets(key);
        }
    }
}


unordered_set<string> Phase2::get_first_sets(string non_terminal) {
    if(visited.find(non_terminal) != visited.end()) {
        return first[non_terminal];
    }
    visited.insert(non_terminal);
    if(terminals.find(non_terminal) != terminals.end()) {
        unordered_set<string> terminal ;
        terminal.insert(non_terminal);
        first[non_terminal] = terminal;
        return first[non_terminal];
    }
    vector<vector<string>> value = productions[non_terminal];
    for(int i = 0 ; i < value.size() ; i++) {
        vector<string> pro_i = value[i];
        for(int j = 0; j < pro_i.size() ; j++) {
            unordered_set<string> temp = get_first_sets(pro_i[j]);
            first[non_terminal] = Utilities::add_sets(first[non_terminal] , temp);
            if(temp.find("\\L") == temp.end()) {
                break;
            }
        }
    }
    return first[non_terminal];
}


void Phase2::print_first_sets(void) {
  for(auto key : first) {
    if(terminals.find(key.first) != terminals.end()) {
            continue;
    }
    cout << "First of " << key.first  << " is " << endl;
    unordered_set<string> temp = key.second;
    for(auto f : temp ){
      cout << f << " ";
    }
    cout << endl;
  }
}
