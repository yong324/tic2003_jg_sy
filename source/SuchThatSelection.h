#pragma once
#include "SelectionStructure.h"
#include <set>

class SuchThatSelection : public SelectionStructure
{
public:
    SuchThatSelection(string synonyms, string ref1_type, string ref1, string ref2_type,
        string ref2);

    void select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym) override;

private:
    string synonyms;
    string ref1_type;
    string ref1;
    string ref2_type;
    string ref2;
    vector<vector<string>> selected;
    const vector<vector<string>>* tableX;
    const vector<vector<string>>* tableY;
    const vector<vector<string>>* table1;
    const vector<vector<string>>* table2;

    //Method declarations for "Modifies"
    void filterDataWithBothTables(const vector<vector<string>>& modifiesTable);
    bool findMatchingModification(const vector<string>& recordX, const vector<string>& recordY, const vector<vector<string>>& modifiesTable);
    void filterDataWithSingleTable(const vector<vector<string>>& modifiesTable, bool isTableX);
    void filterDataWithoutTables(const vector<vector<string>>& modifiesTable, const vector<vector<string>>& table);

    //Method declarations for "Parent"
    void processParent(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);
    void filterParentWithBothTables(const vector<vector<string>>& statementsTable);
    bool checkParentRelation(const vector<string>& recordX, const vector<string>& recordY, const vector<vector<string>>& statementsTable);
    void filterParentWithSingleTable(const vector<vector<string>>& statementsTable, bool isTableX);
    void filterParentWithoutTables(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);

    //method declarations for "Parent*"
    void processParentStar(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);
    void filterParentStarWithBothTables(const vector<vector<string>>& statementsTable);
    void filterParentStarWithSingleTable(const vector<vector<string>>& statementsTable, bool isTableX);
    void filterParentStarWithoutTables(const vector<vector<string>>& statementsTable, const vector<vector<string>>& table);

    // Utility methods for "Parent*"
    bool isTransitiveParent(const string& parent, const string& child, const vector<vector<string>>& statementsTable);
    bool checkTransitiveParent(const string& currentParent, const string& targetChild, const vector<vector<string>>& statementsTable, std::set<string>& visited);
};
