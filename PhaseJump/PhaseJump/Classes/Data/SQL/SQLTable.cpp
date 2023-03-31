#include "SQLTable.h"
#include "SQLStatement.h"
#include "SQLDatabase.h"
#include <sqlite3.h>

using namespace std;
using namespace PJ;

SQLTable::SQLTable(String name, SQLDatabaseSharedPtr db) : name(name), db(db)
{
}

SQLStatement SQLTable::BuildStatement(String columnName, std::optional<SQLWhereArgument> where) {
    // EXAMPLE: SELECT "lastName" FROM Persons WHERE City='Chicago'
    SQLStatement result("SELECT ");
    result.AppendIdentifier(columnName);
    result.AppendString(" FROM ");
    result.AppendIdentifier(name);

    if (where) {
        result.AppendString(" WHERE ");
        result.AppendIdentifier(where.value().columnName);

        result.AppendString("=");
        result.AppendValue(where.value().value);
        result.AppendString("");
    }

    return result;
}

/**

 From: https://www.sqlite.org/c3ref/column_decltype.html

 "SQLite uses dynamic run-time typing. So just because a column is declared to contain a particular type does not mean that the data stored in that column is of the declared type. SQLite is strongly typed, but the typing is dynamic not static. Type is associated with individual values, not with the containers used to hold those values."
 */
Array<int> SQLTable::IntValues(SQLTableQuery query)
{
    Array<int> result;

    auto statement = BuildStatement(query.columnName, query.where);
    if (SQLITE_OK == db->Prepare(statement)) {
        while (SQLITE_ROW == db->Step(statement)) {
            int columnCount = sqlite3_column_count(statement.sqliteStatement);
            if (columnCount > 0) {
                int columnType = sqlite3_column_type(statement.sqliteStatement, 0);
                if (columnType == SQLITE_INTEGER) {
                    auto value = sqlite3_column_int(statement.sqliteStatement, 0);
                    result.Add(value);
                }
            }
        }
    }

    return result;
}

int SQLTable::IntValue(SQLTableQuery query, int defaultValue)
{
    auto values = IntValues(query);
    return values.IsEmpty() ? defaultValue : values[0];
}

Array<float> SQLTable::FloatValues(SQLTableQuery query)
{
   Array<float> result;

   auto statement = BuildStatement(query.columnName, query.where);
   if (SQLITE_OK == db->Prepare(statement)) {
       while (SQLITE_ROW == db->Step(statement)) {
           int columnCount = sqlite3_column_count(statement.sqliteStatement);
           if (columnCount > 0) {
               int columnType = sqlite3_column_type(statement.sqliteStatement, 0);
               if (columnType == SQLITE_FLOAT) {
                   auto value = sqlite3_column_double(statement.sqliteStatement, 0);
                   result.Add((float)value);
               }
           }
       }
   }

   return result;
}

float SQLTable::FloatValue(SQLTableQuery query, float defaultValue)
{
   auto values = FloatValues(query);
   return values.IsEmpty() ? defaultValue : values[0];
}

Array<String> SQLTable::StringValues(SQLTableQuery query)
{
   Array<String> result;

   auto statement = BuildStatement(query.columnName, query.where);
   if (SQLITE_OK == db->Prepare(statement)) {
       while (SQLITE_ROW == db->Step(statement)) {
           int columnCount = sqlite3_column_count(statement.sqliteStatement);
           if (columnCount > 0) {
               int columnType = sqlite3_column_type(statement.sqliteStatement, 0);
               if (columnType == SQLITE3_TEXT) {
                   auto value = String((const char*)sqlite3_column_text(statement.sqliteStatement, 0));
                   result.Add(value);
               }
           }
       }
   }

   return result;
}

String SQLTable::StringValue(SQLTableQuery query, String defaultValue)
{
   auto values = StringValues(query);
   return values.IsEmpty() ? defaultValue : values[0];
}

void SQLTable::InsertRow()
{
    SQLStatement statement("INSERT INTO ");
    statement.AppendIdentifier(name);
    statement.AppendString(" DEFAULT VALUES");

    if (SQLITE_OK == db->Prepare(statement)) {
        db->Step(statement);
    }
}

void SQLTable::DeleteRow(String whereColumn, String whereMatch)
{
    SQLStatement statement("DELETE FROM ");
    statement.AppendIdentifier(name);
    statement.AppendString(" WHERE ");
    statement.AppendIdentifier(whereColumn);
    statement.AppendString("=");
    statement.AppendValue(whereMatch);

    if (SQLITE_OK == db->Prepare(statement)) {
        db->Step(statement);
    }
}

/// Deletes the entire table
void SQLTable::Drop()
{
    SQLStatement statement("DROP TABLE ");
    statement.AppendIdentifier(name);

    if (SQLITE_OK == db->Prepare(statement)) {
        db->Step(statement);
    }
}

