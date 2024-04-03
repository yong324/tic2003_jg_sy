#include "PrintExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

PrintExtractor::PrintExtractor() {};
PrintExtractor::~PrintExtractor() {};

vector<int> PrintExtractor::fetchPrintLines(vector<vector<string>>& tokens) {
	vector<int> printLines;
	vector<string> currline;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		currline = tokens[i];
		if (currline[0] == "print") printLines.push_back(i);
	}
	return printLines;
}