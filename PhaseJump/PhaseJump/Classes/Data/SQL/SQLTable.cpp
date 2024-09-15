#include "SQLTable.h"
#include "SQLCommand.h"
#include "SQLDatabase.h"
#include "SQLRowValues.h"
#include "SQLStatement.h"
#include "VectorList.h"
#include <sqlite3.h>

using namespace std;
using namespace PJ;

SQLTable::SQLTable(String name, SQLDatabaseSharedPtr db) :
    name(name),
    db(db) {}

SQLStatement SQLTable::BuildStatement(String columnName, std::optional<SQLWhereArguments> where) {
    VectorList<String> columnNames;
    Add(columnNames, columnName);

    return BuildStatement(SQLTableQueryArguments(columnNames, where));
}

SQLStatement SQLTable::BuildStatement(SQLTableQueryArguments query) {
    // EXAMPLE: SELECT "lastName" FROM Persons WHERE City='Chicago'
    SQLStatement result("SELECT ");

    auto columnNames = query.columnNames;
    auto where = query.where;

    String columnNamesString = "*";

    if (columnNames.size() > 0) {
        VectorList<String> identifiedColumnNames;
        std::transform(
            columnNames.begin(), columnNames.end(), std::back_inserter(identifiedColumnNames),
            [](const String& value) { return SQLIdentifierFormatter().Formatted(value); }
        );

        columnNamesString = Joined(identifiedColumnNames, ",");
    }

    result.AppendString(columnNamesString);
    result.AppendString(" FROM ");
    result.AppendIdentifier(name);

    if (where) {
        result.AppendString(" WHERE ");
        result.AppendIdentifier(where.value().columnName);

        result.AppendString("=");
        result.Append(SQLValue(SQLValueType::Text, where.value().value));
        result.AppendString("");
    }

    return result;
}

VectorList<SP<Tags>> ColumnValues(VectorList<String> columnNames) {
    VectorList<SP<Tags>> result;

    return result;
}

VectorList<SQLRowValues> SQLTable::RowValuesList(SQLTableQueryArguments query) {
    VectorList<SQLRowValues> result;

    auto statement = BuildStatement(SQLTableQueryArguments(query.columnNames, query.where));
    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        while (SQLITE_ROW == db->Step(command)) {
            SQLRowValues row;

            int columnCount = sqlite3_column_count(command.sqliteStatement);
            for (int i = 0; i < columnCount; i++) {
                String columnName = sqlite3_column_name(command.sqliteStatement, i);
                int columnType = sqlite3_column_type(command.sqliteStatement, i);
                switch (columnType) {
                case SQLITE_INTEGER:
                    {
                        auto value = sqlite3_column_int(command.sqliteStatement, i);
                        row.Add(columnName, value);
                        break;
                    }
                case SQLITE_FLOAT:
                    {
                        auto value = sqlite3_column_double(command.sqliteStatement, i);
                        row.Add(columnName, value);
                        break;
                    }
                case SQLITE_TEXT:
                    {
                        String value = (const char*)sqlite3_column_text(command.sqliteStatement, i);
                        row.Add(columnName, value);
                        break;
                    }
                default:
                    break;
                }
            }

            Add(result, row);
        }
    }

    return result;
}

/**

 From: https://www.sqlite.org/c3ref/column_decltype.html

 "SQLite uses dynamic run-time typing. So just because a column is declared to
 contain a particular type does not mean that the data stored in that column is
 of the declared type. SQLite is strongly typed, but the typing is dynamic not
 static. Type is associated with individual values, not with the containers used
 to hold those values."
 */
VectorList<int> SQLTable::IntValues(SQLTableQueryArguments query) {
    VectorList<int> result;

    auto statement = BuildStatement(SQLTableQueryArguments(query.columnNames, query.where));
    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        while (SQLITE_ROW == db->Step(command)) {
            int columnCount = sqlite3_column_count(command.sqliteStatement);
            for (int i = 0; i < columnCount; i++) {
                int columnType = sqlite3_column_type(command.sqliteStatement, i);
                if (columnType == SQLITE_INTEGER) {
                    auto value = sqlite3_column_int(command.sqliteStatement, i);
                    Add(result, value);
                }
            }
        }
    }

    return result;
}

