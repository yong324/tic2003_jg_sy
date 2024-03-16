#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <map>
#include <iostream>
#include "PatternSelection.h"
#include "SuchThatSelection.h"

// QueryProcessor class is responsible for parsing and evaluating queries.
QueryProcessor::QueryProcessor()
{
};

QueryProcessor::~QueryProcessor()
{
};

// Parses the given query string and constructs a Query object.
// It tokenizes the query, identifies synonyms, selection variables, and conditions.
Query* QueryProcessor::parseQuery(const string& query)
{
    // Tokenize the query
    Tokenizer tk;
    vector<string> tokens{};
    tk.tokenize(query, tokens); // Tokenizing the query string

    vector<Synonym> synonyms;
    // Find the synonym declaration
    size_t tIndex = 0;
    // Parsing synonyms and their types
    parseSynonyms(tokens, synonyms, tIndex);

    // Parsing the selection variable
    tIndex += 2;
    string selectionVar = tokens[tIndex];
    tIndex++;

    vector<SelectionStructure*> selection_structures{};


    if (tIndex >= tokens.size())
    {
        selection_structures.push_back(new SelectionStructure());
    }
    else
    {
        // Parsing additional structures like Such That and Pattern
        parseSelectionStructures(tokens, selection_structures, tIndex);
    }

    return new Query{ synonyms, selectionVar, selection_structures };
}

// Evaluates the given Query object to produce the output results.
void QueryProcessor::evaluateQuery(const Query& query, vector<string>& output)
{
    // Clear the output vector
    output.clear();
    // evaluate query
    query.evaluate(output);
}

// Combines parsing and evaluation of a query to produce the output results.
void QueryProcessor::evaluate(const string& query, vector<string>& output)
{
    // Parse the query
    Query* query_obj = parseQuery(query);

    // Evaluate the parsed query
    evaluateQuery(*query_obj, output);

    delete query_obj;
}

void QueryProcessor::parseSynonyms(vector<string>& tokens, vector<Synonym>& synonyms, size_t& tIndex) {
    for (; tIndex < tokens.size(); ++tIndex)
    {
        const string& token = tokens[tIndex];
        if (token == ";")
        {
            string varName = tokens[tIndex - 1];
            string synonymType = tokens[tIndex - 2];

            synonyms.emplace_back(varName, synonymType);
            if (tokens[tIndex + 1] == "Select")
            {
                break;
            }
        }
    }
}

void QueryProcessor::parseSelectionStructures(vector<string>& tokens, vector<SelectionStructure*>& selection_structures, size_t& tIndex) {
    while (tIndex < tokens.size()) {
        if (tokens[tIndex] == "such") {
            selection_structures.push_back(parseSuchThat(tokens, tIndex));
        }
        else if (tokens[tIndex] == "pattern") {
            selection_structures.push_back(parsePattern(tokens, tIndex));
        }
        else {
            ++tIndex;  // Skip unknown or irrelevant tokens
        }
    }
}

SuchThatSelection* QueryProcessor::parseSuchThat(vector<string>& tokens, size_t& tIndex) {
    tIndex += 2; // Assuming "such that" are two separate tokens and we move past "that"
    string synonym = tokens[tIndex];
    string ref1 = "", ref2 = "", ref2_type = "stmtRef";

    if (synonym == "Parent" && tokens[tIndex + 1] == "*")
    {
        synonym = "Parent*";
        tIndex++;
    }
    tIndex += 2;
    ref1 = tokens[tIndex];
    tIndex += 2;

    if (tIndex >= tokens.size()) {
        return new SuchThatSelection(synonym, "stmtRef", ref1, ref2_type, ref2);
    }
    
    if (tokens[tIndex] == "\"")
    {
        ref2_type = "entRef";
        ref2 = tokens[tIndex + 1];
        tIndex += 4;
    }
    else
    {
        ref2 = tokens[tIndex];
        tIndex += 2;
    }

    return new SuchThatSelection(synonym, "stmtRef", ref1, ref2_type, ref2);
}

PatternSelection* QueryProcessor::parsePattern(vector<string>& tokens, size_t& tIndex) {
    string synonymVar = tokens[tIndex + 1]; // Move to the synonym name
    string entRef = "", entRef_type ="", express_spec ="", spec_type = "";
    tIndex += 3;; // Assuming we move past a bracket or similar

    if (tokens[tIndex] == "_")
    {
        entRef_type = "any";
        entRef = "_";
        tIndex += 2;
    }
    else
    {
        if (tokens[tIndex] == "\"")
        {
            entRef = tokens[tIndex + 1];
            tIndex += 4;
            entRef_type = "IDENT";
        }
        else
        {
            entRef = tokens[tIndex];
            tIndex += 2;
            entRef_type = "synonym";
        }
    }

    if (tokens[tIndex] == "\"")
    {
        while (tokens[++tIndex] != "\"")
        {
            express_spec += tokens[tIndex];
        }
        spec_type = "exact";
        tIndex += 3;
    }
    else if (tokens[tIndex] == "_" && tokens[tIndex + 1] == "\"")
    {
        express_spec = tokens[tIndex + 2];
        spec_type = "factor";
        tIndex += 6;
    }
    else
    {
        express_spec = "_";
        spec_type = "any";
        tIndex += 2;
    }

    return new PatternSelection(synonymVar, entRef_type, entRef, spec_type, express_spec);
}