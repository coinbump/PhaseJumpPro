#ifndef SQLTABLETYPEMUTATEARGUMENTS_H
#define SQLTABLETYPEMUTATEARGUMENTS_H

#include "SQLWhereArguments.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    /**
     Arguments for a standard SQL table mutation
     */
    struct SQLTableMutateArguments {
        /// Name of the column to change values
        String columnName;

        /// Optional where clause
        std::optional<SQLWhereArguments> where;

        /// Value to set for mutation
        SQLValue value;

        SQLTableMutateArguments(
            String columnName, std::optional<SQLWhereArguments> where, SQLValue value
        ) :
            columnName(columnName),
            where(where),
            value(value) {}
    };
} // namespace PJ

#endif
