#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <map>
#include <iostream>

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

// method to evaluate a query
// This method currently only handles queries for getting all the procedure names,
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all required queries.
void QueryProcessor::evaluate(string query, vector<string>& output) {
	// clear the output vector
	output.clear();

	// tokenize the query
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(query, tokens);

	// check what type of synonym is being declared
	map<string, string> synonymMap; //will store (p, procedure)
	string varName;
	string synonymType;
	size_t tIndex = 0;

	for (; tIndex < tokens.size(); ++tIndex) {
		const string& token = tokens[tIndex];
		if (token == ";") {
			varName = tokens[tIndex - 1];
			synonymType = tokens[tIndex - 2];
			synonymMap[varName] = synonymType;
			break;
		}
	}

	// create a vector for storing the results from database
	vector<string> databaseResults;

	// call the method in database to retrieve the results
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the queries are valid.
	if (tokens[tIndex + 1] == "Select") 	// check for select statement
	{
		synonymType = synonymMap[tokens[tIndex + 2]];
		map<std::string, std::string> tableNameMap = {
		{"procedure", "procedures"},
		{"variable", "variables"},
		{"constant", "constants"},
		{"assign", "assignments"},
		{"print", "prints"},
		{"read", "reads"},
		{"stmt", "statements"}
		};


		if (!synonymType.empty()) {
			auto tableName = tableNameMap.find(synonymType);
			if (tableName != tableNameMap.end()) {
				Database::getData(tableName->second, databaseResults);
			}
			else {
				throw std::invalid_argument("Unknown Synonym Type: " + synonymType);
			}
		}
	}


	// post process the results to fill in the output vector
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}
}