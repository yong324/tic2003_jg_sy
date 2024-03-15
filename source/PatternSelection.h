#pragma once
#include "SelectionStructure.h"

class PatternSelection : public SelectionStructure
{
private:
    string synonym;
    string entRef_type;
    string entRef;
    string spec_type;
    string expression_spec;

public:
    PatternSelection(string synonym, string ent_ref_type, string ent_ref, string spec_type,
        string expression_spec);

    void select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym) override;
};
