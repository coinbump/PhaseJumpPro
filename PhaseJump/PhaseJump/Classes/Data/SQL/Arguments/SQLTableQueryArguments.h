#ifndef SQLTABLEQUERYARGUMENTS_H
#define SQLTABLEQUERYARGUMENTS_H

#include "SQLWhereArguments.h"
#include "StringVectorList.h"
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
    struct SQLTableQueryArguments
    {
        // Name of the column to fetch values from
        StringVectorList columnNames;

        // Optional where clause
        std::optional<SQLWhereArguments> where;

        SQLTableQueryArguments(String columnName, std::optional<SQLWhereArguments> where)
        : where(where) {
            columnNames.Append(columnName);
        }

        SQLTableQueryArguments(StringVectorList columnNames, std::optional<SQLWhereArguments> where)
        : columnNames(columnNames), where(where) {
        }
    };
}

#endif
