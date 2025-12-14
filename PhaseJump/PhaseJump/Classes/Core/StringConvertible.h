#pragma once

#include "StringUtils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/13/25
 */
namespace PJ {
    /// Can be converted to a string for logging
    class StringConvertible {
    public:
        virtual ~StringConvertible() {}

        virtual String ToString() const = 0;

        /// Allows this type to be used with cout
        friend std::ostream& operator<<(std::ostream& os, StringConvertible const& value);

        // MARK: Convenience

        String Description() const {
            return ToString();
        }
    };

    // MARK: StringConvertible friend operators

    std::ostream& operator<<(std::ostream& os, StringConvertible const& value);

    /// Produces a string based on the context. Example: verbose, non-verbose, memory cost, etc.
    class ContextStringConvertible {
    public:
        virtual ~ContextStringConvertible() {}

        virtual String ContextString(String context) const = 0;
    };
} // namespace PJ
