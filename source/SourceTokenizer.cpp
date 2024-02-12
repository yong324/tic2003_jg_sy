#include "SourceTokenizer.h"
#include <iostream>
#include <sstream>

using namespace std;

// constructor
SourceTokenizer::SourceTokenizer() {}

// destructor
SourceTokenizer::~SourceTokenizer() {}

vector<string> SourceTokenizer::splitString(string& text, char delimiter) {
	vector<string> split_tokens;
	std::istringstream tokenStream(text);
	string token;
	while (std::getline(tokenStream, token, delimiter)) {
		split_tokens.push_back(token);
	}
	return split_tokens;
}

vector<string> SourceTokenizer::splitLines(string& text) {
	vector<string> lines;
	char delim = '\n';
	lines=splitString(text,delim);
	return lines;
}

vector<vector<string>> SourceTokenizer::tokenizeLines(vector<string>& lines) {
	string curr;
	char delim = ' ';
	vector<string> currTokenizedLine;
	vector<string> currCleanedTokenizedLine;
	vector<vector<string>> tokens;
	for (int i = 0; i < lines.size(); i++) {
		curr = lines[i];
		currTokenizedLine = splitString(curr, delim);
		currCleanedTokenizedLine.clear();
		for (string& token : currTokenizedLine) {
			if (token != "") currCleanedTokenizedLine.push_back(token);
		}
		if (currCleanedTokenizedLine.size() == 0) continue;
		tokens.push_back(currCleanedTokenizedLine);
	}
	return tokens;
}