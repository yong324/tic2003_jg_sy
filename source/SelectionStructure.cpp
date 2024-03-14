#include "SelectionStructure.h"

SelectionStructure::~SelectionStructure() = default;

vector<string> SelectionStructure::select(map<string, vector<vector<string>>>&tables, const Synonym & selection_synonym)
{
    vector<string> result;
    const auto table = tables.at(selection_synonym.get_var_name());
    result.reserve(table.size());
    for (auto& record : table)
        result.push_back(record.at(0));
    return result;
}