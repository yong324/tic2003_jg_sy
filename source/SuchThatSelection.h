#pragma once
#include "SelectionStructure.h"
#include <map>
#include <set>

class SuchThatSelection : public SelectionStructure
{
    string synonyms;
    string ref1_type;
    string ref1;
    string ref2_type;
    string ref2;

public:
    SuchThatSelection(string synonyms, string ref1_type, string ref1, string ref2_type,
        string ref2);

    void select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym) override;

private:
    vector<vector<string>> selected{};

    const vector<vector<string>>* table1;
    const vector<vector<string>>* table2;
    const vector<vector<string>>* tableX;
    const vector<vector<string>>* tableY;

    // Method declarations for "Modifies"
    void processModifies(const vector<vector<string>>& modifiesTable, const vector<vector<string>>& table, const Synonym& selection_synonym);
    void filterDataWithSingleTable(const vector<vector<string>>& modifiesTable, const vector<vector<string>>& table, bool isTableX);
    void filterDataWithBothTables(const vector<vector<string>>& modifiesTable);
    bool findMatchingModification(const vector<string>& recordX, const vector<string>& recordY, const vector<vector<string>>& modifiesTable);
    void filterDataWithoutTables(const vector<vector<string>>& modifiesTable, const vector<vector<string>>& table, const Synonym& selection_synonym);

    // Method declarations for "Parent"
    void processParent(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);
    void filterParentWithBothTables(const vector<vector<string>>& statementsTable);
    bool checkParentRelation(const vector<string>& recordX, const vector<string>& recordY, const vector<vector<string>>& statementsTable);
    void filterParentWithSingleTable(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table, bool isTableX);
    void filterParentWithoutTables(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);

    // Method declarations for "Parent*"
    void processParentStar(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);
    void filterParentStarWithBothTables(const vector<vector<string>>& statementsTable);
    void filterParentStarWithSingleTable(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table, bool isTableX);
    void filterParentStarWithoutTables(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);
    bool isTransitiveParent(const string& parent, const string& child, const vector<vector<string>>& statementsTable);
    bool checkTransitiveParent(const string& currentParent, const string& targetChild, const vector<vector<string>>& statementsTable, std::set<string>& visited);
};
