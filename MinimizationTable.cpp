#include "MinimizationTable.h"

using namespace std;

/*!
 *
 * @param dfa
 * @return
 */
DFA* MinimizationTable::minimize_DFA(DFA* dfa) {
    map<int, map<char,int>>mp;
    map<int, int>type;
    map<int, int>accepting;
    map<int, int>group;
    map<pair<int,int>, int>hamada;
    stack<DFA *> s;
    s.push(dfa);
    set<int> vis;
    while (!s.empty()) {
        DFA *aux = s.top();
        s.pop();
        if (vis.find(aux->id) != vis.end())continue;
        vis.insert(aux->id);
        type[aux->id] = aux->type;
        accepting[aux->id] = aux->isAccepting;
        //cout << "hamada " << type[aux->id] << endl;
        if(hamada.find(make_pair(type[aux->id], accepting[aux->id])) == hamada.end()) {
            int x = hamada.size();
            hamada[make_pair(type[aux->id], accepting[aux->id])] = x;
        }
        group[aux->id] = hamada[make_pair(type[aux->id], accepting[aux->id])];
        mp[aux->id] = map<char,int>();
        for (auto x : aux->adjacent) {
            mp[aux->id][x.first] = (x.second)->id;
            s.push(x.second);
        }
    }
    int g = hamada.size();
    while(true){
        int total = g;
        for(auto x : mp) {
            bool increase = false;
            for(auto y : mp) {
                if(x.first <= y.first)break;
                if(group[x.first] != group[y.first])continue;
                bool equalGroup = true;
                for(auto edge: x.second) {
                    if(y.second.find(edge.first) == y.second.end()) {
                        equalGroup = false;
                        break;
                    }
                    if(group[edge.second] != group[y.second[edge.first]]) {
                        equalGroup = false;
                        break;
                    }
                }
                if(x.second.size() != y.second.size()) {
                    equalGroup = false;
                }
                if(!equalGroup) {
                    increase = true;
                    break;
                } else {
                    break;
                }
            }
            vector<int>to_change;
            to_change.push_back(x.first);
            if(increase) {
                for(auto y : mp) {
                    if(x.first >= y.first)continue;
                    if(group[x.first] != group[y.first])continue;
                    bool equalGroup = true;
                    for(auto edge: x.second) {
                        if(y.second.find(edge.first) == y.second.end()) {
                            equalGroup = false;
                            break;
                        }
                        if(group[edge.second] != group[y.second[edge.first]]) {
                            equalGroup = false;
                            break;
                        }
                    }
                    if(x.second.size() != y.second.size()) {
                        equalGroup = false;
                    }
                    if(equalGroup) {
                        to_change.push_back(y.first);
                    }
                }
                for(auto z : to_change) {
                    group[z] = total;
                }
                total++;
                break;
            }
        }
        if(total == g)break;
        g = total;
    }
    vector<vector<int>> v(g);
    for(auto x : mp) {
        v[group[x.first]].push_back(x.first);
    }
    map<int, DFA*> visited_dfa;
    for(int i = 0; i < g; i++){
        visited_dfa[i] = new DFA();
    }
    for(int i = 0; i < g; i++){
        visited_dfa[i]->isAccepting = false;
        visited_dfa[i]->type = 20;
        for(auto x : v[i]) {
            visited_dfa[i]->isAccepting |= accepting[x];
            if(accepting[x])visited_dfa[i]->type = min(visited_dfa[i]->type, type[x]);
        }
        if(v[i].size() > 0) {
            for(auto x: mp[v[i][0]]) {
                visited_dfa[i]->adjacent[x.first] = visited_dfa[group[x.second]];
            }
        }
    }
    return visited_dfa[hamada[make_pair(20,0)]];
}
