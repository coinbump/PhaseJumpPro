#ifndef PJFILEPATH_H_
#define PJFILEPATH_H_

#include <filesystem>

/*
 RATING: 5 stars
 Simple utility with unit tests
 CODE REVIEW: 11/6/22
 */
namespace PJ {
    /// <summary>
    /// Wraps a file path for easily joining path segments
    /// </summary>
    typedef std::filesystem::path FilePath;
}

#endif
