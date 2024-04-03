#pragma once

#include "Tokenizer.h"
#include "Token.h"
#include <string>
#include <vector>
#include <regex>

class SourceTokenizer : public Tokenizer {

    public:
        
        SourceTokenizer(string& text);
        ~SourceTokenizer();
        Token getNextToken();

    private:
        vector<string> tokens;
        size_t position;
        string sourceCode;
        Token getTokenType(string& lexeme);

};