int SQLTable::IntValue(SQLTableQueryArguments query, int defaultValue) {
    auto values = IntValues(query);
    return IsEmpty(values) ? defaultValue : values[0];
}

VectorList<float> SQLTable::FloatValues(SQLTableQueryArguments query) {
    VectorList<float> result;

    auto statement = BuildStatement(SQLTableQueryArguments(query.columnNames, query.where));
    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        while (SQLITE_ROW == db->Step(command)) {
            int columnCount = sqlite3_column_count(command.sqliteStatement);
            for (int i = 0; i < columnCount; i++) {
                int columnType = sqlite3_column_type(command.sqliteStatement, i);
                if (columnType == SQLITE_FLOAT) {
                    auto value = sqlite3_column_double(command.sqliteStatement, i);
                    Add(result, (float)value);
                }
            }
        }
    }

    return result;
}

float SQLTable::FloatValue(SQLTableQueryArguments query, float defaultValue) {
    auto values = FloatValues(query);
    return IsEmpty(values) ? defaultValue : values[0];
}

VectorList<String> SQLTable::StringValues(SQLTableQueryArguments query) {
    VectorList<String> result;

    auto statement = BuildStatement(SQLTableQueryArguments(query.columnNames, query.where));
    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        while (SQLITE_ROW == db->Step(command)) {
            int columnCount = sqlite3_column_count(command.sqliteStatement);
            for (int i = 0; i < columnCount; i++) {
                int columnType = sqlite3_column_type(command.sqliteStatement, i);
                if (columnType == SQLITE3_TEXT) {
                    auto value =
                        String((const char*)sqlite3_column_text(command.sqliteStatement, i));
                    Add(result, value);
                }
            }
        }
    }

    return result;
}

String SQLTable::StringValue(SQLTableQueryArguments query, String defaultValue) {
    auto values = StringValues(query);
    return IsEmpty(values) ? defaultValue : values[0];
}

void SQLTable::InsertRow() {
    SQLStatement statement("INSERT INTO ");
    statement.AppendIdentifier(name);
    statement.AppendString(" DEFAULT VALUES");

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        db->Step(command);
    }
}

void SQLTable::DeleteRow(String whereColumn, String whereMatch) {
    SQLStatement statement("DELETE FROM ");
    statement.AppendIdentifier(name);
    statement.AppendString(" WHERE ");
    statement.AppendIdentifier(whereColumn);
    statement.AppendString("=");
    statement.Append(SQLValue(SQLValueType::Text, whereMatch));

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        db->Step(command);
    }
}

/// Deletes the entire table
void SQLTable::Drop() {
    SQLStatement statement("DROP TABLE ");
    statement.AppendIdentifier(name);

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        db->Step(command);
    }
}

bool SQLTable::CellExists(SQLTableQueryArguments query) {
    if (!query.where) {
        PJ::Log("ERROR. CellExists requires where clause.");
        return false;
    }
    if (IsEmpty(query.columnNames)) {
        PJ::Log("ERROR. CellExists requires column name");
        return false;
    }

    // EXAMPLE: SELECT * FROM Persons WHERE City='Chicago`
    SQLStatement statement("SELECT ");
    statement.AppendIdentifier(query.columnNames[0]);
    statement.AppendString(" FROM ");
    statement.AppendIdentifier(name);

    statement.AppendString(" WHERE ");
    statement.AppendIdentifier(query.where.value().columnName);
    statement.AppendString("=");
    statement.Append(SQLValue(SQLValueType::Text, query.where.value().value));

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        if (SQLITE_ROW == db->Step(command)) {
            return true;
        }
    }

    return false;
}

void SQLTable::Run(SQLStatement statement) {
    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        while (SQLITE_ROW == db->Step(command)) {}
    }
}

