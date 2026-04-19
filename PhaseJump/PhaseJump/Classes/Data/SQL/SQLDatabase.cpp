#include "SQLDatabase.h"
#include "SQLTool.h"
#include "SQLTypes.h"
#include "VectorList.h"

using namespace PJ;

// #define __SQL_DEBUG__

SQLDatabase::SQLDatabase() {}

SQLDatabase::~SQLDatabase() {
    Close();
}

void SQLDatabase::Close() {
    if (nullptr != db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

/**
 Try to open/create the database. Throw an error if it fails

 Usage: don't forget to create directories to the file path first if you're
 creating a new database

 sqlite3_open: https://www.sqlite.org/c3ref/open.html
 Flags: https://www.sqlite.org/c3ref/c_open_autoproxy.html
 */
bool SQLDatabase::TryOpen(FilePath filePath, SQLDatabaseOpenType openType, int flags) {
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

    int openResult = sqlite3_open_v2(databasePath.c_str(), &db, flags, nullptr);
    auto isSuccess = (SQLITE_OK == openResult && nullptr != db);

    if (!isSuccess) {
        throw(openResult);
    }

    return true;
}

namespace {
    int BindParameter(sqlite3_stmt* stmt, int index, SQLValue const& param) {
        switch (param.type) {
        case SQLValueType::Int:
            {
                char const* begin = param.value.c_str();
                char* end = nullptr;
                long long parsed = std::strtoll(begin, &end, 10);
                if (end == begin) {
                    return SQLITE_MISMATCH;
                }
                return sqlite3_bind_int64(stmt, index, parsed);
            }
        case SQLValueType::Real:
            {
                char const* begin = param.value.c_str();
                char* end = nullptr;
                double parsed = std::strtod(begin, &end);
                if (end == begin) {
                    return SQLITE_MISMATCH;
                }
                return sqlite3_bind_double(stmt, index, parsed);
            }
        case SQLValueType::Text:
        case SQLValueType::Any:
            return sqlite3_bind_text(stmt, index, param.value.c_str(), -1, SQLITE_TRANSIENT);
        case SQLValueType::Blob:
            return sqlite3_bind_blob(
                stmt, index, param.value.data(), (int)param.value.size(), SQLITE_TRANSIENT
            );
        }
        return SQLITE_ERROR;
    }
} // namespace

/**
 Prepare sql statement for execution and bind its parameters.

 https://www.sqlite.org/c3ref/prepare.html
 https://www.sqlite.org/c3ref/bind_blob.html
 */
int SQLDatabase::Prepare(SQLCommand& command) {
    if (command.statement.value.length() == 0) {
        PJ::Log("ERROR. Statement is empty");
        return SQLITE_MISUSE;
    }

    int result = sqlite3_prepare_v2(
        db, command.statement.value.c_str(), -1, &command.sqliteStatement, nullptr
    );

#ifdef __SQL_DEBUG__
    PJ::Log("SQL. Prepare ", command.statement.value, " RESULT: ", result);
#endif

    if (result != SQLITE_OK) {
        return result;
    }

    int paramIndex = 1;
    for (auto const& param : command.statement.parameters) {
        int bindResult = BindParameter(command.sqliteStatement, paramIndex, param);
        if (bindResult != SQLITE_OK) {
            PJ::Log("ERROR. Bind parameter ", paramIndex, " RESULT: ", bindResult);
            sqlite3_finalize(command.sqliteStatement);
            command.sqliteStatement = nullptr;
            return bindResult;
        }
        paramIndex++;
    }

    return SQLITE_OK;
}

/**
 Called one or more times to evaluate a prepared statement

 https://www.sqlite.org/c3ref/step.html
 */
int SQLDatabase::Step(SQLCommand& command) {
    if (!command.IsPrepared()) {
        PJ::Log("ERROR. Statement must be prepared before Step");
        return SQLITE_MISUSE;
    }

    int result = sqlite3_step(command.sqliteStatement);

#ifdef __SQL_DEBUG__
    PJ::Log("SQL. Step RESULT: ", result);
#endif

    return result;
}

void SQLDatabase::TryRun(SQLStatement statement) {
    SQLCommand command(statement);

    // EXAMPLE: "insert into bank (game, paid) values (30, 400)"
    if (SQLITE_OK == Prepare(command)) {
        bool isDone = false;

        while (!isDone) {
            auto result = Step(command);

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
bool SQLDatabase::TableExists(String tableName) {
    bool result = false;

    SQLStatement statement("SELECT name FROM sqlite_master WHERE type='table' AND name=");
    statement.Append(SQLValue(SQLValueType::Text, tableName));

    SQLCommand command(statement);

    if (SQLITE_OK == Prepare(command)) {
        // If table exists, a row is returned.
        if (SQLITE_ROW == Step(command)) {
            result = true;
        }
    }

    return result;
}

VectorList<String> SQLDatabase::TableNames() {
    VectorList<String> result;

    SQLStatement statement("SELECT name FROM sqlite_master WHERE type='table'");
    SQLCommand command(statement);

    if (SQLITE_OK == Prepare(command)) {
        // If table exists, a row is returned.
        while (SQLITE_ROW == Step(command)) {
            String tableName((const char*)sqlite3_column_text(command.sqliteStatement, 0));

            if (tableName == "sqlite_autoindex") {
                continue; // Skip the index tables.
            }
            Add(result, tableName);
        }
    }

    return result;
}

bool SQLDatabase::CreateTable(String tableName, SQLTableSchema schema) {
    String paramsString = " (";
    VectorList<String> paramList;
    for (auto& column : schema.columns) {
        String pString;
        String name = column.name;
        String sqlValueType = "ANY";

        switch (column.valueType) {
        case SQLValueType::Text:
            sqlValueType = "TEXT";
            break;
        case SQLValueType::Real:
            sqlValueType = "REAL";
            break;
        case SQLValueType::Int:
            sqlValueType = "INTEGER";
            break;
        case SQLValueType::Blob:
            sqlValueType = "BLOB";
            break;
        case SQLValueType::Any:
            sqlValueType = "ANY";
            break;
        }

        pString.append("[");
        pString.append(name);
        pString.append("] ");
        pString.append(sqlValueType);

        if (column.defaultValue) {
            pString.append(" DEFAULT " + column.defaultValue.value());
        }

        Add(paramList, pString);
    }
    paramsString.append(Joined(paramList, ", "));
    paramsString += ")";

    return CreateTable(tableName, paramsString);
}

bool SQLDatabase::CreateTable(String tableName, String paramsString) {
    // EXAMPLE: "create table bank (game int, paid int)"
    SQLStatement statement("CREATE TABLE IF NOT EXISTS ");
    statement.AppendIdentifier(tableName);
    statement.AppendString(" " + paramsString);

    SQLCommand command(statement);

    if (SQLITE_OK == Prepare(command)) {
        if (SQLITE_DONE == Step(command)) {
            return true;
        }
    }

    return false;
}

void SQLDatabase::BeginTransaction() {
    switch (transactionStateMachine.State()) {
    case SQLDatabase::TransactionState::InTransaction:
        return; // Already in transaction
        break;
    default:
        break;
    }

    char* errorMessage = nullptr;
    if (SQLITE_OK == sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errorMessage)) {
        transactionStateMachine.SetState(TransactionState::InTransaction);
    }
}

void SQLDatabase::EndTransaction() {
    char* errorMessage = nullptr;
    if (SQLITE_OK == sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, &errorMessage)) {
        transactionStateMachine.SetState(TransactionState::Default);
    }
}
