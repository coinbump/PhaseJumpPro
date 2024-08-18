#ifndef PJFILEPATH_H_
#define PJFILEPATH_H_

#include <filesystem>

/*
 RATING: 5 stars
 Simple utility with unit tests
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    /// Wraps a file path for easily joining path segments
    using FilePath = std::filesystem::path;
} // namespace PJ

#endif
