#pragma once

#include <list>
#include <map>

#include "SelectionStructure.h"

enum ENUM
{
    MODIFIES, USES, PARENT, CALLS, CALLS_STAR, PARENT_STAR,
    ANY, SYNONYM, IDENT, INTEGER, EXACT, PARTIAL
};

class Query
{
    map<string, string> synonyms;
    vector<string> selectionVars;
    list<SelectionStructure*> structures;

public:
    static map<string, string> tableNameMap;

    Query(const map<string, string>& synonyms, const vector<string> &selectionVars, list<SelectionStructure*> structures);

    ~Query();

    void evaluate(vector<string>& output) const;
};
