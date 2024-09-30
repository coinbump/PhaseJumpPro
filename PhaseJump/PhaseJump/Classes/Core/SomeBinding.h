#pragma once

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Type erasure interface
    class AnyBinding {};

    /// Wrapper used to alter the attribute of an object
    template <class T>
    class SomeBinding : public AnyBinding {
    public:
        using This = SomeBinding;

        virtual ~SomeBinding() = default;

        virtual T Value() const = 0;
        virtual void SetValue(T const& value) const = 0;
    };
} // namespace PJ
