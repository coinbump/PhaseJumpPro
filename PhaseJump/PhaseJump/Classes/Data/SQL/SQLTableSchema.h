#ifndef SQLTABLESCHEMA_H
#define SQLTABLESCHEMA_H

#include "Array.h"
#include <optional>

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 4/4/23
 */
namespace PJ {
    /// Specifies how a column will be built when the SQLite table is created
    struct SQLColumnSchema
    {
        String name;
        SQLValueType valueType;
        std::optional<String> defaultValue;

        SQLColumnSchema(String name, SQLValueType valueType, std::optional<String> defaultValue = std::nullopt)
        : name(name),
        valueType(valueType),
        defaultValue(defaultValue) {
        }
    };

    /// Specifies how a table will be built
    struct SQLTableSchema
    {
        Array<SQLColumnSchema> columns;
    };
}

#endif
