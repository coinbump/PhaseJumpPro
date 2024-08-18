#pragma once

#include "SomeSimplePublisher.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    /// Backing for a publisher
    template <typename Input>
    struct SomeSimpleSubject : public SomeSimplePublisher<Input> {};

    /// Stored value backing for a publisher
    template <typename Input>
    struct ValueSimpleSubject : public SomeSimpleSubject<Input> {
        using Base = SomeSimpleSubject<Input>;

    protected:
        Input value = Input();

    public:
        ValueSimpleSubject(Input const& value) :
            value(value) {}

        void Send(Input const& value) {
            if (this->isComplete) {
                return;
            }
            this->value = value;

            auto iterSubscriptions = this->subscriptions;
            for (auto& subscription : iterSubscriptions) {
                if (auto subLock = subscription.lock()) {
                    if (subLock->IsCancelled()) {
                        continue;
                    }
                    subLock->Receive(value);
                }
            }
        }

        void SubscriberAdded(Base::Subscription& subscription) override {
            Base::SubscriberAdded(subscription);

            // Send the initial value
            subscription.Receive(value);
        }

        Input const& Value() const {
            return value;
        }
    };
} // namespace PJ
