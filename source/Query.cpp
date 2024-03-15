#include "Query.h"

#include <stdexcept>

#include "Database.h"

map<string, string> Query::tableNameMap = {
    {"procedure", "procedures"},
    {"variable", "variables"},
    {"constant", "constants"},
    {"assign", "assignments"},
    {"print", "prints"},
    {"read", "reads"},
    {"stmt", "statements"},
    {"while", "whiles"},
    {"if", "ifs"}
};

Query::Query(const vector<Synonym>& synonyms, const string& selection_var, vector<SelectionStructure*> structures) :
    synonyms(synonyms),
    selectionVar(selection_var),
    structures(structures)
{
}

Query::~Query()
{
    for (const auto structure : structures)
        delete structure;
}

void Query::evaluate(vector<string>& output) const
{
    map<string, vector<vector<string>>> tables{};
    const Synonym* selection_synonym = nullptr;
    for (const auto& synonym : synonyms)
    {
        tables.insert_or_assign(synonym.get_var_name(), vector<vector<string>>());
        Database::getData(tableNameMap.at(synonym.get_type()), tables.at(synonym.get_var_name()));
        if (synonym.get_var_name() == selectionVar)
        {
            selection_synonym = &synonym;
        }
    }

    // TODO: Later implement multiple structures here
    for (const auto& structure : structures)
    {
        structure->select(tables, *selection_synonym);
    }

    const auto& table = tables.at(selectionVar);
    // Post-process results
    for (const auto& record : table)
    {
        output.push_back(record[0]);
    }
}
