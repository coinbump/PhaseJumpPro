#pragma once

#include "SomeCancellable.h"

namespace PJ {
    template <class Input>
    struct SomeSimpleSubscription : public Cancellable {
        virtual ~SomeSimpleSubscription() {}

        virtual void Receive(Input const& value) = 0;

        // FUTURE: Support advanced reactive subscriptions with demand-based request if needed
        // virtual void Request(int demand);
    };

    template <class Input>
    struct InSimpleSubscription : public SomeSimpleSubscription<Input> {
        using ReceiveValue = Input const&;
        using ReceiveFunc = std::function<void(ReceiveValue)>;

        ReceiveFunc receiveFunc;

        InSimpleSubscription(ReceiveFunc receiveFunc) : receiveFunc(receiveFunc) {
        }

        void Receive(ReceiveValue value) override {
            receiveFunc(value);
        }
    };
}
