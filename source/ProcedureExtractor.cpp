#include "ProcedureExtractor.h"
#include <iostream>
#include <sstream>

using namespace std;

ProcedureExtractor::ProcedureExtractor() {};
ProcedureExtractor::~ProcedureExtractor() {};

vector<string> ProcedureExtractor::fetchProcedureNames(string procName) {

	vector<string> procedureNames{procName};
	return procedureNames;
}