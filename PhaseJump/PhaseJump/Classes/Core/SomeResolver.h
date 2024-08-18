#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/10/24
 */
namespace PJ {
    /// Provides a dependency, and hides the mechanism used to provide it
    template <class T>
    class SomeResolver {
    public:
        virtual ~SomeResolver() {}

        virtual T Resolve() = 0;
    };
} // namespace PJ
