#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class PrintExtractor : public GrammarExtractor {
public:
	PrintExtractor();
	~PrintExtractor();

	vector<int> fetchPrintLines(vector<vector<string>>& tokens);
};