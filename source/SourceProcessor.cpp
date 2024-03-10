#include "SourceProcessor.h"

#include <iostream>
#include <stack>
#include <string>

SourceProcessor::SourceProcessor() { lineIdx = 1;}

SourceProcessor::~SourceProcessor() {}

void SourceProcessor::process(string& program) {
	// initialize the database
	Database::initialize();
	ProcedureExtractor procedureExtractor;

	string lastReferredVariable="";

	// tokenize the program
	SourceTokenizer sourceTokenizer(program);
	Token tk = sourceTokenizer.getNextToken();
	

	while (tk.type != ENDOFPROGRAM) {
		if (tk.type == PROCEDURE) {
			tk = sourceTokenizer.getNextToken();
			string procName = tk.value;
			vector<string> procedures = procedureExtractor.fetchProcedureNames(procName);
			processProcStatementList(sourceTokenizer, tk);
		} 
		tk = sourceTokenizer.getNextToken();
	}
	
	return;
	
	/*
	vector<string> procedures = procedureExtractor.fetchProcedureNames(tokens);

	AssignmentExtractor assignmentExtractor;
	vector<int> assignmentLineIdx = assignmentExtractor.fetchAssignmentLines(tokens);

	VariableExtractor variableExtractor;
	vector<string> variables = variableExtractor.fetchVariableNames(tokens);

	PrintExtractor printExtractor;
	vector<int> printIdx = printExtractor.fetchPrintLines(tokens);

	ConstantExtractor constantExtractor;
	vector<string> constants = constantExtractor.fetchConstantNames(tokens);
	
	StatementExtractor statementExtractor;
	vector<int> statementsIdx = statementExtractor.fetchStatementLines(tokens);

	ReadExtractor readExtractor;
	vector<int> readsIdx = readExtractor.fetchReadLines(tokens);

	Database::insertProcedure(procedures);
	Database::insertAssignment(assignmentLineIdx);
	Database::insertVariable(variables);
	Database::insertPrint(printIdx);
	Database::insertConstant(constants);
	Database::insertStatement(statementsIdx);
	Database::insertRead(readsIdx);
	*/

}


void SourceProcessor::processProcStatementList(SourceTokenizer& sourceTokenizer, Token& tk) {
	tk = sourceTokenizer.getNextToken();
	string variableName = "";
	string lastReferencedVariable = "";

	while (tk.type != CLOSEBRACKET) {
		if (tk.type == READ) {
			processReadStatement(sourceTokenizer, tk);
		}
		else if (tk.type == PRINT) {
			processPrintStatement(sourceTokenizer, tk);
		}
		else if (tk.type == ASSIGN) {
			processAssignStatement(sourceTokenizer, tk, lastReferencedVariable);
		}
		else if (tk.type == VARIABLE) {
			processVariables(sourceTokenizer, lastReferencedVariable, tk.value);
		}
		else if (tk.type == WHILE) {
			processWhileStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == IF) {
			processIfStatementList(sourceTokenizer, tk);
			while (tk.type != ELSE) tk = sourceTokenizer.getNextToken();
			processElseStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == CONSTANT) {
			processConstants(sourceTokenizer, tk.value);
		}
		
		if (tk.type == ENDOFLINE) {
			lineIdx++;
		}
		tk = sourceTokenizer.getNextToken();
	}

	return;
}

void SourceProcessor::processWhileStatementList(SourceTokenizer& sourceTokenizer, Token& tk) {
	tk = sourceTokenizer.getNextToken();
	tk = sourceTokenizer.getNextToken(); // once more to skip the opening left paranthesis
	string relExpr = "";
	
	while (tk.type != RIGHTPARANTHESIS) { relExpr.append(tk.value); tk = sourceTokenizer.getNextToken();};
	lineIdx++;

	string variableName = "";
	string lastReferencedVariable = "";
	
	while (tk.type != CLOSEBRACKET) {
		if (tk.type == READ) {
			processReadStatement(sourceTokenizer, tk);
		}
		else if (tk.type == PRINT) {
			processPrintStatement(sourceTokenizer, tk);
		}
		else if (tk.type == ASSIGN) {
			processAssignStatement(sourceTokenizer, tk, lastReferencedVariable);
		}
		else if (tk.type == VARIABLE) {
			processVariables(sourceTokenizer, lastReferencedVariable, tk.value);
		}
		else if (tk.type == WHILE) {
			processWhileStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == IF) {
			processIfStatementList(sourceTokenizer, tk);
			while (tk.type != ELSE) tk = sourceTokenizer.getNextToken();
			processElseStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == CONSTANT) {
			processConstants(sourceTokenizer, tk.value);
		}

		if (tk.type == ENDOFLINE) {
			lineIdx++;
		}
		tk = sourceTokenizer.getNextToken();
	}

	return;
}

