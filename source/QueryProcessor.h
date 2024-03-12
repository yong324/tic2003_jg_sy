#pragma once

#include <string>
#include <vector>
#include "Database.h"

using namespace std;

class QueryProcessor {
public:
    // Default constructor
    QueryProcessor();

    // Destructor
    ~QueryProcessor();

    // Method for evaluating a query
    void evaluate(const string& query, vector<string>& results);

private:
    // Helper method for parsing the query
    void parseQuery(const string& query, string& varName, string& synonymType);

    // Helper method for evaluating the parsed query
    void evaluateSelectQuery(const string& varName, const string& synonymType, vector<string>& output);

    bool findSuchThat(const std::string& query, std::string& extractedQuery);
    void parseSuchThatQuery(const std::string& str, std::string& functionName, std::string& paraLeft, std::string& paraRight);
    bool evaluateSuchThatQuery(std::string& suchThatQuery);
};