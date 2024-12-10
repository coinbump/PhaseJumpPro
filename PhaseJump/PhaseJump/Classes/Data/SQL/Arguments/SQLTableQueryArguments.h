#pragma once

#include "SQLWhereArguments.h"
#include "VectorList.h"
#include <optional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
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
    };
} // namespace PJ
