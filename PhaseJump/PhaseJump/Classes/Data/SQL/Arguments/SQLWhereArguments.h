#ifndef SQLWHEREARGUMENTS_H
#define SQLWHEREARGUMENTS_H

#include "_String.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 3/30/23
 */
namespace PJ
{
    /// Selects values "WHERE columnName='value'"
    struct SQLWhereArguments
    {
        String columnName;
        String value;

        SQLWhereArguments(String columnName, String value) : columnName(columnName), value(value) {
        }
    };
}

#endif
