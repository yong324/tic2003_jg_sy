#pragma once
#include <vector>
#include "Database.h"
#include "SourceTokenizer.h"
#include "Token.h"

class ProcedureExtractor {
public:
	string procedureName;
	Token tk;
	SourceTokenizer tokenizer;
	// method for processing the source program
	ProcedureExtractor(SourceTokenizer& tokenizer);
	~ProcedureExtractor();
	void processProcedure(string& program);

private:
	void processStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processProcStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processWhileStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processIfStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processElseStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processAssignStatement(SourceTokenizer& sourceTokenizer, Token& tk, string& lastReferencedVariable, int parent);
	void processReadStatement(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processPrintStatement(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
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
