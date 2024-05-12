#ifndef PJVECTORLIST_H
#define PJVECTORLIST_H

#include "Macros_Collections.h"
#include <vector>

/*
 RATING: 5 stars
 Has unit tests (in ListTests)
 CODE REVIEW: 11/20/22
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
        virtual uint32_t ItemSize() const { return sizeof(T); }

        uint32_t DataSize() const override { return (uint32_t)ItemCount() * ItemSize(); }
    };

    template <class T>
    class CollectionData : public SomeDataCollection<T> {
    public:
        size_t itemCount;
        void* data;

        CollectionData(size_t itemCount, void* data) : itemCount(itemCount), data(data) {
        }

        size_t ItemCount() const override { return itemCount; }
        void* Data() const override { return data; }
    };

    /// <summary>
    /// Extends std::vector with convenience methods
    /// Called VectorList to avoid confusion with Vector geometry objects.
    /// </summary>
    template <class T, class Allocator = std::allocator<T>>
    class VectorList : public std::vector<T, Allocator> {
    public:
        using Base = std::vector<T, Allocator>;
        using This = VectorList<T, Allocator>;

        VectorList() {
        }

        constexpr VectorList(std::initializer_list<T> init,
                             const Allocator& alloc = Allocator()) : Base(init, alloc) {
        }
        explicit VectorList(typename Base::size_type count) : Base(count) {
        }

        // Convenience
        void Append(T const& value) { this->push_back(value); }
        void Add(T const& value) { this->push_back(value); }

        // NOTE: Avoid Remove for large lists, it is inefficient
        COLLECTION_METHODS(VectorList<T>, T)

        operator CollectionData<T>() const {
            CollectionData<T> result(this->size(), (void*)this->data());
            return result;
        }
    };

    // Convenience names
    template <class T> using Vector = VectorList<T>;
    template <class T> using VL = VectorList<T>;
}

#endif
