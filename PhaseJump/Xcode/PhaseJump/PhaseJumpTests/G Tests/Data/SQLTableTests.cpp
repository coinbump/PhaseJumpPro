#include "gtest/gtest.h"

#include "SQLDatabase.h"
#include "SQLTable.h"
#include <memory>

using namespace std;
using namespace PJ;

TEST(SQLTableTests, TestIntValues) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    EXPECT_TRUE(db->CreateTable("test", "(test_l INTEGER, test_r TEXT)"));

    EXPECT_NO_THROW({
        db->TryRun("INSERT INTO test (test_l, test_r) values (1, 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values (11, 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values (2, 'two')");
    });

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);
    auto intValues = table->IntValues(SQLTableQuery("test_l", std::nullopt));

    EXPECT_EQ(3, intValues.size());

    auto intValuesOne = table->IntValues(SQLTableQuery("test_l", SQLWhereArgument("test_r", "one")));
    EXPECT_EQ(2, intValuesOne.size());

    auto twoQuery = SQLTableQuery("test_l", SQLWhereArgument("test_r", "two"));
    auto intValuesTwo = table->IntValues(twoQuery);
    EXPECT_EQ(1, intValuesTwo.size());
    EXPECT_EQ(2, intValuesTwo[0]);

    EXPECT_EQ(2, table->IntValue(twoQuery, 0));
    EXPECT_EQ(0, table->IntValue(SQLTableQuery("test_l", SQLWhereArgument("test_r", "never")), 0));
}

TEST(SQLTableTests, TestFloatValues) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    EXPECT_TRUE(db->CreateTable("test", "(test_l DOUBLE, test_r TEXT)"));

    EXPECT_NO_THROW({
        db->TryRun("INSERT INTO test (test_l, test_r) values (1.5, 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values (11.5, 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values (2.5, 'two')");
    });

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);
    auto floatValues = table->FloatValues(SQLTableQuery("test_l", std::nullopt));

    EXPECT_EQ(3, floatValues.size());

    auto floatValuesOne = table->FloatValues(SQLTableQuery("test_l", SQLWhereArgument("test_r", "one")));
    EXPECT_EQ(2, floatValuesOne.size());

    auto twoQuery = SQLTableQuery("test_l", SQLWhereArgument("test_r", "two"));
    auto floatValuesTwo = table->FloatValues(twoQuery);
    EXPECT_EQ(1, floatValuesTwo.size());
    EXPECT_EQ(2.5f, floatValuesTwo[0]);

    EXPECT_EQ(2.5f, table->FloatValue(twoQuery, 0));
    EXPECT_EQ(0, table->FloatValue(SQLTableQuery("test_l", SQLWhereArgument("test_r", "never")), 0));
}

TEST(SQLTableTests, TestStringValues) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("test"));

    EXPECT_TRUE(db->CreateTable("test", "(test_l TEXT, test_r TEXT)"));

    EXPECT_NO_THROW({
        db->TryRun("INSERT INTO test (test_l, test_r) values ('a', 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values ('b', 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values ('c', 'two')");
    });

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);
    auto stringValues = table->StringValues(SQLTableQuery("test_l", std::nullopt));

    EXPECT_EQ(3, stringValues.size());

    auto stringValuesOne = table->StringValues(SQLTableQuery("test_l", SQLWhereArgument("test_r", "one")));
    EXPECT_EQ(2, stringValuesOne.size());

    auto twoQuery = SQLTableQuery("test_l", SQLWhereArgument("test_r", "two"));
    auto stringValuesTwo = table->StringValues(twoQuery);
    EXPECT_EQ(1, stringValuesTwo.size());
    EXPECT_EQ(String("c"), stringValuesTwo[0]);

    EXPECT_EQ(String("c"), table->StringValue(twoQuery, ""));
    EXPECT_EQ("", table->StringValue(SQLTableQuery("test_l", SQLWhereArgument("test_r", "never")), ""));
}

TEST(SQLTableTests, TestDeleteRow) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l TEXT, test_r TEXT)"));

    EXPECT_NO_THROW({
        db->TryRun("INSERT INTO test (test_l, test_r) values ('a', 'one')");
        db->TryRun("INSERT INTO test (test_l, test_r) values ('b', 'one')");
    });

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);

    auto uniqueStrings = table->SelectUniqueStrings("test_l");
    EXPECT_EQ(2, uniqueStrings.size());

    table->DeleteRow("test_l", "a");

    uniqueStrings = table->SelectUniqueStrings("test_l");
    EXPECT_EQ(1, uniqueStrings.size());
    EXPECT_EQ(String("b"), *uniqueStrings.begin());
}

TEST(SQLTableTests, TestDropTable) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l TEXT, test_r TEXT)"));
    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);

    EXPECT_TRUE(db->TableExists("test"));
    table->Drop();
    EXPECT_FALSE(db->TableExists("test"));
}

TEST(SQLTableTests, TestCellExists) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l INTEGER, test_r TEXT)"));

    EXPECT_NO_THROW({
        db->TryRun("INSERT INTO test (test_l, test_r) values (2, 'two')");
    });

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);
    EXPECT_TRUE(table->CellExists(SQLTableQuery("test_l", SQLWhereArgument("test_r", "two"))));
    EXPECT_FALSE(table->CellExists(SQLTableQuery("test_l", SQLWhereArgument("test_r", "three"))));
}

TEST(SQLTableTests, TestInsertRow) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT)"));

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);

    auto intValues = table->IntValues(SQLTableQuery("test_l", std::nullopt));
    EXPECT_EQ(0, intValues.size());

    table->InsertRow();

    intValues = table->IntValues(SQLTableQuery("test_l", std::nullopt));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(0, intValues[0]);
}

TEST(SQLTableTests, TestSetValueUpdate) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT DEFAULT 'a')"));

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);
    table->InsertRow();

    auto intValues = table->IntValues(SQLTableQuery("test_l", SQLWhereArgument("test_r", "a")));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(0, intValues[0]);

    table->SetValue(SQLTableTypeMutation<String>("test_l", SQLWhereArgument("test_r", "a"), "3"), SQLTable::SetValueType::Update);

    intValues = table->IntValues(SQLTableQuery("test_l", SQLWhereArgument("test_r", "a")));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(3, intValues[0]);
}

TEST(SQLTableTests, TestSetValueInsert) {
    std::shared_ptr<SQLDatabase> db = std::make_shared<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_TRUE(db->CreateTable("test", "(test_l INTEGER DEFAULT 0, test_r TEXT DEFAULT 'a')"));

    std::shared_ptr<SQLTable> table = std::make_shared<SQLTable>("test", db);

    table->SetValue(SQLTableTypeMutation<String>("test_l", std::nullopt, "3"), SQLTable::SetValueType::Insert);

    auto intValues = table->IntValues(SQLTableQuery("test_l", SQLWhereArgument("test_r", "a")));
    EXPECT_EQ(1, intValues.size());
    EXPECT_EQ(3, intValues[0]);
}
