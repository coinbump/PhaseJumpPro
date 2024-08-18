#pragma once

#include "_String.h"

/*
 RATING: 5 stars
 Has unit tests
 Code review: 7/12/24
 */
namespace PJ {
    /// SQLite utilities
    struct SQLTool {
        bool IsReservedKeyword(String value);
    };
} // namespace PJ
