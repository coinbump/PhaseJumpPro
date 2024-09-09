#pragma once

#include "SomeSimplePublisher.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// Backing for a publisher
    template <typename Output>
    struct SomeSimpleSubject : public SomeSimplePublisher<Output> {};

    /// Stored value backing for a publisher
    template <typename Output>
    struct ValueSimpleSubject : public SomeSimpleSubject<Output> {
        using Base = SomeSimpleSubject<Output>;

    protected:
        Output value = Output();

    public:
        ValueSimpleSubject(Output const& value) :
            value(value) {}

        void Send(Output const& value) {
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

        void OnAdd(Base::Subscription& subscription) override {
            Base::OnAdd(subscription);

            // Send the initial value
            subscription.Receive(value);
        }

        Output const& Value() const {
            return value;
        }
    };
} // namespace PJ
