#ifndef SQLTABLEQUERY_H
#define SQLTABLEQUERY_H

#include "SQLWhereArgument.h"
#include <optional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 3/30/23
 */
namespace PJ
{
    /**
     Arguments for a standard SQL table query
     */
    struct SQLTableQuery
    {
        // Name of the column to fetch values from
        String columnName;

        // Optional where clause
        std::optional<SQLWhereArgument> where;

        SQLTableQuery(String columnName, std::optional<SQLWhereArgument> where)
        : columnName(columnName),
        where(where) {
        }
    };
}

#endif
