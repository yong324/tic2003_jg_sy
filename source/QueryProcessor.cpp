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

    // Parsing the selection variable
    tIndex += 2;
    string selectionVar = tokens[tIndex];
    tIndex++;

    vector<SelectionStructure*> selection_structures{};

    // Parsing additional structures like SuchThat and Pattern
    if (tIndex >= tokens.size())
    {
        selection_structures.push_back(new SelectionStructure());
    }
    else
    {
        while (tIndex < tokens.size())
        {
            if (tokens[tIndex] == "such")
            {
                tIndex += 2;
                string synonym = tokens[tIndex];
                if (synonym == "Parent" && tokens[tIndex + 1] == "*")
                {
                    synonym = "Parent*";
                    tIndex++;
                }
                tIndex += 2;
                string ref1 = tokens[tIndex];
                tIndex += 2;
                string ref2_type = "stmtRef";
                string ref2;
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

                selection_structures.push_back(new SuchThatSelection(synonym, "stmtRef", ref1, ref2_type, ref2));
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

                string spec_type;
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

                selection_structures.push_back(
                    new PatternSelection(synonymVar, entRef_type, entRef, spec_type, express_spec));
            }
            else
            {
                break;
            }
        }
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
