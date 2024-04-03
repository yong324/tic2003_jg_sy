#include "AssignmentExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

AssignmentExtractor::AssignmentExtractor() {};
AssignmentExtractor::~AssignmentExtractor() {};

vector<int> AssignmentExtractor::fetchAssignmentLines(vector<vector<string>>& tokens) {
	vector<int> assignmentLines;
	vector<string> currline;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		currline = tokens[i];
		if (currline.size() > 1 && currline[1] == "=") assignmentLines.push_back(i);
	}
	return assignmentLines;
}