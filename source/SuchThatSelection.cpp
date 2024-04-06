#include "SuchThatSelection.h"

#include <algorithm>
#include <iterator>
#include <queue>
#include <set>

#include "Database.h"
#include "Query.h"

map<string, vector<vector<string>>> SuchThatSelection::cachedTables{};

// Constructor for SuchThatSelection initializes the object with the details of the "such that" clause.
SuchThatSelection::SuchThatSelection(int relRef, int ref1_type, string ref1, int ref2_type, string ref2) :
    relRef(relRef),
    ref1_type(ref1_type),
    ref1(std::move(ref1)),
    ref2_type(ref2_type),
    ref2(std::move(ref2))
{
}

// The select method applies the "such that" condition to filter data in the tables map.
// This method specifically evaluates the "such that" relation and retains rows in the table that meet the condition.
void SuchThatSelection::select(vector<map<string, vector<string>>>& cartesian_table,
    const map<string, string>& synonyms)
{
    vector<map<string, vector<string>>> new_cartesian_table{};

    switch (relRef)
    {
    case MODIFIES:
    {
        copy_if(
            cartesian_table.begin(),
            cartesian_table.end(),
            back_inserter(new_cartesian_table),
            [&](const map<string, vector<string>>& record) -> bool
            {
                string modifier;
                string modified;

                switch (ref2_type)
                {
                case IDENT:
                    modified = ref2;
                    break;
                case SYNONYM:
                    modified = record.at(ref2)[0];
                }

                switch (ref1_type)
                {
                case INTEGER:
                    modifier = ref1;
                    if (ifCallIndexGetProcedure(modifier))
                        return doesProcedureModify(modifier, modified);
                    break;
                case IDENT:
                    return doesProcedureModify(ref1, modified);
                case SYNONYM:
                    modifier = record.at(ref1)[0];
                    if (synonyms.at(ref1) == "procedure")
                        return doesProcedureModify(modifier, modified);
                    if (synonyms.at(ref1) == "call")
                        return doesProcedureModify(record.at(ref1)[2], modified);
                    break;
                }

                return doesModify(modifier, modified);
            }
        );
        break;
    }
    case PARENT:
    {
        copy_if(
            cartesian_table.begin(),
            cartesian_table.end(),
            back_inserter(new_cartesian_table),
            [&](const map<string, vector<string>>& record) -> bool
            {
                string parent;
                string child;

                switch (ref2_type)
                {
                case INTEGER:
                    child = ref2;
                    break;
                case SYNONYM:
                    child = record.at(ref2)[0];
                    break;
                }

                switch (ref1_type)
                {
                case ANY:
                    return all_of(
                        synonyms.begin(),
                        synonyms.end(),
                        [&](const pair<string, string>& pair)
                        {
                            if (ref2_type == SYNONYM && pair.first == ref2)
                                return getParent(record.at(ref2)[0]) != "0";
                            return isParent(record.at(pair.first)[0], child);
                        }
                    );
                case INTEGER:
                    parent = ref1;
                    break;
                case SYNONYM:
                    parent = record.at(ref1)[0];
                    break;
                }

                return isParent(parent, child);
            }
        );

        break;
    }
    case PARENT_STAR:
    {
        copy_if(
            cartesian_table.begin(),
            cartesian_table.end(),
            back_inserter(new_cartesian_table),
            [&](const map<string, vector<string>>& record) -> bool
            {
                string parent;
                string child;

                switch (ref2_type)
                {
                case INTEGER:
                    child = ref2;
                    break;
                case SYNONYM:
                    child = record.at(ref2)[0];
                    break;
                }

                switch (ref1_type)
                {
                case ANY:
                    return all_of(
                        synonyms.begin(),
                        synonyms.end(),
                        [&](const pair<string, string>& pair)
                        {
                            if (ref2_type == SYNONYM && pair.first == ref2)
                                return getParent(record.at(ref2)[0]) != "0";
                            return isParentStar(record.at(pair.first)[0], child);
                        }
                    );
                case INTEGER:
                    parent = ref1;
                    break;
                case SYNONYM:
                    parent = record.at(ref1)[0];
                    break;
                }

                return isParentStar(parent, child);
            }
        );
        break;
    }
    case USES:
    {
        copy_if(
            cartesian_table.begin(),
            cartesian_table.end(),
            back_inserter(new_cartesian_table),
            [&](const map<string, vector<string>>& record) -> bool
            {
                string user;
                string used;

                switch (ref2_type)
                {
                case IDENT:
                    used = ref2;
                    break;
                case SYNONYM:
                    used = record.at(ref2)[0];
                }

                switch (ref1_type)
                {
                case INTEGER:
                    user = ref1;
                    if (ifCallIndexGetProcedure(user))
                        return doesProcedureUse(user, used);
                    break;
                case IDENT:
                    return doesProcedureUse(ref1, used);
                case SYNONYM:
                    user = record.at(ref1)[0];
                    if (synonyms.at(ref1) == "procedure")
                        return doesProcedureUse(user, used);
                    if (synonyms.at(ref1) == "call")
                        return doesProcedureUse(record.at(ref1)[2], used);
                    break;
                }

                return doesUse(user, used);
            }
        );

        break;
    }
    case CALLS:
    {
        copy_if(
            cartesian_table.begin(),
            cartesian_table.end(),
            back_inserter(new_cartesian_table),
            [&](const map<string, vector<string>>& record) -> bool
            {
                string caller;
                string callee;

                switch (ref2_type)
                {
                case IDENT:
                    callee = ref2;
                    break;
                case SYNONYM:
                    callee = record.at(ref2)[0];
                    break;
                }

                switch (ref1_type)
                {
                case ANY:
                    return all_of(
                        synonyms.begin(),
                        synonyms.end(),
                        [&](const pair<string, string>& pair)
                        {
                            if (ref2_type == SYNONYM && pair.first == ref2)
                                return true;
                            return doesCall(record.at(pair.first)[0], callee);
                        }
                    );
                case IDENT:
                    caller = ref1;
                    break;
                case SYNONYM:
                    caller = record.at(ref1)[0];
                    break;
                }

                return doesCall(caller, callee);
            }
        );

        break;
    }
    case CALLS_STAR:
    {
        copy_if(
            cartesian_table.begin(),
            cartesian_table.end(),
            back_inserter(new_cartesian_table),
            [&](const map<string, vector<string>>& record) -> bool
            {
                string caller;
                string callee;

                switch (ref2_type)
                {
                case IDENT:
                    callee = ref2;
                    break;
                case SYNONYM:
                    callee = record.at(ref2)[0];
                    break;
                }

                switch (ref1_type)
                {
                case ANY:
                    return all_of(
                        synonyms.begin(),
                        synonyms.end(),
                        [&](const pair<string, string>& pair)
                        {
                            if (ref2_type == SYNONYM && pair.first == ref2)
                                return true;
                            return doesCallStar(record.at(pair.first)[0], callee);
                        }
                    );
                case IDENT:
                    caller = ref1;
                    break;
                case SYNONYM:
                    caller = record.at(ref1)[0];
                    break;
                }

                return doesCallStar(caller, callee);
            }
        );

        break;
    }
    }

    cartesian_table.clear();
    cartesian_table = new_cartesian_table;
}

