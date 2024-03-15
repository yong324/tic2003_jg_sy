#pragma once

#include <map>
#include <string>
#include <vector>

#include "Synonym.h"
using namespace std;

class SelectionStructure
{
public:
    virtual ~SelectionStructure();
    virtual void select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym);
};
