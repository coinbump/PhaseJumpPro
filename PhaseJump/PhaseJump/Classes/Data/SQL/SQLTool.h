#pragma once

#include "StringUtils.h"

/*
 RATING: 5 stars
 Has unit tests
 Code review: 12/29/24
 */
namespace PJ {
    /// SQLite utilities
    struct SQLTool {
        bool IsReservedKeyword(String value);
    };
} // namespace PJ
