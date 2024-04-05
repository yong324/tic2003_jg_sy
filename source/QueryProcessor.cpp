#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <iostream>

#include "PatternSelection.h"
#include "SuchThatSelection.h"

// QueryProcessor class is responsible for parsing and evaluating queries.
QueryProcessor::QueryProcessor() = default;

QueryProcessor::~QueryProcessor() = default;

// Parses the given query string and constructs a Query object.
// It tokenizes the query, identifies relRef, selection variables, and conditions.
Query* QueryProcessor::parseQuery(const string & query)
{
    // Tokenize the query
    Tokenizer tk;
    vector<string> tokens{};
    tk.tokenize(query, tokens); // Tokenizing the query string

    map<string, string> synonyms;
    // Find the synonym declaration
    size_t tIndex = 0;
    // Parsing relRef and their types
    parseSynonyms(tokens, synonyms, tIndex);

    tIndex += 1;
    // Parsing the selection variables
    vector<string> selectionVars{};
    if (tokens[tIndex] == "<")
    {
        tIndex++;
        do
        {
            selectionVars.push_back(tokens[tIndex++]);
        } while (tokens[tIndex++] != ">");
    }
    else
    {
        selectionVars.push_back(tokens[tIndex++]);
    }

    list<SelectionStructure*> selection_structures{};

    if (tIndex < tokens.size())
    {
        // Parsing additional structures like Such That and Pattern
        parseSelectionStructures(tokens, selection_structures, tIndex);
    }

    return new Query{ synonyms, selectionVars, selection_structures };
}

// Evaluates the given Query object to produce the output results.
void QueryProcessor::evaluateQuery(const Query & query, vector<string>&output)
{
    // Clear the output vector
    output.clear();
    // evaluate query
    query.evaluate(output);
}

// Combines parsing and evaluation of a query to produce the output results.
void QueryProcessor::evaluate(const string & query, vector<string>&output)
{
    // Parse the query
    const Query* query_obj = parseQuery(query);

    // Evaluate the parsed query
    evaluateQuery(*query_obj, output);

    delete query_obj;
}

void QueryProcessor::parseSynonyms(vector<string>&tokens, map<string, string>&synonyms, size_t & tIndex)
{
    // example: procedure c1; variable v; assign a; if if1, if2;
    do
    {
        string type = tokens[tIndex++];
        string var = tokens[tIndex++];

        while (tokens[tIndex] == ",")
        {
            synonyms.insert_or_assign(var, type);
            tIndex++;

            var = tokens[tIndex++];

            if (tokens[tIndex] == ";")
                break;
        }

        synonyms.insert_or_assign(var, type);
        tIndex++;
    } while (tokens[tIndex] != "Select");
}

void QueryProcessor::parseSelectionStructures(vector<string>&tokens, list<SelectionStructure*>&selection_structures,
    size_t & tIndex)
{
    while (tIndex < tokens.size())
    {
        if (tokens[tIndex] == "such")
        {
            selection_structures.push_back(parseSuchThat(tokens, tIndex));
        }
        else if (tokens[tIndex] == "pattern")
        {
            selection_structures.push_front(parsePattern(tokens, tIndex));
        }
        else
        {
            ++tIndex; // Skip unknown or irrelevant tokens
        }
    }
}

SuchThatSelection* QueryProcessor::parseSuchThat(vector<string>&tokens, size_t & tIndex)
{
    tIndex += 2; // Assuming "such that" are two separate tokens and we move past "that"
    int relRef;
    string ref1;
    string ref2;
    int ref1_type;
    int ref2_type;

    if (tokens[tIndex] == "Modifies")
    {
        relRef = MODIFIES;
    }
    else if (tokens[tIndex] == "Uses")
    {
        relRef = USES;
    }
    else if (tokens[tIndex] == "Parent")
    {
        if (tokens[tIndex + 1] == "*")
        {
            relRef = PARENT_STAR;
            tIndex++;
        }
        else
        {
            relRef = PARENT;
        }
    }
    else if (tokens[tIndex] == "Calls")
    {
        if (tokens[tIndex + 1] == "*")
        {
            relRef = CALLS_STAR;
            tIndex++;
        }
        else
        {
            relRef = CALLS;
        }
    }

    tIndex += 2;
    if (tokens[tIndex] == "\"")
    {
        ref1_type = IDENT;
        ref1 = tokens[tIndex + 1];
        tIndex += 4;
    }
    else
    {
        ref1 = tokens[tIndex];
        if (ref1 == "_")
            ref1_type = ANY;
        else if (isdigit(ref1[0]))
            ref1_type = INTEGER;
        else
            ref1_type = SYNONYM;
        tIndex += 2;
    }

    if (tokens[tIndex] == "\"")
    {
        ref2_type = IDENT;
        ref2 = tokens[tIndex + 1];
        tIndex += 4;
    }
    else
    {
        ref2 = tokens[tIndex];
        if (ref2 == "_")
            ref2_type = ANY;
        else if (isdigit(ref2[0]))
            ref2_type = INTEGER;
        else
            ref2_type = SYNONYM;
        tIndex += 2;
    }

    return new SuchThatSelection(relRef, ref1_type, ref1, ref2_type, ref2);
}

PatternSelection* QueryProcessor::parsePattern(vector<string>&tokens, size_t & tIndex)
{
    string entRef;
    int entRef_type;
    string express_spec;
    int spec_type;
    string syn_assign = tokens[tIndex + 1];
    tIndex += 3; // Assuming we move past a bracket or similar

    if (tokens[tIndex] == "_")
    {
        entRef_type = ANY;
        entRef = "_";
        tIndex += 2;
    }
    else
    {
        if (tokens[tIndex] == "\"")
        {
            entRef = tokens[tIndex + 1];
            tIndex += 4;
            entRef_type = IDENT;
        }
        else
        {
            entRef = tokens[tIndex];
            tIndex += 2;
            entRef_type = SYNONYM;
        }
    }

    if (tokens[tIndex] == "\"")
    {
        while (tokens[++tIndex] != "\"")
        {
            express_spec += tokens[tIndex];
        }
        spec_type = EXACT;
        tIndex += 3;
    }
    else if (tokens[tIndex] == "_" && tokens[tIndex + 1] == "\"")
    {
        tIndex++;
        while (tokens[++tIndex] != "\"")
        {
            express_spec += tokens[tIndex];
        }
        spec_type = PARTIAL;
        tIndex += 3;
    }
    else
    {
        express_spec = "_";
        spec_type = ANY;
        tIndex += 2;
    }

    return new PatternSelection(syn_assign, entRef_type, entRef, spec_type, express_spec);
}
