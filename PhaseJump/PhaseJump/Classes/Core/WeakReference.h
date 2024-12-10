#pragma once

#include "SomeReference.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Holds weak reference to object
    template <class T>
    struct WeakReference : public SomeReference<T> {
    protected:
        WP<T> value;

    public:
        WeakReference(SP<T> value) :
            value(value) {}

        WeakReference(WeakReference const& b) {
            value = b.value;
        }

        bool IsExpired() const {
            return value.expired();
        }

        // MARK: SomeReference

        SP<T> Value() const override {
            return value.lock();
        }

        void SetValue(SP<T> const& value) override {
            this->value = value;
        }
    };
} // namespace PJ
