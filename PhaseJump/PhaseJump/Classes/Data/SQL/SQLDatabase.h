#ifndef PJSQLDATABASE_H
#define PJSQLDATABASE_H

#include "FilePath.h"
#include "OrderedSet.h"
#include "SQLCommand.h"
#include "SQLStatement.h"
#include "SQLTableSchema.h"
#include "StateMachine.h"
#include "StringUtils.h"
#include "VectorList.h"
#include <sqlite3.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 3/30/23
 */
namespace PJ {
    /**
     SQLDatabase

     Manages a connection to an SQL database using sqlite.

     USAGE: in general, use PJ::SQLTable for operations on a specific table.

     OPTIMIZE: On older devices, SQL selects can cause render slowdowns. Cache
     values that will be accessed often.

     OPTIMIZE: Always use Begin/EndTransaction when writing values.
     This can speed up SQL writes by 10 times.

     OPTIMIZATION REFERENCE:
     http://stackoverflow.com/questions/1711631/how-do-i-improve-the-performance-of-sqlite
     */
    class SQLDatabase {
    protected:
        enum class TransactionState { Default, InTransaction };
        // TODO: SP audit
        SP<StateMachine<TransactionState>> transactionStateMachine =
            MAKE<StateMachine<TransactionState>>();

    public:
        FilePath filePath;
        sqlite3* db = NULL;

        SQLDatabase();
        virtual ~SQLDatabase();

        bool TryOpen(FilePath filePath, SQLDatabaseOpenType openType, int flags);
        void Close();

        bool TableExists(String tableName);
        bool CreateTable(String tableName, SQLTableSchema schema);
        bool CreateTable(String tableName, String paramsString);

        VectorList<String> TableNames();

        int Prepare(SQLCommand& command);
        int Step(SQLCommand& command);
        void TryRun(SQLStatement statement);

        // OPTIMIZE: use these to speed up data writes to SQL
        void BeginTransaction();
        void EndTransaction();

        bool IsInTransaction() const {
            return transactionStateMachine->State() == TransactionState::InTransaction;
        }
    };

    using SQLDatabaseSharedPtr = SP<SQLDatabase>;
} // namespace PJ

#endif
