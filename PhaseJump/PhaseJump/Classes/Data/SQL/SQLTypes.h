#pragma once

#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    enum class SQLValueType { Int, Real, Text, Blob, Any };

    /// Specifies how the database should be opened
    /// https://www.sqlite.org/c3ref/open.html
    enum class SQLDatabaseOpenType {
        // Open database at path
        AtPath,

        // Create database in memory
        InMemory,

        // Create temporary database on disk
        TemporaryOnDisk
    };
} // namespace PJ
