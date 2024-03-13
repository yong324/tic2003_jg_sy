#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
	// method to connect to the database and initialize tables in the database
	static void initialize();

	// method to close the database connection
	static void close();

	static void insertProcedure(string procedureName);
	static void insertVariable(string variableName);
	static void insertConstant(string constant);
	static void insertAssignment(int assignmentLineIdx, string variable, string assignmentExpression);
	static void insertPrint(int printIdx, string printVariable);
	static void insertRead(int readsIdx, string readVariable);
	static void insertStatement(int statementsIdx, int parent, string statementType);
	static void insertWhile(int whileIdx, string relExpression);
	static void insertIf(int ifIdx, string relExpression);
	static void insertModify(int modifyIdx, string variableModified, string modifyType);


	// method to get all the procedures from the database
	static void getProcedures(vector<string>& results);

	// Select * from tableName
	static void getData(const string& tableName, vector<string>& results);

	static void getModifies(std::string& paraLeft, std::string& paraRight, vector<string>& results);

	static int getParentLine(int childLine);

private:
	// the connection pointer to the database
	static sqlite3* dbConnection; 
	// a vector containing the results from the database
	static vector<vector<string>> dbResults; 
	// the error message from the database
	static char* errorMessage;
	// callback method to put one row of results from the database into the dbResults vector
	// This method is called each time a row of results is returned from the database
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
};

