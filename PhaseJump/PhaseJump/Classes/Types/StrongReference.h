#ifndef PJSTRONGREFERENCE_H
#define PJSTRONGREFERENCE_H

#include "SomeReference.h"
#include "Macros.h"
#include <memory>

/*
 RATING: 5 stars
 Utility class with unit tests
 CODE REVIEW: 11/16/22
 */
namespace PJ
{
    /// <summary>
    /// Holds strong reference to an object
    /// </summary>
    template <class T> struct StrongReference : public SomeReference<T>
    {
    protected:
        SP<T> value;

    public:
        StrongReference(SP<T> value) : value(value)
        {
        }

        StrongReference(StrongReference const& b)
        {
            value = b.value;
        }

        SP<T> Value() const override {
            return value;
        }

        void SetValue(SP<T> const& value) override {
            this->value = value;
        }
    };
}

#endif

