#ifndef PJSTRONGREFERENCE_H
#define PJSTRONGREFERENCE_H

#include "SomeReference.h"
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
        std::shared_ptr<T> value;

    public:
        StrongReference(std::shared_ptr<T> value) : value(value)
        {
        }

        StrongReference(StrongReference const& b)
        {
            value = b.value;
        }

        std::shared_ptr<T> Value() const override {
            return value;
        }

        void SetValue(std::shared_ptr<T> const& value) override {
            this->value = value;
        }
    };
}

#endif

