#include "ProcedureExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

ProcedureExtractor::ProcedureExtractor() {};
ProcedureExtractor::~ProcedureExtractor() {};

vector<string> ProcedureExtractor::fetchProcedureNames(vector<vector<string>>& tokens) {

	vector<string> procedureNames;
	for (vector<string>& line : tokens) {
		if (line[0] == "procedure") procedureNames.push_back(line[1]);
	}
	return procedureNames;
}