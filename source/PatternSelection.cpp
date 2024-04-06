#include "PatternSelection.h"

#include <utility>

#include "Database.h"
#include "Query.h"

// Constructor for PatternSelection, initializing the object with pattern details.
PatternSelection::PatternSelection(string syn_assign, int ent_ref_type, string ent_ref, int spec_type,
    string expression_spec) :
    syn_assign(std::move(syn_assign)),
    entRef_type(ent_ref_type),
    entRef(std::move(ent_ref)),
    spec_type(spec_type),
    expression_spec(std::move(expression_spec))
{
}

// The select method filters data based on the pattern specified in the query.
// It manipulates the tables map to retain only those rows that match the pattern criteria.
void PatternSelection::select(vector<map<string, vector<string>>>& cartesian_table, const map<string, string>& synonyms)
{
    vector<map<string, vector<string>>> new_cartesian_table;
    for (const auto& record : cartesian_table)
    {
        const auto& assign_record = record.at(syn_assign);
        if (
            (
                entRef_type == ANY ||
                entRef_type == IDENT && entRef == assign_record[1] ||
                entRef_type == SYNONYM && record.at(entRef)[0] == assign_record[1]
                ) && (
                    spec_type == ANY ||
                    spec_type == EXACT && expression_spec == assign_record[2] ||
                    spec_type == PARTIAL && partialMatch(assign_record[2])
                    )
            )
        {
            new_cartesian_table.push_back(record);
        }
    }

    cartesian_table = new_cartesian_table;
}

// The partialMatch method checks if the expression_spec is a substring of the assign_record.
bool PatternSelection::partialMatch(const string& expression) const
{
    const bool plus_minus =
        expression_spec.find('+') != string::npos ||
        expression_spec.find('-') != string::npos;

    size_t index = -1;
    while ((index = expression.find(expression_spec, index + 1)) != string::npos)
    {
        if (
            (
                index == 0 ||
                !isalnum(expression[index - 1]) &&
                (
                    !plus_minus ||
                    expression[index - 1] != '*' &&
                    expression[index - 1] != '/'
                    )
                ) && (
                    index + expression_spec.size() == expression.size() ||
                    !isalnum(expression[index + expression_spec.size()]) &&
                    (
                        !plus_minus ||
                        expression[index + expression_spec.size()] != '*' &&
                        expression[index + expression_spec.size()] != '/'
                        )
                    )
            )
            return true;
    }

    return false;
}
