#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <map>
#include <iostream>

QueryProcessor::QueryProcessor() {};
QueryProcessor::~QueryProcessor() {};

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
void QueryProcessor::evaluateSelectQuery(const string& varName, const string& synonymType, vector<string>& output) {
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

bool QueryProcessor::findSuchThat(const std::string& query, std::string& extractedQuery)
{
    // Find the position of "such that"
    size_t pos = query.find("such that");
    if (pos != std::string::npos) {
        // Extract the substring after "such that"
        extractedQuery = query.substr(pos + 10); // "such that" has 10 characters
        return true;
    }
    return false;
}

void QueryProcessor::parseSuchThatQuery(const std::string& suchThatQuery, std::string& functionName, std::string& paraLeft, std::string& paraRight) {
    // Find the position of "(" and ")"
    size_t start_pos = suchThatQuery.find("(");
    size_t end_pos = suchThatQuery.find(")");

    if (start_pos != std::string::npos && end_pos != std::string::npos && end_pos > start_pos) {
        // Extract the function name
        functionName = suchThatQuery.substr(0, start_pos);
        
        // Extract the substring between "(" and ")"
        std::string parameters = suchThatQuery.substr(start_pos + 1, end_pos - start_pos - 1);

        // Find the position of ","
        size_t comma_pos = parameters.find(",");

        // Extract the parameters before and after ","
        paraLeft = parameters.substr(0, comma_pos);
        paraRight = parameters.substr(comma_pos + 1);

        // Remove quotation marks from parameters
        paraLeft.erase(std::remove(paraLeft.begin(), paraLeft.end(), '\"'), paraLeft.end());
        paraRight.erase(std::remove(paraRight.begin(), paraRight.end(), '\"'), paraRight.end());

        // Trim leading and trailing whitespaces from parameters
        paraLeft.erase(0, paraLeft.find_first_not_of(" \t"));
        paraLeft.erase(paraLeft.find_last_not_of(" \t") + 1);
        paraRight.erase(0, paraRight.find_first_not_of(" \t"));
        paraRight.erase(paraRight.find_last_not_of(" \t") + 1);
    }
    else {
        // Handle the case where "(" or ")" is not found or they're in the wrong order
        std::cerr << "Invalid format: " << suchThatQuery << std::endl;
        paraLeft = "";
        paraRight = "";
    }
}

bool QueryProcessor::evaluateSuchThatQuery(std::string& suchThatQuery)
{
    string functionName, paraLeft, paraRight;
    vector<string> databaseResults;

    parseSuchThatQuery(suchThatQuery, functionName, paraLeft, paraRight);

    if (functionName == "Modifies") {
        Database::getModifies(paraLeft, paraRight, databaseResults);
    }
    
    // Return true if vector is not empty, false otherwise
    return !databaseResults.empty();
}


// Evaluate method combining Query Parser and Query Evaluator
void QueryProcessor::evaluate(const string& query, vector<string>& output) {
    // Parse the query
    string varName, synonymType, suchThatQuery;
    parseQuery(query, varName, synonymType);

    if (findSuchThat(query, suchThatQuery)){
        if (evaluateSuchThatQuery(suchThatQuery))
        {
            // Evaluate the parsed query
            evaluateSelectQuery(varName, synonymType, output);
        }
    }
    else
    {
        // Evaluate the parsed query
        evaluateSelectQuery(varName, synonymType, output);
    }


}