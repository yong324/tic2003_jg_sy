#include "PatternSelection.h"

#include "Database.h"

PatternSelection::PatternSelection(string synonym, string ent_ref_type, string ent_ref, string spec_type,
    string expression_spec) :
    synonym(std::move(synonym)),
    entRef_type(std::move(ent_ref_type)),
    entRef(std::move(ent_ref)),
    spec_type(
        std::move(
            spec_type)),
    expression_spec(std::move(expression_spec))
{
}

void PatternSelection::select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym)
{
    const auto& table = tables.at(selection_synonym.get_var_name());

    vector<vector<string>> selected{};

    vector<vector<string>> assignmentsTable;
    Database::getData("assignments", assignmentsTable);
    for (const auto& record : table)
    {
        for (const auto& assignment : assignmentsTable)
        {
            if ((entRef == "_" || entRef == assignment[1]) && (expression_spec == "_" ||
                assignment[2].find(expression_spec) != string::npos))
            {
                selected.push_back(record);
                break;
            }
        }
    }

    tables.insert_or_assign(selection_synonym.get_var_name(), selected);
}
