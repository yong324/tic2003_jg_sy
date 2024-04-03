#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class VariableExtractor : public GrammarExtractor {
public:
	VariableExtractor();
	~VariableExtractor();

	vector<string> fetchVariableNames(vector<vector<string>>& tokens);
};