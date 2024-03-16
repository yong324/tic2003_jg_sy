#include "SuchThatSelection.h"
#include "Database.h"

// Constructor for SuchThatSelection initializes the object with the details of the "such that" clause.
SuchThatSelection::SuchThatSelection(string synonyms, string ref1_type, string ref1, string ref2_type, string ref2) :
    synonyms(std::move(synonyms)),
    ref1_type(std::move(ref1_type)),
    ref1(std::move(ref1)),
    ref2_type(std::move(ref2_type)),
    ref2(std::move(ref2))
{
}

// The select method applies the "such that" condition to filter data in the tables map.
// This method specifically evaluates the "such that" relation and retains rows in the table that meet the condition.
void SuchThatSelection::select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym)
{
    // The implementation details will depend on the specific "such that" relationship (e.g., Modifies, Uses, Parent, etc.).
    string var1 = ref1;
    string var2 = ref2;

    table1 = nullptr;
    table2 = nullptr;
    tableX = nullptr;
    tableY = nullptr;

    // Determine the table and column mappings based on the provided reference types and values.
    if (!isdigit(ref1[0]) && ref1 != "_" && ref1_type == "stmtRef")
    {
        table1 = &tables.at(ref1);
        if (ref1 == selection_synonym.get_var_name())
        {
            tableX = table1;
        }
        else
        {
            tableY = table1;
        }
    }

    if (!isdigit(ref2[0]) && ref2 != "_" && ref2_type == "stmtRef")
    {
        table2 = &tables.at(ref2);
        if (ref2 == selection_synonym.get_var_name())
        {
            tableX = table2;
        }
        else
        {
            tableY = table2;
        }
    }

    const auto& table = tables.at(selection_synonym.get_var_name());
    if (synonyms == "Modifies")
    {
        vector<vector<string>> modifiesTable;
        Database::getData("modifies", modifiesTable);

        if (tableX != nullptr && tableY != nullptr) {
            filterDataWithBothTables(modifiesTable);
        }
        else if (tableX != nullptr) {
            filterDataWithSingleTable(modifiesTable, true);
        }
        else if (tableY != nullptr) {
            filterDataWithSingleTable(modifiesTable, false);
        }
        else {
            filterDataWithoutTables(modifiesTable, table);
        }
    }
    else if (synonyms == "Parent")
    {
        vector<vector<string>> statementsTable;
        Database::getData("statements", statementsTable);
        if (tableX != nullptr) {
            if (tableY != nullptr) {
                filterParentWithBothTables(statementsTable);
            }
            else {
                filterParentWithSingleTable(statementsTable, true);
            }
        }
        else if (tableY != nullptr) {
            filterParentWithSingleTable(statementsTable, false);
        }
        else {
            filterParentWithoutTables(statementsTable, table);
        }
    }
    else if (synonyms == "Parent*")
    {
        vector<vector<string>> statementsTable;
        Database::getData("statements", statementsTable);
        if (tableX != nullptr) {
            if (tableY != nullptr) {
                filterParentStarWithBothTables(statementsTable);
            }
            else {
                filterParentStarWithSingleTable(statementsTable, true);
            }
        }
        else if (tableY != nullptr) {
            filterParentStarWithSingleTable(statementsTable, false);
        }
        else {
            filterParentStarWithoutTables(statementsTable, table);
        }
    }
    // After applying all filters, update the table for the selection synonym to only include the selected rows.
    tables.insert_or_assign(selection_synonym.get_var_name(), selected);
}

void SuchThatSelection::filterDataWithBothTables(const vector<vector<string>>& modifiesTable) {
    for (const auto& recordX : *tableX) {
        for (const auto& recordY : *tableY) {
            if (findMatchingModification(recordX, recordY, modifiesTable)) {
                selected.push_back(recordX);
                break;
            }
        }
    }
}

bool SuchThatSelection::findMatchingModification(const vector<string>& recordX, const vector<string>& recordY, const vector<vector<string>>& modifiesTable) {
    for (const auto& modify : modifiesTable) {
        if ((tableX == table1 && modify[1] == recordY[0] && modify[0] == recordX[0]) ||
            (tableY == table1 && modify[1] == recordX[0] && modify[0] == recordY[0])) {
            return true;
        }
    }
    return false;
}

