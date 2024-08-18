#ifndef SQLCOMMAND_H
#define SQLCOMMAND_H

#include "SQLStatement.h"
#include <sqlite3.h>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    // Used to run a SQLite command from a statement
    struct SQLCommand {
        sqlite3_stmt* sqliteStatement = NULL;

        SQLStatement statement;

        SQLCommand(SQLStatement statement) :
            statement(statement) {}

        ~SQLCommand();

        bool IsPrepared() const {
            return NULL != sqliteStatement;
        }
    };
} // namespace PJ

#endif
