#ifndef PJSOMEREFERENCE_H
#define PJSOMEREFERENCE_H

#include "Utils.h"
#include <memory>
#include <optional>

/*
 RATING: 5 stars
 Simple protocol with unit tests
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    /// Interface for a reference type. Allows either strong or weak reference
    /// to value
    template <class T>
    struct SomeReference {
        virtual ~SomeReference() {}

        virtual SP<T> Value() const = 0;
        virtual void SetValue(SP<T> const& value) = 0;
    };
} // namespace PJ

#endif
