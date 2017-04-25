//
// Created by sabra on 26/03/17.
//

#include "bits/stdc++.h"
#include "Phase1.h"
#include "Phase2.h"
#include "Utilities.h"

using namespace std;

int main() {
    vector<string> rules = Utilities::read_file("Rules.txt");

    Phase1 *phase1 = new Phase1(rules);
    Phase1::startPhase1();

    // reading the code file
    vector<string> code = Utilities::read_file("Code.txt");

    vector<string> matches = Phase1::finishPhase1(code);

    vector<string> cfg = Utilities::read_file("Grammar.txt");

    Phase2 *phase2 = new Phase2(cfg, matches);

    phase2->startPhase2();

    return 0;
}