#if _PJ_SQL_
using Mono.Data.Sqlite;
#endif

using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using UnityEngine;
using System.Linq;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 4/4/23
*/
namespace PJ
{
#if _PJ_SQL_
    using SQLRowValues = Tags;

    /// <summary>
    /// Utilities for modifying and querying data in a SQLite table
    /// </summary>
    public class SQLTable
    {
        /// <summary>
        /// The type of value modification operation to perform
        /// </summary>
        public enum SetValueType
        {
            Update,
            Insert
        }

        protected string name;
        protected SQLDatabase db;

        public string Name => name;
        public SQLDatabase DB => db;

        public SQLTable(string name, SQLDatabase db)
        {
            this.name = name;
            this.db = db;
        }

        /// Deletes the entire table
        public void Drop()
        {
            if (null == db) { return; }

            SQLStatement statement = new("DROP TABLE ");
            statement.AppendIdentifier(name);

            db.Run(statement);
        }

        public void InsertRow()
        {
            if (null == db) { return; }

            SQLStatement statement = new("INSERT INTO ");
            statement.AppendIdentifier(name);
            statement.AppendString(" DEFAULT VALUES");

            db.Run(statement);
        }

        public bool CellExists(SQLTableQueryArguments query)
        {
            if (null == db) { return false; }

            if (null == query.where)
            {
                Debug.Log("ERROR. CellExists requires where clause.");
                return false;
            }
            if (query.columnNames.Count < 1)
            {
                Debug.Log("ERROR. CellExists requires 1 column");
                return false;
            }

            // EXAMPLE: SELECT * FROM Persons WHERE City='Chicago`
            SQLStatement statement = new("SELECT ");
            statement.AppendIdentifier(query.columnNames[0]);
            statement.AppendString(" FROM ");
            statement.AppendIdentifier(name);

            statement.AppendString(" WHERE ");
            statement.AppendIdentifier(query.where.value.columnName);
            statement.AppendString("=");
            statement.Append(new SQLValue(SQLValueType.Text, query.where.value.value));

            return db.RunScalar(statement) != null;
        }

        public bool AddColumn(string columnName, string _params)
        {
            if (null == db) { return false; }

            var tableName = name;

            // EXAMPLE: ALTER TABLE "Persons" ADD "DateOfBirth" date.
            SQLStatement statement = new("ALTER TABLE ");
            statement.AppendIdentifier(tableName);
            statement.AppendString(" ADD ");
            statement.AppendIdentifier(columnName);
            statement.AppendString(" " + _params);

            db.Run(statement);
            return true;
        }

        public void DeleteRow(string whereColumn, string whereMatch)
        {
            if (null == db) { return; }

            SQLStatement statement = new("DELETE FROM ");
            statement.AppendIdentifier(name);
            statement.AppendString(" WHERE ");
            statement.AppendIdentifier(whereColumn);
            statement.AppendString("=");
            statement.Append(new SQLValue(SQLValueType.Text, whereMatch));

            db.Run(statement);
        }

        public HashSet<string> UniqueStrings(string columnName)
        {
            var tableName = name;
            HashSet<string> result = new();

            if (null == db) { return result; }

            SQLStatement statement = new("SELECT ");
            statement.AppendIdentifier(columnName);
            statement.AppendString(" FROM ");
            statement.AppendIdentifier(tableName);

            db.RunReader(statement, (reader) =>
            {
                var value = reader[columnName];
                if (null == value) { return; }

                var text = value.ToString();
                if (text.Length > 0)
                {
                    result.Add(text);
                }
            });

            return result;
        }

        public SQLStatement BuildStatement(string columnName, Optional<SQLWhereArguments> where)
        {
            return BuildStatement(new SQLTableQueryArguments(columnName, where));
        }

        public SQLStatement BuildStatement(SQLTableQueryArguments query)
        {
            var columNames = query.columnNames;
            var where = query.where;

            // EXAMPLE: SELECT "lastName" FROM Persons WHERE City='Chicago'
            SQLStatement result = new("SELECT ");

            var columnNamesString = "*";
            if (columNames.Count > 0)
            {
                var identifiedColumNames = columNames.Select(columnName => new SQLIdentifierFormatter().Formatted(columnName)).ToList();
                columnNamesString = String.Join(",", identifiedColumNames);
            }

            result.AppendString(columnNamesString);

            result.AppendString(" FROM ");
            result.AppendIdentifier(name);

            if (null != where)
            {
                result.AppendString(" WHERE ");
                result.AppendIdentifier(where.value.columnName);

                result.AppendString("=");
                result.Append(new SQLValue(SQLValueType.Text, where.value.value));
                result.AppendString("");
            }

            return result;
        }

        public List<SQLRowValues> RowValuesList()
        {
            List<String> columnNames = new();
            return RowValuesList(new SQLTableQueryArguments(columnNames, null));
        }

        public List<SQLRowValues> RowValuesList(SQLTableQueryArguments query)
        {
            List<SQLRowValues> result = new();
            if (null == db) { return result; }

            var statement = BuildStatement(query);
            db.RunReader(statement, (reader) =>
            {
                SQLRowValues row = new();

                for (int i = 0; i < reader.FieldCount; i++)
                {
                    var value = reader[i];
                    var columnName = reader.GetName(i);
                    var type = reader.GetFieldType(columnName);

                    // The default integer type is 64 bit, but practically we will usually use Int32
                    // FUTURE: add a parameter for this if needed
                    if (value is Int64 && null != value)
                    {
                        value = Convert.ToInt32(value);
                    }

                    row[columnName] = value;
                }

                result.Add(row);
            });

            return result;
        }

