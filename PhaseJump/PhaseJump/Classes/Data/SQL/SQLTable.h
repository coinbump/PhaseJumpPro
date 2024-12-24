#pragma once

#include "OrderedSet.h"
#include "SQLRowValues.h"
#include "SQLStatement.h"
#include "SQLTableMutateArguments.h"
#include "SQLTableQueryArguments.h"
#include "SQLTypes.h"
#include "StringUtils.h"
#include "VectorList.h"
#include <memory>
#include <set>
#include <sqlite3.h>
#include <vector>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    class SQLDatabase;
    class Tags;

    using SQLDatabaseSharedPtr = SP<SQLDatabase>;

    /**
        Manages a SQL table, must be connected to an SQL database to work (via
       Connect).

     Additional Info:
        Managing Unique row IDs:
       http://www.sqlabs.com/blog/2010/12/sqlite-and-unique-rowid-something-you-really-need-to-know/
     */
    class SQLTable {
    public:
        enum class SetValueType { Update, Insert };

        String name;
        SQLDatabase& db;

        SQLTable(String name, SQLDatabase& db);

        String Name() const {
            return name;
        }

        SQLDatabase& DB() {
            return db;
        }

        SQLDatabase const& DB() const {
            return db;
        }

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

        /// Deletes the entire table
        void Drop();

        SQLStatement BuildStatement(String columnName, std::optional<SQLWhereArguments> where);
        SQLStatement BuildStatement(SQLTableQueryArguments query);

        void Run(SQLStatement statement);

        bool ColumnExists(String columnName);
        bool AddColumn(String columnName, String params);
        OrderedSet<String> UniqueStrings(String columnName);

        void SetValue(SQLTableMutateArguments mutation, SetValueType type);
        void SetIntValue(SQLTableMutateArguments mutation, SetValueType type);
        void SetFloatValue(SQLTableMutateArguments mutation, SetValueType type);
    };
} // namespace PJ
