#pragma once

#include <deque>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/19/26
 */
namespace PJ {
    /// Fixed-size FIFO history buffer
    /// When the limit is exceeded, the oldest items are pruned first
    template <class Type>
    class History {
    public:
        using This = History<Type>;
        using Storage = std::deque<Type>;

        size_t limit = 100;

    protected:
        Storage items;

    public:
        History() = default;

        History(size_t limit) :
            limit(limit) {}

        Storage const& Items() const {
            return items;
        }

        size_t Size() const {
            return items.size();
        }

        bool IsEmpty() const {
            return items.empty();
        }

        void Clear() {
            items.clear();
        }

        void Add(Type const& value) {
            items.push_back(value);
            Prune();
        }

        void Add(Type&& value) {
            items.push_back(std::move(value));
            Prune();
        }

    protected:
        void Prune() {
            while (items.size() > limit) {
                items.pop_front();
            }
        }
    };
} // namespace PJ
