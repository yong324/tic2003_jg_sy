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

	PrintExtractor printExtractor;
	vector<int> printIdx = printExtractor.fetchPrintLines(tokens);

	ConstantExtractor constantExtractor;
	vector<string> constants = constantExtractor.fetchConstantNames(tokens);
	
	StatementExtractor statementExtractor;
	vector<int> statementsIdx = statementExtractor.fetchStatementLines(tokens);

	ReadExtractor readExtractor;
	vector<int> readsIdx = readExtractor.fetchReadLines(tokens);

	Database::insertProcedure(procedures);
	Database::insertAssignment(assignmentLineIdx);
	Database::insertVariable(variables);
	Database::insertPrint(printIdx);
	Database::insertConstant(constants);
	Database::insertStatement(statementsIdx);
	Database::insertRead(readsIdx);

}