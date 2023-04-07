#if _PJ_SQL_
using Mono.Data.Sqlite;
#endif

using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 4/3/23
PORTED TO: C++
*/
namespace PJ
{
    /**
    Creates a connection to a SQL Database.

    Usage:
    - Follow install instructions for sqlite.dll, sqlite.def (from win64) and Mono.Data.sqlite from MonoBleedingEdge/lib
    - Place database in StreamingAssets folder inside the Unity Assets folder
    - Copy database into application persistentDataPath (can't open SQLTable inside StreamingAssets)
    - IMPORTANT: Android support requires using WWW to copy the database file

    Unity recommends adding 1 Mono.Data.Sqlite for each platform:
    https://forum.unity.com/threads/how-do-i-use-sqlite-with-unity-2021.1259430/
    
    Install instructions: 
    - Download sqlite.dll, sqlite.def from https://sqlite.org/download.html (use win64 download for both Mac and Windows)
    - Add sqlite files to plugins
    - Add Mono.Data.Sqlite files for MacOS, Windows, Linux and set their OS restrictions in Unity Inspector
    
    Install Instructions: https://www.mongodb.com/developer/code-examples/csharp/saving-data-in-unity3d-using-sqlite/
    Install instructions: https://forum.unity.com/threads/installing-sqlite-on-macos-and-probably-for-windows-unity-2021-1-16.1179430/

    Mono.Data.Sqlite Documentation: http://docs.go-mono.com/?link=T%3aMono.Data.Sqlite.SqliteConnection
    Sqlite and Unity: https://medium.com/@rizasif92/sqlite-and-unity-how-to-do-it-right-31991712190
    Import SQLite assets in Unity: https://forum.unity.com/threads/import-sqlite-database-as-asset.914438/
    Store SQLite databases in Streaming Assets: https://docs.unity3d.com/Manual/StreamingAssets.html
    Open/Close Connection: https://stackoverflow.com/questions/4439409/open-close-sqlconnection-or-keep-open
    Examples: https://csharp.hotexamples.com/examples/Mono.Data.Sqlite/SqliteConnection/-/php-sqliteconnection-class-examples.html
    */
#if _PJ_SQL_
    public class SQLDatabase : IDisposable
    {
        public SqliteConnection dbConnection;

        public SQLDatabase()
        {
        }

        public void Dispose()
        {
            Close();
        }

        public bool TryOpen(string filePath, SQLDatabaseOpenType openType)
        {
            string dbUri = "URI=file:" + filePath;

            // https://www.mono-project.com/docs/database-access/providers/sqlite/
            switch (openType)
            {
                case SQLDatabaseOpenType.AtPath:
                    break;
                    // FUTURE: support if needed
                    // case SQLDatabaseOpenType.InMemory:
                    //     dbUri = "URI=file::memory:";
                    //     break;
            }

            var dbConnection = new SqliteConnection(dbUri);
            this.dbConnection = dbConnection;

            return dbConnection != null;
        }

        public void Close()
        {
            if (null != dbConnection)
            {
                dbConnection.Close();
                dbConnection = null;
            }
        }

        public object RunScalar(SQLStatement statement)
        {
            object result = null;

            if (null == dbConnection) { return result; }

            // Recommended best practice is to open and close connection for each operation
            // Connections will be pooled
            dbConnection.Open();

            using (SqliteCommand command = new SqliteCommand(statement.value, dbConnection))
            {
                result = command.ExecuteScalar();
            }

            dbConnection.Close();

            return result;
        }

        public void Run(SQLStatement statement)
        {
            if (null == dbConnection) { return; }

            // Recommended best practice is to open and close connection for each operation
            // Connections will be pooled
            dbConnection.Open();

            using (SqliteCommand command = new SqliteCommand(statement.value, dbConnection))
            {
                command.ExecuteNonQuery();
            }

            dbConnection.Close();
        }

        public void RunReader(SQLStatement statement, Action<SqliteDataReader> action)
        {
            if (null == dbConnection) { return; }

            dbConnection.Open();

            using (SqliteCommand command = new SqliteCommand(statement.value, dbConnection))
            {
                var reader = command.ExecuteReader();

                while (reader.Read())
                {
                    action(reader);
                }

                reader.Close();
            }

            dbConnection.Close();
        }

        public void TryRun(SQLStatement statement)
        {
            // FUTURE: we might throw exceptions for malformed queries if needed.
            Run(statement);
        }

        public bool TableExists(string tableName)
        {
            SQLStatement statement = new("SELECT name FROM sqlite_master WHERE type='table' AND name=");
            statement.Append(new SQLValue(SQLValueType.Text, tableName));
            return RunScalar(statement) != null;
        }

        public bool CreateTable(string tableName, string _params)
        {
            if (null == dbConnection) { return false; }

            SQLStatement statement = new("CREATE TABLE IF NOT EXISTS ");
            statement.AppendIdentifier(tableName);
            statement.AppendString(" " + _params);

            Run(statement);

            return true;
        }

        public List<string> TableNames()
        {
            List<String> result = new();

            if (null == dbConnection) { return result; }

            SQLStatement statement = new("SELECT name FROM sqlite_master WHERE type='table'");
            RunReader(statement, (reader) =>
            {
                var tableName = reader[0] as string;
                if (null != tableName)
                {
                    result.Add(tableName);
                }
            });

            return result;
        }

        // OPTIMIZE: use these to speed up data writes to SQL
        // FUTURE: Support BeginTransaction, EndTransaction
        // public void BeginTransaction()
        // public void EndTransaction()
        // public bool IsInTransaction() { return false; }
    }
#endif
}
