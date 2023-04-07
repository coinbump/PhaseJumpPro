#ifndef SQLVALUE_H
#define SQLVALUE_H

#include "SQLTypes.h"
#include "_String.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/4/23
 */
namespace PJ
{
    struct SQLValue
    {
        SQLValueType type;
        String value;

        SQLValue(SQLValueType type, String value) : type(type), value(value) {
        }
    };
}

#endif
