#ifndef PJSQLSTATEMENT_H
#define PJSQLSTATEMENT_H

#include <sqlite3.h>
#include "SQLTypes.h"
#include "SQLUtils.h"
#include "SQLValue.h"
#include "_String.h"
#include "Log.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 3/29/23
 */
namespace PJ {
    struct SQLIdentifierFormatter
    {
        String Formatted(String value)
        {
            String result;

            result.append("[");
            result.append(value);
            result.append("]");

            return result;
        }
    };

    struct SQLStatement
    {
        String value;

        SQLStatement()
        {
        }

        SQLStatement(String value) : value(value) {
        }

        void AppendString(String string) {
            value.append(string);
        }

        void AppendIdentifier(String identifier, bool isEscaped = false) {
            if (SQLUtils().IsReservedKeyword(identifier)) {
                PJLog("WARNING. %s is reserved by SQL.", identifier.c_str());
            }

            SQLIdentifierFormatter formatter;

            if (isEscaped) {
                value.append(identifier);
            } else {
                value.append(formatter.Formatted(identifier));
            }
        }

        void Append(SQLValue value) {
            switch (value.type) {
            case SQLValueType::Text:
            case SQLValueType::Any:
            case SQLValueType::Blob:
                // SQL value strings are wrapped in single quotes.
                // Example: name='myName'
                this->value.append("'");
                this->value.append(value.value);
                this->value.append("'");
                break;
            case SQLValueType::Int:
            case SQLValueType::Real:
                this->value.append(value.value);
                break;
            }
        }
    };
}

#endif
