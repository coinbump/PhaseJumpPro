#pragma once

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    enum class FileSearchType {
        /// Search an individual file
        File,

        /// Search the direct children of the directory
        Directory,

        /// Search the files and files in children
        Recursive
    };
}
