#pragma once

#include "SomeSimpleSubject.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    template <class Type>
        requires std::equality_comparable<Type>
    struct PublishedValue {
        using Subscription = SomeSimpleSubscription<Type>;
        ValueSimpleSubject<Type> subject;

        PublishedValue(Type const& value) :
            subject(value) {}

        Type const& Value() const {
            return subject.Value();
        }

        void SetValue(Type const& value) {
            if (subject.Value() == value) {
                return;
            }
            subject.Send(value);
        }
    };
} // namespace PJ
