#ifndef PJFILEPATH_H_
#define PJFILEPATH_H_

#include <filesystem>

/*
 RATING: 5 stars
 Simple utility with unit tests
 CODE REVIEW: 5/3/23
 */
namespace PJ {
    /// <summary>
    /// Wraps a file path for easily joining path segments
    /// </summary>
    using FilePath = std::filesystem::path;
}

#endif
