#include "SQLDatabase.h"
#include "SQLUtils.h"
#include "SQLTypes.h"

using namespace PJ;

//#define __SQL_DEBUG__

SQLDatabase::SQLDatabase()
{
}

SQLDatabase::~SQLDatabase()
{
	Close();
}

void SQLDatabase::Close()
{
    if (NULL != db) {
        sqlite3_close(db);
        db = NULL;
    }
}

/**
 Try to open/create the database. Throw an error if it fails

 Usage: don't forget to create directories to the file path first if you're creating a new database

 sqlite3_open: https://www.sqlite.org/c3ref/open.html
 Flags: https://www.sqlite.org/c3ref/c_open_autoproxy.html
 */
bool SQLDatabase::TryOpen(FilePath filePath, SQLDatabaseOpenType openType, int flags)
{
	Close();

    String databasePath(filePath.c_str());
    switch (openType) {
        case SQLDatabaseOpenType::AtPath:
            break;
        case SQLDatabaseOpenType::InMemory:
            databasePath = ":memory:";
            break;
        case SQLDatabaseOpenType::TemporaryOnDisk:
            databasePath = "";
            break;
    }

    this->filePath = filePath;

	int	openResult = sqlite3_open_v2(databasePath.c_str(), &db, flags, NULL);
    auto isSuccess = (SQLITE_OK == openResult && NULL != db);

    if (!isSuccess) {
        throw(openResult);
    }

    return true;
}

/**
 Prepare sql statement for execution.

 https://www.sqlite.org/c3ref/prepare.html
 */
int SQLDatabase::Prepare(SQLStatement& statement)
{
    if (statement.value.length() == 0) {
        PJLog("ERROR. Statement is empty");
        return SQLITE_MISUSE;
    }

    int	result = sqlite3_prepare_v2(db, statement.value.c_str(), -1, &statement.sqliteStatement, NULL);

#ifdef __SQL_DEBUG__
	PJLog("SQL. Prepare %s, RESULT: %d", statement.value.c_str(), result);
#endif

	return result;
}

/**
 Called one or more times to evaluate a prepared statement

 https://www.sqlite.org/c3ref/step.html
 */
int SQLDatabase::Step(SQLStatement& statement)
{
    if (!statement.IsPrepared()) {
        PJLog("ERROR. Statement must be prepared before Step");
        return SQLITE_MISUSE;
    }

	int	result = sqlite3_step(statement.sqliteStatement);

#ifdef __SQL_DEBUG__
	PJLog("SQL. Step RESULT: %d", result);
#endif

	return result;
}

void SQLDatabase::TryRun(String command)
{
    // EXAMPLE: "insert into bank (game, paid) values (30, 400)"
    SQLStatement statement(command);
    if (SQLITE_OK == Prepare(statement)) {
        bool isDone = false;

        while (!isDone) {
            auto result = Step(statement);

            switch (result) {
                case SQLITE_DONE:
                    isDone = true;
                    break;
                case SQLITE_ROW:
                    continue;
                    break;
                default:
                    throw(result);
                    break;
            }
        }
    }
}

// http://stackoverflow.com/questions/1601151/how-do-i-check-in-sqlite-whether-a-table-exists
bool SQLDatabase::TableExists(String tableName)
{
    bool result = false;

    SQLStatement statement("SELECT name FROM sqlite_master WHERE type='table' AND name=");
    statement.AppendValue(tableName);

    if (SQLITE_OK == Prepare(statement)) {
        // If table exists, a row is returned.
        if (SQLITE_ROW == Step(statement)) {
            result = true;
        }
    }

    return result;
}

Array<String> SQLDatabase::SelectTableNames()
{
    Array<String> result;

    SQLStatement statement("SELECT name FROM sqlite_master WHERE type='table'");

    if (SQLITE_OK == Prepare(statement)) {
        // If table exists, a row is returned.
        while (SQLITE_ROW == Step(statement)) {
            String tableName((const char*)sqlite3_column_text(statement.sqliteStatement, 0));

            if (tableName == "sqlite_autoindex") {
                continue;    // Skip the index tables.
            }
            result.Add(tableName);
        }
    }

    return result;
}

bool SQLDatabase::CreateTable(String tableName, String params)
{
    // EXAMPLE: "create table bank (game int, paid int)"
    SQLStatement statement("CREATE TABLE IF NOT EXISTS ");
    statement.AppendIdentifier(tableName);
    statement.AppendString(" " + params);

    if (SQLITE_OK == Prepare(statement)) {
        if (SQLITE_DONE == Step(statement)) {
            return true;
        }
    }

    return false;
}

void SQLDatabase::BeginTransaction() {
	switch (transactionStateMachine->State()) {
		case SQLDatabase::TransactionState::InTransaction:
			return;	// Already in transaction
			break;
		default:
			break;
	}

	char *errorMessage = NULL;
	if (SQLITE_OK == sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage)) {
		transactionStateMachine->SetState(TransactionState::InTransaction);
	}
}

void SQLDatabase::EndTransaction() {
	char *errorMessage = NULL;
	if (SQLITE_OK == sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &errorMessage)) {
		transactionStateMachine->SetState(TransactionState::Default);
	}
}