bool SQLTable::AddColumn(String colName, String params) {
    auto tableName = name;

    // EXAMPLE: ALTER TABLE "Persons" ADD "DateOfBirth" date
    SQLStatement statement("ALTER TABLE ");
    statement.AppendIdentifier(tableName);
    statement.AppendString(" ADD ");
    statement.AppendIdentifier(colName);
    statement.AppendString(" " + params);

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        if (SQLITE_DONE == db->Step(command)) {
            return true;
        }
    }

    return false;
}

/**
 OPTIMIZE: this is inefficient to check this every time a SQL value is altered,
 to optimize SQL, build the table first.

 http://stackoverflow.com/questions/2520945/sqlite-if-column-exists
 http://stackoverflow.com/questions/928865/find-sqlite-column-names-in-empty-table
 http://www.sqlite.org/pragma.html#pragma_table_info
 */
bool SQLTable::ColumnExists(String columnName) {
    auto tableName = name;

    SQLStatement statement("PRAGMA table_info(");
    statement.AppendIdentifier(tableName);
    statement.AppendString(");");

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        int columnCount = -1;

        while (SQLITE_ROW == db->Step(command)) {
            if (columnCount < 0) {
                columnCount = sqlite3_column_count(command.sqliteStatement);
            }

            for (int i = 0; i < columnCount; i++) {
                String colName(sqlite3_column_name(command.sqliteStatement, i));

                if (CompareNoCase(colName, "name")) {
                    String colValue((const char*)sqlite3_column_text(command.sqliteStatement, i));
                    if (CompareNoCase(colValue, columnName)) {
                        return true;
                    }
                    break;
                }
            }
        }
    }

    return false;
}

OrderedSet<String> SQLTable::UniqueStrings(String columnName) {
    auto tableName = name;
    OrderedSet<String> result;

    SQLStatement statement("SELECT ");
    statement.AppendIdentifier(columnName);
    statement.AppendString(" FROM ");
    statement.AppendIdentifier(tableName);

    SQLCommand command(statement);

    if (SQLITE_OK == db->Prepare(command)) {
        while (SQLITE_ROW == db->Step(command)) {
            String text((const char*)sqlite3_column_text(command.sqliteStatement, 0));
            result.insert(text);
        }
    }

#ifdef __DEBUG__
    for (auto& _i : uniqueNames) {
        PJ::Log("Unique value: ", (_i).c_str());
    }
#endif

    return result;
}

void SQLTable::SetValue(SQLTableMutateArguments mutation, SetValueType type) {
    // Modify existing values
    // EXAMPLE: UPDATE table_name SET column1=value, column2=value2,... WHERE
    // some_column=some_value
    if (type == SetValueType::Update || mutation.where) {
        SQLStatement statement;
        statement.AppendString("UPDATE ");
        statement.AppendIdentifier(name);
        statement.AppendString(" SET ");
        statement.AppendIdentifier(mutation.columnName);
        statement.AppendString("=");
        statement.Append(mutation.value);

        if (mutation.where) {
            statement.AppendString(" WHERE ");
            statement.AppendIdentifier(mutation.where.value().columnName);
            statement.AppendString("=");
            statement.Append(SQLValue(SQLValueType::Text, mutation.where.value().value));
            statement.AppendString("");
        }

        Run(statement);
    } else {
        // SYNTAX: INSERT INTO table_name (column1, column2, column3,...) VALUES
        // (value1, value2, value3,...)
        SQLStatement statement("INSERT INTO ");
        statement.AppendIdentifier(name);
        statement.AppendString(" (");
        statement.AppendIdentifier(mutation.columnName);
        statement.AppendString(") VALUES (");
        statement.Append(mutation.value);
        statement.AppendString(")");

        Run(statement);
    }
}

void SQLTable::SetIntValue(SQLTableMutateArguments mutation, SetValueType type) {
    SetValue(SQLTableMutateArguments(mutation.columnName, mutation.where, mutation.value), type);
}

void SQLTable::SetFloatValue(SQLTableMutateArguments mutation, SetValueType type) {
    SetValue(SQLTableMutateArguments(mutation.columnName, mutation.where, mutation.value), type);
}
