#include "gtest/gtest.h"

#include "SQLDatabase.h"
#include "SQLTable.h"
#include "CollectionUtils.h"
#include <memory>

using namespace std;
using namespace PJ;

TEST(SQLTable, TestRowValuesList_MultiColumn_AndStarSelect) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Int));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (1, 'a')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (11, 'b')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    VectorList<String> columnNames;

    // Test SELECT * (no column names)
    auto values = table->RowValuesList(SQLTableQueryArguments(columnNames, std::nullopt));

    EXPECT_EQ(2, values.size());
    EXPECT_EQ(1, values[0].SafeValue<int>("test_l"));
    EXPECT_EQ("a", values[0].SafeValue<String>("test_r"));
    EXPECT_EQ(11, values[1].SafeValue<int>("test_l"));
    EXPECT_EQ("b", values[1].SafeValue<String>("test_r"));
}

TEST(SQLTable, TestIntValues_SingleColumn) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Int));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (1, 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (11, 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (2, 'two')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    auto intValues = table->IntValues(SQLTableQueryArguments("test_l", std::nullopt));

    EXPECT_EQ(3, intValues.size());

    auto intValuesOne = table->IntValues(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "one")));
    EXPECT_EQ(2, intValuesOne.size());

    auto twoQuery = SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "two"));
    auto intValuesTwo = table->IntValues(twoQuery);
    EXPECT_EQ(1, intValuesTwo.size());
    EXPECT_EQ(2, intValuesTwo[0]);

    EXPECT_EQ(2, table->IntValue(twoQuery, 0));
    EXPECT_EQ(0, table->IntValue(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "never")), 0));
}

TEST(SQLTable, TestIntValues_MultiColumn) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Int));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Int));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (1, 3)"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (11, 33)"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    VectorList<String> columnNames;
    columnNames.push_back("test_l");
    columnNames.push_back("test_r");
    auto values = table->IntValues(SQLTableQueryArguments(columnNames, std::nullopt));

    EXPECT_EQ(4, values.size());
    EXPECT_TRUE(Contains(values, 1));
    EXPECT_TRUE(Contains(values, 11));
    EXPECT_TRUE(Contains(values, 3));
    EXPECT_TRUE(Contains(values, 33));
}

TEST(SQLTableTests, TestFloatValues_SingleColumn) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Real));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (1.5, 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (11.5, 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (2.5, 'two')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    auto floatValues = table->FloatValues(SQLTableQueryArguments("test_l", std::nullopt));

    EXPECT_EQ(3, floatValues.size());

    auto floatValuesOne = table->FloatValues(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "one")));
    EXPECT_EQ(2, floatValuesOne.size());

    auto twoQuery = SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "two"));
    auto floatValuesTwo = table->FloatValues(twoQuery);
    EXPECT_EQ(1, floatValuesTwo.size());
    EXPECT_EQ(2.5f, floatValuesTwo[0]);

    EXPECT_EQ(2.5f, table->FloatValue(twoQuery, 0));
    EXPECT_EQ(0, table->FloatValue(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "never")), 0));
}

TEST(SQLTable, TestFloatValues_MultiColumn) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Real));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Real));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (1.5, 3.5)"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (11.5, 33.5)"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    VectorList<String> columnNames;
    columnNames.push_back("test_l");
    columnNames.push_back("test_r");
    auto values = table->FloatValues(SQLTableQueryArguments(columnNames, std::nullopt));

    EXPECT_EQ(4, values.size());
    EXPECT_TRUE(Contains(values, 1.5f));
    EXPECT_TRUE(Contains(values, 11.5f));
    EXPECT_TRUE(Contains(values, 3.5f));
    EXPECT_TRUE(Contains(values, 33.5f));
}