void SourceProcessor::processIfStatementList(SourceTokenizer& sourceTokenizer, Token& tk) {
	tk = sourceTokenizer.getNextToken();
	string relExpr = "";

	while (tk.type != RIGHTPARANTHESIS) { relExpr.append(tk.value); tk = sourceTokenizer.getNextToken(); };
	lineIdx++;

	string variableName = "";
	string lastReferencedVariable = "";

	while (tk.type != CLOSEBRACKET) {
		if (tk.type == READ) {
			processReadStatement(sourceTokenizer, tk);
		}
		else if (tk.type == PRINT) {
			processPrintStatement(sourceTokenizer, tk);
		}
		else if (tk.type == ASSIGN) {
			processAssignStatement(sourceTokenizer, tk, lastReferencedVariable);
		}
		else if (tk.type == VARIABLE) {
			processVariables(sourceTokenizer, lastReferencedVariable, tk.value);
		}
		else if (tk.type == WHILE) {
			processWhileStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == IF) {
			processIfStatementList(sourceTokenizer, tk);
			while (tk.type != ELSE) tk = sourceTokenizer.getNextToken();
			processElseStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == CONSTANT) {
			processConstants(sourceTokenizer, tk.value);
		}

		if (tk.type == ENDOFLINE) {
			lineIdx++;
		}
		tk = sourceTokenizer.getNextToken();
	}

	return;
}

void SourceProcessor::processElseStatementList(SourceTokenizer& sourceTokenizer, Token& tk) {
	tk = sourceTokenizer.getNextToken();
	lineIdx++;

	string variableName = "";
	string lastReferencedVariable = "";

	while (tk.type != CLOSEBRACKET) {
		if (tk.type == READ) {
			processReadStatement(sourceTokenizer, tk);
		}
		else if (tk.type == PRINT) {
			processPrintStatement(sourceTokenizer, tk);
		}
		else if (tk.type == ASSIGN) {
			processAssignStatement(sourceTokenizer, tk, lastReferencedVariable);
		}
		else if (tk.type == VARIABLE) {
			processVariables(sourceTokenizer, lastReferencedVariable, tk.value);
		}
		else if (tk.type == WHILE) {
			processWhileStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == IF) {
			processIfStatementList(sourceTokenizer, tk);
			while (tk.type != ELSE) tk = sourceTokenizer.getNextToken();
			processElseStatementList(sourceTokenizer, tk);
		}
		else if (tk.type == CONSTANT) {
			processConstants(sourceTokenizer, tk.value);
		}

		if (tk.type == ENDOFLINE) {
			lineIdx++;
		}
		tk = sourceTokenizer.getNextToken();
	}

	return;
}



void SourceProcessor::processAssignStatement(SourceTokenizer& sourceTokenizer, Token& tk, string& lastReferencedVariable) {
	string lhs_expression = lastReferencedVariable;
	string rhs_expression = "";
	tk = sourceTokenizer.getNextToken();

	while (tk.type != ENDOFLINE) {
		rhs_expression.append(tk.value);
		tk = sourceTokenizer.getNextToken();
	}

	return;
}

void SourceProcessor::processVariables(SourceTokenizer& sourceTokenizer, string& lastReferencedVariable, string& variable) {
	lastReferencedVariable = variable;
	variables.push_back(variable);
	return;
}


void SourceProcessor::processConstants(SourceTokenizer& sourceTokenizer, string& constant) {
	constants.push_back(stoi(constant));
	return;
}

void SourceProcessor::processReadStatement(SourceTokenizer& sourceTokenizer, Token& tk) {
	tk = sourceTokenizer.getNextToken();
	variables.push_back(tk.value);
	return;
}

void SourceProcessor::processPrintStatement(SourceTokenizer& sourceTokenizer, Token& tk) {
	printIdx.push_back(lineIdx);
	tk = sourceTokenizer.getNextToken();
	if (tk.type == CONSTANT) constants.push_back(stoi(tk.value));
	return;
}
