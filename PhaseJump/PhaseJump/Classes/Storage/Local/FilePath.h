#pragma once

#include <filesystem>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Wraps a file path for easily joining path segments
    using FilePath = std::filesystem::path;
} // namespace PJ
