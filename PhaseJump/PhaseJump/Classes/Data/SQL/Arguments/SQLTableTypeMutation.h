#ifndef SQLTABLETYPEMUTATION_H
#define SQLTABLETYPEMUTATION_H

#include "SQLWhereArgument.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 3/30/23
 */
namespace PJ
{
    /**
     Arguments for a standard SQL table mutation
     */
    template <class Type>
    struct SQLTableTypeMutation
    {
        // Name of the column to change values
        String columnName;

        // Optional where clause
        std::optional<SQLWhereArgument> where;

        // Value to set for mutation
        Type value;

        SQLTableTypeMutation(String columnName, std::optional<SQLWhereArgument> where, Type value)
        : columnName(columnName),
        where(where),
        value(value) {
        }
    };
}

#endif
