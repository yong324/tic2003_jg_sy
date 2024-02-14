#include "Database.h"

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
	// open a database connection and store the pointer into dbConnection
	sqlite3_open("database.db", &dbConnection);

	// drop the existing procedure table (if any)
	string dropProcedureTableSQL = "DROP TABLE IF EXISTS procedures";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	dropProcedureTableSQL = "DROP TABLE IF EXISTS variables";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	dropProcedureTableSQL = "DROP TABLE IF EXISTS constants";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	dropProcedureTableSQL = "DROP TABLE IF EXISTS assignments";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	dropProcedureTableSQL = "DROP TABLE IF EXISTS prints";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	dropProcedureTableSQL = "DROP TABLE IF EXISTS reads";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	dropProcedureTableSQL = "DROP TABLE IF EXISTS statements";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a procedure table
	string createProcedureTableSQL = "CREATE TABLE procedures ( procedureName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	createProcedureTableSQL = "CREATE TABLE variables ( variableName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	createProcedureTableSQL = "CREATE TABLE constants ( constantNum INTEGER PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	createProcedureTableSQL = "CREATE TABLE assignments ( assignmentIndex INTEGER PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	createProcedureTableSQL = "CREATE TABLE prints ( printIndex INTEGER PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	createProcedureTableSQL = "CREATE TABLE reads ( readIndex INTEGER PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	createProcedureTableSQL = "CREATE TABLE statements ( statementIndex INTEGER PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// initialize the result vector
	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

// method to insert a procedure into the database
void Database::insertProcedure(vector<string> procedures) {
	for (string& procedureName : procedures) {
		string insertProcedureSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}
}

// method to insert an assignment index into the database
void Database::insertAssignment(vector<int> assignmentLineIdx) {
	for (int& idx : assignmentLineIdx) {
		string insertProcedureSQL = "INSERT INTO assignments ('assignmentIndex') VALUES (" + to_string(idx) + ");";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}	
}

// method to insert a procedure into the database
void Database::insertVariable(vector<string> variables) {
	for (string& variableName : variables) {
		string insertProcedureSQL = "INSERT INTO variables ('variableName') VALUES ('" + variableName + "');";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}
}

void Database::insertPrint(vector<int> printIdx) {
	for (int& idx : printIdx) {
		string insertProcedureSQL = "INSERT INTO prints ('printIndex') VALUES (" + to_string(idx) + ");";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}
}

void Database::insertConstant(vector<string> constants) {
	for (string& num : constants) {
		string insertProcedureSQL = "INSERT INTO constants ('constantNum') VALUES (" + num + ");";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}
}

void Database::insertStatement(vector<int> statementsIdx) {
	for (int& idx : statementsIdx) {
		string insertProcedureSQL = "INSERT INTO statements ('statementIndex') VALUES (" + to_string(idx) + ");";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}
}

void Database::insertRead(vector<int> readsIdx) {
	for (int& idx : readsIdx) {
		string insertProcedureSQL = "INSERT INTO reads ('readIndex') VALUES (" + to_string(idx) + ");";
		sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
	}
}

// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results){
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getProceduresSQL = "SELECT * FROM procedures;";
	sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
	NotUsed = 0;
	vector<string> dbRow;

	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	for (int i = 0; i < argc; i++) {
		dbRow.push_back(argv[i]);
	}

	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);

	return 0;
}
