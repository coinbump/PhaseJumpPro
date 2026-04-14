#pragma once

#include "Macros.h"
#include "UnorderedSet.h"
#include "Utils.h"
#include "VectorList.h"
#include <functional>
#include <stack>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/13/26
 */
namespace PJ {
    /// Wraps access to a items
    template <class Type>
    class SomeGetter {
    public:
        virtual ~SomeGetter() {}

        virtual Type Get() const = 0;

        operator Type() const {
            return Get();
        }
    };

    /// List of objects that can be re-used, so we don't have the cost of allocation and
    /// de-allocation. Used in systems with large numbers of objects. Example: simulations, particle
    /// emitters
    template <class Type>
    class Pool final {
    public:
        using ResetFunc = std::function<void(Type&)>;

        ResetFunc resetFunc;

        DELETE_COPY(Pool)
        DELETE_MOVE(Pool)

        class Getter : public SomeGetter<Type&> {
        public:
            using This = Getter;

            friend class Pool<Type>;

        protected:
            Pool<Type>& pool;
            size_t index{};

        public:
            Getter(Pool<Type>& pool, size_t index) :
                pool(pool),
                index(index) {}

            // MARK: SomeGetter

            Type& Get() const override {
                return pool.items[index].value;
            }
        };

        class Item {
        public:
            Type value;
            bool isActive{};
        };

    protected:
        size_t activeCount = 0;

        VectorList<Item> items;
        std::stack<size_t> inactiveAvailable;

    public:
        Pool(size_t size, ResetFunc resetFunc) :
            items(size),
            resetFunc(resetFunc) {}

        /// @return Returns the max index possible after the final active element
        size_t ActiveScanEnd() const {
            return activeCount + inactiveAvailable.size();
        }

        /// @return Returns the element at the specified index, or throws an exception if it is not
        /// available
        Type const& at(size_t index) const {
            GUARD_THROW(
                index < items.size() && items[index].isActive,
                std::out_of_range("Invalid pool index")
            )
            return items[index].value;
        }

        /// @return Returns the element at the specified index, or throws an exception if it is not
        /// available
        Type& at(size_t index) {
            GUARD_THROW(
                index < items.size() && items[index].isActive,
                std::out_of_range("Invalid pool index")
            )
            return items[index].value;
        }

        void ForActive(std::function<void(Type&)> action) {
            GUARD(action)

            auto activeEnd = ActiveScanEnd();
            for (size_t i = 0; i < activeEnd && i < items.size(); i++) {
                Item& item = items[i];
                GUARD_CONTINUE(item.isActive)
                action(item.value);
            }
        }

        VectorList<Item> const& Items() const {
            return items;
        }

        size_t Size() const {
            return items.size();
        }

        size_t ActiveCount() const {
            return activeCount;
        }

        size_t InactiveAvailableCount() const {
            return inactiveAvailable.size();
        }

        /// Adds item to the pool or reuses it
        std::optional<Getter> Add() {
            auto firstInactiveIndex = activeCount;

            if (inactiveAvailable.size() > 0) {
                firstInactiveIndex = inactiveAvailable.top();
                inactiveAvailable.pop();
            }

            GUARDR(firstInactiveIndex < items.size(), {})

            SetIsActive(firstInactiveIndex, true);

            return Getter(*this, firstInactiveIndex);
        }

        std::optional<Getter> Add(Type const& value) {
            auto result = Add();
            GUARDR(result, {})

            result->Get() = value;
            return result;
        }

        void Remove(std::optional<Getter> const& getter) {
            GUARD(getter)
            RemoveAt(getter->index);
        }

        void Remove(Getter const& getter) {
            RemoveAt(getter.index);
        }

        void RemoveAt(size_t index) {
            GUARD(index < items.size())
            GUARD(items[index].isActive)

            SetIsActive(index, false);
            inactiveAvailable.push(index);
        }

        bool IsActive(size_t index) const {
            GUARDR(index < items.size(), false)
            return items[index].isActive;
        }

    protected:
        void SetIsActive(size_t index, bool value) {
            GUARD(index < items.size())

            auto isActive = IsActive(index);

            GUARD(isActive != value)
            items[index].isActive = value;

            if (value) {
                activeCount++;
            } else {
                activeCount--;
            }

            GUARD(value)
            Reset(items[index]);
        }

        void Reset(Item& item) {
            GUARD(resetFunc)

            try {
                resetFunc(item.value);
            } catch (...) {}
        }
    };
} // namespace PJ
