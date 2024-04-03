#pragma once

#include "Query.h"
#include "SuchThatSelection.h"
#include "PatternSelection.h"

using namespace std;

class QueryProcessor
{
public:
    // Default constructor
    QueryProcessor();

    // Destructor
    ~QueryProcessor();

    // Method for evaluating a query
    void evaluate(const string& query, vector<string>& results);

    

private:
    // Helper method for parsing the query
    Query* parseQuery(const string& query);

    // Helper method for evaluating the parsed query
    static void evaluateQuery(const Query& query, vector<string>& output);
    static void parseSynonyms(vector<string>& tokens, map<string, string>& synonyms, size_t& tIndex);
    void parseSelectionStructures(vector<string>& tokens, list<SelectionStructure*>& selection_structures, size_t& tIndex);
    static SuchThatSelection* parseSuchThat(vector<string>& tokens, size_t& tIndex);
    PatternSelection* parsePattern(vector<string>& tokens, size_t& tIndex);
};
