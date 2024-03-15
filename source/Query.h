#pragma once

#include <map>

#include "Synonym.h"
#include "SelectionStructure.h"

class Query
{
private:
    vector<Synonym> synonyms;
    string selectionVar;
    vector<SelectionStructure*> structures;

public:
    static map<string, string> tableNameMap;

    Query(const vector<Synonym>& synonyms, const string& selection_var, vector<SelectionStructure*> structures);

    ~Query();

    void evaluate(vector<string>& output) const;
};
