#pragma once
#include "SelectionStructure.h"

class SuchThatSelection : public SelectionStructure
{
    string synonyms;
    string ref1_type;
    string ref1;
    string ref2_type;
    string ref2;

public:
    SuchThatSelection(string synonyms, string ref1_type, string ref1, string ref2_type,
        string ref2);

    void select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym) override;
};
