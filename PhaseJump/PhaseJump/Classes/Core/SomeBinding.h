#ifndef PJSOMEBINDING_H
#define PJSOMEBINDING_H

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Type erasure interface
    class AnyBinding {};

    /// Wrapper used to alter the attribute of an object
    template <class T>
    class SomeBinding : public AnyBinding {
    public:
        virtual T Value() = 0;
        virtual void SetValue(T const& value) = 0;
    };
} // namespace PJ

#endif
