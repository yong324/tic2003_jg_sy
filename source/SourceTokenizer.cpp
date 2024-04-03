#include "SourceTokenizer.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// constructor
SourceTokenizer::SourceTokenizer(string& text) {
	sourceCode = text;
	position = 0;
}

// destructor
SourceTokenizer::~SourceTokenizer() {}

Token SourceTokenizer::getNextToken() {
	if (sourceCode.size() == 0) return { ENDOFPROGRAM,"" };

	regex lexemeRegex(R"((read|print|if|else|while|call|[+\-*/();=\}><]|\(([^)]*)\)|\d+|[a-zA-Z][a-zA-Z0-9]*))");
	smatch match;
	size_t pos = 0;
	string lexeme = "";
	
	regex_search(sourceCode, match, lexemeRegex);
	lexeme = match[0].str();
	Token tk = SourceTokenizer::getTokenType(lexeme);

	pos = match.position() + match.length();
	sourceCode = match.suffix().str();

	return tk;
}


Token SourceTokenizer::getTokenType(string& lexeme) {
	if (lexeme == "read") return{ READ,"" };
	if (lexeme == "procedure") return{ PROCEDURE,"" };
	if (lexeme == "print") return{ PRINT,"" };
	if (lexeme == "=") return{ ASSIGN,"=" };
	if (lexeme == "}") return{ CLOSEBRACKET,"}" };
	if (lexeme == "(") return{ LEFTPARANTHESIS,"(" };
	if (lexeme == ")") return{ RIGHTPARANTHESIS,")" };
	if (lexeme == "while") return{ WHILE,"" };
	if (lexeme == "if") return{ IF,"" };
	if (lexeme == "else") return{ ELSE,"" };
	if (lexeme == "call") return{ CALL,"" };
	if (lexeme == ";") return{ ENDOFLINE,";" };
	if (lexeme == "+") return{ PLUS,"+" };
	if (lexeme == "-") return{ MINUS,"-" };
	if (lexeme == "*") return{ MULTIPLY,"*" };
	if (lexeme == "/") return{ DIVIDE,"/" };
	if (lexeme == ">") return{ DIVIDE,">" };
	if (lexeme == "<") return{ DIVIDE,"<" };
	if (regex_match(lexeme, regex(R"(\(([^)]*)\))"))) return{ RELEXPR, lexeme };
	if (regex_match(lexeme, regex(R"(\d+)"))) return{ CONSTANT,lexeme };
	return {VARIABLE,lexeme};
}