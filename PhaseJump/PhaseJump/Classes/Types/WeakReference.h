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
        std::weak_ptr<T> value;

    public:
        WeakReference(std::shared_ptr<T> value) : value(value)
        {
        }

        std::shared_ptr<T> Value() override {
            return value.lock();
        }

        bool IsExpired() const {
            return value.expired();
        }

        void SetValue(std::shared_ptr<T> const& value) override {
            this->value = value;
        }
    };
}

#endif
