#ifndef SQLTYPES_H
#define SQLTYPES_H

#include "_String.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 3/29/23
 */
namespace PJ {
    enum class DataType {
        Data,
        XML
    };

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
}

#endif
