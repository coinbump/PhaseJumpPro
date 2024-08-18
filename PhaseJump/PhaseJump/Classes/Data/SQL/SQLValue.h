#ifndef SQLVALUE_H
#define SQLVALUE_H

#include "_String.h"
#include "SQLTypes.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    struct SQLValue {
        SQLValueType type;
        String value;

        SQLValue(SQLValueType type, String value) :
            type(type),
            value(value) {}
    };
} // namespace PJ

#endif
