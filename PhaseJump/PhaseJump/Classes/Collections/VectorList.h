#pragma once

#include "_String.h"
#include "CollectionUtils.h"
#include "InfixOStreamIterator.h"
#include "Utils.h"
#include <vector>

// TODO: rethink using inheritance here
/*
 RATING: 5 stars
 Has unit tests (in ListTests)
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// An object with accessible raw memory data
    class SomeDataContainer {
    public:
        virtual ~SomeDataContainer() {}

        virtual void* Data() const = 0;
        virtual uint32_t DataSize() const = 0;
    };

    /// A collection with accessible raw memory data
    template <class T>
    class SomeDataCollection : public SomeDataContainer {
    public:
        virtual size_t ItemCount() const = 0;

        virtual uint32_t ItemSize() const {
            return sizeof(T);
        }

        uint32_t DataSize() const override {
            return (uint32_t)ItemCount() * ItemSize();
        }
    };

    template <class T>
    class CollectionData : public SomeDataCollection<T> {
    public:
        size_t itemCount;
        void* data;

        CollectionData(size_t itemCount, void* data) :
            itemCount(itemCount),
            data(data) {}

        size_t ItemCount() const override {
            return itemCount;
        }

        void* Data() const override {
            return data;
        }
    };

    /// Extends std::vector with convenience methods
    /// Called VectorList to avoid confusion with Vector geometry objects.
    template <class T, class Allocator = std::allocator<T>>
    class VectorList : public std::vector<T, Allocator> {
    public:
        using Base = std::vector<T, Allocator>;
        using This = VectorList<T, Allocator>;

        VectorList() {}

        constexpr VectorList(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
            Base(init, alloc) {}

        explicit VectorList(typename Base::size_type count) :
            Base(count) {}

        explicit VectorList(typename Base::size_type count, T const& value) :
            Base(count, value) {}

        // Convenience
        void Append(T value) {
            this->push_back(value);
        }

        void Add(T value) {
            this->push_back(value);
        }

        operator CollectionData<T>() const {
            CollectionData<T> result(this->size(), (void*)this->data());
            return result;
        }

        // MARK: Extension- std::String

        SPECIALIZE_IS_BASE_OF(T, std::string) T Joined(T delimiter) const {
            std::stringstream stream;
            std::copy(
                this->begin(), this->end(), InfixOStreamIterator<T>(stream, delimiter.c_str())
            );
            return stream.str();
        }
    };

    // Convenience names
    template <class T>
    using Vector = VectorList<T>;

    template <class T>
    using VL = VectorList<T>;
} // namespace PJ
