using UnityEngine;
using System.Collections;
using NUnit.Framework;
using System.Collections.Generic;
using PJ;
using System.IO;

namespace PJ
{
#if _PJ_SQL_
    public class UnitTests_SQLDatabase
    {
        [Test]
        public void Tests()
        {
            var db = new SQLDatabase();

            // In-memory databases are not working in the Mono.Data.sqlite implementation
            var filePath = Path.Combine(Application.temporaryCachePath, "Unit_Test.sqlite");

            Assert.DoesNotThrow(() =>
            {
                File.Delete(filePath);
                db.TryOpen(filePath, SQLDatabaseOpenType.AtPath);
            });

            Assert.IsFalse(db.TableExists("never"), "Empty database shouldn't have tables");

            Assert.IsTrue(db.CreateTable("bank", "(game INTEGER, paid INTEGER)"), "Couldn't create bank table");
            Assert.IsTrue(db.TableExists("bank"), "Table bank missing");

            var bankTable = new SQLTable("bank", db);

            Assert.IsFalse(bankTable.ColumnExists("missing"));

            Assert.IsTrue(bankTable.ColumnExists("game"), "Column game missing");
            Assert.IsTrue(bankTable.ColumnExists("paid"), "Column bank missing");

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO bank (game, paid) values (30, 400)"));
            });

            Assert.IsFalse(bankTable.ColumnExists("invalid_col"), "Column invalid_col shouldn't exist");
            Assert.IsTrue(bankTable.AddColumn("new_col", "INTEGER"), "Couldn't add column new_col");
            Assert.IsTrue(bankTable.ColumnExists("new_col"), "Column new_col missing");

            Assert.IsTrue(db.CreateTable("test", "(test_column INTEGER)"));
            var testTable = new SQLTable("test", db);

            // FUTURE: test this when it is supported
            // db.BeginTransaction();
            // Assert.IsTrue(db.IsInTransaction());
            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_column) values (1)"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_column) values (2)"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_column) values (1)"));
            });
            // db.EndTransaction();
            // Assert.IsFalse(db.IsInTransaction());

            var uniqueStrings = testTable.UniqueStrings("test_column");
            Assert.IsTrue(uniqueStrings.Contains("1"));
            Assert.IsTrue(uniqueStrings.Contains("2"));
            Assert.AreEqual(2, uniqueStrings.Count);

            var tableNames = db.TableNames();
            Assert.AreEqual(2, tableNames.Count);
            Assert.IsTrue(tableNames.Contains("bank"));
            Assert.IsTrue(tableNames.Contains("test"));
        }
    }
#endif
}
