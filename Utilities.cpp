//
// Created by sabra on 28/03/17.
//

#include <bits/stdc++.h>
#include "Utilities.h"

using namespace std;

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

void Utilities::add_concatenation_def(map<string, string> &def_map) {
    map<string, string> substituted;
    for (auto const &x : def_map) {
        substituted[x.first] = Utilities::add_concatenation_to_string(x.second);
    }
    def_map = substituted;
}

void Utilities::add_concatenation_expression(map<string, string> &exp_map) {
    map<string, string> substituted;
    for (auto const &x : exp_map) {
        substituted[x.first] = Utilities::add_concatenation_to_string(x.second);
    }
    exp_map = substituted;
}

void Utilities::add_concatenation_keywords(vector<string> &keywords) {
    for (int i = 0; i < keywords.size(); i++) {
        keywords[i] = Utilities::add_concatenation_to_string(Utilities::remove_spaces(keywords[i]));
    }
}

int Utilities::strstr(string haystack, string needle) {
    int f[needle.size()];
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