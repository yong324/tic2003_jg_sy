#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class AssignmentExtractor : public GrammarExtractor {
public:
	AssignmentExtractor();
	~AssignmentExtractor();

	vector<int> fetchAssignmentLines(vector<vector<string>>& tokens);
};