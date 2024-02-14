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

	for (size_t i = 0; i < tokens.size(); ++i) {
		const string& token = tokens[i];
		if (token == ";") {
			varName = tokens[i - 1];
			synonymType = tokens[i - 2];
			synonymMap[synonymType] = varName;
			break;
		}
	}

	// create a vector for storing the results from database
	vector<string> databaseResults;

	// call the method in database to retrieve the results
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the queries are valid.
	
	if (!synonymType.empty()) {
		string tableName = "";

		if (synonymType == "procedure"){
			tableName = "procedures";
		}
		if (synonymType == "variable") {
			tableName = "variables";
		}
		if (synonymType == "constant") {
			tableName = "constants";
		}
		if (synonymType == "assign") {
			tableName = "assignments";
		}
		if (synonymType == "print") {
			tableName = "prints";
		}
		if (synonymType == "read") {
			tableName = "reads";
		}
		if (synonymType == "stmt") {
			tableName = "statements";
		}
		Database::getData(tableName,databaseResults);
	}

	// post process the results to fill in the output vector
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}
}