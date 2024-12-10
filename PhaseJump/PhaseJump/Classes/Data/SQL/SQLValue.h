#pragma once

#include "SQLTypes.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Stores a SQL value
    struct SQLValue {
        SQLValueType type;
        String value;

        SQLValue(SQLValueType type, String value) :
            type(type),
            value(value) {}
    };
} // namespace PJ
