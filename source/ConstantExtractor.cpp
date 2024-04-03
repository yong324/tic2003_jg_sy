#include "ConstantExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

ConstantExtractor::ConstantExtractor() {};
ConstantExtractor::~ConstantExtractor() {};

vector<string> ConstantExtractor::fetchConstantNames(vector<vector<string>>& tokens) {

	vector<string> constantNames;
	string varName("");
	for (vector<string>& line : tokens) {
		varName = "";
		if (line.size() > 2 && line[1] == "=") {
			varName = line[2];
			if (varName.back() == ';') varName.pop_back();
			if (checkFactorIsConstant(varName)) constantNames.push_back(varName);
		}
	}
	return constantNames;
}

bool ConstantExtractor::checkFactorIsConstant(string input) {
	if (input.size() == 0) return false;
	for (char& c : input) {
		if (c < '0' || c > '9') return false;
	}
	return true;
}