#include "ConstantExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

ConstantExtractor::ConstantExtractor() {};
ConstantExtractor::~ConstantExtractor() {};

vector<int> ConstantExtractor::fetchConstantNames(vector<vector<string>>& tokens) {

	vector<int> constantNames;
	for (vector<string>& line : tokens) {
		if (line[0] == "read") constantNames.push_back(0);
		if (line.size() > 1 && line[1] == "=") constantNames.push_back(0);
	}
	return constantNames;
}