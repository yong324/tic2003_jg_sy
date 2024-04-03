#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class SelectionStructure
{
public:
    SelectionStructure() = default;
    virtual ~SelectionStructure() = default;
    virtual void select(vector<map<string, vector<string>>>& cartesian_table, const map<string, string>& synonyms) = 0;
};
