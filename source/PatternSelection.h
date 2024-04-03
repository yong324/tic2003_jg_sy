#pragma once
#include "SelectionStructure.h"

class PatternSelection : public SelectionStructure
{
    string syn_assign;
    int entRef_type;
    string entRef;
    int spec_type;
    string expression_spec;

public:
    PatternSelection(string syn_assign, int ent_ref_type, string ent_ref, int spec_type, string expression_spec);
    bool partialMatch(const string& expression) const;

    void select(vector<map<string, vector<string>>>& cartesian_table, const map<string, string>& synonyms) override;
};
