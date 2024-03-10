#include "SourceProcessor.h"

#include <iostream>
#include <stack>
#include <string>

SourceProcessor::SourceProcessor() { lineIdx = 1;}

SourceProcessor::~SourceProcessor() {}

void SourceProcessor::process(string& program) {
	Database::initialize();
	ProcedureExtractor procedureExtractor;

	string lastReferredVariable="";

	SourceTokenizer sourceTokenizer(program);
	Token tk = sourceTokenizer.getNextToken();

	while (tk.type != ENDOFPROGRAM) {
		if (tk.type == PROCEDURE) {
			processProcStatementList(sourceTokenizer, tk, 0);
		} 
		tk = sourceTokenizer.getNextToken();
	}
	
	return;
}

void SourceProcessor::processStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {

	string variableName = "";
	string lastReferencedVariable = "";

	while (tk.type != CLOSEBRACKET) {
		if (tk.type == READ) {
			Database::insertStatement(lineIdx, parent, "READ");
			processReadStatement(sourceTokenizer, tk, parent);
		}
		else if (tk.type == PRINT) {
			Database::insertStatement(lineIdx, parent, "PRINT");
			processPrintStatement(sourceTokenizer, tk, parent);
		}
		else if (tk.type == ASSIGN) {
			Database::insertStatement(lineIdx, parent, "ASSIGN");
			processAssignStatement(sourceTokenizer, tk, lastReferencedVariable, parent);
		}
		else if (tk.type == VARIABLE) {
			processVariables(sourceTokenizer, lastReferencedVariable, tk.value);
		}
		else if (tk.type == WHILE) {
			Database::insertStatement(lineIdx, parent, "WHILE");
			processWhileStatementList(sourceTokenizer, tk, lineIdx);
		}
		else if (tk.type == IF) {
			int forIdx = lineIdx;
			Database::insertStatement(lineIdx, parent, "IF");
			processIfStatementList(sourceTokenizer, tk, forIdx);
			while (tk.type != ELSE) tk = sourceTokenizer.getNextToken();
			Database::insertStatement(lineIdx, forIdx, "ELSE");
			processElseStatementList(sourceTokenizer, tk, forIdx);
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

void SourceProcessor::processProcStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {
	tk = sourceTokenizer.getNextToken();
	string procedureName = tk.value;
	Database::insertProcedure(procedureName);

	processStatementList(sourceTokenizer, tk, parent);
	return;
}

void SourceProcessor::processWhileStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {
	tk = sourceTokenizer.getNextToken();
	tk = sourceTokenizer.getNextToken(); // once more to skip the opening left paranthesis
	string relExpr = "";
	
	while (tk.type != RIGHTPARANTHESIS) { 
		if (tk.type == CONSTANT) processConstants(sourceTokenizer, tk.value);
		relExpr.append(tk.value); 
		tk = sourceTokenizer.getNextToken();
	};

	Database::insertWhile(lineIdx, relExpr);

	lineIdx++;

	processStatementList(sourceTokenizer, tk, parent);

	return;
}

void SourceProcessor::processIfStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {
	tk = sourceTokenizer.getNextToken();
	tk = sourceTokenizer.getNextToken(); // once more to skip the opening left paranthesis
	string relExpr = "";

	while (tk.type != RIGHTPARANTHESIS) { 
		if (tk.type == CONSTANT) processConstants(sourceTokenizer, tk.value);
		relExpr.append(tk.value); 
		tk = sourceTokenizer.getNextToken(); 
	};

	Database::insertIf(lineIdx, relExpr);

	lineIdx++;
	processStatementList(sourceTokenizer, tk, parent);

	return;
}

void SourceProcessor::processElseStatementList(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {
	tk = sourceTokenizer.getNextToken();
	lineIdx++;
	processStatementList(sourceTokenizer, tk, parent);

	return;
}

void SourceProcessor::processAssignStatement(SourceTokenizer& sourceTokenizer, Token& tk, string& lastReferencedVariable, int parent) {
	string lhs_expression = lastReferencedVariable;
	string rhs_expression = "";
	tk = sourceTokenizer.getNextToken();

	while (tk.type != ENDOFLINE) {
		if (tk.type == CONSTANT) processConstants(sourceTokenizer, tk.value);
		rhs_expression.append(tk.value);
		tk = sourceTokenizer.getNextToken();
	}

	Database::insertAssignment(lineIdx, lhs_expression, rhs_expression);
	Database::insertModify(lineIdx, lhs_expression, "ASSIGN");
	return;
}

void SourceProcessor::processVariables(SourceTokenizer& sourceTokenizer, string& lastReferencedVariable, string& variable) {
	lastReferencedVariable = variable;
	Database::insertVariable(variable);
	return;
}

void SourceProcessor::processConstants(SourceTokenizer& sourceTokenizer, string& constant) {
	Database::insertConstant(constant);
	return;
}

void SourceProcessor::processReadStatement(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {
	tk = sourceTokenizer.getNextToken();
	Database::insertRead(lineIdx,tk.value);
	Database::insertModify(lineIdx, tk.value, "READ");
	return;
}

void SourceProcessor::processPrintStatement(SourceTokenizer& sourceTokenizer, Token& tk, int parent) {
	tk = sourceTokenizer.getNextToken();
	Database::insertPrint(lineIdx, tk.value);
	return;
}
