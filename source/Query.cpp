#include "Query.h"

#include <stdexcept>

#include "Database.h"

// Static map to associate string representations with table names in the database.
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

// Constructor for the Query class that initializes the Query object with provided details.
Query::Query(const vector<Synonym>& synonyms, const string& selection_var, vector<SelectionStructure*> structures) :
    synonyms(synonyms),
    selectionVar(selection_var),
    structures(structures)
{
}

// Destructor for the Query class that cleans up dynamically allocated SelectionStructures.
Query::~Query()
{
    for (const auto structure : structures)
        delete structure;
}

// The evaluate method processes the Query object to fetch and filter data as per the query's definition.
void Query::evaluate(vector<string>& output) const
{
    map<string, vector<vector<string>>> tables{};
    const Synonym* selection_synonym = nullptr;

    // Populate tables with data from the database for each synonym.
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
    // Apply selection structures to filter the results.
    if (structures.size() == 2)
    {
        structures[1]->select(tables, *selection_synonym);
        structures[0]->select(tables, *selection_synonym);
    }
    else
    {
        structures[0]->select(tables, *selection_synonym);
    }

    // Collect the results post selection and filtering.
    const auto& table = tables.at(selectionVar);
    for (const auto& record : table)
    {
        output.push_back(record[0]);
    }
}
