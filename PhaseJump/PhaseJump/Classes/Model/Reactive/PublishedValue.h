#pragma once

#include "SomeSimpleSubject.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// When the value changes, notify the subscribers via a reactive subject
    /// To receive change events, use subject.Receive()
    /// Careful: don't store references to objects that might go out of scope
    template <class Type>
        requires std::equality_comparable<Type>
    struct PublishedValue {
    public:
        using Subscription = SomeSimpleSubscription<Type>;
        using ReceiveFunc = SomeSimplePublisher<Type>::ReceiveFunc;

    protected:
        ValueSimpleSubject<Type> subject;

    public:
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

        /// Call to receive new values when the published value changes
        /// Store the subscription to hold a reference to it
        /// To stop receiving values, cancel the subscription
        SP<Subscription> Receive(ReceiveFunc receiveFunc) {
            return subject.Receive(receiveFunc);
        }

        operator Type const&() {
            return subject.Value();
        }
    };
} // namespace PJ