void SuchThatSelection::filterDataWithSingleTable(const vector<vector<string>>& modifiesTable, bool isTableX) {
    const vector<vector<string>>* table = isTableX ? tableX : tableY;
    string ref = isTableX ? ref2 : ref1;
    auto refTable = isTableX ? table1 : table2;

    for (const auto& record : *table) {
        for (const auto& modify : modifiesTable) {
            if ((table == refTable && (ref == "_" || (isTableX ? modify[1] == ref : modify[0] == ref)) && modify[0] == record[0]) ||
                (table != refTable && modify[1] == record[0] && (ref == "_" || (isTableX ? modify[0] == ref : modify[1] == ref)))) {
                selected.push_back(record);
                break;
            }
        }
    }
}

void SuchThatSelection::filterDataWithoutTables(const vector<vector<string>>& modifiesTable, const vector<vector<string>>& table) {
    for (auto& record : table) {
        for (const auto& modify : modifiesTable) {
            if ((ref1 == "_" || modify[0] == ref1) && (ref2 == "_" || modify[1] == ref2)) {
                selected.push_back(record);
                break;
            }
        }
    }
}

void SuchThatSelection::filterParentWithBothTables(const vector<vector<string>>& statementsTable) {
    for (const auto& recordX : *tableX) {
        for (const auto& recordY : *tableY) {
            if (checkParentRelation(recordX, recordY, statementsTable)) {
                selected.push_back(recordX);
                break;
            }
        }
    }
}

bool SuchThatSelection::checkParentRelation(const vector<string>& recordX, const vector<string>& recordY, const vector<vector<string>>& statementsTable) {
    for (const auto& statement : statementsTable) {
        if ((tableX == table1 && statement[1] == recordX[0] && statement[0] == recordY[0]) ||
            (tableY == table1 && statement[1] == recordY[0] && statement[0] == recordX[0])) {
            return true;
        }
    }
    return false;
}

void SuchThatSelection::filterParentWithSingleTable(const vector<vector<string>>& statementsTable, bool isTableX) {
    const vector<vector<string>>* table = isTableX ? tableX : tableY;
    string ref = isTableX ? ref2 : ref1;
    auto refTable = isTableX ? table1 : table2;

    for (const auto& record : *table) {
        for (const auto& statement : statementsTable) {
            if (isTableX && table == refTable && statement[1] == record[0] && statement[0] == ref ||
                !isTableX && table == refTable && statement[0] == record[0] && statement[1] == ref) {
                selected.push_back(record);
                break;
            }
        }
    }
}

void SuchThatSelection::filterParentWithoutTables(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table) {
    for (auto& record : table) {
        for (const auto& statement : statementsTable) {
            if (statement[0] == ref2 && statement[1] == ref1) {
                selected.push_back(record);
                break;
            }
        }
    }
}

void SuchThatSelection::filterParentStarWithBothTables(const vector<vector<string>>& statementsTable) {
    for (const auto& recordX : *tableX) {
        for (const auto& recordY : *tableY) {
            if (isTransitiveParent(recordX[0], recordY[0], statementsTable)) {
                selected.push_back(recordX);
                break;
            }
        }
    }
}

bool SuchThatSelection::isTransitiveParent(const string& parent, const string& child, const vector<vector<string>>& statementsTable) {
    std::set<string> visited;
    return checkTransitiveParent(parent, child, statementsTable, visited);
}

bool SuchThatSelection::checkTransitiveParent(const string& currentParent, const string& targetChild, const vector<vector<string>>& statementsTable, std::set<string>& visited) {
    if (visited.count(currentParent)) {
        return false; // Avoid infinite loops in cyclic structures
    }
    visited.insert(currentParent);

    for (const auto& statement : statementsTable) {
        if (statement[1] == currentParent) {
            if (statement[0] == targetChild || checkTransitiveParent(statement[0], targetChild, statementsTable, visited)) {
                return true;
            }
        }
    }
    return false;
}

void SuchThatSelection::filterParentStarWithSingleTable(const vector<vector<string>>& statementsTable, bool isTableX) {
    const vector<vector<string>>* table = isTableX ? tableX : tableY;
    string fixedRef = isTableX ? ref2 : ref1;

    for (const auto& record : *table) {
        string variableRef = record[0];
        if (isTableX) {
            if (isTransitiveParent(variableRef, fixedRef, statementsTable)) {
                selected.push_back(record);
            }
        }
        else {
            if (isTransitiveParent(fixedRef, variableRef, statementsTable)) {
                selected.push_back(record);
            }
        }
    }
}

void SuchThatSelection::filterParentStarWithoutTables(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table) {
    for (const auto& record : table) {
        if (ref1 == "_" || isTransitiveParent(ref1, record[0], statementsTable)) {
            if (ref2 == "_" || isTransitiveParent(record[0], ref2, statementsTable)) {
                selected.push_back(record);
            }
        }
    }
}
