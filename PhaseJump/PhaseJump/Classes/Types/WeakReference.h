#ifndef PJWEAKREFERENCE_H
#define PJWEAKREFERENCE_H

#include "SomeReference.h"
#include <memory>

/*
 * RATING: 5 stars
 * Simple utility with unit tests
 * CODE REVIEW: 11/16/22
 */
namespace PJ
{
    /// <summary>
    /// Holds weak reference to object
    /// </summary>
    template <class T> struct WeakReference : public SomeReference<T>
    {
    protected:
        WP<T> value;

    public:
        WeakReference(SP<T> value) : value(value)
        {
        }

        WeakReference(WeakReference const& b)
        {
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
}

#endif
