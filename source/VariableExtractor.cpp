#include "VariableExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

VariableExtractor::VariableExtractor() {};
VariableExtractor::~VariableExtractor() {};

vector<string> VariableExtractor::fetchVariableNames(vector<vector<string>>& tokens) {

	vector<string> variableNames;
	string varName;
	for (vector<string>& line : tokens) {
		varName = "";
		if (line[0] == "read") varName =  line[1];
		if (line.size() > 1 && line[1] == "=") varName = line[0];
		if (varName.size() >= 1 && varName.back() == ';') varName.pop_back();
		if (varName.size() >= 1) variableNames.push_back(varName);
	}
	return variableNames;
}