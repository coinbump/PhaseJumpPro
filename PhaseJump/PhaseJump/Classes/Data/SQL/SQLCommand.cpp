#include <stdio.h>
#include "SQLCommand.h"

using namespace PJ;

SQLCommand::~SQLCommand() {
    if (NULL != sqliteStatement) {
        sqlite3_finalize(sqliteStatement);
    }
}
