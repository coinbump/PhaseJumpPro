#ifndef SQLTABLE_H
#define SQLTABLE_H

#include "_String.h"
#include "SQLTypes.h"
#include "SQLStatement.h"
#include "SQLTableQuery.h"
#include "SQLTableTypeMutation.h"
#include "Array.h"
#include "_Set.h"
#include <vector>
#include <set>
#include <sqlite3.h>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 3/30/23
 */
namespace PJ
{
    class SQLDatabase;
    using SQLDatabaseSharedPtr = std::shared_ptr<SQLDatabase>;

    /**
        Manages a SQL table, must be connected to an SQL database to work (via Connect).

        Managing Unique row IDs: http://www.sqlabs.com/blog/2010/12/sqlite-and-unique-rowid-something-you-really-need-to-know/
     */
    class SQLTable
    {
    public:
        enum class SetValueType {
            Update,
            Insert
        };

        String name;
        SQLDatabaseSharedPtr db;

        String Name() const { return name; }
        SQLDatabaseSharedPtr DB() const { return db; }

        SQLTable(String name, SQLDatabaseSharedPtr db);

        Array<int> IntValues(SQLTableQuery query);
        int IntValue(SQLTableQuery query, int defaultValue);
        Array<float> FloatValues(SQLTableQuery query);
        float FloatValue(SQLTableQuery query, float defaultValue);
        Array<String> StringValues(SQLTableQuery query);
        String StringValue(SQLTableQuery query, String defaultValue);

        bool CellExists(SQLTableQuery query);

        void InsertRow();
        void DeleteRow(String whereColumn, String whereMatch);
        void Drop();

        SQLStatement BuildStatement(String columnName, std::optional<SQLWhereArgument> where);

        void Run(SQLStatement statement);

        bool ColumnExists(String columnName);
        bool AddColumn(String columnName, String params);
        Set<String> SelectUniqueStrings(String columnName);

        void SetValue(SQLTableTypeMutation<String> mutation, SetValueType type);
        void SetIntValue(SQLTableTypeMutation<int> mutation, SetValueType type);
        void SetFloatValue(SQLTableTypeMutation<float> mutation, SetValueType type);
    };
}

#endif
