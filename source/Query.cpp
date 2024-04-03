#include "Query.h"

#include <algorithm>
#include <set>
#include <stdexcept>
#include <unordered_set>

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
    {"if", "ifs"},
    {"call", "calls"},
};

// Constructor for the Query class that initializes the Query object with provided details.
Query::Query(const map<string, string>& synonyms, const vector<string>& selectionVars,
             list<SelectionStructure*> structures) :
    synonyms(synonyms),
    selectionVars(selectionVars),
    structures(std::move(structures))
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
    map<string, vector<vector<string>>> all_tables{};

    // Populate tables with data from the database for each synonym.
    for (const auto& pair : synonyms)
    {
        all_tables.insert_or_assign(pair.first, vector<vector<string>>());
        Database::getData(tableNameMap.at(pair.second), all_tables.at(pair.first));
    }

    // Create a table with the cartesian product of the selected tables.
    vector<map<string, vector<string>>> cartesian_table{};
    for (const auto& pair : all_tables)
    {
        // Create a new selection table with the current synonym.
        vector<map<string, vector<string>>> new_selection_table{};
        for (const auto& next_record : pair.second)
        {
            if (cartesian_table.empty())
            {
                new_selection_table.push_back(map<string, vector<string>>{{pair.first, next_record}});
            }
            else
            {
                for (const auto& record : cartesian_table)
                {
                    map<string, vector<string>> new_record = record;
                    new_record.insert_or_assign(pair.first, next_record);
                    new_selection_table.push_back(new_record);
                }
            }
        }

        cartesian_table.clear();
        cartesian_table = new_selection_table;
    }

    // Apply selection structures to filter the results.
    for (const auto& structure : structures)
        structure->select(cartesian_table, synonyms);

    // Generate the output strings based on the selection variables.
    for (const auto& record : cartesian_table)
    {
        string result;

        size_t i = 0;
        for (const auto& selectionVar : selectionVars)
        {
            result += record.at(selectionVar)[0];
            if (i < selectionVars.size() - 1)
            {
                result += " ";
            }
            i++;
        }

        // Add the result to the output if it is unique.
        if (find(output.begin(), output.end(), result) == output.end())
        {
            output.push_back(result);
        }
    }
}
