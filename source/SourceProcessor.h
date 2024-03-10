#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "SourceTokenizer.h"
#include "ProcedureExtractor.h"
#include "AssignmentExtractor.h"
#include "VariableExtractor.h"
#include "PrintExtractor.h"
#include "ConstantExtractor.h"
#include "StatementExtractor.h"
#include "ReadExtractor.h"
#include "Token.h"


using namespace std;

class SourceProcessor {
public:
	int lineIdx;
	Token tk;
	// method for processing the source program
	SourceProcessor();
	~SourceProcessor();
	void process(string& program);

private:

	void processProcStatementList(SourceTokenizer& sourceTokenizer, Token& tk);
	void processWhileStatementList(SourceTokenizer& sourceTokenizer, Token& tk);
	void processIfStatementList(SourceTokenizer& sourceTokenizer, Token& tk);
	void processElseStatementList(SourceTokenizer& sourceTokenizer, Token& tk);
	void processAssignStatement(SourceTokenizer& sourceTokenizer, Token& tk, string& lastReferencedVariable);
	void processReadStatement(SourceTokenizer& sourceTokenizer, Token& tk);
	void processPrintStatement(SourceTokenizer& sourceTokenizer, Token& tk);
	void processVariables(SourceTokenizer& sourceTokenizer, string& lastReferencedVariable, string& variable);
	void processConstants(SourceTokenizer& sourceTokenizer, string& constant);
	vector<string> procedures;
	vector<string> reads;
	vector<int> readsIdx;
	vector<int> printIdx;
	vector<int> constants;
	vector<int> assignmentLineIdx;
	vector<string> variables;

};