bool SQLTable::CellExists(SQLTableQuery query)
{
    if (!query.where) {
        PJLog("ERROR. CellExists requires where clause.");
        return false;
    }

    // EXAMPLE: SELECT * FROM Persons WHERE City='Chicago`
    SQLStatement statement("SELECT ");
    statement.AppendIdentifier(query.columnName);
    statement.AppendString(" FROM ");
    statement.AppendIdentifier(name);

    statement.AppendString(" WHERE ");
    statement.AppendIdentifier(query.where.value().columnName);
    statement.AppendString("=");
    statement.AppendValue(query.where.value().value);

    if (SQLITE_OK == db->Prepare(statement)) {
        if (SQLITE_ROW == db->Step(statement)) {
            return true;
        }
    }

    return false;
}

void SQLTable::Run(SQLStatement statement)
{
    if (SQLITE_OK == db->Prepare(statement)) {
        while (SQLITE_ROW == db->Step(statement)) {
        }
    }
}

bool SQLTable::AddColumn(String colName, String params)
{
    auto tableName = name;

    // EXAMPLE: ALTER TABLE "Persons" ADD "DateOfBirth" date
    SQLStatement statement("ALTER TABLE ");
    statement.AppendIdentifier(tableName);
    statement.AppendString(" ADD ");
    statement.AppendIdentifier(colName);
    statement.AppendString(" " + params);

    if (SQLITE_OK == db->Prepare(statement)) {
        if (SQLITE_DONE == db->Step(statement)) {
            return true;
        }
    }

    return false;
}

/**
    OPTIMIZE: this is inefficient to check this every time a SQL value is altered, to optimize SQL,
    build the table first.

    http://stackoverflow.com/questions/2520945/sqlite-if-column-exists
    http://stackoverflow.com/questions/928865/find-sqlite-column-names-in-empty-table
    http://www.sqlite.org/pragma.html#pragma_table_info
 */
bool SQLTable::ColumnExists(String columnName)
{
    auto tableName = name;

    SQLStatement statement("PRAGMA table_info(");
    statement.AppendIdentifier(tableName);
    statement.AppendString(");");

    if (SQLITE_OK == db->Prepare(statement)) {
        int columnCount = -1;

        while (SQLITE_ROW == db->Step(statement)) {
            if (columnCount < 0) {
                columnCount = sqlite3_column_count(statement.sqliteStatement);
            }

            for (int i = 0; i < columnCount; i++) {
                String colName(sqlite3_column_name(statement.sqliteStatement, i));

                if (colName.CompareNoCase("name")) {
                    String colValue((const char*)sqlite3_column_text(statement.sqliteStatement, i));
                    if (colValue.CompareNoCase(columnName)) {
                        return true;
                    }
                    break;
                }
            }
        }
    }

    return false;
}

Set<String> SQLTable::SelectUniqueStrings(String columnName) {
    auto tableName = name;
    Set<String> result;

    SQLStatement statement("SELECT ");
    statement.AppendIdentifier(columnName);
    statement.AppendString(" FROM ");
    statement.AppendIdentifier(tableName);

    if (SQLITE_OK == db->Prepare(statement)) {
        while (SQLITE_ROW == db->Step(statement)) {
            String text((const char*)sqlite3_column_text(statement.sqliteStatement, 0));
            result.insert(text);
        }
    }

#ifdef __DEBUG__
    for (auto& _i : uniqueNames) {
        PJLog("Unique value: %s", (_i).c_str());
    }
#endif

    return result;
}

void SQLTable::SetValue(SQLTableTypeMutation<String> mutation, SetValueType type)
{
    // Modify existing values
    // EXAMPLE: UPDATE table_name SET column1=value, column2=value2,... WHERE some_column=some_value
    if (type == SetValueType::Update || mutation.where) {
        SQLStatement statement;
        statement.AppendString("UPDATE ");
        statement.AppendIdentifier(name);
        statement.AppendString(" SET ");
        statement.AppendIdentifier(mutation.columnName);
        statement.AppendString("=");
        statement.AppendValue(mutation.value);

        if (mutation.where) {
            statement.AppendString(" WHERE ");
            statement.AppendIdentifier(mutation.where.value().columnName);
            statement.AppendString("=");
            statement.AppendValue(mutation.where.value().value);
            statement.AppendString("");
        }

        Run(statement);
    }
    else {
        // SYNTAX: INSERT INTO table_name (column1, column2, column3,...) VALUES (value1, value2, value3,...)
        SQLStatement statement("INSERT INTO ");
        statement.AppendIdentifier(name);
        statement.AppendString(" (");
        statement.AppendIdentifier(mutation.columnName);
        statement.AppendString(") VALUES (");
        statement.AppendValue(mutation.value);
        statement.AppendString(")");

        Run(statement);
    }
}

void SQLTable::SetIntValue(SQLTableTypeMutation<int> mutation, SetValueType type)
{
    SetValue(SQLTableTypeMutation<String>(mutation.columnName, mutation.where, String(mutation.value)), type);
}

void SQLTable::SetFloatValue(SQLTableTypeMutation<float> mutation, SetValueType type)
{
    SetValue(SQLTableTypeMutation<String>(mutation.columnName, mutation.where, String(mutation.value)), type);
}
