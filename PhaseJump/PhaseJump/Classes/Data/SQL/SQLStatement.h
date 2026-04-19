#pragma once

#include "Dev.h"
#include "SQLTool.h"
#include "SQLTypes.h"
#include "SQLValue.h"
#include "StringUtils.h"
#include "VectorList.h"
#include <sqlite3.h>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    struct SQLIdentifierFormatter {
        String Formatted(String value) {
            String result;

            result.append("[");
            result.append(value);
            result.append("]");

            return result;
        }
    };

    /// Builds a parameterized SQL statement.
    ///
    /// Values added via `Append(SQLValue)` are emitted as `?` placeholders and
    /// collected in `parameters`; the database binds them at prepare time via
    /// `sqlite3_bind_*`. Callers must not embed raw user values via
    /// `AppendString` — only via `Append(SQLValue)`.
    struct SQLStatement {
        String value;
        VectorList<SQLValue> parameters;

        SQLStatement() {}

        SQLStatement(String value) :
            value(value) {}

        void AppendString(String string) {
            value.append(string);
        }

        void AppendIdentifier(String identifier, bool isEscaped = false) {
            if (SQLTool().IsReservedKeyword(identifier)) {
                PJ::Log("WARNING. ", identifier, " is reserved by SQL.");
            }

            SQLIdentifierFormatter formatter;

            if (isEscaped) {
                value.append(identifier);
            } else {
                value.append(formatter.Formatted(identifier));
            }
        }

        /// Appends a `?` placeholder and records the value for binding.
        void Append(SQLValue value) {
            this->value.append("?");
            parameters.push_back(value);
        }
    };
} // namespace PJ
