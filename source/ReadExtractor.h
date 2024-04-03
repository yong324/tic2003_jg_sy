#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class ReadExtractor : public GrammarExtractor {
public:
	ReadExtractor();
	~ReadExtractor();

	vector<int> fetchReadLines(vector<vector<string>>& tokens);
};