#ifndef PJWEAKREFERENCE_H
#define PJWEAKREFERENCE_H

#include "SomeReference.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
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

        SP<T> Value() const override {
            return value.lock();
        }

        bool IsExpired() const {
            return value.expired();
        }

        void SetValue(SP<T> const& value) override {
            this->value = value;
        }
    };
} // namespace PJ

#endif
