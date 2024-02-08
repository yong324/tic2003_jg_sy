#include "SourceProcessor.h"

#include <iostream>
#include <stack>
#include <string>

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.
void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the programs are valid.
	string procedureName = tokens.at(1);

	// insert the procedure into the database
	Database::insertProcedure(procedureName);

	stack<pair<string, int>> conditions;
	bool newProcedure = false;


	for (int currToken = 3; currToken < tokens.size(); currToken++) {
		if (tokens.at(currToken) == "procedure") //procedure p;
		{
			currToken++; //get p with tokens.at(currToken) after increment;
		}
		if (tokens.at(currToken) == "select") //Select p;
		{
			currToken++; //get p with tokens.at(currToken) after increment;
			//output select statement of "select * from procedure"
		}
	}
}