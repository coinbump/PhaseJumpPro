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
    struct SQLUtils {
        bool IsReservedKeyword(String value);
    };
}

#endif
