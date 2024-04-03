#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class StatementExtractor : public GrammarExtractor {
public:
	StatementExtractor();
	~StatementExtractor();

	vector<int> fetchStatementLines(vector<vector<string>>& tokens);
};