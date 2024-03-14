#include "SuchThatSelection.h"

#include "Database.h"

SuchThatSelection::SuchThatSelection(string synonyms, string ref1_type, string ref1, string ref2_type, string ref2) :
    synonyms(std::move(synonyms)),
    ref1_type(std::move(ref1_type)),
    ref1(std::move(ref1)),
    ref2_type(std::move(ref2_type)),
    ref2(std::move(ref2))
{
}

vector<string> SuchThatSelection::select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym)
{
    const auto table = tables.at(selection_synonym.get_var_name());
    vector<string> selected{};
    if (synonyms == "Modifies")
    {
        vector<vector<string>> modifiesTable;
        Database::getData("modifies", modifiesTable);
        for (auto& record : table)
        {
            for (const auto& modify : modifiesTable)
            {
                if (modify[0] == ref1 && modify[1] == ref2)
                {
                    selected.push_back(record[0]);
                }
            }
        }
    }
    else if (synonyms == "Parent")
    {
        vector<vector<string>> statementsTable;
        Database::getData("statements", statementsTable);
        for (auto& record : table)
        {
            for (const auto& statement : statementsTable)
            {
                if (statement[0] == ref2 && statement[1] == ref1)
                {
                    selected.push_back(record[0]);
                    break;
                }
            }
        }
    }
    else if (synonyms == "Parent*")
    {
        vector<vector<string>> statementsTable;
        Database::getData("statements", statementsTable);
        for (auto& record : table)
        {
            string child = ref2;
            bool end = false;
            while (!end)
            {
                bool found = false;
                for (const auto& statement : statementsTable)
                {
                    if (statement[0] == child)
                    {
                        found = true;
                        if (statement[1] == ref1)
                        {
                            selected.push_back(record[0]);
                            end = true;
                        }
                        else
                        {
                            child = statement[1];
                        }
                        break;
                    }
                }

                if (!found)
                {
                    break;
                }
            }
        }
    }
    return selected;
}
