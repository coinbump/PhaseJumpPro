#include "gtest/gtest.h"

#include "SQLDatabase.h"
#include "SQLTable.h"

using namespace std;
using namespace PJ;

TEST(SQLDatabase, Tests) {
    auto db = NEW<SQLDatabase>();

    EXPECT_NO_THROW({
        db->TryOpen("", SQLDatabaseOpenType::InMemory, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    });

    EXPECT_FALSE(db->TableExists("never")) << "Empty database shouldn't have tables";

    EXPECT_TRUE(db->CreateTable("bank", "(game INTEGER, paid INTEGER)")) << "Couldn't create bank table";
    EXPECT_TRUE(db->TableExists("bank")) << "Table bank missing";

    auto bankTable = NEW<SQLTable>("bank", db.get());

    EXPECT_FALSE(bankTable->ColumnExists("missing"));

    EXPECT_TRUE(bankTable->ColumnExists("game")) << "Column game missing";
    EXPECT_TRUE(bankTable->ColumnExists("paid")) << "Column bank missing";

    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO bank (game, paid) values (30, 400)"));
    });

    EXPECT_FALSE(bankTable->ColumnExists("invalid_col")) << "Column invalid_col shouldn't exist";
    EXPECT_TRUE(bankTable->AddColumn("new_col", "INTEGER")) << "Couldn't add column new_col";
    EXPECT_TRUE(bankTable->ColumnExists("new_col")) << "Column new_col missing";

    EXPECT_TRUE(db->CreateTable("test", "(test_column INTEGER)"));
    auto testTable = NEW<SQLTable>("test", db.get());

    db->BeginTransaction();
    EXPECT_TRUE(db->IsInTransaction());
    EXPECT_NO_THROW({
        db->TryRun(SQLStatement("INSERT INTO test (test_column) values (1)"));
        db->TryRun(SQLStatement("INSERT INTO test (test_column) values (2)"));
        db->TryRun(SQLStatement("INSERT INTO test (test_column) values (1)"));
    });
    db->EndTransaction();
    EXPECT_FALSE(db->IsInTransaction());

    auto uniqueStrings = testTable->UniqueStrings("test_column");
    EXPECT_TRUE(Contains(uniqueStrings, "1"));
    EXPECT_TRUE(Contains(uniqueStrings, "2"));
    EXPECT_EQ(2, uniqueStrings.size());

    auto tableNames = db->TableNames();
    EXPECT_EQ(2, tableNames.size());
    EXPECT_TRUE(Contains(tableNames, "bank"));
    EXPECT_TRUE(Contains(tableNames, "test"));
}