bool SuchThatSelection::doesModify(const string& modifier, const string& modified)
{
    const auto& modifiesTable = requestTable("modifies");
    return any_of(
        modifiesTable.begin(),
        modifiesTable.end(),
        [&](const vector<string>& modifies)
        {
            return (modified.empty() || modifies[1] == modified) &&
                (modifier.empty() || modifies[0] == modifier ||
                    isParentStar(modifier, modifies[0]));
        }
    );
}

bool SuchThatSelection::doesModifyInCalls(const string& procedure, const string& modified)
{
    const auto& modifiesTable = requestTable("modifies");
    return any_of(
        modifiesTable.begin(),
        modifiesTable.end(),
        [&](const vector<string>& modifies)
        {
            return (modified.empty() || modifies[1] == modified) &&
                doesCallStar(procedure, getProcedure(modifies[0]));
        }
    );
}

bool SuchThatSelection::doesProcedureModify(const string& procedure, const string& modified)
{
    auto statementsTable = requestTable("statements");
    return any_of(
        statementsTable.begin(),
        statementsTable.end(),
        [&](const vector<string>& statements)
        {
            return statements[2] == procedure && doesModify(statements[0], modified);
        }
    ) || doesModifyInCalls(procedure, modified);
}

bool SuchThatSelection::isParentParent(const string& parent)
{
    const auto& statementTable = requestTable("statements");
    return any_of(
        statementTable.begin(),
        statementTable.end(),
        [&](const vector<string>& statements)
        {
            return statements[1] == parent;
        }
    );
}

bool SuchThatSelection::isParent(const string& parent, const string& child)
{
    const auto& statementsTable = requestTable("statements");

    return isParentParent(parent) && any_of(
        statementsTable.begin(),
        statementsTable.end(),
        [&](const vector<string>& statements)
        {
            return (child.empty() || statements[0] == child) && (parent.empty() || statements[1] == parent);
        }
    );
}

bool SuchThatSelection::isParentStar(const string& parent, const string& child)
{
    if (isParent(parent, child))
        return true;

    if (child.empty() || parent.empty())
        return false;

    string child_parent = child;
    do
    {
        child_parent = getParent(child_parent);
        if (child_parent == parent)
            return true;
    } while (!child_parent.empty());

    return false;
}

