#include "PatternSelection.h"

#include "Database.h"

// Constructor for PatternSelection, initializing the object with pattern details.
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

// The select method filters data based on the pattern specified in the query.
// It manipulates the tables map to retain only those rows that match the pattern criteria.
void PatternSelection::select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym)
{
    // Fetch the table associated with the synonym of this pattern.
    const auto& synonym_table = tables.at(synonym);
    vector<vector<string>> selected{};

    // Iterate over each row in the table and apply the pattern criteria.
    for (const auto& assign : synonym_table)
    {
        // Check if the entity reference matches (if applicable) and if the expression specification is satisfied.
        if ((entRef == "_" || entRef == assign[1]) && (expression_spec == "_" ||
            spec_type == "exact" && expression_spec == assign[2] || spec_type == "factor" && assign[2].
            find(expression_spec) != string::npos))
        {
            selected.push_back(assign);
        }
    }

    tables.insert_or_assign(synonym, selected);
}
