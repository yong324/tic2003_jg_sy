#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <map>
#include <iostream>

#include "PatternSelection.h"
#include "SuchThatSelection.h"

QueryProcessor::QueryProcessor()
{
};

QueryProcessor::~QueryProcessor()
{
};

// Query Parser
Query* QueryProcessor::parseQuery(const string& query)
{
    // Tokenize the query
    Tokenizer tk;
    vector<string> tokens{};
    tk.tokenize(query, tokens);

    vector<Synonym> synonyms;
    // Find the synonym declaration
    size_t tIndex = 0;
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
    tIndex++;

    if (tokens[tIndex] == "Select")
    {
        tIndex++;

        string selectionVar = tokens[tIndex];

        tIndex++;

        SelectionStructure* selection_structure = nullptr;

        if (tIndex >= tokens.size())
        {
            selection_structure = new SelectionStructure();
        }
        else if (tokens[tIndex] == "such")
        {
            tIndex += 2;
            string synonym = tokens[tIndex];
            tIndex += 2;
            string ref1 = tokens[tIndex];
            tIndex += 2;
            string ref2_type = "stmtRef";
            if (tokens[tIndex] == "\"")
            {
                ref2_type = "entRef";
                tIndex++;
            }
            string ref2 = tokens[tIndex];

            selection_structure = new SuchThatSelection(synonym, "stmtRef", ref1, ref2_type, ref2);
        }
        else if (tokens[tIndex] == "pattern")
        {
            string entRef;
            string express_spec;

            string synonymVar = tokens[tIndex + 1];
            tIndex += 3;

            string entRef_type;
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
                    tIndex += 3;
                    entRef_type = "IDENT";
                }
                else
                {
                    entRef = tokens[tIndex];
                    tIndex += 2;
                    entRef_type = "synonym";
                }
            }

            string spec_type;
            if (tokens[tIndex] == "_")
            {
                express_spec = "_";
                spec_type = "any";
            }
            else
            {
                express_spec = tokens[tIndex + 2];
                spec_type = "factor";
            }

            selection_structure = new PatternSelection(synonymVar, entRef_type, entRef, spec_type, express_spec);
        }

        return new Query{ synonyms, selectionVar, selection_structure };
    }
}

// Query Evaluator
void QueryProcessor::evaluateQuery(const Query& query, vector<string>& output)
{
    // Clear the output vector
    output.clear();
    // evaluate query
    query.evaluate(output);
}

// Evaluate method combining Query Parser and Query Evaluator
void QueryProcessor::evaluate(const string& query, vector<string>& output)
{
    // Parse the query
    Query* query_obj = parseQuery(query);

    // Evaluate the parsed query
    evaluateQuery(*query_obj, output);

    delete query_obj;
}
