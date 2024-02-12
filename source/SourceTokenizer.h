#pragma once

#include "Tokenizer.h"
#include <string>
#include <vector>

class SourceTokenizer : public Tokenizer {

    public:
        SourceTokenizer();
        ~SourceTokenizer();

        vector<string> splitString(string& text, char delimiter);
        vector<string> splitLines(string& text);
        vector<vector<string>> tokenizeLines(vector<string>& tokens);

};
