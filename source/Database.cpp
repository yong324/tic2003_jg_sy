#include "Database.h"

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize()
{
    // open a database connection and store the pointer into dbConnection
    sqlite3_open("database.db", &dbConnection);
    vector<string> dropTableSQL = {
        "DROP TABLE IF EXISTS procedures",
        "DROP TABLE IF EXISTS variables",
        "DROP TABLE IF EXISTS constants",
        "DROP TABLE IF EXISTS assignments",
        "DROP TABLE IF EXISTS prints",
        "DROP TABLE IF EXISTS reads",
        "DROP TABLE IF EXISTS statements",
        "DROP TABLE IF EXISTS parents",
        "DROP TABLE IF EXISTS ifs",
        "DROP TABLE IF EXISTS whiles",
        "DROP TABLE IF EXISTS modifies",
        "DROP TABLE IF EXISTS uses",
        "DROP TABLE IF EXISTS calls"
    };

    for (string& dropQuery : dropTableSQL)
        sqlite3_exec(dbConnection, dropQuery.c_str(), nullptr, nullptr,
            &errorMessage);

    vector<string> createTableSQL = {
        "CREATE TABLE procedures ( procedureName VARCHAR(255) PRIMARY KEY);",
        "CREATE TABLE variables ( variableName VARCHAR(255) PRIMARY KEY);",
        "CREATE TABLE constants ( constantNum INTEGER PRIMARY KEY);",
        "CREATE TABLE assignments ( assignmentIndex INTEGER PRIMARY KEY, variableName VARCHAR(255), assignmentExpression VARCHAR(255));",
        "CREATE TABLE prints ( printIndex INTEGER PRIMARY KEY, printVariable VARCHAR(255));",
        "CREATE TABLE reads ( readIndex INTEGER PRIMARY KEY, readVariable VARCHAR(255));",
        "CREATE TABLE statements ( statementIndex INTEGER PRIMARY KEY, parent INTEGER, procedure VARCHAR(255), statementType VARCHAR(255));",
        "CREATE TABLE whiles (whileIndex  INTEGER PRIMARY KEY, relExpression VARCHAR(255));",
        "CREATE TABLE ifs (ifIndex  INTEGER PRIMARY KEY, relExpression VARCHAR(255));",
        "CREATE TABLE modifies (modifyIndex INTEGER PRIMARY KEY, variableModified VARCHAR(255), modifyType VARCHAR(30));",
        "CREATE TABLE uses (usesIdx INTEGER, variableUsed VARCHAR(255), usesType VARCHAR(30), PRIMARY KEY (usesIdx, variableUsed));",
        "CREATE TABLE calls (callIndex INTEGER PRIMARY KEY, callerProcedure VARCHAR(255), calledProcedure VARCHAR(30));"
    };

    for (string& createQuery : createTableSQL)
        sqlite3_exec(dbConnection, createQuery.c_str(), nullptr, nullptr,
            &errorMessage);

    // initialize the result vector
    dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close()
{
    sqlite3_close(dbConnection);
}

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName)
{
    string insertSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

// method to insert a procedure into the database
void Database::insertVariable(string variableName)
{
    string insertSQL = "INSERT INTO variables ('variableName') VALUES ('" + variableName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertConstant(string constant)
{
    string insertSQL = "INSERT INTO constants ('constantNum') VALUES (" + constant + ");";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

// method to insert an assignment index into the database
void Database::insertAssignment(int assignmentLineIdx, string variable, string assignmentExpression)
{
    string insertSQL = "INSERT INTO assignments ('assignmentIndex','variableName','assignmentExpression') VALUES (" +
        to_string(assignmentLineIdx) + ",'" + variable + "','" + assignmentExpression + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}


void Database::insertPrint(int printIdx, string printVariable)
{
    string insertSQL = "INSERT INTO prints ('printIndex','printVariable') VALUES (" + to_string(printIdx) + ", '" +
        printVariable + "'); ";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertRead(int readsIdx, string readVariable)
{
    string insertSQL = "INSERT INTO reads ('readIndex','readVariable') VALUES (" + to_string(readsIdx) + ",'" +
        readVariable + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertCall(int callIdx, string callerProcedure, string calledProcedure)
{
    string insertSQL = "INSERT INTO calls ('callIndex','callerProcedure','calledProcedure') VALUES (" + to_string(callIdx) + ",'" +
        callerProcedure + "','" + calledProcedure + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertStatement(string currProcName, int statementsIdx, int parent, string statementType)
{
    string insertSQL = "INSERT INTO statements ('statementIndex','parent','procedure','statementType') VALUES (" + to_string(statementsIdx) + "," +
        to_string(parent) + ",'" + currProcName + "','" + statementType + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertWhile(int whileIdx, string relExpression)
{
    string insertSQL = "INSERT INTO whiles ('whileIndex','relExpression') VALUES (" + to_string(whileIdx) + ",'" +
        relExpression + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertIf(int ifIdx, string relExpression)
{
    string insertSQL = "INSERT INTO ifs ('ifIndex','relExpression') VALUES (" + to_string(ifIdx) + ",'" + relExpression
        + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertModify(int modifyIdx, string variableModified, string modifyType)
{
    string insertSQL = "INSERT INTO modifies ('modifyIndex','variableModified','modifyType') VALUES (" +
        to_string(modifyIdx) + ",'" + variableModified + "','" + modifyType + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}

void Database::insertUses(int usesIdx, string variableUsed, string usesType)
{
    string insertSQL = "INSERT INTO uses ('usesIdx','variableUsed','usesType') VALUES (" +
        to_string(usesIdx) + ",'" + variableUsed + "','" + usesType + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), nullptr, nullptr, &errorMessage);
}


//Extract the whole data from the table
void Database::getData(const string& tableName, vector<vector<string>>& results)
{
    // Clear existing results
    dbResults.clear();

    // Construct the SQL query dynamically
    string sql = "SELECT * FROM " + tableName + ";";

    // Execute the query
    sqlite3_exec(dbConnection, sql.c_str(), callback, nullptr, &errorMessage);

    results = dbResults;
}


// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results)
{
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getProceduresSQL = "SELECT * FROM procedures;";
    sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, nullptr, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults)
    {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    NotUsed = nullptr;
    vector<string> dbRow;

    // argc is the number of columns for this row of results
    // argv contains the values for the columns
    // Each value is pushed into a vector.
    for (int i = 0; i < argc; i++)
    {
        dbRow.push_back(argv[i]);
    }

    // The row is pushed to the vector for storing all rows of results 
    dbResults.push_back(dbRow);

    return 0;
}
