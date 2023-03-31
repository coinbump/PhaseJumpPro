#ifndef PJSQLSTATEMENT_H
#define PJSQLSTATEMENT_H

#include "stdafx.h"
#include <sqlite3.h>
#include "SQLTypes.h"
#include "SQLUtils.h"
#include "_String.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 3/29/23
 */
namespace PJ {
    struct SQLStatement
    {
        String value;
        sqlite3_stmt *sqliteStatement = NULL;

        SQLStatement()
        {
        }

        SQLStatement(String value) : value(value) {
        }

        ~SQLStatement() {
            if (NULL != sqliteStatement) {
                sqlite3_finalize(sqliteStatement);
            }
        }

        bool IsPrepared() const { return NULL != sqliteStatement; }

        void AppendString(String string) {
            value.append(string);
        }

        void AppendIdentifier(String identifier, bool isEscaped = false) {
            if (SQLUtils().IsReservedKeyword(identifier)) {
                PJLog("WARNING. %s is reserved by SQL.", identifier.c_str());
            }

            if (isEscaped) {
                value.append(identifier);
            } else {
                // SQL identifier strings are wrapped in double quotes.
                // Example: ALTER TABLE "myTable"
                value.append("\"");
                value.append(identifier);
                value.append("\"");
            }
        }

        void AppendValue(String valueString, bool isEscaped = false) {
            if (isEscaped) {
                value.append(valueString);
            } else {
                // SQL value strings are wrapped in single quotes.
                // Example: name='myName'
                value.append("'");
                value.append(valueString);
                value.append("'");
            }
        }
    };
}

#endif
