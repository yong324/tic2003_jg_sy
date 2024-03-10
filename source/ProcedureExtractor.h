#pragma once

#include "GrammarExtractor.h"
#include <string>
#include <vector>

class ProcedureExtractor : public GrammarExtractor {
	public:	
		ProcedureExtractor();
		~ProcedureExtractor();

		vector<string> fetchProcedureNames(string procName);
};