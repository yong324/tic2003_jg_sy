#include "SourceProcessor.h"

#include <iostream>
#include <stack>
#include <string>

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.

// constructor
SourceProcessor::SourceProcessor() {}

// destructor
SourceProcessor::~SourceProcessor() {}

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	SourceTokenizer sourceTokenizer;

	vector<string> lines = sourceTokenizer.splitLines(program);
	vector<vector<string>> tokens = sourceTokenizer.tokenizeLines(lines);

	ProcedureExtractor procedureExtractor;
	vector<string> procedures = procedureExtractor.fetchProcedureNames(tokens);

	AssignmentExtractor assignmentExtractor;
	vector<int> assignmentLineIdx = assignmentExtractor.fetchAssignmentLines(tokens);

	VariableExtractor variableExtractor;
	vector<string> variables = variableExtractor.fetchVariableNames(tokens);

	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the programs are valid.
	// string procedureName = tokens.at(1);

	// insert the procedure into the database
	// Database::insertProcedure(procedureName);

	stack<pair<string, int>> conditions;
	bool newProcedure = false;


	/*for (int currToken = 3; currToken < tokens.size(); currToken++) {
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
	*/
}