string SuchThatSelection::getParent(const string& child)
{
    const auto& statementsTable = requestTable("statements");

    const auto it = find_if(
        statementsTable.begin(),
        statementsTable.end(),
        [&](const vector<string>& statements)
        {
            return statements[0] == child;
        }
    );

    return it != statementsTable.end() ? it->at(1) : "";
}

vector<string> SuchThatSelection::getChildren(const string& parent)
{
    const auto& statementsTable = requestTable("statements");

    vector<string> children{};

    for (const auto& statements : statementsTable)
    {
        if (statements[1] == parent)
        {
            children.push_back(statements[0]);
        }
    }

    return children;
}

bool SuchThatSelection::doesUse(const string& user, const string& used)
{
    const auto& usesTable = requestTable("uses");
    return any_of(
        usesTable.begin(),
        usesTable.end(),
        [&](const vector<string>& uses)
        {
            return (used.empty() || uses[1] == used) &&
                (user.empty() || uses[0] == user ||
                    isParentStar(user, uses[0]));
        }
    );
}

bool SuchThatSelection::doesUseInCalls(const string& procedure, const string& used)
{
    const auto& usesTable = requestTable("uses");
    return any_of(
        usesTable.begin(),
        usesTable.end(),
        [&](const vector<string>& uses)
        {
            return (used.empty() || uses[1] == used) && doesCallStar(procedure, getProcedure(uses[0]));
        }
    );
}

bool SuchThatSelection::doesProcedureUse(const string& procedure, const string& used)
{
    auto statementsTable = requestTable("statements");
    return any_of(
        statementsTable.begin(),
        statementsTable.end(),
        [&](const vector<string>& statements)
        {
            return statements[2] == procedure && doesUse(statements[0], used);
        }
    ) || doesUseInCalls(procedure, used);
}

bool SuchThatSelection::isCallerCaller(const string& caller)
{
    const auto& callsTable = requestTable("calls");
    return any_of(
        callsTable.begin(),
        callsTable.end(),
        [&](const vector<string>& calls)
        {
            return calls[1] == caller;
        }
    );
}

bool SuchThatSelection::doesCall(const string& caller, const string& callee)
{
    const auto& callsTable = requestTable("calls");

    return isCallerCaller(caller) && any_of(
        callsTable.begin(),
        callsTable.end(),
        [&](const vector<string>& calls)
        {
            return (callee.empty() || calls[2] == callee) && (caller.empty() || calls[1] == caller);
        }
    );
}

bool SuchThatSelection::doesCallStar(const string& caller, const string& callee)
{
    if (doesCall(caller, callee))
        return true;

    if (callee.empty() || caller.empty())
        return false;

    set<string> visited{};
    queue<string> calleeQueue{ {callee} };
    while (!calleeQueue.empty())
    {
        string callee_ = calleeQueue.front();
        calleeQueue.pop();

        if (visited.find(callee_) != visited.end())
            continue;

        visited.emplace(callee_);

        vector<string> callers = getCallers(callee_);
        for (const auto& caller_ : callers)
        {
            if (caller_ == caller)
                return true;

            calleeQueue.push(caller_);
        }
    }

    return false;
}

bool SuchThatSelection::ifCallIndexGetProcedure(string& lineIndex)
{
    const auto& callsTable = requestTable("calls");
    const auto it = find_if(
        callsTable.begin(),
        callsTable.end(),
        [&](const vector<string>& calls)
        {
            return calls[0] == lineIndex;
        }
    );

    if (it == callsTable.end())
        return false;

    lineIndex = it->at(2);

    return true;
}


vector<string> SuchThatSelection::getCallers(const string& callee)
{
    const auto& callsTable = requestTable("calls");
    vector<string> callers{};

    for (const auto& calls : callsTable)
    {
        if (calls[2] == callee)
            callers.push_back(calls[1]);
    }

    return callers;
}

vector<string> SuchThatSelection::getCallees(const string& caller)
{
    const auto& callsTable = requestTable("calls");
    vector<string> callees{};

    for (const auto& calls : callsTable)
    {
        if (calls[1] == caller)
            callees.push_back(calls[2]);
    }

    return callees;
}

string SuchThatSelection::getProcedure(const string& statement)
{
    const auto& statementsTable = requestTable("statements");
    return find_if(
        statementsTable.begin(),
        statementsTable.end(),
        [&](const vector<string>& statements)
        {
            return statements[0] == statement;
        }
    )->at(2);
}

const vector<vector<string>>& SuchThatSelection::requestTable(const string& synonym)
{
    if (cachedTables.find(synonym) == cachedTables.end())
    {
        cachedTables.insert_or_assign(synonym, vector<vector<string>>{});
        Database::getData(synonym, cachedTables.at(synonym));
    }

    return cachedTables.at(synonym);
}