        /**
         https://stackoverflow.com/questions/17920146/find-the-datatype-of-field-from-datareader-object
         From: https://www.sqlite.org/c3ref/column_decltype.html

         "SQLite uses dynamic run-time typing. So just because a column is declared to contain a particular type does not mean that the data stored in that column is of the declared type. SQLite is strongly typed, but the typing is dynamic not static. Type is associated with individual values, not with the containers used to hold those values."
         */
        public List<int> IntValues(SQLTableQueryArguments query)
        {
            List<int> result = new();
            if (null == db) { return result; }

            var statement = BuildStatement(query);
            db.RunReader(statement, (reader) =>
            {
                foreach (var columnName in query.columnNames)
                {
                    var value = reader.GetInt32(columnName);
                    result.Add(value);
                }
            });

            return result;
        }

        public int IntValue(SQLTableQueryArguments query, int defaultValue)
        {
            var values = IntValues(query);
            return values.Count == 0 ? defaultValue : values[0];
        }

        public List<float> FloatValues(SQLTableQueryArguments query)
        {
            List<float> result = new();
            if (null == db) { return result; }

            var statement = BuildStatement(query);
            db.RunReader(statement, (reader) =>
            {
                foreach (var columnName in query.columnNames)
                {
                    var value = reader.GetFloat(columnName);
                    result.Add(value);
                }
            });

            return result;
        }

        public float FloatValue(SQLTableQueryArguments query, float defaultValue)
        {
            var values = FloatValues(query);
            return values.Count == 0 ? defaultValue : values[0];
        }

        public List<string> StringValues(SQLTableQueryArguments query)
        {
            List<string> result = new();
            if (null == db) { return result; }

            var statement = BuildStatement(query);
            db.RunReader(statement, (reader) =>
            {
                foreach (var columnName in query.columnNames)
                {
                    var value = reader[columnName];
                    if (null == value) { continue; }
                    if (value.GetType() != typeof(string)) { continue; }

                    result.Add(value.ToString());
                }
            });

            return result;
        }

        public string StringValue(SQLTableQueryArguments query, string defaultValue)
        {
            var values = StringValues(query);
            return values.Count == 0 ? defaultValue : values[0];
        }

        /**
            OPTIMIZE: this is inefficient to check this every time a SQL value is altered, to optimize SQL,
            build the table first.

            http://stackoverflow.com/questions/2520945/sqlite-if-column-exists
            http://stackoverflow.com/questions/928865/find-sqlite-column-names-in-empty-table
            http://www.sqlite.org/pragma.html#pragma_table_info
         */
        public bool ColumnExists(string columnName)
        {
            if (null == db) { return false; }
            var tableName = name;

            SQLStatement statement = new("PRAGMA table_info(");
            statement.AppendIdentifier(tableName);
            statement.AppendString(");");

            bool result = false;

            db.RunReader(statement, (reader) =>
            {
                var value = reader["name"];
                if (null == value) { return; }

                if (value.ToString().ToLower() == columnName.ToLower())
                {
                    result = true;
                }
            });

            return result;
        }

        public void SetValue(SQLTableMutateArguments mutation, SetValueType type)
        {
            if (null == db) { return; }

            // Modify existing values
            // EXAMPLE: UPDATE table_name SET column1=value, column2=value2,... WHERE some_column=some_value
            if (type == SetValueType.Update || null != mutation.where)
            {
                SQLStatement statement = new();
                statement.AppendString("UPDATE ");
                statement.AppendIdentifier(name);
                statement.AppendString(" SET ");
                statement.AppendIdentifier(mutation.columnName);
                statement.AppendString("=");
                statement.Append(mutation.value);

                if (null != mutation.where)
                {
                    statement.AppendString(" WHERE ");
                    statement.AppendIdentifier(mutation.where.value.columnName);
                    statement.AppendString("=");
                    statement.Append(new SQLValue(SQLValueType.Text, mutation.where.value.value));
                    statement.AppendString("");
                }

                db.Run(statement);
            }
            else
            {
                // SYNTAX: INSERT INTO table_name (column1, column2, column3,...) VALUES (value1, value2, value3,...)
                SQLStatement statement = new("INSERT INTO ");
                statement.AppendIdentifier(name);
                statement.AppendString(" (");
                statement.AppendIdentifier(mutation.columnName);
                statement.AppendString(") VALUES (");
                statement.Append(mutation.value);
                statement.AppendString(")");

                db.Run(statement);
            }
        }

        public void SetIntValue(SQLTableMutateArguments mutation, SetValueType type)
        {
            SetValue(new SQLTableMutateArguments(mutation.columnName, mutation.where, mutation.value), type);
        }

        public void SetFloatValue(SQLTableMutateArguments mutation, SetValueType type)
        {
            SetValue(new SQLTableMutateArguments(mutation.columnName, mutation.where, mutation.value), type);
        }
    }
#endif
}
