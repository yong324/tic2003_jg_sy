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
    {"stmt", "statements"}
};

Query::Query(const vector<Synonym>& synonyms, const string& selection_var, SelectionStructure* structure):
    synonyms(synonyms),
    selectionVar(selection_var),
    structure(structure)
{
}

Query::~Query()
{
    delete structure;
}

void Query::evaluate(vector<string>& output) const
{
    map<string, vector<vector<string>>> tables{};
    const Synonym *selection_synonym = nullptr;
    for (const auto &synonym : synonyms)
    {
        tables.insert_or_assign(synonym.get_var_name(), vector<vector<string>>());
        Database::getData(tableNameMap.at(synonym.get_type()), tables.at(synonym.get_var_name()));
        if (synonym.get_var_name() == selectionVar)
        {
            selection_synonym = &synonym;
        }
    }

    const auto results = structure->select(tables, *selection_synonym);

    // Post-process results
    for (const string& result : results)
    {
        output.push_back(result);
    }
}
