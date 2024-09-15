#pragma once

#include "UnorderedSet.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    class Poolable {
    public:
        using ResetFunc = std::function<void(Poolable&)>;

    protected:
        bool isActive = false;

    public:
        // Used for pool remove
        size_t index = 0;

        ResetFunc resetFunc;

        bool IsActive() const {
            return isActive;
        }

        void SetIsActive(bool value) {
            GUARD(isActive != value)
            isActive = value;

            // When re-activating a poolable object, it gets reset for reuse
            GUARD(isActive)
            Reset();
        }

        void Reset() {
            GUARD(resetFunc)
            resetFunc(*this);
        }
    };

    /// List of objects that can be re-used, so we don't have the cost of allocation and
    /// de-allocation. Used in systems with large numbers of objects. Example: simulations, particle
    /// emitters
    template <std::derived_from<Poolable> Type>
    class Pool {
    protected:
        int64_t lastActiveIndex = -1;
        size_t activeCount = 0;

        VectorList<UP<Type>> value;
        UnorderedSet<size_t> inactiveAvailable;

    public:
        /// (Optional). If > 0 indicates the max size for this pool
        size_t maxSize = 0;

        Pool(size_t size, size_t maxSize = 0) :
            value(size),
            maxSize(maxSize) {}

        /// Returns the element at the specified index, or throws an exception if it is not
        /// available
        Type const& at(size_t index) const {
            GUARD_THROW(
                index >= 0 && index <= lastActiveIndex && index < value.size() && value[index],
                std::out_of_range("Invalid pool index")
            )
            return *value[index];
        }

        /// Returns the element at the specified index, or throws an exception if it is not
        /// available
        Type& at(size_t index) {
            GUARD_THROW(
                index >= 0 && index <= lastActiveIndex && index < value.size() && value[index],
                std::out_of_range("Invalid pool index")
            )
            return *value[index];
        }

        int64_t LastActiveIndex() const {
            return lastActiveIndex;
        }

        VectorList<UP<Type>> const& Value() {
            return value;
        }

        size_t PoolSize() const {
            return value.size();
        }

        size_t ActiveCount() const {
            return activeCount;
        }

        size_t InactiveAvailableCount() const {
            return inactiveAvailable.size();
        }

        /// Adds item to the pool or reuses it
        Type* Add() {
            auto firstInactiveIndex = this->lastActiveIndex + 1;

            if (inactiveAvailable.size() > 0) {
                firstInactiveIndex = *inactiveAvailable.begin();
                inactiveAvailable.erase(firstInactiveIndex);
            }

            /// Grow pool as needed, up to max size
            if (firstInactiveIndex >= value.size()) {
                size_t newSize = value.size() * 2;
                if (maxSize > 0) {
                    value.resize(std::min(maxSize, newSize));
                } else {
                    value.resize(newSize);
                }
            }

            GUARDR(firstInactiveIndex < value.size(), nullptr)

            if (nullptr == value[firstInactiveIndex]) {
                value[firstInactiveIndex] = std::make_unique<Type>();
            }
            value[firstInactiveIndex]->SetIsActive(true);
            value[firstInactiveIndex]->index = firstInactiveIndex;
            activeCount++;

            lastActiveIndex = std::max(lastActiveIndex, (int64_t)firstInactiveIndex);

            return value[firstInactiveIndex].get();
        }

        void Remove(Type* typeValue) {
            GUARD(typeValue)
            Remove(*typeValue);
        }

        void Remove(Type typeValue) {
            RemoveAt(typeValue.index);
        }

        void RemoveAt(size_t index) {
            GUARD(index >= 0 && index < value.size())
            GUARD(value[index]->IsActive())

            value[index]->SetIsActive(false);
            inactiveAvailable.insert(index);
            activeCount--;

            GUARD(lastActiveIndex == index)

            // We removed the element for lastActiveIndex, so update it
            int64_t newLastActiveIndex = -1;
            for (int64_t i = index - 1; i >= 0; i--) {
                if (value[i]->IsActive()) {
                    newLastActiveIndex = i;
                    break;
                }
            }
            lastActiveIndex = newLastActiveIndex;
        }
    };
} // namespace PJ
