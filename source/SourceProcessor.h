#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "SourceTokenizer.h"
#include "ProcedureExtractor.h"
#include "AssignmentExtractor.h"
#include "VariableExtractor.h"
#include "PrintExtractor.h"
#include "ConstantExtractor.h"
#include "StatementExtractor.h"
#include "ReadExtractor.h"

using namespace std;

class SourceProcessor {
public:
	// method for processing the source program
	SourceProcessor();
	~SourceProcessor();

	void process(string program);
};
