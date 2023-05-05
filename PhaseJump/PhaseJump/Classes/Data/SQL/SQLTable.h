#ifndef SQLTABLE_H
#define SQLTABLE_H

#include "_String.h"
#include "SQLTypes.h"
#include "SQLStatement.h"
#include "SQLTableQueryArguments.h"
#include "SQLTableMutateArguments.h"
#include "SQLRowValues.h"
#include "VectorList.h"
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
    class StringVectorList;
    class Tags;

    using SQLDatabaseSharedPtr = SP<SQLDatabase>;

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

        VectorList<SQLRowValues> RowValuesList(SQLTableQueryArguments query);

        VectorList<int> IntValues(SQLTableQueryArguments query);
        int IntValue(SQLTableQueryArguments query, int defaultValue);
        VectorList<float> FloatValues(SQLTableQueryArguments query);
        float FloatValue(SQLTableQueryArguments query, float defaultValue);
        VectorList<String> StringValues(SQLTableQueryArguments query);
        String StringValue(SQLTableQueryArguments query, String defaultValue);

        bool CellExists(SQLTableQueryArguments query);

        void InsertRow();
        void DeleteRow(String whereColumn, String whereMatch);
        void Drop();

        SQLStatement BuildStatement(String columnName, std::optional<SQLWhereArguments> where);
        SQLStatement BuildStatement(SQLTableQueryArguments query);

        void Run(SQLStatement statement);

        bool ColumnExists(String columnName);
        bool AddColumn(String columnName, String params);
        Set<String> UniqueStrings(String columnName);

        void SetValue(SQLTableMutateArguments mutation, SetValueType type);
        void SetIntValue(SQLTableMutateArguments mutation, SetValueType type);
        void SetFloatValue(SQLTableMutateArguments mutation, SetValueType type);
    };
}

#endif
