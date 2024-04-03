#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "SourceTokenizer.h"
#include "Token.h"

using namespace std;

class SourceProcessor {
public:
	int lineIdx;
	string currProcName;
	Token tk;
	// method for processing the source program
	SourceProcessor();
	~SourceProcessor();
	void process(string& program);

private:
	void processStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processProcStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processWhileStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processIfStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processElseStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent);
	void processCallStatement(SourceTokenizer& sourceTokenizer, Token& tk);
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
