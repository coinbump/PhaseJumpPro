#ifndef SQLUTILS_H
#define SQLUTILS_H

#pragma once

#include "_String.h"

/*
 RATING: 5 stars
 Has unit tests
 Code review: 3/29/23
 */
namespace PJ {
    /// <summary>
    /// SQLite utilities
    /// </summary>
    struct SQLUtils {
        bool IsReservedKeyword(String value);
    };
}

#endif
