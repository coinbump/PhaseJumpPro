#pragma once

#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Selects values "WHERE columnName='value'"
    struct SQLWhereArguments {
        String columnName;
        String value;
    };
} // namespace PJ
