#ifndef PJSTRONGREFERENCE_H
#define PJSTRONGREFERENCE_H

#include "SomeReference.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Holds strong reference to an object
    template <class T>
    struct StrongReference : public SomeReference<T> {
    protected:
        SP<T> value;

    public:
        StrongReference(SP<T> value) :
            value(value) {}

        StrongReference(StrongReference const& b) {
            value = b.value;
        }

        SP<T> Value() const override {
            return value;
        }

        void SetValue(SP<T> const& value) override {
            this->value = value;
        }
    };
} // namespace PJ

#endif
