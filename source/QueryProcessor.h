#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"
#include "SelectionStructure.h"

using namespace std;

class QueryProcessor
{
public:
    // Default constructor
    QueryProcessor();

    // Destructor
    ~QueryProcessor();

    // Method for evaluating a query
    void evaluate(const string& query, vector<string>& results);

private:
    // Helper method for parsing the query
    Query* parseQuery(const string& query);

    // Helper method for evaluating the parsed query
    void evaluateQuery(const Query& query, vector<string>& output);
};
