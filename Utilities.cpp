//
// Created by sabra on 28/03/17.
//

#include <bits/stdc++.h>
#include "Utilities.h"

using namespace std;

/*!
 *
 * @param a
 * @return
 */
string Utilities::add_concatenation_to_string(string a) {
    string ans;
    ans = +a[0];
    for (int i = 1; i < a.size(); i++) {
        if (a[i] == '|' || a[i - 1] == '|' || a[i - 1] == '\\') {
            ans += a[i];
            continue;
        }
        if (a[i - 1] == '(') {
            ans += a[i];
            continue;
        }
        if (a[i] == ')' || a[i] == '@') {
            ans += a[i];
            continue;
        }
        if (a[i] == '-' || a[i] == '+' || a[i] == '*') {
            ans += a[i];
            continue;
        }
        ans += '@';
        ans += a[i];
    }
    return ans;
}

/*!
 *
 * @param def_map
 */
void Utilities::add_concatenation_def(map<string, string> &def_map) {
    map<string, string> substituted;
    for (auto const &x : def_map) {
        substituted[x.first] = Utilities::add_concatenation_to_string(x.second);
    }
    def_map = substituted;
}

/*!
 *
 * @param exp_map
 */
void Utilities::add_concatenation_expression(map<string, string> &exp_map) {
    map<string, string> substituted;
    for (auto const &x : exp_map) {
        substituted[x.first] = Utilities::add_concatenation_to_string(x.second);
    }
    exp_map = substituted;
}

/*!
 *
 * @param keywords
 */
void Utilities::add_concatenation_keywords(vector<string> &keywords) {
    for (int i = 0; i < keywords.size(); i++) {
        keywords[i] = Utilities::add_concatenation_to_string(Utilities::remove_spaces(keywords[i]));
    }
}

/*!
 *
 * @param haystack
 * @param needle
 * @return
 */
int Utilities::strstr(string haystack, string needle) {
    int f[needle.size()];
    f[0] = 0;
    for (int i = 1, k = 0; i < needle.size(); i++) {
        while (k > 0 && needle[i] != needle[k])
            k = f[k - 1];
        if (needle[i] == needle[k])
            k++;
        f[i] = k;
    }
    for (int i = 0, k = 0; i < haystack.length(); i++) {
        while (k > 0 && haystack[i] != needle[k])
            k = f[k - 1];
        if (haystack[i] == needle[k]) {
            k++;
        }
        if (k == needle.length()) {
            return i - k + 1;
        }
    }
    return -1;
}

/*!
 *
 * @param s
 * @return
 */
string Utilities::remove_spaces(string s) {
    string s_without_space;
    size_t pos = 0, found;
    while ((found = s.find_first_of(' ', pos)) != string::npos) {
        s_without_space += s.substr(pos, found - pos);
        pos = found + 1;
    }
    s_without_space += s.substr(pos);
    return s_without_space;
}

/*!
 *
 * @param line
 * @param delimeter
 * @return
 */
vector<string> Utilities::split(char *line, char *delimeter) {
    char *temp = strtok(line, delimeter);
    vector<string> result;
    while (temp != NULL) {//spliting the paths into string[]
        // debuging : printf("%s\n",temp);
        result.push_back(temp);
        temp = strtok(NULL, delimeter);
    }
    return result;
}

/*!
 *
 * @param in
 * @param added
 * @return
 */
unordered_set<string> Utilities::add_sets(unordered_set<string> in, unordered_set<string> added) {
    for (auto str : added) {
        in.insert(str);
    }
    return in;
}

/*!
 *
 * @param str
 * @return
 */
string Utilities::add_redundant_spaces(string str) {
    bool comma = false;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\'') {
            if (!comma) {
                comma = true;
            } else {
                str = str.insert(i + 1, " ");
                comma = false;
            }
        }
    }
    return str;
}

/*!
 *
 * @param in
 * @param added
 * @return
 */
unordered_set<string> Utilities::add_sets_except_epsilon(
        unordered_set<string> in, unordered_set<string> added) {
    for (auto str : added) {
        if (str != "\\L") {
            in.insert(str);
        }
    }
    return in;
}

vector<string> Utilities::read_file(string file_path) {
    vector<string> vec;
    ifstream myFile(file_path);
    string line;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            vec.push_back(line);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    myFile.close();
    return vec;
}