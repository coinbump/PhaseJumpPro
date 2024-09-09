#pragma once

#include "List.h"
#include "SomeSimpleSubscription.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// Lightweight publisher, without the complexity of full Reactive support
    template <typename Output>
    struct SomeSimplePublisher {
        using Subscription = SomeSimpleSubscription<Output>;
        using ReceiveFunc = std::function<void(Output const&)>;
        using SubscriptionList = List<WP<Subscription>>;

    protected:
        bool isComplete = false;
        SubscriptionList subscriptions;

    protected:
        virtual void OnAdd(Subscription& subscription) {}

    public:
        virtual ~SomeSimplePublisher() {}

        /// Call to receive new values
        /// Store the subscription to hold a reference to it
        /// To stop receiving values, cancel the subscription
        SP<Subscription> Receive(ReceiveFunc func) {
            auto result = MAKE<SimpleSubscription<Output>>(func);
            this->subscriptions.push_back(result);

            OnAdd(*result);

            return result;
        }

        SubscriptionList ValidSubscriptions() const {
            return Filter(subscriptions, [](WP<Subscription> const& subscription) {
                return !subscription.expired();
            });
        }

        std::size_t SubscriptionsCount() const {
            return subscriptions.size();
        }

        void Clean() {
            RemoveIf(subscriptions, [](WP<Subscription> const& subscription) {
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
} // namespace PJ
