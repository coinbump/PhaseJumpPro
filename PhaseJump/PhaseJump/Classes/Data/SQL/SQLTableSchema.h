#pragma once

#include "VectorList.h"
#include <optional>

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/29/24
 */
namespace PJ {
    /// Specifies how a column will be built when the SQLite table is created
    struct SQLColumnSchema {
        String name;
        SQLValueType valueType;
        std::optional<String> defaultValue;

        SQLColumnSchema(
            String name, SQLValueType valueType, std::optional<String> defaultValue = std::nullopt
        ) :
            name(name),
            valueType(valueType),
            defaultValue(defaultValue) {}
    };

    /// Specifies how a table will be built
    struct SQLTableSchema {
        VectorList<SQLColumnSchema> columns;
    };
} // namespace PJ
