#pragma once

#include <string>
#include <vector>
#include "Database.h"
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
    void evaluateQuery(const Query& query, vector<string>& output);
    void parseSynonyms(vector<string>& tokens, vector<Synonym>& synonyms, size_t& tIndex);
    void parseSelectionStructures(vector<string>& tokens, vector<SelectionStructure*>& selection_structures, size_t& tIndex);
    SuchThatSelection* parseSuchThat(vector<string>& tokens, size_t& tIndex);
    PatternSelection* parsePattern(vector<string>& tokens, size_t& tIndex);
};
