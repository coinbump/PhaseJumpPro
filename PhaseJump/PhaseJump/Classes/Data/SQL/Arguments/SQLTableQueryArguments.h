#ifndef SQLTABLEQUERYARGUMENTS_H
#define SQLTABLEQUERYARGUMENTS_H

#include "SQLWhereArguments.h"
#include "VectorList.h"
#include <optional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    /**
     Arguments for a standard SQL table query
     */
    struct SQLTableQueryArguments {
        // Name of the column to fetch values from
        VectorList<String> columnNames;

        // Optional where clause
        std::optional<SQLWhereArguments> where;

        SQLTableQueryArguments(String columnName, std::optional<SQLWhereArguments> where) :
            where(where) {
            Add(columnNames, columnName);
        }

        SQLTableQueryArguments(
            VectorList<String> columnNames, std::optional<SQLWhereArguments> where
        ) :
            columnNames(columnNames),
            where(where) {}
    };
} // namespace PJ

#endif
