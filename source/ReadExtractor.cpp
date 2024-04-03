#include "ReadExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

ReadExtractor::ReadExtractor() {};
ReadExtractor::~ReadExtractor() {};

vector<int> ReadExtractor::fetchReadLines(vector<vector<string>>& tokens) {
	vector<int> readLines;
	vector<string> currline;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		currline = tokens[i];
		if (currline[0] == "read") readLines.push_back(i);
	}
	return readLines;
}