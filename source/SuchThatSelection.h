#pragma once
#include "SelectionStructure.h"
#include <map>

class SuchThatSelection : public SelectionStructure
{
    int relRef;
    int ref1_type;
    string ref1;
    int ref2_type;
    string ref2;

public:
    SuchThatSelection(int relRef, int ref1_type, string ref1, int ref2_type,
                      string ref2);

    void select(vector<map<string, vector<string>>>& cartesian_table, const map<string, string>& synonyms) override;

private:
    static map<string, vector<vector<string>>> cachedTables;
    static const vector<vector<string>>& requestTable(const string& synonym);

    static bool doesModify(const string& modifier, const string& modified);
    static bool doesModifyInCalls(const string& procedure, const string& modified);
    static bool doesProcedureModify(const string& procedure, const string& modified);
    static bool isParentParent(const string& parent);
    static bool isParent(const string& parent, const string& child);
    static bool isParentStar(const string& parent, const string& child);
    static bool doesUse(const string& user, const string& used);
    static bool doesUseInCalls(const string& procedure, const string& used);
    static bool doesProcedureUse(const string& procedure, const string& used);
    static bool isCallerCaller(const string& caller);
    static bool ifCallIndexGetProcedure(string& lineIndex);
    static bool doesCall(const string& caller, const string& callee);
    static bool doesCallStar(const string& caller, const string& callee);
    

    static vector<string> getChildren(const string& parent);
    static string getParent(const string& child);
    static vector<string> getCallers(const string& callee);
    static vector<string> getCallees(const string& caller);
    static string getProcedure(const string &statement);
};
