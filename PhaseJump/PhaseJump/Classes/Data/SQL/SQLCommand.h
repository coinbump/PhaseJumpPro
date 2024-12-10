#pragma once

#include "SQLStatement.h"
#include <sqlite3.h>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Used to run a SQLite command from a statement
    struct SQLCommand {
        sqlite3_stmt* sqliteStatement{};

        SQLStatement statement;

        SQLCommand(SQLStatement statement) :
            statement(statement) {}

        ~SQLCommand();

        bool IsPrepared() const {
            return nullptr != sqliteStatement;
        }
    };
} // namespace PJ
