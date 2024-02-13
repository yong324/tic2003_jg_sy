#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class ConstantExtractor : public GrammarExtractor {
public:
	ConstantExtractor();
	~ConstantExtractor();

	vector<int> fetchConstantNames(vector<vector<string>>& tokens);
};