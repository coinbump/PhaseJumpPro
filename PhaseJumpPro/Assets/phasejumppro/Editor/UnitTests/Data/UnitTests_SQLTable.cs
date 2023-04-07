using UnityEngine;
using System.Collections;
using NUnit.Framework;
using System.Collections.Generic;
using PJ;
using System.IO;

namespace PJ
{
#if _PJ_SQL_
    public class UnitTests_SQLTable
    {
        private static SQLDatabase Setup()
        {
            var db = new SQLDatabase();

            // In-memory databases are not working in the Mono.Data.sqlite implementation
            var filePath = Path.Combine(Application.temporaryCachePath, "Unit_Test.sqlite");

            Assert.DoesNotThrow(() =>
            {
                File.Delete(filePath);
                db.TryOpen(filePath, SQLDatabaseOpenType.AtPath);
            });

            return db;
        }

        [Test]
        public void TestRowValuesList_MultiColumn_AndStarSelect()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (1, 'a')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (11, 'b')"));
            });

            SQLTable table = new("test", db);

            // Test SELECT * (no column names)
            var values = table.RowValuesList(new SQLTableQueryArguments(new List<string>(), null));

            Assert.AreEqual(2, values.Count);
            Assert.AreEqual(1, values[0].SafeValue<int>("test_l", () => 0));
            Assert.AreEqual("a", values[0].SafeValue<string>("test_r", () => ""));
            Assert.AreEqual(11, values[1].SafeValue<int>("test_l", () => 0));
            Assert.AreEqual("b", values[1].SafeValue<string>("test_r", () => ""));
        }

        [Test]
        public void TestIntValues_SingleColumn()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (1, 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (11, 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (2, 'two')"));
            });

            SQLTable table = new("test", db);
            var intValues = table.IntValues(new SQLTableQueryArguments("test_l", null));

            Assert.AreEqual(3, intValues.Count);

            var intValuesOne = table.IntValues(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "one"))));
            Assert.AreEqual(2, intValuesOne.Count);

            var twoQuery = new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "two")));
            var intValuesTwo = table.IntValues(twoQuery);
            Assert.AreEqual(1, intValuesTwo.Count);
            Assert.AreEqual(2, intValuesTwo[0]);

            Assert.AreEqual(2, table.IntValue(twoQuery, 0));
            Assert.AreEqual(0, table.IntValue(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "never"))), 0));
        }

        [Test]
        public void TestIntValues_MultiColumn()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER, test_r INTEGER)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (1, 3)"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (11, 33)"));
            });

            SQLTable table = new("test", db);
            List<string> columnNames = new();
            columnNames.Add("test_l");
            columnNames.Add("test_r");
            var values = table.IntValues(new SQLTableQueryArguments(columnNames, null));

            Assert.AreEqual(4, values.Count);
            Assert.IsTrue(values.Contains(1));
            Assert.IsTrue(values.Contains(11));
            Assert.IsTrue(values.Contains(3));
            Assert.IsTrue(values.Contains(33));
        }

        [Test]
        public void TestFloatValues_SingleColumn()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l REAL, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (1.5, 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (11.5, 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (2.5, 'two')"));
            });

            SQLTable table = new("test", db);
            var floatValues = table.FloatValues(new SQLTableQueryArguments("test_l", null));

            Assert.AreEqual(3, floatValues.Count);

            var floatValuesOne = table.FloatValues(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "one"))));
            Assert.AreEqual(2, floatValuesOne.Count);

            var twoQuery = new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "two")));
            var floatValuesTwo = table.FloatValues(twoQuery);
            Assert.AreEqual(1, floatValuesTwo.Count);
            Assert.AreEqual(2.5f, floatValuesTwo[0]);

            Assert.AreEqual(2.5f, table.FloatValue(twoQuery, 0));
            Assert.AreEqual(0, table.FloatValue(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "never"))), 0));
        }

        [Test]
        public void TestFloatValues_MultiColumn()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l REAL, test_r REAL)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (1.5, 3.5)"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (11.5, 33.5)"));
            });

            SQLTable table = new("test", db);
            List<string> columnNames = new();
            columnNames.Add("test_l");
            columnNames.Add("test_r");
            var values = table.FloatValues(new SQLTableQueryArguments(columnNames, null));

            Assert.AreEqual(4, values.Count);
            Assert.IsTrue(values.Contains(1.5f));
            Assert.IsTrue(values.Contains(11.5f));
            Assert.IsTrue(values.Contains(3.5f));
            Assert.IsTrue(values.Contains(33.5f));
        }

        [Test]
        public void TestStringValues_SingleColumn()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l TEXT, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('a', 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('b', 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('c', 'two')"));
            });

            SQLTable table = new("test", db);
            var stringValues = table.StringValues(new SQLTableQueryArguments("test_l", null));

            Assert.AreEqual(3, stringValues.Count);

            var stringValuesOne = table.StringValues(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "one"))));
            Assert.AreEqual(2, stringValuesOne.Count);

            var twoQuery = new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "two")));
            var stringValuesTwo = table.StringValues(twoQuery);
            Assert.AreEqual(1, stringValuesTwo.Count);
            Assert.AreEqual("c", stringValuesTwo[0]);

            Assert.AreEqual("c", table.StringValue(twoQuery, ""));
            Assert.AreEqual("", table.StringValue(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "never"))), ""));
        }

        [Test]
        public void TestStringValues_MultiColumn()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsFalse(db.TableExists("test"));

            Assert.IsTrue(db.CreateTable("test", "(test_l TEXT, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('a', 'c')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('b', 'd')"));
            });

            SQLTable table = new("test", db);
            List<string> columnNames = new();
            columnNames.Add("test_l");
            columnNames.Add("test_r");
            var values = table.StringValues(new SQLTableQueryArguments(columnNames, null));

            Assert.AreEqual(4, values.Count);
            Assert.IsTrue(values.Contains("a"));
            Assert.IsTrue(values.Contains("b"));
            Assert.IsTrue(values.Contains("c"));
            Assert.IsTrue(values.Contains("d"));
        }

        [Test]
        public void TestDeleteRow()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsTrue(db.CreateTable("test", "(test_l TEXT, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('a', 'one')"));
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values ('b', 'one')"));
            });

            SQLTable table = new("test", db);

            var uniqueStrings = table.UniqueStrings("test_l");
            Assert.AreEqual(2, uniqueStrings.Count);

            table.DeleteRow("test_l", "a");

            uniqueStrings = table.UniqueStrings("test_l");
            Assert.AreEqual(1, uniqueStrings.Count);
            Assert.IsTrue(uniqueStrings.Contains("b"));
        }

        [Test]
        public void TestDropTable()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsTrue(db.CreateTable("test", "(test_l TEXT, test_r TEXT)"));
            SQLTable table = new("test", db);

            Assert.IsTrue(db.TableExists("test"));
            table.Drop();
            Assert.IsFalse(db.TableExists("test"));
        }

        [Test]
        public void TestCellExists()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER, test_r TEXT)"));

            Assert.DoesNotThrow(() =>
            {
                db.TryRun(new SQLStatement("INSERT INTO test (test_l, test_r) values (2, 'two')"));
            });

            SQLTable table = new("test", db);
            Assert.IsTrue(table.CellExists(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "two")))));
            Assert.IsFalse(table.CellExists(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "three")))));
        }

        [Test]
        public void TestInsertRow()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT)"));

            SQLTable table = new("test", db);

            var intValues = table.IntValues(new SQLTableQueryArguments("test_l", null));
            Assert.AreEqual(0, intValues.Count);

            table.InsertRow();

            intValues = table.IntValues(new SQLTableQueryArguments("test_l", null));
            Assert.AreEqual(1, intValues.Count);
            Assert.AreEqual(0, intValues[0]);
        }

        [Test]
        public void TestSetValueUpdate()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT DEFAULT 'a')"));

            SQLTable table = new("test", db);
            table.InsertRow();

            var intValues = table.IntValues(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "a"))));
            Assert.AreEqual(1, intValues.Count);
            Assert.AreEqual(0, intValues[0]);

            table.SetValue(new SQLTableMutateArguments("test_l", new(new SQLWhereArguments("test_r", "a")), new SQLValue(SQLValueType.Int, "3")), SQLTable.SetValueType.Update);

            intValues = table.IntValues(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "a"))));
            Assert.AreEqual(1, intValues.Count);
            Assert.AreEqual(3, intValues[0]);
        }

        [Test]
        public void TestSetValueInsert()
        {
            SQLDatabase db = new();

            Assert.DoesNotThrow(() =>
            {
                db = Setup();
            });

            Assert.IsTrue(db.CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT DEFAULT 'a')"));

            SQLTable table = new("test", db);

            table.SetValue(new SQLTableMutateArguments("test_l", null, new SQLValue(SQLValueType.Int, "3")), SQLTable.SetValueType.Insert);

            var intValues = table.IntValues(new SQLTableQueryArguments("test_l", new(new SQLWhereArguments("test_r", "a"))));
            Assert.AreEqual(1, intValues.Count);
            Assert.AreEqual(3, intValues[0]);
        }
    }
#endif
}