TEST(SQLTableTests, TestStringValues_SingleColumn) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Text));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('a', 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('b', 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('c', 'two')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    auto stringValues = table->StringValues(SQLTableQueryArguments("test_l", std::nullopt));

    EXPECT_EQ(3, stringValues.size());

    auto stringValuesOne = table->StringValues(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "one")));
    EXPECT_EQ(2, stringValuesOne.size());

    auto twoQuery = SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "two"));
    auto stringValuesTwo = table->StringValues(twoQuery);
    EXPECT_EQ(1, stringValuesTwo.size());
    EXPECT_EQ(String("c"), stringValuesTwo[0]);

    EXPECT_EQ(String("c"), table->StringValue(twoQuery, ""));
    EXPECT_EQ("", table->StringValue(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "never")), ""));
}

TEST(SQLTable, TestStringValues_MultiColumn) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Text));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('a', 'c')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('b', 'd')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    VectorList<String> columnNames;
    columnNames.push_back("test_l");
    columnNames.push_back("test_r");
    auto values = table->StringValues(SQLTableQueryArguments(columnNames, std::nullopt));

    EXPECT_EQ(4, values.size());
    EXPECT_TRUE(Contains(values, "a"));
    EXPECT_TRUE(Contains(values, "b"));
    EXPECT_TRUE(Contains(values, "c"));
    EXPECT_TRUE(Contains(values, "d"));
}

TEST(SQLTableTests, TestDeleteRow) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Text));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('a', 'one')"));
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values ('b', 'one')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());

    auto uniqueStrings = table->UniqueStrings("test_l");
    EXPECT_EQ(2, uniqueStrings.size());

    table->DeleteRow("test_l", "a");

    uniqueStrings = table->UniqueStrings("test_l");
    EXPECT_EQ(1, uniqueStrings.size());
    EXPECT_EQ(String("b"), *uniqueStrings.begin());
}

TEST(SQLTableTests, TestDropTable) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Text));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());

    EXPECT_TRUE(db->TableExists("test"));
    table->Drop();
    EXPECT_FALSE(db->TableExists("test"));
}

TEST(SQLTableTests, TestCellExists) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Int));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text));
    EXPECT_TRUE(db->CreateTable("test", schema));

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_l, test_r) values (2, 'two')"));
    });

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    EXPECT_TRUE(table->CellExists(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "two"))));
    EXPECT_FALSE(table->CellExists(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "three"))));
}

TEST(SQLTableTests, TestInsertRow) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT)"));

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());

    auto intValues = table->IntValues(SQLTableQueryArguments("test_l", std::nullopt));
    EXPECT_EQ(0, intValues.size());

    table->InsertRow();

    intValues = table->IntValues(SQLTableQueryArguments("test_l", std::nullopt));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(0, intValues[0]);
}

TEST(SQLTableTests, TestSetValueUpdate) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Int, std::make_optional("0")));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text, std::make_optional("'a'")));
    EXPECT_TRUE(db->CreateTable("test", schema));

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());
    table->InsertRow();

    auto intValues = table->IntValues(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "a")));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(0, intValues[0]);

    table->SetValue(SQLTableMutateArguments("test_l", SQLWhereArguments("test_r", "a"), SQLValue(SQLValueType::Int, "3")), SQLTable::SetValueType::Update);

    intValues = table->IntValues(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "a")));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(3, intValues[0]);
}

TEST(SQLTableTests, TestSetValueInsert) {
    UP<SQLDatabase> db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    SQLTableSchema schema;
    schema.columns.push_back(SQLColumnSchema("test_l", SQLValueType::Int, std::make_optional("0")));
    schema.columns.push_back(SQLColumnSchema("test_r", SQLValueType::Text, std::make_optional("'a'")));
    EXPECT_TRUE(db->CreateTable("test", schema));

    UP<SQLTable> table = NEW<SQLTable>("test", db.get());

    table->SetValue(SQLTableMutateArguments("test_l", std::nullopt, SQLValue(SQLValueType::Int, "3")), SQLTable::SetValueType::Insert);

    auto intValues = table->IntValues(SQLTableQueryArguments("test_l", SQLWhereArguments("test_r", "a")));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(3, intValues[0]);
}
