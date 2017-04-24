//
// Created by sabra on 26/03/17.
//

#include "bits/stdc++.h"
#include "Phase1.h"
#include "Phase2.h"

using namespace std;
/*
vector<string> rules;

int main() {
    ifstream myFile("Rules.txt");
    string line;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            rules.push_back(line);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    myFile.close();

    Phase1 *phase1 = new Phase1(rules);
    Phase1::startPhase1();

    // reading the code file
    vector<string> code;
    ifstream myCode("Code.txt");
    string lineOfCode;
    if(myCode.is_open()){
        while(getline(myCode,lineOfCode)){
            lineOfCode = Parser::trim(lineOfCode);
            code.push_back(lineOfCode);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    myCode.close();

    Phase1::finishPhase1(code);

    return 0;
}

*/

int main() {
    vector<string> cfg;
    ifstream cfgFile("Grammar.txt");
    string line;
    if(cfgFile.is_open()) {
        while(getline(cfgFile,line)) {
            cfg.push_back(line);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    cfgFile.close();
    Phase2 *phase2 = new Phase2(cfg);

    phase2->startPhase2();
}


