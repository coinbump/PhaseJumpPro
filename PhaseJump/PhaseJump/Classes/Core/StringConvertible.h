#pragma once

#include "StringUtils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/10/24
 */
namespace PJ {
    /// Can be converted to a string for logging
    class StringConvertible {
    public:
        virtual ~StringConvertible() {}

        virtual String ToString() const = 0;

        friend std::ostream& operator<<(std::ostream& os, StringConvertible const& value);

        // Convenience
        String Description() const {
            return ToString();
        }
    };

    // TODO: rethink this. ToString and << might not be the same
    // MARK: StringConvertible friend operators
    std::ostream& operator<<(std::ostream& os, StringConvertible const& value);

    /// Produces a string based on the context. Example: verbose, non-verbose, memory cost, etc.
    template <class Context = String>
    class ContextStringConvertible {
    public:
        virtual ~ContextStringConvertible() {}

        virtual String ContextString(Context context) const = 0;
    };
} // namespace PJ
