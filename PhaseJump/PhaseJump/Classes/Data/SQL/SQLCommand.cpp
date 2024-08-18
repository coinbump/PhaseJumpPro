#include "SQLCommand.h"
#include <stdio.h>

using namespace PJ;

SQLCommand::~SQLCommand() {
    GUARD(sqliteStatement)
    sqlite3_finalize(sqliteStatement);
}
