#ifndef SQLWHEREARGUMENTS_H
#define SQLWHEREARGUMENTS_H

#include "_String.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    /// Selects values "WHERE columnName='value'"
    struct SQLWhereArguments {
        String columnName;
        String value;

        SQLWhereArguments(String columnName, String value) :
            columnName(columnName),
            value(value) {}
    };
} // namespace PJ

#endif
