//
// Created by sabra on 26/03/17.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H


using namespace std;

class Parser {
public:
    map<string, string> parse_out_definitions(vector<string> &);
    map<string, string> parse_out_expressions(vector<string> &);
    vector<string> parse_out_keywords(vector<string> &);
    vector<string> parse_out_punctuations(vector<string> &);

private:
    static inline string left_trim(string &);
    static inline string right_trim(string &);
    static inline string trim(string &);

};


#endif //COMPILER_PARSER_H
