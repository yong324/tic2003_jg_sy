#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <map>
#include <iostream>

// Query Parser
void QueryProcessor::parseQuery(const string& query, string& varName, string& synonymType) {
    // Tokenize the query
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

    // Find the synonym declaration
    size_t tIndex = 0;
    for (; tIndex < tokens.size(); ++tIndex) {
        const string& token = tokens[tIndex];
        if (token == ";") {
            varName = tokens[tIndex - 1];
            synonymType = tokens[tIndex - 2];
            break;
        }
    }
}

// Query Evaluator
void QueryProcessor::evaluateQuery(const string& varName, const string& synonymType, vector<string>& output) {
    // Clear the output vector
    output.clear();

    // Map synonym type to database table
    map<string, string> tableNameMap = {
        {"procedure", "procedures"},
        {"variable", "variables"},
        {"constant", "constants"},
        {"assign", "assignments"},
        {"print", "prints"},
        {"read", "reads"},
        {"stmt", "statements"}
    };

    // Get the corresponding table name
    string tableName;
    auto tableNameIter = tableNameMap.find(synonymType);
    if (tableNameIter != tableNameMap.end()) {
        tableName = tableNameIter->second;
    }
    else {
        throw std::invalid_argument("Unknown Synonym Type: " + synonymType);
    }

    // Retrieve data from the database
    vector<string> databaseResults;
    Database::getData(tableName, databaseResults);

    // Post-process results
    for (const string& databaseResult : databaseResults) {
        output.push_back(databaseResult);
    }
}

// Evaluate method combining Query Parser and Query Evaluator
void QueryProcessor::evaluate(const string& query, vector<string>& output) {
    // Parse the query
    string varName, synonymType;
    parseQuery(query, varName, synonymType);

    // Evaluate the parsed query
    evaluateQuery(varName, synonymType, output);
}