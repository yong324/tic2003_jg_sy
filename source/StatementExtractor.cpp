#include "StatementExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

StatementExtractor::StatementExtractor() {};
StatementExtractor::~StatementExtractor() {};

vector<int> StatementExtractor::fetchStatementLines(vector<vector<string>>& tokens) {
	vector<int> statementLines;
	vector<string> currline;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		currline = tokens[i];
		if (currline[0] == "read") statementLines.push_back(i);
		if (currline[0] == "print") statementLines.push_back(i);
		if (currline.size() > 1 && currline[1] == "=") statementLines.push_back(i);
	}
	return statementLines;
}