#pragma once

#include "SomeSimpleSubscription.h"
#include "Macros.h"
#include "List.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    /// Lightweight publisher, without the complexity of full Reactive support
    template <typename Input>
    struct SomeSimplePublisher {
        using Subscription = SomeSimpleSubscription<Input>;
        using ReceiveFunc = std::function<void(Input const&)>;
        using SubscriptionList = List<WP<Subscription>>;

    protected:
        bool isComplete = false;
        SubscriptionList subscriptions;

    protected:
        virtual void SubscriberAdded(Subscription& subscription) {
        }

    public:
        virtual ~SomeSimplePublisher() {}

        virtual SP<Subscription> Receive(ReceiveFunc func) {
            auto result = MAKE<InSimpleSubscription<Input>>(func);
            this->subscriptions.push_back(result);

            SubscriberAdded(*result);

            return result;
        }

        SubscriptionList ValidSubscriptions() const {
            return subscriptions.Filter([](WP<Subscription> const& subscription) { return !subscription.expired(); });
        }

        std::size_t SubscriptionsCount() const {
            return subscriptions.size();
        }

        void Clean() {
            subscriptions.RemoveIf([](WP<Subscription> const& subscription) {
                if (subscription.expired()) {
                    return true;
                }
                return subscription.lock()->IsCancelled();
            });
        }

    public:
        void Complete() {
            isComplete = true;
        }

        bool IsComplete() const {
            return isComplete;
        }
    };
}
