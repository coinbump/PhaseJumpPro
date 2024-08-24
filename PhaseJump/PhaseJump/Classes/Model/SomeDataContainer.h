#pragma once

// CODE REVIEW: ?/23
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
} // namespace PJ
