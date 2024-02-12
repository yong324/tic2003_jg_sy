#include "VariableExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

VariableExtractor::VariableExtractor() {};
VariableExtractor::~VariableExtractor() {};

vector<string> VariableExtractor::fetchVariableNames(vector<vector<string>>& tokens) {

	vector<string> variableNames;
	for (vector<string>& line : tokens) {
		if (line[0] == "read") variableNames.push_back(line[1]);
		if (line.size() > 1 && line[1] == "=") variableNames.push_back(line[0]);
	}
	return variableNames;
}