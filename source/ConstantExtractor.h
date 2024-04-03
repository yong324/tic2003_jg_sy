#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class ConstantExtractor : public GrammarExtractor {
public:
	ConstantExtractor();
	~ConstantExtractor();

	vector<string> fetchConstantNames(vector<vector<string>>& tokens);

private:
	bool checkFactorIsConstant(string input);
};