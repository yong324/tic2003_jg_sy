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

void SuchThatSelection::select(map<string, vector<vector<string>>>& tables, const Synonym& selection_synonym)
{
    string var1 = ref1;
    string var2 = ref2;

    vector<vector<string>> selected{};

    const vector<vector<string>>* table1 = nullptr;
    const vector<vector<string>>* table2 = nullptr;
    const vector<vector<string>>* tableX = nullptr;
    const vector<vector<string>>* tableY = nullptr;

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

        if (tableX != nullptr)
        {
            if (tableY != nullptr)
            {
                for (const auto& recordX : *tableX)
                {
                    for (const auto& recordY : *tableY)
                    {
                        bool found = false;
                        for (const auto& modify : modifiesTable)
                        {
                            if (tableX == table1 && modify[1] == recordY[0] && modify[0] == recordX[0] ||
                                tableY == table1 && modify[1] == recordX[0] && modify[0] == recordY[0])
                            {
                                selected.push_back(recordX);
                                found = true;
                                break;
                            }
                        }
                        if (found)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                for (const auto& recordX : *tableX)
                {
                    for (const auto& modify : modifiesTable)
                    {
                        if (tableX == table1 && (ref2 == "_" || modify[1] == ref2) && modify[0] == recordX[0] ||
                            tableX == table2 && modify[1] == recordX[0] && (ref1 == "_" || modify[0] == ref1))
                        {
                            selected.push_back(recordX);
                            break;
                        }
                    }
                }
            }
        }
        else if (tableY != nullptr)
        {
            for (auto& record : table)
            {
                for (const auto& recordY : *tableY)
                {
                    bool found = false;
                    for (const auto& modify : modifiesTable)
                    {
                        if (tableY == table1 && (ref2 == "_" || modify[1] == ref2) && modify[0] == recordY[0] ||
                            tableY == table2 && modify[1] == recordY[0] && (ref1 == "_" || modify[0] == ref1))
                        {
                            selected.push_back(record);
                            found = true;
                            break;
                        }
                    }
                    if (found)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            if (ref1 == "_" && selection_synonym.get_type() == "assign")
            {
                for (auto& record : table)
                {
                    for (const auto& modify : modifiesTable)
                    {
                        if (record[0] != modify[0])
                        {
                            continue;
                        }
                        if (ref2 == "_" || modify[1] == ref2)
                        {
                            selected.push_back(record);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (auto& record : table)
                {
                    for (const auto& modify : modifiesTable)
                    {
                        if ((ref1 == "_" || modify[0] == ref1) && (ref2 == "_" || modify[1] == ref2))
                        {
                            selected.push_back(record);
                            break;
                        }
                    }
                }
            }
        }
    }
    else if (synonyms == "Parent")
    {
        vector<vector<string>> statementsTable;
        Database::getData("statements", statementsTable);
        if (tableX != nullptr)
        {
            if (tableY != nullptr)
            {
                for (const auto& recordX : *tableX)
                {
                    for (const auto& recordY : *tableY)
                    {
                        bool found = false;
                        for (const auto& statement : statementsTable)
                        {
                            if (tableX == table1 && statement[0] == recordY[0] && statement[1] == recordX[0] ||
                                tableY == table1 && statement[0] == recordX[0] && statement[1] == recordY[0])
                            {
                                selected.push_back(recordX);
                                found = true;
                                break;
                            }
                        }
                        if (found)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                for (const auto& recordX : *tableX)
                {
                    for (const auto& statement : statementsTable)
                    {
                        if (tableX == table1 && statement[0] == ref2 && statement[1] == recordX[0] ||
                            tableX == table2 && statement[0] == recordX[0] && statement[1] == ref1)
                        {
                            selected.push_back(recordX);
                            break;
                        }
                    }
                }
            }
        }
        else if (tableY != nullptr)
        {
            for (auto& record : table)
            {
                for (const auto& recordY : *tableY)
                {
                    bool found = false;
                    for (const auto& statement : statementsTable)
                    {
                        if (tableY == table1 && statement[0] == ref2 && statement[1] == recordY[0] ||
                            tableY == table2 && statement[0] == recordY[0] && statement[1] == ref1)
                        {
                            selected.push_back(record);
                            found = true;
                            break;
                        }
                    }
                    if (found)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            for (auto& record : table)
            {
                for (const auto& statement : statementsTable)
                {
                    if (statement[0] == ref2 && statement[1] == ref1)
                    {
                        selected.push_back(record);
                        break;
                    }
                }
            }
        }
    }
    else if (synonyms == "Parent*")
    {
        vector<vector<string>> statementsTable;
        Database::getData("statements", statementsTable);
        if (tableX != nullptr)
        {
            if (tableY != nullptr)
            {
                for (const auto& recordX : *tableX)
                {
                    for (const auto& recordY : *tableY)
                    {
                        bool _found = false;
                        string child = tableX == table2 ? recordX[0] : recordY[0];
                        bool end = false;
                        while (!end)
                        {
                            bool found = false;
                            for (const auto& statement : statementsTable)
                            {
                                if (statement[0] == child)
                                {
                                    found = true;
                                    if (statement[1] == (tableX == table1 ? recordX[0] : recordY[0]))
                                    {
                                        selected.push_back(recordX);
                                        _found = true;
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

                        if (_found)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                for (const auto& recordX : *tableX)
                {
                    string child = tableX == table2 ? recordX[0] : ref2;
                    bool end = false;
                    while (!end)
                    {
                        bool found = false;
                        for (const auto& statement : statementsTable)
                        {
                            if (statement[0] == child)
                            {
                                found = true;
                                if (statement[1] == (tableX == table1 ? recordX[0] : ref1))
                                {
                                    selected.push_back(recordX);
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
        }
        else if (tableY != nullptr)
        {
            for (auto& record : table)
            {
                for (const auto& recordY : *tableY)
                {
                    bool _found = false;
                    string child = tableY == table2 ? recordY[0] : ref2;
                    bool end = false;
                    while (!end)
                    {
                        bool found = false;
                        for (const auto& statement : statementsTable)
                        {
                            if (statement[0] == child)
                            {
                                found = true;
                                if (statement[1] == (tableY == table1 ? recordY[0] : ref1))
                                {
                                    selected.push_back(record);
                                    _found = true;
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
                    if (_found)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
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
                                selected.push_back(record);
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
    }
    tables.insert_or_assign(selection_synonym.get_var_name(), selected);
}
