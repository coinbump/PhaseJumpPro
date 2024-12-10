#pragma once

#include "SQLWhereArguments.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
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
    };
} // namespace PJ
