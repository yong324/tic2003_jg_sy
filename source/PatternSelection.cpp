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
    const auto& synonym_table = tables.at(synonym);

    vector<vector<string>> selected{};

    for (const auto& assign : synonym_table)
    {
        if ((entRef == "_" || entRef == assign[1]) && (expression_spec == "_" ||
            spec_type == "exact" && expression_spec == assign[2] || spec_type == "factor" && assign[2].
            find(expression_spec) != string::npos))
        {
            selected.push_back(assign);
        }
    }

    tables.insert_or_assign(synonym, selected);
}
